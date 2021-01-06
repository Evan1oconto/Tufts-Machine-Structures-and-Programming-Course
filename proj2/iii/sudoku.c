/*

 * sudoku.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/28/2020
 * Assignment:   iii

 * Checks to see if supplied sudoku (in pgm format) represents a 
 * valid and solved sudoku puzzle.

*/

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "except.h"
#include "uarray2.h"
#include "uarray.h"
#include "pnmrdr.h"
#include "filehandler.h"


#define WIDTH  9
#define HEIGHT 9


/*--    Function declarations     --*/
void checkImageProperties(Pnmrdr_T reader);
UArray2_T populateSudoku(Pnmrdr_T reader); 
void checkSudoku(UArray2_T sudoku);
void checkSudokuLine(int i, int j, UArray2_T UArray2, void *value, void *cl);
void storeInArray(int i, int j, UArray2_T UArray2, void *value, void *cl);
int getArrayLength(UArray_T array);
void checkSudokuBox(UArray2_T sudoku, UArray_T *visited);
/*--                              --*/


static FILE *fp = NULL;



int main(int argc, char **argv) 
{
        /* validating the number of command line arguments supplied */
        assert(argc <= 2);
        if (argc == 2) {
                fp = getFileStream(argv[1]);
        } else {
                fp = getFileStream(NULL);
        }

        Pnmrdr_T reader;
        UArray2_T sudoku;
  
        /* read the image data and populate the array with the intensity */
        reader = Pnmrdr_new(fp);
        checkImageProperties(reader);
        sudoku = populateSudoku(reader);

        closeFileStream(fp);    
        Pnmrdr_free(&reader);

        /* check if the pgm represents a solved sudoku */
        checkSudoku(sudoku);

        /* free the array and return 0 if sudoku is solved */
        UArray2_free(&sudoku);
        return 0;
}




/* checkSudoku:  Checks the sudoku by calling respective functions that checks
                 each column, row, and box to make sure the sudoku is valid.

*  Expectations: The parameter (sudoku) is non-null. The function does not 
                 return anything.
*/
void checkSudoku(UArray2_T sudoku)
{
        assert(sudoku != NULL);

        /* an array to keep track of duplicates */
        UArray_T visited = UArray_new(9, sizeof(int));

        checkSudokuBox(sudoku, &visited);
        UArray2_map_col_major(sudoku, &checkSudokuLine, &visited);
        UArray2_map_row_major(sudoku, &checkSudokuLine, &visited);

        UArray_free(&visited);
}





/* checkSudokuBox:  Checks each 3x3 box in the sudoku for duplicates.
*  Expectations:    The first parameter (sudoku) is non-null, and visited 
                    and *visited are non-null. The function does not return
                    anything.                       
*/
void checkSudokuBox(UArray2_T sudoku, UArray_T *visited)
{
        assert(sudoku != NULL && visited != NULL && *visited != NULL);

        for (int num_col = 0; num_col < (WIDTH / 3); num_col++) {
                for (int num_row = 0; num_row < (HEIGHT / 3); num_row++) {
                        for (int x = num_col * (WIDTH / 3); 
                                 x < ((WIDTH / 3) * (num_col + 1)); 
                                 x++) 
                        {
                                for (int y = num_row * (HEIGHT / 3);
                                         y < ((HEIGHT / 3) * (num_row + 1));
                                         y++) 
                                {
                                        checkSudokuLine(x, y, sudoku, 
                                                UArray2_at(sudoku, x, y), 
                                                        (void *)visited);
                                }
                        }
                }
        }
 
       
}



/* getArrayLength:  Returns the length of a given UArray object excluding 
                    zeros.
*  Expectations:    The first parameter (array) is non-null, and assumed to be
                    of length 9.                       
*/
int getArrayLength(UArray_T array)
{
        assert(array != NULL);
        assert(UArray_length(array) == 9);

        int length = 0;
        for (int i = 0; i < UArray_length(array); i++) {
                if ((*(int *)UArray_at(array, i)) != 0) {
                        length++;
                }
        }

        return length;
}





/* checkSudokuLine:  Checks each line in the supplied Uarray2 to see whether 
                     the pixel intensities represent a valid sudoku line.
*  Expectations:    The first two parameters are optional. UArray2 is 
                    expected to be non-null. The last two parameters are 
                    allowed to be null.       
*/
void checkSudokuLine(int col, int row, UArray2_T UArray2, void *value,
                     void *cl)
{
        (void) col; /* ignore compiler warnings */
        (void) row;
        assert(UArray2 != NULL);
        
        UArray_T *visited = cl;
        int *intensity = value;

        /* for a new row or a new column, clear the array and 
                                                    start with empty array */
        if (getArrayLength(*visited) == 9) {
                for (int i = 0; i < UArray_length(*visited); i++) {
                        (*(int *)UArray_at(*visited, i)) = 0;
                }
        }

        /* check if duplicate exists */
        if ((*(int *)UArray_at(*visited, *intensity - 1)) != 0) { 
                UArray_free(&(*visited));
                UArray2_free(&UArray2);
                exit(1);
        } else { /* store the value in the set */
                *(int *)UArray_at(*visited, *intensity - 1) = *intensity;
        }

}




/* populateSudoku:  Creates and populates a UArray2 based on the Pnm file.
*  Expectations:    reader (the first parameter) is expected to be non-null.
*/
UArray2_T populateSudoku(Pnmrdr_T reader)
{
        assert(reader != NULL);
        
        UArray2_T array = UArray2_new(WIDTH, HEIGHT, sizeof(int));
        UArray2_map_row_major(array, &storeInArray, &reader);

        return array;
}





/* storeInArray:    Stores the pixel intensities supplied by 5th parameter
                    in UArray2 (the third parameter). 
*  Expectations:    It is expected that the third paramter (UArray2) is 
                    non-null.
*/
void storeInArray(int i, int j, UArray2_T UArray2, void *value, void *cl)
{
        (void) value;
        assert(UArray2 != NULL);
        
        Pnmrdr_T *reader = cl;
        int intensity = Pnmrdr_get(*reader);

        if (intensity < 1 || intensity > 9) {
                closeFileStream(fp);
                UArray2_free(&UArray2);
                TRY
                        Pnmrdr_free(&(*reader));
                EXCEPT (Pnmrdr_Count)
                        exit(1); /* catch to avoid memory leaks */
                END_TRY;
                exit(1);
        }
        
        /* store the intensity in the UArray2 */
        *((int *)UArray2_at(UArray2, i, j)) = intensity;
}




/* checkImageProperties: Checks to make sure the Pnm file is valid
*  Expectations:         The reader is not null, and the file is a portable 
                         graymap file.
*/
void checkImageProperties(Pnmrdr_T reader)
{
        assert(reader != NULL);
        
        Pnmrdr_mapdata map_data = Pnmrdr_data(reader);
        assert(map_data.type == Pnmrdr_gray); /* Ensuring that the file is 
                                                  Portable Graymap */

        if (map_data.denominator != 9) {
                closeFileStream(fp);
                Pnmrdr_free(&reader);
                exit(1);
        } else if (map_data.width != WIDTH || map_data.height != HEIGHT) {
                closeFileStream(fp);
                Pnmrdr_free(&reader);
                exit(1);
        }
}

