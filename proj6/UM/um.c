/*

 * um.c
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         11/20/2020
 * Assignment:   A Universal Machine

 * Implementation um module where main is held and the machine is run.

*/

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "Handler.h"
#include "execute.h"


int main(int argc, char *argv[])
{
        /* checks that there are no contract violations */
        if (argc < 2) {
                fprintf(stderr, "ERROR: Not enough arguments\n");
                exit(EXIT_FAILURE);
        }

        if (argc > 2) {
                fprintf(stderr, "ERROR: Too many arguments\n");
                exit(EXIT_FAILURE);
        }

        /* open file */
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL) {
                fprintf(stderr, "ERROR: Could not open file %s for reading\n",
                        argv[1]);
                exit(EXIT_FAILURE);
        }

        Seg_handler program = initialize(fp);

        fclose(fp);

        execute(program);

        exit(0);
}