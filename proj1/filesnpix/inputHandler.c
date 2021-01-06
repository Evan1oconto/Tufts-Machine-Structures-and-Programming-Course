/*

 * inputHandler.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/18/2020
 * Assignment:   Filesnpix (Files, Pictures and Interfaces)

 * Reads the line from the file and populates the table with the relevant 
 * key-value pairs.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "atom.h"
#include "seq.h"

#include "inputHandler.h"
#include "readaline.h"




/*--                   Function declarations                       --*/
static int isCharacter(char c);
static char *getKey(char *line, size_t bytes_read);
static char *getDelimiters(void);
static void freeMemory(char *line, char *delim, char *key);
static Filedata *getNewFileData(char *filename, uint64_t linenum);
void populateTable(char *line, uint64_t line_num, char *filename,
                        void *container, size_t bytes_read);
/*--                                                               --*/




/*
function populateTable
Arguments: char *line - line read from file
           uint64_t line_num - line number of the line in the file
           char *filename - the name of the file opened for reading
           void  *container - the container to store the key-value pair to 
           size_t bytes_read - the number of bytes read in line
Returns: Nothing
Does: populates the table with the relevant key-value pair where the keys and 
      values are constructed from the relevant information from the line
*/
void populateTable(char *line, uint64_t line_num, char *filename,
                        void *container, size_t bytes_read)
{
        Table_T table = (Table_T)container;
        char *formatted_words = getKey(line, bytes_read);
        if (formatted_words == NULL) {  /* do not insert empty key */
                return;
        }

        const char *key = Atom_string(formatted_words);
        free(formatted_words);

        Seq_T value = Table_get(table, key);
        Filedata *struct_p = getNewFileData(filename, line_num);

        if (value == NULL) { /* unique key found, add struct to sequence */
                value = Seq_seq(struct_p, NULL);
        } else { /* key already exists in table, add struct to back of queue */
                Seq_addhi(value, struct_p);
        }

        /* store the generated key-value pair in the table */
        Table_put(table, key, value);   
}




/*
function getNewFileData
Arguments: char *filename - the name of the file
           uint64_t line_num - line number in the file
Returns: Filedata * - pointer to dynamically allocated Filedata struct
                      containing filename and line_num
Does: Creates and populates the Filedata struct with the supplied arguments.
      Returns the pointer to this newly allocated struct.
*/
static Filedata *getNewFileData(char *filename, uint64_t line_num) 
{
        Filedata *value = malloc(sizeof(*value));
        assert(value != NULL);

        value->filename = filename;
        value->line_num = line_num;
       
        return value;
}




/*
function getKey
Arguments: char *line - the line to be parsed
           size_t bytes_read - the size of the line (in bytes)
Returns: char * - the formed key
Does: Parses the line, constructs a key with the words being separated by 
      spaces and returns the formed key.
*/
static char *getKey(char *line, size_t bytes_read) 
{
        /* check for empty string */
        if (line == NULL || bytes_read == 0) {
                return NULL;
        }

        /* get the separators to use to tokenize the line */
        char *delimiters = getDelimiters();

        /* allocate enough memory for the key to prevent buffer overflow */
        char *key = malloc(bytes_read + 32);
        assert(key != NULL);
        key[0] = '\0'; /* starting with empty string */

        /* extracting individual "words" from the line to construct the key */
        char *words = strtok(line, delimiters);
        while (words != NULL) {
                strcat(strcat(key, words), " "); 
                words = strtok(NULL, delimiters);                   
        }

        /* check if the key is empty */
        if (strlen(key) == 0) { 
               freeMemory(line, delimiters, key);
               return NULL;
        }

        key[strlen(key) - 1] = '\0'; /* replace last space with \0 */

        freeMemory(line, delimiters, NULL);
        return key;
}




/*
function freeMemory
Arguments: char *line - the line
           char *delim - array of delimiters
           char *key - the formed key
Returns: Nothing
Does: Frees the dynamically allocated memory.
*/
static void freeMemory(char *line, char *delim, char *key) 
{
        free(line);
        free(delim);
        if (key != NULL) {
                free(key);
        }
}
        



/*
function isCharacter
Arguments: char c - the character to be checked
Returns: int - 1 if the supplied character is a valid character
               ( 1-9, A-Z, a-z and _ ) otherwise returns 0.
Does: Returns zero if the argument is not a valid word character as defined
      in the spec, otherwise returns 1.
*/
static int isCharacter(char c)
{
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
             (c >= '0' && c <= '9') || (c == 95)) {
                     return 1;
        }
        return 0;
}




/*
function getDelimiters
Arguments: None
Returns: char * - the array of delimiters/separators 
Does: Creates and returns array of delimiters (except the valid word 
      characters).
*/
static char *getDelimiters(void)
{
        char *delimiters = malloc(256);
        assert(delimiters != NULL);

        int index = 0;
        for (int i = 1; i <= 255; i++) {
                if (!isCharacter(i)) {
                        delimiters[index] = i;
                        index++;
                }
        }
        delimiters[index] = '\0';
        return delimiters;
}