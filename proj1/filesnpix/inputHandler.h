/*

 * inputHandler.h
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/18/2020
 * Assignment:   Filesnpix (Files, Pictures and Interfaces)

 * Header file containing function declarations for inputHandler.c. Helps 
 * to read in the lines of the files and populate the container

*/

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "table.h"
#include <stdint.h>



/* struct to store the data for each line that is read */
typedef struct Filedata {
        char *filename;
        uint64_t line_num;
} Filedata;



void populateTable(char *line, uint64_t line_num, char *filename,
                                        void *container, size_t bytes_read);




#endif