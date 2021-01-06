/*

 * displayHandler.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/18/2020
 * Assignment:   Filesnpix (Files, Pictures and Interfaces)

 * Handles displaying and formatting of the match groups and outputs them
 * to stdout

*/

#include <stdio.h>
#include <stdlib.h>
#include "seq.h"
#include "inputHandler.h"
#include "displayHandler.h"



static void printGroups(const void *key, void **value, void *cl);


 /*
  function displayMatches
  Arguments: Table_T table - the container that contains match groups data
  Returns: nothing
  Does: Displays the formatted match groups output to stdout
 */
void displayMatches(Table_T table)
{
        int need_newline = 0; /* boolean variable to keep track of \n */
        Table_map(table, &printGroups, &need_newline);
}



 /*
  function printGroups
  Arguments: const void *key - key in the table
             void **value - address of the address of the value in the table 
             void *cl - closure for folding the data in the table
  Returns: nothing
  Does: Displays the formatted match groups output to stdout. Frees the memory
        associated with the values.
 */
static void printGroups(const void *key, void **value, void *cl) 
{        
        int *need_newline = (int *)cl;
        Seq_T *value_p = (Seq_T *)value;
        int seq_length = Seq_length(*value_p);

        if (seq_length > 1) { 
                if (!(*need_newline)) { /* first match group */
                        *need_newline = 1;
                } else { /* display newline before each matchgroup */
                        putchar('\n');      
                }

                printf("%s\n", (char *)key);
                /* loop through every struct in the sequence and print the 
                                                                match info */
                for (int i = 0; i < seq_length; i++) {
                        Filedata *filedata = Seq_get(*value_p, i);
                        printf("%-20s %7ld\n", filedata->filename, 
                                                        filedata->line_num);  
                }

                /* free each struct in the sequence */
                for (int i = 0; i < seq_length; i++) {
                        free(Seq_remlo(*value_p));   
                }

        } else {
                free(Seq_remlo(*value_p));        
        }

        /* free the entire sequence at last */
        Seq_free(&(*value_p));
}