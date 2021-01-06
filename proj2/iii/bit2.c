/*

 * bit2.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/28/2020
 * Assignment:   iii

 * Implementation of 2-dimensional unboxed bit array.

*/



#include <stdlib.h>
#include "bit2.h"
#include "bit.h"
#include "mem.h"
#include "assert.h"


#define T Bit2_T


struct T {
        int width; /* the width of the Bit2 */
        int height; /* the height of the Bit2 */

        Bit_T array; /* single dimensional Bit vector to store the 
                        bits  */
};



/* Bit2_new:     Allocates and returns a new Bit2 with specified width, 
                 height.
*  Expectations: It is expected that all the parameters are nonnegative. 
                 The output is the newly
*                allocated Bit2 instance.
*/
T Bit2_new (int width, int height)
{
        assert(width >= 0 && height >= 0);

        T bit2;

        NEW(bit2);
        assert(bit2 != NULL);

        bit2->width = width;
        bit2->height = height;

        int array_length = bit2->width * bit2->height;
        bit2->array = Bit_new(array_length);  

        return bit2;
}


/* Bit2_free:    Deallocates and clears the *Bit2 (the first parameter)
*  Expectations: The parameter (Bit2) is non-null. Likewise, it is 
*                expected that *Bit2 is non-null. The function does not 
                 return anything as the output.
*/
void Bit2_free(T *Bit2)
{
        assert(Bit2 != NULL && *Bit2 != NULL);

        Bit_free(&((*Bit2)->array));
        FREE(*Bit2);
}



/* Bit2_width:    Returns the width of the supplied Bit2 
                  (the first parameter).
*  Expectations:  The parameter (Bit2) is non-null. The output is an integer
*                 representing the width of the supplied parameter.
*/
int Bit2_width(T Bit2)
{
        assert(Bit2 != NULL);
        return Bit2->width;
}

    
/* Bit2_height:    Returns the height of the supplied Bit2 
                   (the first parameter).
*  Expectations:   The parameter (Bit2) is non-null. The output is an 
                   integer representing the height of the supplied parameter.
*/
int Bit2_height(T Bit2)
{
        assert(Bit2 != NULL);
        return Bit2->height;
}





/* Bit2_put:     Store the bit (the fourth parameter) in the Bit2 array
                 (the first parameter)
                 in the col and row specified (third and fourth parameters
                 respectively)
*  Expectations: The parameter (Bit2) is non-null. The second and third 
                 parameters (col and row) are expected to be within valid 
                 range. There is no output.
*/
int Bit2_put(T Bit2, int col, int row, int bit)
{
        assert(Bit2 != NULL);
        assert(col >= 0 && col < Bit2_width(Bit2));
        assert(row >= 0 && row < Bit2_height(Bit2));


        int width = Bit2_width(Bit2);
        int position = (width * row) + col; 

        return Bit_put(Bit2->array, position, bit);
}


/* Bit2_get:     Returns the integer to the element stored in the 
                 supplied Bit2 (the first parameter) at the position 
                 supplied with col and row (the second and third parameters).
*  Expectations: The parameter (Bit2) is non-null. The second and third 
                 parameters (col and row) are expected to be within valid 
                 range. There is no output.
*/
int Bit2_get(T Bit2, int col, int row)
{
        assert(Bit2 != NULL);
        assert(col >= 0 && col < Bit2_width(Bit2));
        assert(row >= 0 && row < Bit2_height(Bit2));

        int width = Bit2_width(Bit2);
        int position = (width * row) + col;

        return Bit_get(Bit2->array, position);
}






/* Bit2_map_row_major: Calls the apply function (the second parameter) on 
                          every elements stored  in Bit2 (the first 
                          parameter), traversing the array in row-major order.
*  Expectations:          The parameter (Bit2) is non-null, and we are 
                          expecting a non-null function pointer for the second 
                          parameter. The function does not return anything as 
                          the output.
*/
void Bit2_map_row_major(T Bit2, 
             void apply(int col, int row, T Bit2, int bit, void *cl), 
             void *cl)
{
        assert(Bit2 != NULL);
        assert(apply != NULL);

        int i = 0, j = 0;

        int col = Bit2_width(Bit2);
        int row = Bit2_height(Bit2);

        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                apply(j, i, Bit2, Bit2_get(Bit2, j, i), cl);
            }
        }
}


/* Bit2_map_col_major:    Calls the apply function (the second parameter) on 
                          every elements stored in Bit2 
                          (the first parameter), traversing the array in 
                          column-major order.
*  Expectations:          The parameter (Bit2) is non-null, and we 
                          are expecting a non-null function pointer for the 
                          second parameter. The function does not return 
                          anything as the output.
*/
void Bit2_map_col_major(T Bit2, 
             void apply(int col, int row, T Bit2, int bit, void *cl), 
             void *cl)
{
        assert(Bit2 != NULL);
        assert(apply != NULL);

        int i = 0, j = 0;

        int col = Bit2_width(Bit2);
        int row = Bit2_height(Bit2);

        for (i = 0; i < col; i++) {
            for (j = 0; j < row; j++) {
                apply(i, j, Bit2, Bit2_get(Bit2, i, j), cl);
            }
        } 
}




#undef T