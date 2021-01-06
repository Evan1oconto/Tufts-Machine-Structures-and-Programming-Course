/*

 * bitpack.c
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         10/23/2020
 * Assignment:   Machine Arithmetic

 * Implementation of the bitpack interface. Contains methods to perform 
 * various methodsincvolving packing bits. This is used to pack/unpack
 * codewords in the compress and decompress algorithms.
 
*/

#include "bitpack.h"
#include "math.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/* Bitpack_fitsu
*   Arguments: a uint64_t representing the integer, width representing 
*              the # of bits to fit into
*   Return: bool (true if it can fit)
*   Description: test to see if an unsigend integer can be represented 
*                 in k bits
*   Assumptions: width <= 64
*/
bool Bitpack_fitsu(uint64_t n, unsigned width) 
{
        assert(width <= 64);

        uint64_t max = (((uint64_t)1 << width) - 1);
        if ( n <= max){
                return true;
        }else{
                return false;
        }
}

/* Bitpack_fitss
*   Arguments: an int64_t representing the signed integer, 
*              width representing the # of bits to fit into
*   Return: bool (true if it can fit)
*   Description: test to see if a signed integer can be 
*                  represented in k bits
*   Assumptions: width <= 64
*/
bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width <= 64);
        
        int64_t max = (((int64_t)1 << (width-1)) - 1);
        int64_t min = ((((int64_t)1 << (width-1)) * -1) + 1);
        if (n <= max && n >= min) {
                return true;
        } else {
                return false;
        }
}

/* Bitpack_getu
*   Arguments: codeword, width of value, least significant bit
*   Return: extracted uint64
*   Description: extract value from a word
*   Assumptions: width <= 64 && (width + lsb) <= 64
*/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64 && (width + lsb) <= 64);
        uint64_t mask = (((uint64_t)1 << width) - 1);
        mask = mask << lsb;
        uint64_t extract = (mask & word);
        extract = extract >> lsb;

        return extract; 
}

/* Bitpack_gets
*   Arguments: codeword, width of value, least significant bit
*   Return: extracted int64
*   Description: extract value from a word
*   Assumptions: width <= 64 && (width + lsb) <= 64
*/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64 && width + lsb <= 64);
        uint64_t extract = Bitpack_getu(word, width, lsb);
        int64_t num = (int64_t)extract;
        if (num > ((int64_t)1 << (width - 1))) {
                return (num - (((uint64_t)1 << width)));
        }else{
                return num;
        }
}

/* Bitpack_newu
*   Arguments: codeword, width of value, least significant bit,
                        value of unisgned int to insert
*   Return: new uint64 word 
*   Description: set field value in a word
*   Assumptions: width <= 64 && (width + lsb) <= 64,
                        the value fits in given width
*/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value) 
{
        assert(width <= 64 && width + lsb <= 64);

        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        uint64_t mask1 = (((uint64_t)1 << 32) - 1);
        
        mask1 = mask1 << ((width + lsb));
        uint64_t mask2 = (((uint64_t)1 << 32) - 1);
        mask2 = mask2 >> (32 - (lsb));
        uint64_t mask3 = (mask2 | mask1);

        uint64_t prototype = (mask3 & word);
        uint64_t translate_val = value << (lsb);
        uint64_t translate = (translate_val | prototype);

        return translate;        
}

/* Bitpack_news
*   Arguments: codeword, width of value, least significant bit,
                        value of unisgned int to insert
*   Return: new uint64 word 
*   Description: set field value in a word
*   Assumptions: width <= 64 && (width + lsb) <= 64,
                        the value fits in given width
*/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value)
{       
        assert(width <= 64 && width + lsb <= 64);

        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        return Bitpack_newu(word, width, lsb, Bitpack_getu(value, width, 0));
}

