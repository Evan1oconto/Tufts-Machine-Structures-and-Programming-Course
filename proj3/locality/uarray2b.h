/*

 * uarray2b.h
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         10/10/2020
 * Assignment:   Locality and the costs of loads and stores

 * Interface of 2-dimensional polymorphic unboxed blocked array.

*/


#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED



#define T UArray2b_T

typedef struct T *T;



/* UArray2b_new: Allocates and returns a new UArray2b with specified width, 
*                height, size, and blocksize.
*  Expectations: It is a checked runtime error for width, height, size or 
                 blocksize to be negative. In particular, size and blocksize 
                 must be a non-negative integer greater than 0.
*/
extern T UArray2b_new (int width, int height, int size, int blocksize);



/* UArray2b_new_64k_block: Allocates and returns a new UArray2b with specified 
*                          width, height, and size. The blocksize is defaulted
*                          which is as large as possible while still allowing
*                          a block to fit in 64 KB of RAM.
*  Expectations:           It is a checked runtime error for width, height, and
*                          size to be negative. In particular, size must be a 
*                          non-negative integer greater than 0.
*/
extern T UArray2b_new_64K_block(int width, int height, int size);





/* UArray2b_free: Deallocates and clears the *UArray2b (the first parameter)
*  Expectations: It is a checked runtime error for array2b and *array2b to be 
                 NULL.
*/
extern void UArray2b_free (T *array2b);




/* UArray2b_width: Returns the width of the supplied UArray2b 
                                                        (the first parameter).
*  Expectations:   It is a checked runtime error for array2b to be NULL.
*/
extern int UArray2b_width (T array2b);




/* UArray2b_height: Returns the height of the supplied UArray2b 
                                                        (the first parameter).
*  Expectations:   It is a checked runtime error for array2b to be NULL.
*/
extern int UArray2b_height (T array2b);






/* UArray2b_size: Returns the size of each element stored in the supplied 
                 UArray2 (the first parameter, array2b).
*  Expectations: It is a checked runtime error for array2b to be NULL. The 
                 output is the integer representing the size of each element
                 in array2b.
*/
extern int UArray2b_size (T array2b);





/* UArray2b_blocksize: Returns the blocksize of the supplied UArray2b 
                                                        (the first parameter).
*  Expectations:   It is a checked runtime error for array2b to be NULL.
                   The output is an integer representing the blocksize of 
                   array2b.
*/
extern int UArray2b_blocksize(T array2b);






/* UArray2b_at:   Returns the pointer to the element stored in the 
                supplied UArray2b (the first parameter, array2b) 
                at the position supplied with column and row.
*  Expectations: It is a checked runtime error for array2b to be NULL.
                 It is also a checked runtime error for column and row to be 
                 out of bounds.
                 The output is a pointer to the element retrieved
                 at the given position in the supplied UArray2.
*/
extern void *UArray2b_at(T array2b, int column, int row);





/* UArray2b_map: Calls the apply function (the second parameter) on 
                 every elements stored in UArray2b (the first parameter), 
                 traversing the array in block-major order.
*  Expectations: It is a checked runtime error for array2b and apply to be 
                 NULL. There is no output, but this mapping method is 
                 responsible for traversing the UArray2b in block-major 
                 order, i.e. it visits all the cells in one block before
                 moving to the other blocks.
*/
extern void UArray2b_map(T array2b,
                        void apply(int col, int row, T array2b,
                        void *elem, void *cl),
                        void *cl);


#undef T
#endif