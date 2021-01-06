/*

 * readaline.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/18/2020
 * Assignment:   Filesnpix (Files, Pictures and Interfaces)

 * Reads a line from the input file and returns the number of characters 
 * read

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "assert.h"
#include "readaline.h"



/*--        Function declarations        --*/
size_t readaline(FILE *inputfd, char **datapp);
static int isEndOfFile(char c, FILE *inputfd);
static char *storeCharacter(char *line, 
                       size_t position, 
                       char character, 
                       uint64_t *line_size);
/*--                                     --*/



/*
function readaline
Arguments: FILE *inputfd - pointer to file opened for reading
           char **datapp - address of address of the location where we 
                     want to store the line after reading from the file
Returns: an unsigned integer representing the number of bytes read
Does: Reads each line of the file pointed to by inputfd and stores the 
      line raed in *datapp. Returns the number of characters read.
      Terminates the program if either or both of the supplied arguments 
      is NULL.
*/
size_t readaline(FILE *inputfd, char **datapp) 
{
        assert(inputfd != NULL && datapp != NULL);
        assert(!ferror(inputfd));

        char *line = NULL, character_read;
        size_t bytes_read = 0, position = 0;
        uint64_t line_size = 200;

        character_read = fgetc(inputfd);
        if (isEndOfFile(character_read, inputfd)) { /* return 0 if EOF */
                *datapp = NULL;
                return 0;
        }

        line = malloc(line_size * sizeof(*line));
        assert(line != NULL);

        /* store the character in the line repeatedly until \n or EOF seen */
        while (character_read != '\n' && character_read != EOF) {
                line = storeCharacter(line, position, 
                                      character_read, &line_size);
                position += 1;
                character_read = fgetc(inputfd);
        }

        if (isEndOfFile(character_read, inputfd)) {
                bytes_read = position;             
        } else {
                /* New-line at the end so include \n in our line */
                bytes_read = position + 1;
                line = storeCharacter(line, position, '\n', &line_size);
        }
        /* NUL-terminate the string after everything is read */
        line = storeCharacter(line, bytes_read, '\0', &line_size);
        *datapp = line; 

        return bytes_read;
}


/*
function isEndOfFile
Arguments: char c - the character read from file pointed to by inputfd
           FILE *inputfd - file opened for reading
Returns: an integer - 1 if c is end of file, otherwise returns zero.
Does: Checks if c is EOF or not. Returns zero if c is not EOF, else returns
      1. Terminates the program if there is an error reading from file.
*/
static int isEndOfFile(char c, FILE *inputfd)
{
        if (c == EOF) {
                assert(!ferror(inputfd)); /* ensure error not encountered */
                return 1;
        }
        return 0;
}




/*
function storeCharacter
Arguments: char *line - the line to store the character in
           size_t position - position in the line to store the character
           char character - character to be stored in line
           uint64_t *line_size - pointer to unsigned long denoting the line
                                 length
Returns: char *line - the line after inserting the required character
Does: Stores character in line at given position and returns the updated 
      line. If needed, expands the line and adjusts the line_size.
*/
static char *storeCharacter(char *line, size_t position, char character, 
                                                        uint64_t *line_size)
{
        /* expand the array if needed, then insert the character in 
                                                 the correct position     */
        if (position == *line_size) {
                uint64_t new_size = *line_size * 2 + 2;
                *line_size = new_size;

                char *temp = realloc(line, new_size);
                assert(temp != NULL);
                
                line = temp;
        }

        line[position] = character;

        return line;
}

