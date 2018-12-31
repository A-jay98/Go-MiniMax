#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "gtp.h"
#include "board.h"
#include "clock.h"
#include "config.h"
#include "gg_utils.h"
#include "core.h"
#include "info.h"

int gtp_version = 2;

/* Internal state that's not part of the engine. */
static int report_uncertainty = 0;
static int gtp_orientation = 0;

static void rotate_on_input(int ai, int aj, int *bi, int *bj);

static void rotate_on_output(int ai, int aj, int *bi, int *bj);


/* Imported functions from other GNU Go files */
int check_boardsize(int boardsize, FILE *out);  // interface.c

void gnugo_play_move(int move, int color);  // interface.c

int genmove(int color, float *value, int *resign);  // genmove.c


#define DECLARE(func) static int func(char *s)

DECLARE(gtp_clear_board);

DECLARE(gtp_genmove);

DECLARE(gtp_genmove_black);

DECLARE(gtp_genmove_white);

DECLARE(gtp_gg_undo);

DECLARE(gtp_list_commands);

DECLARE(gtp_name);

DECLARE(gtp_play);

DECLARE(gtp_playblack);

DECLARE(gtp_playwhite);

DECLARE(gtp_program_version);

DECLARE(gtp_protocol_version);

DECLARE(gtp_quit);

DECLARE(gtp_set_boardsize);

DECLARE(gtp_set_free_handicap);

DECLARE(gtp_set_komi);

DECLARE(gtp_time_left);

DECLARE(gtp_time_settings);

DECLARE(gtp_undo);


/* List of known commands. */
static struct gtp_command commands[] = {
        {"black",             gtp_playblack},
        {"boardsize",         gtp_set_boardsize},
        {"clear_board",       gtp_clear_board},
        {"genmove",           gtp_genmove},
        {"genmove_black",     gtp_genmove_black},
        {"genmove_white",     gtp_genmove_white},
        {"gg-undo",           gtp_gg_undo},
        {"help",              gtp_list_commands},
        {"komi",              gtp_set_komi},
        {"list_commands",     gtp_list_commands},
        {"name",              gtp_name},
        {"play",              gtp_play},
        {"protocol_version",  gtp_protocol_version},
        {"quit",              gtp_quit},
        {"set_free_handicap", gtp_set_free_handicap},
        {"time_left",         gtp_time_left},
        {"time_settings",     gtp_time_settings},
        {"undo",              gtp_undo},
        {"version",           gtp_program_version},
        {"white",             gtp_playwhite},
        {NULL, NULL}
};


void reset_engine() {
    /* To improve the reproducability of games, we restart the random
   * number generator with the same seed for each move. Thus we don't
   * have to know how many previous moves have been played, nor
   * actually play through them, in order to get the right random
   * numbers.
   */
    reuse_random_seed();

    hashdata_recalc(&board_hash, board, board_ko_pos);

    reset();
}

/* Start playing using the Go Text Protocol. */
void play_gtp(FILE *gtp_input, FILE *gtp_output, FILE *gtp_dump_commands, int gtp_initial_orientation) {
    /* Make sure `gtp_output' is unbuffered. (Line buffering is also
     * okay but not necessary. Block buffering breaks GTP mode.)
     *
     * FIXME: Maybe should go to `gtp.c'?
     */
    setbuf(gtp_output, NULL);

    /* Inform the GTP utility functions about the board size. */
    gtp_internal_set_boardsize(board_size);
    gtp_orientation = gtp_initial_orientation;
    gtp_set_vertex_transform_hooks(rotate_on_input, rotate_on_output);

    /* Initialize time handling. */
    init_timers();

    /* Prepare pattern matcher and reading code. */
    reset_engine();
    gtp_main_loop(commands, gtp_input, gtp_output, gtp_dump_commands);
}


/* Function:  List all known commands
 * Arguments: none
 * Fails:     never
 * Returns:   list of known commands, one per line
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_list_commands(char *s) {
    int k;
    UNUSED(s);

    gtp_start_response(GTP_SUCCESS);

    for (k = 0; commands[k].name != NULL; k++)
        gtp_printf("%s\n", commands[k].name);

    gtp_printf("\n");
    return GTP_OK;
}


/****************************
 * Administrative commands. *
 ****************************/

/* Function:  Quit
 * Arguments: none
 * Fails:     never
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_quit(char *s) {
    UNUSED(s);
    gtp_success("");
    return GTP_QUIT;
}


/* Function:  Report protocol version.
 * Arguments: none
 * Fails:     never
 * Returns:   protocol version number
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_protocol_version(char *s) {
    UNUSED(s);
    return gtp_success("%d", gtp_version);
}


/****************************
 * Program identity.        *
 ****************************/

/* Function:  Report the name of the program.
 * Arguments: none
 * Fails:     never
 * Returns:   program name
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_name(char *s) {
    UNUSED(s);
    return gtp_success(TEAM_NAME);
}


/* Function:  Report the version number of the program.
 * Arguments: none
 * Fails:     never
 * Returns:   version number
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_program_version(char *s) {
    UNUSED(s);
    return gtp_success(VERSION);
}


/***************************
 * Setting the board size. *
 ***************************/

