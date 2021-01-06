/*

 * compress.c
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         10/23/2020
 * Assignment:   Machine Arithmetic

 * Implementation of the compress interface. Contains methods to perform 
 * various computations and transformations of a ppm image into
 * a compressed codeword 

*/


#include "compress.h"
#include "bitpack.h"



/* compress
*   Arguments: an array of component structs, the width and height of the 
*              image, and methods
*   Return: none
*   Description: takes an array of component structs and converts/prints it 
*                to a codeword in stdout
*   Assumptions: none
*
*/
void compress(A2Methods_UArray2 image, int width, int height, 
              A2Methods_T methods)
{    
        printf("COMP40 Compressed image format 2\n%u %u", width, height);
        printf("\n");

        /* for loop loops through the image and compresses 4 components into
           a codeword. Prints and frees the individual blocks here too      */
        for (int row = 0; row < height; row += 2) {
                for (int col = 0; col < width; col += 2) {
                
                        Component comp1 = methods->at(image, col, row);
                        Component comp2 = methods->at(image, col, row+1);
                        Component comp3 = methods->at(image, col+1, row);
                        Component comp4 = methods->at(image, col+1, row+1);

                        unpacked_word *block;

                        
                        block = compress_computations(comp1, comp2, comp3, 
                                                      comp4);

                
                        uint64_t word = block_to_word(block);
                        uint32_t word1 = (uint32_t)word;
                        print_word(word1);
                        
                        free(block);
                }
        }
        
}



/* RGB_to_Comp
*   Arguments: an array of RGB structs, methods, the denominator, and map 
*   Return: a uarray2 of component structs
*   Description: takes an array of rgb structs and maps it to an array with
*                the same width and height, but with component structs
*   Assumptions: the original array, methods, and map are not null
*/
A2Methods_UArray2 RGB_to_Comp(A2Methods_UArray2 original, A2Methods_T methods,
                                             int denom, A2Methods_mapfun *map)
{
        assert(original != NULL && methods != NULL && map != NULL);


        int width = methods->width(original);
        int height = methods->height(original);
        int size = methods->size(original);
        (void) size;


        A2Methods_UArray2 comp_image = methods->new(width, height,
                                                 sizeof(struct Component));


        closure_struct cl = {comp_image, denom, methods};
        map(original, map_RGB_to_Comp, &cl);


        return comp_image;
}


/* map_RGB_to_Comp
*   Arguments: the column, row, rgb image, element, and a closure 
*   Return: none
*   Description: takes an elements from the rgb image and maps it to a comp
*                array that was passed in via the closure. It does the 
*                necessary calculations based on the arguments
*   Assumptions: none
*/
void map_RGB_to_Comp(int col, int row, A2Methods_UArray2 image,
                                                        void *elem, void *cl)
{
        (void) image;
        Pnm_rgb pix = elem;

        closure_struct *mycl = cl;

        int denom = mycl->denominator;
        A2Methods_UArray2 comp_image = mycl->image_arr;
        A2Methods_T methods = mycl->methods;

        /* convert from int to float and normalize */ 
        float r = (pix->red * 1.0) / (denom);
        float g = (pix->green * 1.0) / (denom);
        float b = (pix->blue * 1.0) / (denom);

        /* perform computations from rgb to component form */ 
        float y  = 0.299 * r + 0.587 * g + 0.114 * b;
        float pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
        float pr = 0.5 * r - 0.418688 * g - 0.081312 * b;

        Component new_comp = methods->at(comp_image, col, row);
        new_comp->Y  = y;
        new_comp->Pb = pb;
        new_comp->Pr = pr;

}

