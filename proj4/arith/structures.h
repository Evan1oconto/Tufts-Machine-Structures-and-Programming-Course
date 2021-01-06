/*

 * structures.h
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         10/23/2020
 * Assignment:   Machine Arithmetic

 * Interface for structures used throughout compress and 
 * decompress. Also contains the constants used throughout. 

*/

#include "a2plain.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "pnm.h"
#include <stdlib.h>
#include <stdio.h>


/* this is where all the constants are defined */
#define NINE_BIT_MP 63
#define FIVE_BIT_MP 103
#define MAX_BCD 0.3
#define BYTE_SIZE 8
#define DENOMINATOR 255
#define   WORD_SIZE  32
#define SIZE_OF_BYTE  8


#ifndef STRUCTURES_INCLUDED
#define STRUCTURES_INCLUDED


/* this is a structure representing a component. Holds floats for Y, Pb, Pr */
typedef struct Component {
        float Y, Pb, Pr;
} *Component;

/* this is a struct representing a closure for mapping functions. It holds 
   an image array, the denominator and the methods needed for the mapping   */
typedef struct closure_struct {
        A2Methods_UArray2 *image_arr;
        int denominator;
        A2Methods_T methods;
} closure_struct; 

/* this is a structure representing an unpacked word. 
        Holds signed/unsiged ints for a, b, c, d, pb, pr */
typedef struct unpacked_word {
        uint64_t a;
        int64_t b, c, d;
        uint64_t pb, pr;
} unpacked_word;

#endif 