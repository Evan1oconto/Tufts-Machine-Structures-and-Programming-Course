/*

 * uarray2.h
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/28/2020
 * Assignment:   iii

 * Interface of 2-dimensional polymorphic unboxed array.

*/


#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED


#define T UArray2_T 
typedef struct T *T;


/* UArray2_new:  Allocates and returns a new UArray2 with specified width, 
                 height, and size.
*  Expectations: It is expected that all three parameters are positive. 
                 The output is the newly
*                allocated UArray2 instance.
*/
extern T UArray2_new (int width, int height, int size);



/* UArray2_free: Deallocates and clears the *UArray2 (the first parameter)
*  Expectations: The parameter (UArray2) is non-null. Likewise, it is 
*                expected that *UArray2 is non-null. The function does not 
                 return anything as the output.
*/
extern void UArray2_free(T *UArray2);



/* UArray2_width: Returns the width of the supplied UArray2 
                                                        (the first parameter).
*  Expectations:  The parameter (UArray2) is non-null. The output is an integer
*                 representing the width of the supplied parameter.
*/
extern int UArray2_width(T UArray2);


/* UArray2_height: Returns the height of the supplied UArray2 
                   (the first parameter).
*  Expectations:   The parameter (UArray2) is non-null. The output is an 
                   integer representing the height of the supplied parameter.
*/
extern int UArray2_height(T UArray2);



/* UArray2_size: Returns the size of each element stored in the supplied 
                 UArray2 (the first parameter).
*  Expectations: The parameter (UArray2) is non-null. The output is an integer 
                 representing the
*                size of each element stored in the supplied parameter.
*/
extern int UArray2_size (T UArray2);





/* UArray2_at:   Returns the pointer to the element stored in the 
                supplied UArray2 (the first parameter) at the position 
                supplied with col and row (the second and third parameters).
*  Expectations: The parameter (UArray2) is non-null. The second and third 
                 parameters (col and row) are expected to be within valid 
                 range. The output is a pointer to the element retrieved
                 at the given position in the supplied UArray2.
*/
extern void *UArray2_at(T UArray2, int col, int row);






/* UArray2_map_row_major: Calls the apply function (the second parameter) on 
                          every elements stored  in UArray2 (the first 
                          parameter), traversing the array in row-major order.
*  Expectations:          The parameter (UArray2) is non-null, and we are 
                          expecting a non-null function pointer for the second 
                          parameter. The function does not return anything as 
                          the output.
*/
extern void UArray2_map_row_major(T UArray2, 
        void apply(int col, int row, T UArray2, void *value, void *cl), 
        void *cl);





/* UArray2_map_col_major: Calls the apply function (the second parameter) on 
                          every elements stored in UArray2 
                          (the first parameter), traversing the array in 
                          column-major order.
*  Expectations:          The parameter (UArray2) is non-null, and we 
                          are expecting a non-null function pointer for the 
                          second parameter. The function does not return 
                          anything as the output.
*/
extern void UArray2_map_col_major(T UArray2, 
        void apply(int col, int row, T UArray2, void *value, void *cl), 
        void *cl);




#undef T
#endif