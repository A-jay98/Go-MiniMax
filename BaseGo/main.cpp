#include <stdio.h>
#include <stdlib.h>
#include "play_gtp.cpp"

char *gtpfile = NULL;
char *gtp_dump_commands_file = NULL;
int orientation = 0;  // [0, 7]

int main() {
    FILE *gtp_input_FILE = stdin;
    FILE *gtp_output_FILE = stdout;
    FILE *gtp_dump_commands_FILE = NULL;

    if (gtpfile != NULL) {
        gtp_input_FILE = fopen(gtpfile, "r");
        if (gtp_input_FILE == NULL) {
            fprintf(stderr, "Cannot open file %s\n", gtpfile);
            return EXIT_FAILURE;
        }
    }

    if (gtp_dump_commands_file != NULL) {
        gtp_dump_commands_FILE = fopen(gtp_dump_commands_file, "w");
        if (gtp_dump_commands_FILE == NULL) {
            fprintf(stderr, "Cannot open file %s\n",
                    gtp_dump_commands_file);
            return EXIT_FAILURE;
        }
    }

    play_gtp(gtp_input_FILE, gtp_output_FILE, gtp_dump_commands_FILE, orientation);

    if (gtp_dump_commands_FILE)
        fclose(gtp_dump_commands_FILE);

    return 0;
}