/* compress_computations
*   Arguments: 4 components
*   Return: a pointer to an unpacked_word struct
*   Description: takes in 4 components that will be the basis of the unpacked
*                word. Each component will contribute to the pb, pr, and a, b,
*                c, and d values to make an unpacked word. It will then return
*                the unpacked word.
*   Assumptions: none
*/
unpacked_word *compress_computations(Component comp1, Component comp2, 
                         Component comp3, Component comp4)
{

        unpacked_word *block = malloc(sizeof(unpacked_word));

        /* compute a, b, c, d, pb, pr values from components */
        block->pb = (uint64_t)(Arith40_index_of_chroma(((comp1->Pb + comp2->Pb
                                             + comp3->Pb + comp4->Pb) / 4.0)));
        block->pr = (uint64_t)(Arith40_index_of_chroma(((comp1->Pr + comp2->Pr
                                             + comp3->Pr + comp4->Pr) / 4.0)));


        float a = ((comp4->Y + comp3->Y + comp2->Y + comp1->Y) / 4.0);
        float b = ((comp4->Y + comp3->Y - comp2->Y - comp1->Y) / 4.0);
        float c = ((comp4->Y - comp3->Y + comp2->Y - comp1->Y) / 4.0);
        float d = ((comp4->Y - comp3->Y - comp2->Y + comp1->Y) / 4.0);

        /* store in unpacked word struct */
        block->a = a_to_9bit(a);
        block->b = bcd_to_5bit(b);
        block->c = bcd_to_5bit(c);
        block->d = bcd_to_5bit(d);


        return block;
}

/* a_to_9bit
*   Arguments: a float
*   Return: a rounded uint64_t representing 9 bits
*   Description: takes in a computed "a" value (which will be a float) and 
*                compacts it into a 9 bit uint64_t by multiplying by the
*                appropriate constant and rounding it via the math.h function.
*   Assumptions: none
*/
uint64_t a_to_9bit(float a) {
        return (uint64_t)(round(a * NINE_BIT_MP));
}

/* bcd_to_5bit
*   Arguments: a float
*   Return: a rounded int64_t representing 5 bits
*   Description: takes in a computed "b", or "c", or "d" value (which will be 
*                floats) and compacts it into a 5 bit int64_t by multiplying
*                by the appropriate constant and rounding.
*   Assumptions: none
*/
int64_t bcd_to_5bit(float val)
{
        /* conditional statements to check value is inbouds for both signs */
        if (val < (-1*MAX_BCD)){
                val = (-1*MAX_BCD);
        }
        if (val > MAX_BCD){
                val = MAX_BCD;
        }
        return (int64_t)(round(val * FIVE_BIT_MP));
}

/* block_to_word
*   Arguments: a pointer to an unpacked_word struct
*   Return: a uint64_t representing the codeword
*   Description: takes in an unpacked word and modifies it by appending each
*                specific aspect of a codeword to the necessary spot as 
*                specified in the homework document.
*   Assumptions: the argument is not equal to null
*/
uint64_t block_to_word (unpacked_word *block)
{
        assert(block != NULL);
        uint64_t word = 0;

        word = Bitpack_newu(word, 6, 26, block->a);
        word = Bitpack_news(word, 6, 20, block->b);
        word = Bitpack_news(word, 6, 14, block->c); 
        word = Bitpack_news(word, 6, 8, block->d);
        word = Bitpack_newu(word, 4, 4, block->pb);
        word = Bitpack_newu(word, 4, 0, block->pr);

        return word;
}


/* print_word
*   Arguments: a uint32_t representing the codeword
*   Return: none
*   Description: takes in a codeword and prints the characters using putchar.
*   Assumptions: none
*/
void print_word(uint32_t word)
{
        char byte1 = (char)Bitpack_getu(word, BYTE_SIZE, BYTE_SIZE*0);
        char byte2 = (char)Bitpack_getu(word, BYTE_SIZE, BYTE_SIZE*1);
        char byte3 = (char)Bitpack_getu(word, BYTE_SIZE, BYTE_SIZE*2);
        char byte4 = (char)Bitpack_getu(word, BYTE_SIZE, BYTE_SIZE*3);
        
        putchar(byte4); 
        putchar(byte3);
        putchar(byte2);
        putchar(byte1);

}