/* Function:  Set the board size to NxN and clear the board.
 * Arguments: integer
 * Fails:     board size outside engine's limits
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_set_boardsize(char *s) {
    int boardsize;

    if (sscanf(s, "%d", &boardsize) < 1)
        return gtp_failure("boardsize not an integer");

    if (!check_boardsize(boardsize, NULL)) {
        if (gtp_version == 1)
            return gtp_failure("unacceptable boardsize");
        else
            return gtp_failure("unacceptable size");
    }

    board_size = boardsize;
    clear_board();
    gtp_internal_set_boardsize(boardsize);
    reset_engine();
    return gtp_success("");
}


/***********************
 * Clearing the board. *
 ***********************/

/* Function:  Clear the board.
 * Arguments: none
 * Fails:     never
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_clear_board(char *s) {
    UNUSED(s);

    clear_board();
    init_timers();

    return gtp_success("");
}


// TODO: Check the use of komi
/***************************
 * Setting komi.           *
 ***************************/

/* Function:  Set the komi.
 * Arguments: float
 * Fails:     incorrect argument
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_set_komi(char *s) {
    if (sscanf(s, "%f", &komi) < 1)
        return gtp_failure("komi not a float");

    return gtp_success("");
}


/******************
 * Playing moves. *
 ******************/

/* Function:  Play a black stone at the given vertex.
 * Arguments: vertex
 * Fails:     invalid vertex, illegal move
 * Returns:   nothing
 *
 * Status:    Obsolete GTP version 1 command.
 */
static int gtp_playblack(char *s) {
    int i, j;
    char *c;

    for (c = s; *c; c++)
        *c = tolower((int) *c);

    if (strncmp(s, "pass", 4) == 0) {
        i = -1;
        j = -1;
    } else if (!gtp_decode_coord(s, &i, &j))
        return gtp_failure("invalid coordinate");

    if (!is_allowed_move(POS(i, j), BLACK))
        return gtp_failure("illegal move");

    gnugo_play_move(POS(i, j), BLACK);
    return gtp_success("");
}


/* Function:  Play a white stone at the given vertex.
 * Arguments: vertex
 * Fails:     invalid vertex, illegal move
 * Returns:   nothing
 *
 * Status:    Obsolete GTP version 1 command.
 */
static int gtp_playwhite(char *s) {
    int i, j;
    char *c;

    for (c = s; *c; c++)
        *c = tolower((int) *c);

    if (strncmp(s, "pass", 4) == 0) {
        i = -1;
        j = -1;
    } else if (!gtp_decode_coord(s, &i, &j))
        return gtp_failure("invalid coordinate");

    if (!is_allowed_move(POS(i, j), WHITE))
        return gtp_failure("illegal move");

    gnugo_play_move(POS(i, j), WHITE);
    return gtp_success("");
}


/* Function:  Play a stone of the given color at the given vertex.
 * Arguments: color, vertex
 * Fails:     invalid vertex, illegal move
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_play(char *s) {
    int i, j;
    int color;

    if (!gtp_decode_move(s, &color, &i, &j))
        return gtp_failure("invalid color or coordinate");

    if (!is_allowed_move(POS(i, j), color))
        return gtp_failure("illegal move");

    gnugo_play_move(POS(i, j), color);
    return gtp_success("");
}


/* Function:  Put free placement handicap stones on the board.
 * Arguments: list of vertices with handicap stones
 * Fails:     board not empty, bad list of vertices
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_set_free_handicap(char *s) {
    int n;
    int i, j;
    int k;

    if (stones_on_board(BLACK | WHITE) > 0)
        return gtp_failure("board not empty");

    for (k = 0; k < MAX_BOARD * MAX_BOARD; k++) {
        n = gtp_decode_coord(s, &i, &j);
        if (n > 0) {
            if (board[POS(i, j)] != EMPTY) {
                clear_board();
                return gtp_failure("repeated vertex");
            }
            add_stone(POS(i, j), BLACK);
            s += n;
        } else if (sscanf(s, "%*s") != EOF)
            return gtp_failure("invalid coordinate");
        else
            break;
    }

    if (k < 2) {
        clear_board();
        return gtp_failure("invalid handicap");
    }

    handicap = k;

    return gtp_success("");
}


/********************
 * generating moves *
 ********************/

/* Function:  Generate and play the supposedly best black move.
 * Arguments: none
 * Fails:     never
 * Returns:   a move coordinate or "PASS"
 *
 * Status:    Obsolete GTP version 1 command.
 */
static int gtp_genmove_black(char *s) {
    int move;
    UNUSED(s);

    if (stackp > 0)
        return gtp_failure("genmove cannot be called when stackp > 0");

    move = genmove(BLACK, NULL, NULL);

    gnugo_play_move(move, BLACK);

    gtp_start_response(GTP_SUCCESS);
    gtp_print_vertex(I(move), J(move));
    return gtp_finish_response();
}


