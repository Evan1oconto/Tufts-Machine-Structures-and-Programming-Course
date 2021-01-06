/*

 * bit2.h
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/28/2020
 * Assignment:   iii

 * Interface for 2-dimensional unboxed bit array.

*/


#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED


#define T Bit2_T 
typedef struct T *T;


/* Bit2_new:     Allocates and returns a new Bit2 with specified width, 
                 height.
*  Expectations: It is expected that all the parameters are nonnegative. 
                 The output is the newly
*                allocated Bit2 instance.
*/
extern T Bit2_new (int width, int height);




/* Bit2_free:    Deallocates and clears the *Bit2 (the first parameter)
*  Expectations: The parameter (Bit2) is non-null. Likewise, it is 
*                expected that *Bit2 is non-null. The function does not 
                 return anything as the output.
*/
extern void Bit2_free(T *Bit2);



/* Bit2_width:    Returns the width of the supplied Bit2 
                  (the first parameter).
*  Expectations:  The parameter (Bit2) is non-null. The output is an integer
*                 representing the width of the supplied parameter.
*/
extern int Bit2_width(T Bit2);



/* Bit2_height:    Returns the height of the supplied Bit2 
                   (the first parameter).
*  Expectations:   The parameter (Bit2) is non-null. The output is an 
                   integer representing the height of the supplied parameter.
*/
extern int Bit2_height(T Bit2);



/* Bit2_put:     Store the bit (the fourth parameter) in the Bit2 array
                 (the first parameter)
                 in the col and row specified (third and fourth parameters
                 respectively)
*  Expectations: The parameter (Bit2) is non-null. The second and third 
                 parameters (col and row) are expected to be within valid 
                 range. There is no output.
*/
extern int Bit2_put(T Bit2, int col, int row, int bit);



/* Bit2_get:     Returns the integer to the element stored in the 
                 supplied Bit2 (the first parameter) at the position 
                 supplied with col and row (the second and third parameters).
*  Expectations: The parameter (Bit2) is non-null. The second and third 
                 parameters (col and row) are expected to be within valid 
                 range. There is no output.
*/
extern int Bit2_get(T Bit2, int col, int row);




/* Bit2_map_row_major: Calls the apply function (the second parameter) on 
                          every elements stored  in Bit2 (the first 
                          parameter), traversing the array in row-major order.
*  Expectations:          The parameter (Bit2) is non-null, and we are 
                          expecting a non-null function pointer for the second 
                          parameter. The function does not return anything as 
                          the output.
*/
extern void Bit2_map_row_major(T Bit2, 
             void apply(int col, int row, T Bit2, int bit, void *cl), 
             void *cl);





/* Bit2_map_col_major:    Calls the apply function (the second parameter) on 
                          every elements stored in Bit2 
                          (the first parameter), traversing the array in 
                          column-major order.
*  Expectations:          The parameter (Bit2) is non-null, and we 
                          are expecting a non-null function pointer for the 
                          second parameter. The function does not return 
                          anything as the output.
*/
extern void Bit2_map_col_major(T Bit2, 
             void apply(int col, int row, T Bit2, int bit, void *cl), 
             void *cl);




#undef T
#endif