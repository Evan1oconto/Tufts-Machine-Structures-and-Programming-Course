/*

 * pbmhandler.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/28/2020
 * Assignment:   iii 

 * Handles storing the image data into the portable bitmap. Also, writes the 
 * pbm in plain bitmap format to stdout.

*/

#include <stdio.h>
#include <stdlib.h>
#include "bit2.h"
#include "pnmrdr.h"
#include "assert.h"
#include "pbmhandler.h"


#define WHITE 0
#define BLACK 1


void storeInArray(int i, int j, Bit2_T image, int bit, void *cl);




/* pbmread:        Stores the bit in the reader (first parameter) to a 
                   Bit2 instance and returns the newly created Bit2 instance.
*  Expectations:   The reader is non-null.
*/
Bit2_T pbmread(Pnmrdr_T reader)
{
        assert(reader != NULL);

        Pnmrdr_mapdata map_data = Pnmrdr_data(reader); 
        Bit2_T image = Bit2_new(map_data.width, map_data.height);
        Bit2_map_row_major(image, &storeInArray, &reader);

        return image;
}




/* storeInArray:   Stores the bit at (col, row) position 
                   in the image (the third parameter) Bit2 instance. 
*  Expectations:   image is expected to be non-null.
*/
static void storeInArray(int col, int row, Bit2_T image, int bit, void *cl)
{
        assert(image != NULL);

        (void) bit;
        Pnmrdr_T *reader = cl;

        int intensity = Pnmrdr_get(*reader);
        assert(intensity == BLACK || intensity == WHITE); /* sanity check */
        
        /* store the intensity in the image */
        Bit2_put(image, col, row, intensity);
}






/* pbmwrite:       Prints to outputfp (the first parameter) the pbm image 
                   stored in bitmap (the second parameter)
*  Expectations:   outputfp is expected to be non-null and bitmap is expected
                   to be non-null. The function does not return anything.
*/
void pbmwrite(FILE *outputfp, Bit2_T bitmap)
{ 
        fprintf(outputfp, "P1\n");
        int col = Bit2_width(bitmap);
        int row = Bit2_height(bitmap);
        fprintf(outputfp, "# Image of width %d and height %d\n", col, row);
        fprintf(outputfp, "%d %d\n", col, row);
        for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                        if (j % 69 == 0 && j != 0) {
                                fprintf(outputfp, "\n");
                        }
                        fprintf(outputfp, "%d", Bit2_get(bitmap, j, i));
                }
                fprintf(outputfp, "\n");
        }
}



