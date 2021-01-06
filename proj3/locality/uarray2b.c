/*

 * uarray2b.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         10/10/2020
 * Assignment:   Locality and the costs of loads and stores

 * Implementation for 2-dimensional polymorphic unboxed blocked array.

*/

#include <stdlib.h>
#include "uarray2b.h"
#include "assert.h"
#include "mem.h"
#include "uarray2.h"
#include "uarray.h"
#include "math.h"



#define T UArray2b_T



struct T { /* a 2-dimensional array, whose elements are each of 'size' bytes */

        /* The unboxed 2 dimensional blocked array is represented internally 
        as a UArray2_T, each of whose element represents a "block"
        Each block is then represented by a single one dimensional UArray_T,
        thereby guaranteeing that cells in the same block are in 
        contiguous memory locations.  
         */


        int width; /* the width of the blocked 2d array */
        int height; /* the height of the blocked 2d array */
        int size; /* size of each element in the array */
        int blocksize; /* number of cells on one side of a block */
        
        UArray2_T array; /* a two-dimensional array of "blocks", where 
                            each block is a one-dimensional unboxed array */
};





/* UArray2b_new: Allocates and returns a new UArray2b with specified width, 
*                height, size, and blocksize.
*  Expectations: It is a checked runtime error for width, height, size or 
                 blocksize to be negative. In particular, size and blocksize 
                 must be a non-negative integer greater than 0.
*/
T UArray2b_new(int width, int height, int size, int blocksize)
{
        assert(blocksize > 0); 
        assert(size > 0);

        T array2b;
        NEW(array2b);

        array2b->width = width;
        array2b->height = height;
        array2b->blocksize = blocksize;
        array2b->size = size;

        int block_width = (width - 1 + blocksize) / blocksize;
        int block_height = (height - 1 + blocksize) / blocksize;

        array2b->array = UArray2_new(block_width, block_height, 
                                                             sizeof(UArray_T));
        
        int length = blocksize * blocksize;
        
        /* for each block in the array, create a 1D array of length 
           (blocksize * blocksize)  
        */
        for (int col = 0; col < block_width; col++) {
                for (int row = 0; row < block_height; row++) {
                        UArray_T block = UArray_new(length, size);
                        *((UArray_T *)UArray2_at(array2b->array, col, row)) = 
                                                                         block;
                }
        }
        return array2b;
}




/* UArray2b_new_64k_block: Allocates and returns a new UArray2b with specified 
*                          width, height, and size. The blocksize is defaulted
*                          which is as large as possible while still allowing
*                          a block to fit in 64 KB of RAM.
*  Expectations:           It is a checked runtime error for width, height, and
*                          size to be negative. In particular, size must be a 
*                          non-negative integer greater than 0.
*/
T UArray2b_new_64K_block(int width, int height, int size)
{
        int blocksize = 0;
        const int MAX_SIZE = 64 * 1024; /* 64 KB */
        
        if (size > MAX_SIZE) {
                blocksize = 1;
        } else { /* compute the maximum blocksize allowed */
                blocksize =  floor(sqrt(((double) MAX_SIZE / (double) size)));
        }
        
        return UArray2b_new(width, height, size, blocksize);
}





/* UArray2b_free: Deallocates and clears the *UArray2b (the first parameter)
*  Expectations: It is a checked runtime error for array2b and *array2b to be 
                 NULL.
*/
void UArray2b_free (T *array2b)
{
        assert(array2b != NULL && *array2b != NULL);

        int block_width  = UArray2_width((*array2b)->array);
        int block_height = UArray2_height((*array2b)->array);

        /* Free individual 1D UArray_T's followed by the UArray2_T */
        for (int col = 0; col < block_width; col++) {
                for (int row = 0; row < block_height; row++) {
                        UArray_free((UArray_T *)UArray2_at((*array2b)->array, 
                                                             col, row));
                }
        }
        
        UArray2_free(&((*array2b)->array));
        FREE(*array2b);
}




