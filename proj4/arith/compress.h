/*

 * compress.h
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         10/23/2020
 * Assignment:   Machine Arithmetic

 * Interface for compressing the image.  

*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "mem.h"
#include "arith40.h"
#include "math.h"
#include "bitpack.h"
#include "structures.h"


#ifndef COMPRESS_40_INCLUDED
#define COMPRESS_40_INCLUDED


extern void compress(A2Methods_UArray2 image, int width, int height,
                     A2Methods_T methods);

extern A2Methods_UArray2 RGB_to_Comp(A2Methods_UArray2 original, 
                        A2Methods_T methods, int denom, A2Methods_mapfun *map);

extern void map_RGB_to_Comp(int col, int row, A2Methods_UArray2 image, 
                                                     void *elem, void *cl);


extern unpacked_word *compress_computations(Component comp1, Component comp2,
                                            Component comp3, Component comp4);


extern uint64_t a_to_9bit(float a);
extern int64_t bcd_to_5bit(float val);
extern uint64_t block_to_word (unpacked_word *block);

void print_word(uint32_t word);



#endif