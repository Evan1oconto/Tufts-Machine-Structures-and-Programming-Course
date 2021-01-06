/*

 * decompress.c
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         10/23/2020
 * Assignment:   Machine Arithmetic

 * Implementation of the decompress interface. Contains methods to perform 
 * various computations and transformations of a compressed image 
 * into a pnm_ppm. 

*/

#include "decompress.h"
#include "bitpack.h"



/* decompress
*   Arguments: an array of codewords, the width and height of the 
*              codeword array, and methods
*   Return: an array of the image pixels in component form 
*   Description: takes an array of codewords and makes and array 
                        component structs 
*   Assumptions: none
*
*/
A2Methods_UArray2 decompress(A2Methods_UArray2 codeword, int width, int height,
                             A2Methods_T methods)
{
        A2Methods_UArray2 test_image = methods->new(width*2, height*2,
                                                    sizeof(struct Component));
        
        for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                        
                        uint64_t *word = methods->at(codeword, col, row);
                        unpacked_word *block = word_to_block(*word); 
                     
                        /* convert from ints to floats */ 
                        float a = a_to_float(block->a);
                        float b = bcd_to_float(block->b);
                        float c = bcd_to_float(block->c);
                        float d = bcd_to_float(block->d);

                        float Pb = Arith40_chroma_of_index(block->pb);
                        float Pr = Arith40_chroma_of_index(block->pr);

                        /* calculate components and set pixels */
                        Component val1 = methods->at(test_image, (2 * col),
                                                    (2 * row));
                        (val1)->Y = a - b - c + d;
                        (val1)->Pb = Pb;
                        (val1)->Pr = Pr;
                
                        Component val2 = methods->at(test_image, (2 * col),
                                                    (2 * row) + 1);
                        (val2)->Y = a - b + c - d;
                        (val2)->Pb = Pb;
                        (val2)->Pr = Pr;
                        
                        Component val3 = methods->at(test_image, (2 * col) + 1,
                                                    (2 * row));
                        (val3)->Y = a + b - c - d;
                        (val3)->Pb = Pb;
                        (val3)->Pr = Pr;
                        
                        Component val4 = methods->at(test_image, (2 * col) + 1,
                                                    (2 * row) + 1);
                        (val4)->Y = a + b + c + d;
                        (val4)->Pb = Pb;
                        (val4)->Pr = Pr;

                        free(block);


                }
        }
  
        return test_image;
}

/* get_codeword_arr
*   Arguments: column and row, codeword array, element to retrieve,
                closure struct 
*   Return: none
*   Description: mapping function to get codewords 
*   Assumptions: none
*
*/
void get_codeword_arr(int col, int row, A2Methods_UArray2 codeword_arr, 
                                                          void *elem, void *cl)
{
        (void) col;
        (void) row;
        (void) codeword_arr;

        FILE *fp = cl;
        uint64_t *word = elem;

        uint64_t one_byte;
        uint64_t placeholder = 0;
        uint64_t starting_pos = WORD_SIZE - SIZE_OF_BYTE;

        for (int i = starting_pos; i >= 0; i -= SIZE_OF_BYTE) {
                one_byte = getc(fp);
                one_byte = one_byte << i;
                placeholder = placeholder | one_byte;
        }
        *word = placeholder;
}


/* Comp_to_RGB
*   Arguments: an array of component structs, methods, denominator, map 
*   Return: a uarray2 of RGB structs
*   Description: takes an array of component structs and maps to an array 
*                with the same width and height, but with RGB structs 
*   Assumptions: the original array, methods, and map are not null
*/
A2Methods_UArray2 Comp_to_RGB(A2Methods_UArray2 original, A2Methods_T methods,
                                             int denom, A2Methods_mapfun *map)
{
        assert(original != NULL && methods != NULL && map != NULL);
        int width = methods->width(original);
        int height = methods->height(original);
        int size = methods->size(original);
        (void) size;

        A2Methods_UArray2 RGB_image = methods->new(width, height,
                                                 sizeof(struct Pnm_rgb));
        closure_struct cl = {RGB_image, denom, methods};     

        map(original, map_Comp_to_RGB, &cl);

        return RGB_image;
}

/* map_Comp_to_RGB
*   Arguments: the column, row, comp image, element, and a closure 
*   Return: none
*   Description: takes an elements from the comp image and maps to an RGB
*                array that was passed in via the closure. It does the 
*                necessary calculations based on the arguments
*   Assumptions: none
*/
void map_Comp_to_RGB(int col, int row, A2Methods_UArray2 image,
                                                        void *elem, void *cl)
{
        (void) image;
        Component pix = elem;

        closure_struct *mycl = cl;

        int denom = mycl->denominator;
        A2Methods_UArray2 RGB_image = mycl->image_arr;
        A2Methods_T methods = mycl->methods;

        /* convert from component to rgb form */
        int r = (pix->Y + 1.402 * pix->Pr) * denom;
        int g = (pix->Y - 0.344136 * pix->Pb - 0.714136 * pix->Pr) * denom;
        int b = (pix->Y + 1.772 * pix->Pb) * denom;

        /* set values in rgb struct */ 
        Pnm_rgb rgb = methods->at(RGB_image, col, row);

        rgb->red = r;
        rgb->green = g;
        rgb->blue = b;

}

/* word_to_block 
*   Arguments: a packed_word
*   Return: a struct of the unpacked values 
*   Description: unpacks a word using bitpack get and 
                 stores the values in an unpacked word
                 struct 
*   Assumptions: none 
*/
unpacked_word *word_to_block(uint64_t word)
{
        unpacked_word *block = malloc(sizeof(unpacked_word));

        block->a  = Bitpack_getu(word, 6, 26);
        block->b  = Bitpack_gets(word, 6, 20);
        block->c  = Bitpack_gets(word, 6, 14);
        block->d  = Bitpack_gets(word, 6, 8);
        block->pb = Bitpack_getu(word, 4, 4);
        block->pr = Bitpack_getu(word, 4, 0);

        return block;
}

/* a_to_float
*   Arguments: a 9 bit uint46_t value
*   Return: a float representing the actual value of a in an unpacked word 
*   Description: unpacks the "a" component in a word and returns the float 
*                value
*   Assumptions: none
*/
float a_to_float(uint64_t a)
{
        return (1.0*a)/NINE_BIT_MP;
}

/*  bcd_to_float
*   Arguments: signed int
*   Return: float 
*   Description: takes in a 5 bit signed int and 
                        converts to float
*   Assumptions: none
*/
float bcd_to_float(int64_t val)
{
        return (1.0*val)/FIVE_BIT_MP;
}


