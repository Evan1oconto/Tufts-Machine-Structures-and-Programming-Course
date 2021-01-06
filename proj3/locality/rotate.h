/*

 * rotate.h
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         10/10/2020
 * Assignment:   Locality and the costs of loads and stores

 * Interface for rotating arrays. 

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a2methods.h"


/* rotate_0:     Rotates the original image by zero degrees
*  Expectations: It is a checked runtime error for original to be NULL. 
*                The return value is the array rotated by 0 degrees.
*/
extern A2Methods_UArray2 rotate_0(A2Methods_UArray2 original);



/* rotate_90:     Rotates the original image by 90 degrees clockwise
*  Expectations: It is a checked runtime error for original to be NULL. 
                 It is also a checked runtime error for methods and map to 
                 be NULL.
*                The return value is the array rotated by 90 degrees.
*/
extern A2Methods_UArray2 rotate_90(A2Methods_UArray2 original, 
                                   A2Methods_T methods, A2Methods_mapfun *map);



/* rotate_180:   Rotates the original image by 180 degrees
*  Expectations: It is a checked runtime error for original to be NULL. 
*                It is also a checked runtime error for methods and map to
*                be NULL.
*                The return value is the array rotated by 180 degrees clockwise
*/
extern A2Methods_UArray2 rotate_180(A2Methods_UArray2 original, 
                                   A2Methods_T methods, A2Methods_mapfun *map);



/* rotate_270:   Rotates the original image by 270 degrees
*  Expectations: It is a checked runtime error for original to be NULL. 
*                It is also a checked runtime error for methods and map to
*                be NULL.
*                The return value is the array rotated by 270 degrees clockwise
*/                                      
extern A2Methods_UArray2 rotate_270(A2Methods_UArray2 original,
                                   A2Methods_T methods, A2Methods_mapfun *map);





/* flip_horizontal:  Reflects the original image about the vertical axis
*  Expectations: It is a checked runtime error for original to be NULL. 
                 It is also a checked runtime error for methods and map to
*                be NULL.
*                The return value is the array flipped horizontally.
*/
extern A2Methods_UArray2 flip_horizontal(A2Methods_UArray2 original,
                                   A2Methods_T methods, A2Methods_mapfun *map);




/* flip_vertical: Reflects the original image about the horizontal axis
*  Expectations:  It is a checked runtime error for original to be NULL. 
*                 It is also a checked runtime error for methods and map to
*                 be NULL.
*                 The return value is the array flipped vertically
*/
extern A2Methods_UArray2 flip_vertical(A2Methods_UArray2 original,
                                   A2Methods_T methods, A2Methods_mapfun *map);





/* transpose:    Transposes the image across UL-to-LR axis
*  Expectations: It is a checked runtime error for original to be NULL. 
*                It is also a checked runtime error for methods and map to
*                be NULL.
*                The return value is the array transposed across UL-to-LR axis
*/
extern A2Methods_UArray2 transpose(A2Methods_UArray2 original,
                                   A2Methods_T methods, A2Methods_mapfun *map);