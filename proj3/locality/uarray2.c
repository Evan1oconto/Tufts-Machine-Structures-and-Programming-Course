/*

 * uarray2.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/28/2020
 * Assignment:   iii

 * Implementation of 2-dimensional polymorphic unboxed array.

*/


#include <stdlib.h>
#include "uarray2.h"
#include "uarray.h"
#include "mem.h"
#include "assert.h"

 
#define T UArray2_T


struct T {
        int width, height; /* the width and height of the UArray2 */
        int size; /* size of each element stored in the UArray2 */

        UArray_T array; /* the single dimensional Uarray to store the 
                            elements */
};




/* UArray2_new:  Allocates and returns a new UArray2 with specified width, 
                 height, and size.
*  Expectations: It is expected that all three parameters are positive. 
                 The output is the newly
*                allocated UArray2 instance.
*/
T UArray2_new(int width, int height, int size)
{
        assert(width >= 0 && height >= 0); 
        assert(size > 0);

        T array2;

        NEW(array2);
        assert(array2 != NULL);

        array2->width = width;
        array2->height = height;
        array2->size = size;

        int array_length = array2->width * array2->height;
        array2->array = UArray_new(array_length, size);  

        return array2;
}




/* UArray2_free: Deallocates and clears the *UArray2 (the first parameter)
*  Expectations: The parameter (UArray2) is non-null. Likewise, it is 
*                expected that *UArray2 is non-null. The function does not 
                 return anything as the output.
*/
void UArray2_free(T *UArray2)
{
        assert(UArray2 != NULL && *UArray2 != NULL);

        UArray_free(&((*UArray2)->array));
        FREE(*UArray2);
}




/* UArray2_width: Returns the width of the supplied UArray2 
                  (the first parameter).
*  Expectations:  The parameter (UArray2) is non-null. The output is an integer
*                 representing the width of the supplied parameter.
*/
int UArray2_width(T UArray2)
{
        assert(UArray2 != NULL);
        return UArray2->width;
}





/* UArray2_height: Returns the height of the supplied UArray2 
                   (the first parameter).
*  Expectations:   The parameter (UArray2) is non-null. The output is an 
                   integer representing the height of the supplied parameter.
*/
int UArray2_height(T UArray2)
{
        assert(UArray2 != NULL);
        return UArray2->height;
}




/* UArray2_size: Returns the size of each element stored in the supplied 
                 UArray2 (the first parameter).
*  Expectations: The parameter (UArray2) is non-null. The output is an integer 
                 representing the
*                size of each element stored in the supplied parameter.
*/
int UArray2_size(T UArray2)
{
        assert(UArray2 != NULL);
        return UArray2->size;
}





/* UArray2_at:   Returns the pointer to the element stored in the 
                 supplied UArray2 (the first parameter) at the position 
                 supplied with col and row (the second and third parameters).
*  Expectations: The parameter (UArray2) is non-null. The second and third 
                 parameters (col and row) are expected to be within valid 
                 range. The output is a pointer to the element retrieved
                 at the given position in the supplied UArray2.
*/
void *UArray2_at(T UArray2, int col, int row)
{
        assert(UArray2 != NULL);
        assert(col >= 0 && col < UArray2_width(UArray2));
        assert(row >= 0 && row < UArray2_height(UArray2));

        int width = UArray2_width(UArray2);
        int position = (width * row) + col; 

        return UArray_at(UArray2->array, position);
}






/* UArray2_map_row_major: Calls the apply function (the second parameter) on 
                          every elements stored  in UArray2 (the first 
                          parameter), traversing the array in row-major order.
*  Expectations:          The parameter (UArray2) is non-null, and we are 
                          expecting a non-null function pointer for the second 
                          parameter. The function does not return anything as 
                          the output.
*/
void UArray2_map_row_major(T UArray2, 
        void apply(int col, int row, T UArray2, void *value, void *cl), 
        void *cl)
{
        assert(UArray2 != NULL);
        assert(apply != NULL);

        int i = 0, j = 0;

        int col = UArray2_width(UArray2);
        int row = UArray2_height(UArray2);

        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                apply(j, i, UArray2, UArray2_at(UArray2, j, i), cl);
            }
        }
}





/* UArray2_map_col_major: Calls the apply function (the second parameter) on 
                          every elements stored in UArray2 
                          (the first parameter), traversing the array in 
                          column-major order.
*  Expectations:          The parameter (UArray2) is non-null, and we 
                          are expecting a non-null function pointer for the 
                          second parameter. The function does not return 
                          anything as the output.
*/
void UArray2_map_col_major(T UArray2, 
        void apply(int col, int row, T UArray2, void *value, void *cl), 
        void *cl)
{
        assert(UArray2 != NULL);
        assert(apply != NULL);

        int i = 0, j = 0;

        int col = UArray2_width(UArray2);
        int row = UArray2_height(UArray2);

        for (i = 0; i < col; i++) {
            for (j = 0; j < row; j++) {
                apply(i, j, UArray2, UArray2_at(UArray2, i, j), cl);
            }
        } 
}



#undef T