/*

 * simlines.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/18/2020
 * Assignment:   Filesnpix (Files, Pictures and Interfaces)

 * Program that takes in files as input and detects and prints the 
 * similar lines between them to stdout

*/

#include <stdlib.h>
#include <stdint.h>
#include "assert.h"
#include "table.h"

#include "readaline.h"
#include "inputHandler.h"
#include "displayHandler.h"



#define TABLE_SIZE 12000 /* guess at number of entries in the table */



int main(int argc, char **argv)
{       
        Table_T table = Table_new(TABLE_SIZE, NULL, NULL);

        /* loop through all the lines in each file and populate the table */
        for (int i = 1; i < argc; i++) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);

                char *line = NULL; 
                size_t bytes_read = 0;
                uint64_t line_num = 1;

                bytes_read = readaline(fp, &line);
                while (bytes_read != 0) {
                        populateTable(line, line_num, argv[i], (void *)table, 
                                                                  bytes_read);
                        bytes_read = readaline(fp, &line);
                        line_num++;
                }
     
                assert(fclose(fp) == 0); /* detect errors while closing file */
        }

        /* display the match groups in the format specified by the spec */
        displayMatches(table);

        Table_free(&table);
        return EXIT_SUCCESS;
}