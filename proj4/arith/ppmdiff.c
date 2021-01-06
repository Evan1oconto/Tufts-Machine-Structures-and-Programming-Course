#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "pnm.h"
#include "a2plain.h"
#include "a2blocked.h"
#include <math.h>


int get_smaller_width(int width1, int width2);
int get_smaller_height(int height1, int height2);

int main(int argc, char *argv[]) 
{
        assert(argc == 3);

        FILE *fp1 = NULL;
        FILE *fp2= NULL;
        Pnm_ppm image1;
        Pnm_ppm image2;

        A2Methods_T methods = uarray2_methods_plain;

        fp1 = fopen(argv[1], "rb");
        fp2 = fopen(argv[2], "rb");
        if (fp1 == NULL || fp2 == NULL) {
            fprintf(stderr, "Unable to open file\n");
            exit(1);
        }


        image1 = Pnm_ppmread(fp1, methods);
        image2 = Pnm_ppmread(fp2, methods);

        int width1 = image1->width;
        int height1 = image1->height;
        int width2 = image2->width;
        int height2 = image2->height;

        int width = get_smaller_width(width1, width2);
        int height = get_smaller_height(height1, height2);
        
        if (width == -1 || height == -1) {
                fprintf(stderr, "image width/height differs by more than 1\n");
                printf("1");                
        }
        
        
        
        
        double sum = 0;
        for (int j = 0; j < height; j++){
            for (int i = 0; i < width; i++){
                
                Pnm_rgb pixel1, pixel2; 
                pixel1 = image1->methods->at(image1->pixels, i, j);
                pixel2 = image2->methods->at(image2->pixels, i, j);

                double rdiff, gdiff, bdiff;
                rdiff = ((double)pixel1->red/image1->denominator) -
                        ((double)pixel2->red/image2->denominator);

                gdiff = ((double)pixel1->green/image1->denominator) -
                        ((double)pixel2->green/image2->denominator);

                bdiff = ((double)pixel1->blue/image1->denominator) -
                        ((double)pixel2->blue/image2->denominator);

                rdiff = rdiff * rdiff;
                gdiff = gdiff * gdiff;
                bdiff = bdiff * bdiff;

                sum += rdiff + gdiff + bdiff;
                
            }
        }
        
        double e = sqrt((sum / (3 * width * height)));

        printf("%0.4f \n", e);
        
        Pnm_ppmfree(&image1);
        Pnm_ppmfree(&image2);
        fclose(fp1);
        fclose(fp2);
}







int get_smaller_width(int width1, int width2) 
{
        if (width1 - width2 <= 1 && width1 - width2 >= -1) {
                if (width1 > width2) {
                        return width2;
                }
                return width1;
        }
        return -1;
}





int get_smaller_height(int height1, int height2) 
{
        if (height1 - height2 <= 1 && height1 - height2 >= -1) {
                if (height1 > height2) {
                        return height2;
                }
                return height1;
        }
        return -1;
}