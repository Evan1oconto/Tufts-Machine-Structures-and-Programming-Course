/*

 * brightness.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/18/2020
 * Assignment:   Filesnpix (Files, Pictures and Interfaces)

 * Prints to stdout the average brightness of a Portable Graymap image file
 * supplied.

*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "except.h"
#include "assert.h"
#include "pnmrdr.h"



/*--    Function declarations     --*/
FILE *getFileStream(char *filename);
double getAverageBrightness(FILE *fp);
void closeFileStream(FILE *fp);
/*--                              --*/



int main (int argc, char *argv[]) 
{
        FILE *fp = NULL;
        double average_brightness = 0;

        /* validating the number of command line arguments supplied */
        assert(argc <= 2);
        if (argc == 2) {
                fp = getFileStream(argv[1]);
        } else {
                fp = getFileStream(NULL);
        }
        
        /* compute and display the average brightness of the pgm image */
       TRY
                average_brightness = getAverageBrightness(fp);
                printf("%5.3f\n", average_brightness);
        EXCEPT (Pnmrdr_Badformat)
                fprintf(stderr, "ERROR: File is not a pnm file\n");
                exit(EXIT_FAILURE);
        EXCEPT (Pnmrdr_Count)
                fprintf(stderr, "ERROR: Wrong number of pixels read\n");
                exit(EXIT_FAILURE);
        END_TRY;
        
        closeFileStream(fp);

        return EXIT_SUCCESS;
}



/*
function getFileStream
Arguments: char *filename - the name of the file supplied as the input
Returns: FILE * - pointer to FILE after opening the supplied file for 
         reading
Does: Opens the file for reading if the file was supplied from command line.
      Otherwise, returns stdin. Terminates the program if there was an error
      opening the file.
*/
FILE *getFileStream(char *filename) 
{
        FILE *fp = NULL;
        if (filename == NULL) {
                fp = stdin;
        } else {
                fp = fopen(filename, "rb");
                if (fp == NULL) {
                        fprintf(stderr, "Error opening file %s\n", filename);
                        exit(EXIT_FAILURE);
                }
        } 
        return fp;
}



/*
function getAverageBrightness
Arguments: FILE *fp - a pointer to FILE that is opened for reading
Returns: double - the computed average brightness of the supplied pgm file
Does: Sets up the Pnmrdr reader from the supplied pointer to file and 
      computes the average brightness of the provided pgm image.
*/
double getAverageBrightness(FILE *fp) 
{
        uint64_t num_pixels = 0, sum = 0, i = 0;
        uint64_t intensity = 0, denominator = 0;

        /* setting up Pnmrdr reader and getting the header data */
        Pnmrdr_T reader = Pnmrdr_new(fp);
        Pnmrdr_mapdata map_data = Pnmrdr_data(reader);
        assert(map_data.type == Pnmrdr_gray); /* Ensuring that the file is 
                                                     Portable Graymap */

        denominator = map_data.denominator;
        assert(denominator != 0); /* to prevent dividing by zero */

        num_pixels = map_data.width * map_data.height;
        assert(num_pixels > 0);
        
        for (i = 0; i < num_pixels; i++) {
                intensity = Pnmrdr_get(reader);
                assert(intensity <= denominator); /* intensity can't be 
                                                  greater than denominator */
                sum += intensity;
        }
        Pnmrdr_free(&reader); 

        return ((double) sum / (num_pixels * denominator));
}




/*
function closeFileStream
Arguments: FILE *fp - a pointer to FILE that is opened for reading
Returns: Nothing
Does: Closes the opened file stream if the data was read from a file
      supplied from the command line. Terminates the program if there was 
      an error closing the file.
*/
void closeFileStream(FILE *fp)
{
        if (fp != stdin) {
                if (fclose(fp) != 0) {
                        fprintf(stderr, "Error closing the file!\n");
                        exit(EXIT_FAILURE);
                }
        }
}

