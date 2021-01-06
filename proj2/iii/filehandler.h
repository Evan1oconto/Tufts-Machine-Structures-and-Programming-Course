/*

 * filehandler.h
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/28/2020
 * Assignment:   iii 

 * Interface for filehandler that handles the opening and closing of file 
 * stream.

*/


#ifndef FILEHANDLER_INCLUDED
#define FILEHANDLER_INCLUDED


/*
function getFileStream
Arguments: char *filename - the name of the file supplied as the input
Returns: FILE * - pointer to FILE after opening the supplied file for 
         reading
Does: Opens the file for reading if the file was supplied from command line.
      Otherwise, returns stdin. Terminates the program if there was an error
      opening the file.
*/
FILE *getFileStream(char *filename);





/*
function closeFileStream
Arguments: FILE *fp - a pointer to FILE that is opened for reading
Returns: Nothing
Does: Closes the opened file stream if the data was read from a file
      supplied from the command line. Terminates the program if there was 
      an error closing the file.
Note: It is expected that fp is non-null and points to either an opened file
      or stdin.
*/
void closeFileStream(FILE *fp);

#endif