/* Function:  Generate and play the supposedly best white move.
 * Arguments: none
 * Fails:     never
 * Returns:   a move coordinate or "PASS"
 *
 * Status:    Obsolete GTP version 1 command.
 */
static int gtp_genmove_white(char *s) {
    int move;
    UNUSED(s);

    if (stackp > 0)
        return gtp_failure("genmove cannot be called when stackp > 0");

    move = genmove(WHITE, NULL, NULL);

    gnugo_play_move(move, WHITE);

    gtp_start_response(GTP_SUCCESS);
    gtp_print_vertex(I(move), J(move));
    return gtp_finish_response();
}


/* Function:  Generate and play the supposedly best move for either color.
 * Arguments: color to move
 * Fails:     invalid color
 * Returns:   a move coordinate or "PASS" (or "resign" if resignation_allowed)
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_genmove(char *s) {
    int move;
    int resign;
    int color;
    int n;

    n = gtp_decode_color(s, &color);
    if (!n)
        return gtp_failure("invalid color");

    if (stackp > 0)
        return gtp_failure("genmove cannot be called when stackp > 0");

    move = genmove(color, NULL, &resign);

    if (resign)
        return gtp_success("resign");

    gnugo_play_move(move, color);

    gtp_start_response(GTP_SUCCESS);
    gtp_print_vertex(I(move), J(move));
    return gtp_finish_response();
}


/* Function:  Undo one move
 * Arguments: none
 * Fails:     If move history is too short.
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_undo(char *s) {
    UNUSED(s);

    if (stackp > 0 || !undo_move(1))
        return gtp_failure("cannot undo");

    reset_engine();

    return gtp_success("");
}


/* Function:  Undo a number of moves
 * Arguments: optional int
 * Fails:     If move history is too short.
 * Returns:   nothing
 */
static int gtp_gg_undo(char *s) {
    int number_moves = 1;

    sscanf(s, "%d", &number_moves);

    if (number_moves < 0)
        return gtp_failure("can't undo a negative number of moves");

    if (stackp > 0 || !undo_move(number_moves))
        return gtp_failure("cannot undo");

    reset_engine();

    return gtp_success("");
}


/*****************
 * time handling *
 *****************/

/* Function:  Set time allowance
 * Arguments: int main_time, int byo_yomi_time, int byo_yomi_stones
 * Fails:     syntax error
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_time_settings(char *s) {
    int main_time, byoyomi_time, byoyomi_stones;

    if (sscanf(s, "%d %d %d", &main_time, &byoyomi_time, &byoyomi_stones) < 3)
        return gtp_failure("not three integers");

    clock_settings(main_time, byoyomi_time, byoyomi_stones);
    return gtp_success("");
}


/* Function:  Report remaining time
 * Arguments: color color, int time, int stones
 * Fails:     syntax error
 * Returns:   nothing
 *
 * Status:    GTP version 2 standard command.
 */
static int gtp_time_left(char *s) {
    int color;
    int time;
    int stones;
    int n;

    n = gtp_decode_color(s, &color);
    if (!n)
        return gtp_failure("invalid color");

    if (sscanf(s + n, "%d %d", &time, &stones) < 2)
        return gtp_failure("time and stones not two integers");

    update_time_left(color, time, stones);

    return gtp_success("");
}


/*************
 * transform *
 *************/

static void rotate_on_input(int ai, int aj, int *bi, int *bj) {
    rotate(ai, aj, bi, bj, board_size, gtp_orientation);
}

static void rotate_on_output(int ai, int aj, int *bi, int *bj) {
    inv_rotate(ai, aj, bi, bj, board_size, gtp_orientation);
}


/**********************
 * imported functions *
 **********************/

/* Check whether we can accept a certain boardsize. Set out to NULL to
 * suppress informative messages. Return 1 for an acceptable
 * boardsize, 0 otherwise.
 */
int check_boardsize(int boardsize, FILE *out) {
    if (boardsize < MIN_BOARD || boardsize > MAX_BOARD) {
        if (out) {
            fprintf(out, "Unsupported board size: %d. ", boardsize);
            if (boardsize < MIN_BOARD)
                fprintf(out, "Min size is %d.\n", MIN_BOARD);
            else {
                fprintf(out, "Max size is %d.\n", MAX_BOARD);
            }
        }
        return 0;
    }
    return 1;
}


/* Play a move and start the clock */
void gnugo_play_move(int move, int color) {
    play_move(move, color);
    clock_push_button(color);
}

/*
 * Generate computer move for color.
 *
 * Return the generated move.
 */
int genmove(int color, float *value, int *resign) {
    int move = PASS_MOVE;
    if (resign)
        *resign = 0;

    move = play(color);

    gg_assert(move == PASS_MOVE || ON_BOARD(move));
    return move;
}
