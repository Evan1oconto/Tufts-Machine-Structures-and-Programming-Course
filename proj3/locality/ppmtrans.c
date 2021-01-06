/*

 * ppmtrans.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         10/10/2020
 * Assignment:   Locality and the costs of loads and stores
 * 
 * Purpose:  Program that performs simple image transformations. This program
 *           is capable of performing transformations (rotations, flip, 
 *           transpose) on a Portable Pixmap image.

*/

#include "assert.h"
#include "a2plain.h"
#include "cputiming.h"
#include "a2blocked.h"
#include "pnm.h"
#include "rotate.h"


int time_rotation = 0; /* no timing data necessary */
FILE *timing_data = NULL;



#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)


static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-{rotate <angle>,flip {horizontal,vertical},transpose}] "
                        "[-time <time_filename>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}




int main(int argc, char *argv[]) 
{
        char *time_file_name   = NULL;
        int   rotation         = 0;
        int   i;
        char  flip             = 0;
        int   should_transpose = 0;
        char *preferred        = "rotate";

        /* if timing data is not needed, this silences the compiler warnings */
        (void) time_file_name;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
				        "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
				        "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
				        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                        preferred = "rotate";    
                } else if (strcmp(argv[i], "-flip") == 0) {
                        if (!(i + 1 < argc)) {      /* no flip type */
                                usage(argv[0]);
                        }
                        char *flip_type = argv[++i];
                        if (strcmp(flip_type, "horizontal") == 0) {       
                                flip = 'H';
                        } else if (strcmp(flip_type, "vertical") == 0) {   
                                flip = 'V';
                        } else {
                                fprintf(stderr, 
                                        "Flip must be horizontal or "
                                                        "vertical\n");
                                usage(argv[0]);
                        }
                        preferred = "flip";
                } else if (strcmp(argv[i], "-transpose") == 0) {
                        should_transpose = 1;
                        preferred = "transpose";
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
			        argv[i]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }
        
  
        Pnm_ppm image;
        FILE *fp = NULL;
        
        /* Open the input file for reading and store initialise image with 
                                                        the opened ppm image */
        if (i < argc) {
                fp = fopen(argv[i], "rb");
                if (fp == NULL) {
                        fprintf(stderr, "File can't be opened for reading.\n");
                        exit(EXIT_FAILURE);
                }
        } else {
                fp = stdin;
        }
        
        image = Pnm_ppmread(fp, methods);

        if (fp != stdin) {
                assert(fclose(fp) == 0);
        }

        /* declare arrays to store the initial and the rotated images */
        A2Methods_UArray2 original_image = image->pixels;
        A2Methods_UArray2 rotated_image = NULL;


        /* open the file to store timing data if -time switch present */
        if (rotation != 0 || (flip || should_transpose)) {
                 if (time_file_name != NULL) {
                        time_rotation = 1;
                        timing_data = fopen(time_file_name, "a");
                        assert(timing_data != NULL);
                }
        }
        


        /* based on the type of transformation, call the correct method to 
                                             rotate/flip/transpose the image */
        if (strcmp(preferred, "flip") == 0) {
                if (flip == 'H') {
                        rotated_image = flip_horizontal(original_image, 
                                                        methods, map);
                } else if (flip == 'V') {
                        rotated_image = flip_vertical(original_image, methods,
                                                      map);
                }   
        } else if (strcmp(preferred, "transpose") == 0) {
                if (should_transpose) {
                        rotated_image = transpose(original_image, 
                                                                 methods, map);
                }
        } else if (strcmp(preferred, "rotate") == 0) {
                if (rotation == 0) {
                        rotated_image = rotate_0(original_image);
                } else if (rotation == 90) {
                        rotated_image = rotate_90(original_image, methods,
                                                  map);
                } else if (rotation == 180) {
                        rotated_image = rotate_180(original_image, methods, 
                                                   map);
                } else if (rotation == 270) {
                        rotated_image = rotate_270(original_image, methods, 
                                                   map);
                }               
        }
        

        /* set the ppm image to the transformed image and print to stdout */
        image->pixels = rotated_image;
        image->width = methods->width(rotated_image);
        image->height = methods->height(rotated_image);

        Pnm_ppmwrite(stdout, image);   

        if (original_image != rotated_image) {
               methods->free(&original_image); 
        }   
        Pnm_ppmfree(&image);

        return EXIT_SUCCESS;
}