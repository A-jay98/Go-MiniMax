#include "board.h"
#include "core.h"
#include "MiniMax.h"
#include <iostream>

/*
 * Use this function to reset your engine and prepare it for a new game
 */
void reset() {
    // TODO: Your code for resetting your engine goes here...
}

/*
 * Should return a move for the input color
 *
 * If your move is to, say, place a stone on position (i, j) of the board, return POS(i, j)
 * Otherwise you may return PASS_MOVE
 */
int play(int color) {
    int move = alpha_beta_search(color);
    // TODO: Your code for generating a move goes here...
    return move;
}