/* UArray2b_width: Returns the width of the supplied UArray2b 
                                                        (the first parameter).
*  Expectations:   It is a checked runtime error for array2b to be NULL.
*/
int UArray2b_width (T array2b)
{
        assert(array2b != NULL);
        return array2b->width;
}



/* UArray2b_height: Returns the height of the supplied UArray2b 
                                                        (the first parameter).
*  Expectations:   It is a checked runtime error for array2b to be NULL.
*/
int UArray2b_height (T array2b) {
        assert(array2b != NULL);
        return array2b->height;
}



/* UArray2b_size: Returns the size of each element stored in the supplied 
                 UArray2 (the first parameter, array2b).
*  Expectations: It is a checked runtime error for array2b to be NULL. The 
                 output is the integer representing the size of each element
                 in array2b.
*/
int UArray2b_size (T array2b)
{
        assert(array2b != NULL);
        return array2b->size;
}




/* UArray2b_blocksize: Returns the blocksize of the supplied UArray2b 
                                                        (the first parameter).
*  Expectations:   It is a checked runtime error for array2b to be NULL.
                   The output is an integer representing the blocksize of 
                   array2b.
*/
int UArray2b_blocksize(T array2b) {
        assert(array2b != NULL);
        return array2b->blocksize;
}



/* UArray2b_at:   Returns the pointer to the element stored in the 
                supplied UArray2b (the first parameter, array2b) 
                at the position supplied with column and row.
*  Expectations: It is a checked runtime error for array2b to be NULL.
                 It is also a checked runtime error for column and row to be 
                 out of bounds.
                 The output is a pointer to the element retrieved
                 at the given position in the supplied UArray2.
*/
void *UArray2b_at(T array2b, int col, int row) 
{
        assert(array2b != NULL);
        assert(col >= 0 && col < UArray2b_width(array2b));
        assert(row >= 0 && row < UArray2b_height(array2b));
        
        int blocksize = UArray2b_blocksize(array2b);
        
        int block_index_col = col / blocksize;
        int block_index_row = row / blocksize;
        
        int cell_index = (col % blocksize) * blocksize + (row % blocksize);

        /* get the block in the 2d array where the (col, row) resides */
        UArray_T block = *((UArray_T *)UArray2_at(array2b->array, 
                                                        block_index_col, 
                                                        block_index_row));

        /* extract the value in the index from that block (1d array) */
        void *value = UArray_at(block, cell_index);
        
        return value;
}




/* UArray2b_map: Calls the apply function (the second parameter) on 
                 every elements stored in UArray2b (the first parameter), 
                 traversing the array in block-major order.
*  Expectations: It is a checked runtime error for array2b and apply to be 
                 NULL. There is no output, but this mapping method is 
                 responsible for traversing the UArray2b in block-major 
                 order, i.e. it visits all the cells in one block before
                 moving to the other blocks.
*/
void UArray2b_map(T array2b, void apply(int col, int row, T array2b,
                             void *elem, void *cl), void *cl)
{
        assert(array2b != NULL);
        assert(apply != NULL);

        int blocksize = UArray2b_blocksize(array2b);
        int array_width = UArray2b_width(array2b);
        int array_height = UArray2b_height(array2b);
        
        int block_width  = UArray2_width(array2b->array);
        int block_height = UArray2_height(array2b->array);

        int block_length = blocksize * blocksize;

        for (int col = 0; col < block_width; col++) {
                for (int row = 0; row < block_height; row++) {
                        /* start at the corner of the current block */
                        int block_x = col * blocksize;
                        int block_y = row * blocksize;

                        /* go through every cell in the block */
                        for (int cell = 0; cell < block_length; cell++){ 
                                int col_index = block_x + (cell / blocksize);
                                int row_index = block_y + (cell % blocksize);

                        /* call apply() only when the cells are non-empty */
                                if (col_index < array_width && 
                                                   row_index < array_height) {
                                        apply(col_index, 
                                        row_index, 
                                        array2b, 
                                        UArray2b_at(array2b, col_index, 
                                                    row_index),
                                        cl);
                                }
                        }
                }
        }
}




#undef T