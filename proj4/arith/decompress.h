/*
 * decompress.h
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         10/23/2020
 * Assignment:   Machine Arithmetic

 * This is the decompress interface. Contains methods to perform 
 * various computations and transformations of a compressed image 
 * into a pnm_ppm. 
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




#ifndef DECOMPRESS_40_INCLUDED
#define DECOMPRESS_40_INCLUDED

A2Methods_UArray2 decompress(A2Methods_UArray2 codeword, int width, 
                                            int height, A2Methods_T methods);

void get_codeword_arr(int col, int row, A2Methods_UArray2 codeword_arr, 
                                                        void *elem, void *cl);

extern A2Methods_UArray2 Comp_to_RGB(A2Methods_UArray2 original, 
                        A2Methods_T methods, int denom, A2Methods_mapfun *map);

extern void map_Comp_to_RGB(int col, int row, A2Methods_UArray2 image,
                                                     void *elem, void *cl);

extern float bcd_to_float(int64_t val);

extern float a_to_float(uint64_t a);

unpacked_word *word_to_block(uint64_t word);

#endif 