/*

 * compress40.c
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         10/23/2020
 * Assignment:   Machine Arithmetic

 * Implementation of the compress40 interface. Contains methods compress40 and
 * decompress40, which calls and performs the necessary functions in order to 
 * compress and decompress their specific image passed into them.

*/

#include "decompress.h"
#include "compress.h"
#include "bitpack.h"
#include "structures.h"


/* compress40
*   Arguments: a file pointer representing the image
*   Return: none
*   Description: takes in a valid ppm file and calls/performs the necessary 
*                operations to compress the image into a set of 32 bit 
*                codewords and prints it to stdout.
*   Assumptions: methods and map are not null
*
*/
void compress40(FILE *fp)
{
        A2Methods_T methods = uarray2_methods_plain;
        A2Methods_mapfun *map = methods->map_default;

        assert(map);
        assert(methods);


        Pnm_ppm image = Pnm_ppmread(fp, methods);


        int width  = image->width;
        int height = image->height;
        int denominator = image->denominator;


        /* trimming the image if width or height are not even */
        if (image->width % 2 != 0){
                width--;
        }
        if (image->height % 2 != 0) {
              height--;
        }

        /* arrays that will old both RGB image and component image */
        A2Methods_UArray2 RGB_image = methods->new(width, height,
                                                 sizeof(struct Pnm_rgb));
        


        /* for loop to set the image pixels to the RGB_image array */
        for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                        Pnm_rgb newpix = methods->at(RGB_image, col, row);
                        Pnm_rgb oldpix = methods->at(image->pixels, col, row);
                        newpix->red = oldpix->red;
                        newpix->green = oldpix->green;
                        newpix->blue = oldpix->blue;

                }

        }

        A2Methods_UArray2 comp_image = RGB_to_Comp(RGB_image, methods, 
                                                   denominator, map);


        /* frees the image's old pixels and sets them to RGB_image */
        methods->free(&(image->pixels));
        image->pixels = RGB_image;

      
        compress(comp_image, width, height, methods);
        

        Pnm_ppmfree(&image);
        methods->free(&comp_image);
}



/* decompress40
*   Arguments: a file pointer representing the image
*   Return: none
*   Description: takes in a valid file containing codewords and calls/performs
*                operations to decompress the image from a set of 32 bit 
*                codewords to a ppm image and prints the image to stdout.
*   Assumptions: methods and map are not null, read is 2 and c is a new line
*
*/
void decompress40(FILE *fp)
{
        unsigned height, width;
        A2Methods_T methods = uarray2_methods_plain;
        A2Methods_mapfun *map = methods->map_default;

        assert(map);
        assert(methods);

        int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height);

        assert(read == 2);
        int c = getc(fp);
        assert(c == '\n');


        /* width and heigt divided by 2 since the number of 
           codewords is 1/4th the size of an image          */
        A2Methods_UArray2 codeword_arr = methods->new((width / 2), (height / 2),
                                                 sizeof(uint64_t));


        map(codeword_arr, get_codeword_arr, fp);

        A2Methods_UArray2 comp_image = decompress(codeword_arr, (width / 2),
                                                  (height / 2), methods);
    
        A2Methods_UArray2 RGB_image = Comp_to_RGB(comp_image, methods, 
                                                      DENOMINATOR, map);


        Pnm_ppm image = malloc(sizeof(struct Pnm_ppm));

        image->width = width;
        image->height = height;
        image->denominator = DENOMINATOR;
        image->pixels = RGB_image;
        image->methods = methods;


        Pnm_ppmwrite(stdout, image);
        

        Pnm_ppmfree(&image);
        methods->free(&comp_image);
        methods->free(&codeword_arr);

}
