/*

 * rotate.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         10/10/2020
 * Assignment:   Locality and the costs of loads and stores

 * Implementation of the rotate interface. Contains methods to perform 
 * various transformations on a 2 dimensional polymorphic array. 

*/

#include "rotate.h"

#include "a2plain.h"
#include "a2blocked.h"
#include "cputiming.h"
#include "assert.h"


#define NANOSECOND 1000000000 /* 1 second = 1000000000 ns */


/* declared and defined in ppmtrans.c in order to produce timing data */
extern int time_rotation; 
extern FILE *timing_data;



/* struct that gets sent to mapping functions to do the required
                                                              transformation */
typedef struct Mapping_cl {
        A2Methods_UArray2 image; /* the transformed image */
        A2Methods_T methods; /* struct of methods to modify the given image 
                                                                       array */
} Mapping_cl;




/* write_to_file: Prints the timing data of the transformation to the file 
                  pointed at by fp, the first argument.
*  Expectations:  It is a checked runtime error for fp to be NULL. 
*                 It is also a checked runtime error for rotate_type to be 
                  NULL, and for the width, height, size and time to be non-
                  positive.
*                 The return value is nothing, but the effect of calling the 
*                 function is that a file gets created (if does not exist 
*                 already) and the timing data is appended to the file.
*/
static void write_to_file(FILE *fp, int width, int height, double time, 
                                    const char *rotate_type, int size)
{
        assert(fp != NULL);
        assert(width >= 0 && height >= 0);
        assert(rotate_type != NULL);
        assert(size > 0 && time > 0);
        
        int num_pixels = width * height;
        assert(num_pixels != 0); /* avoid division by zero */
                
        /* write relevant data/experimental results to the file */
        fprintf(fp, "Rotation type: %s \n", rotate_type);
        fprintf(fp, "Dimensions of image (wxh): (%dx%d)\n", width, height);
        fprintf(fp, "Number of pixels: %d\n", num_pixels);
        fprintf(fp, "Total time taken: %.0f nanoseconds (%.5f seconds)\n", 
                                         time, 
                                         time / (double) NANOSECOND);
        fprintf(fp, "Time taken per pixel: %.0f nanoseconds\n\n\n\n", 
                                                   time / (double) num_pixels);



        assert(fclose(fp) == 0); /* close the file */
}






/* rotate_image: performs the necessary operations on the image, whether it be
*                rotations, flipping, timing, or transposing the image. 
*  Expectations: We expect all input into this function to already be checked
*                for runtime errors, and is ultimately valid. So, it is 
*                an UNCHECKED runtime error for original, rotation_cl, rotate,
*                map and rotate_type to be NULL. Additionally, is is expected
                 that width and height are within appropriate bounds.
*                The return value is nothing, but the effect of calling this 
*                function is that the image is going to be transformed as 
                 specified by the rotate argument, and timing data is 
                 produced based on whether the user wants it or not.
*  Note: Assertion checks were not included in this function to remove 
         overheads associated with the actual transformation of the image.
*/

// rotate_image(original, rotation_cl, &map_rotate_180, map, width, 
//                                                   height, "180 degrees", size);

// static void map_rotate_180(int col, int row, A2Methods_UArray2 image, 
//                                                           void *elem, void *cl)

static void rotate_image(A2Methods_UArray2 original, Mapping_cl rotation_cl, 
                  void rotate(int col, int row, A2Methods_UArray2 image, 
                  void *elem, void *cl), 
                  A2Methods_mapfun *map, int width, int height, 
                  const char *rotate_type, int size)
{  
        if (time_rotation) {
                CPUTime_T timer;
                timer = CPUTime_New();

                double time_used = 0.0;

                CPUTime_Start(timer);

                map(original, rotate, &rotation_cl);

                time_used = CPUTime_Stop(timer);
                
                CPUTime_Free(&timer);  
                write_to_file(timing_data, width, height, time_used , 
                                                            rotate_type, size);
        } else {
                map(original, rotate, &rotation_cl);
        }
}






/* map_rotate_90: Function that performs the transformation by copying pixels
*                 from one location to another.
*  Expectations: The return value is nothing. It is an UNCHECKED runtime 
*                error for image to be NULL, or for col and row to be out of 
*                bounds.
*  Note: Assertion checks were not included in this function to remove 
*        overheads associated with the actual transformation of the image.
*/
static void map_rotate_90(int col, int row, A2Methods_UArray2 image, 
                                                          void *elem, void *cl)
{
        Mapping_cl *rotation_cl = cl;
 
        A2Methods_UArray2 rotated_image = rotation_cl->image;
        A2Methods_T methods = rotation_cl->methods;
        int height = methods->height(image);

        int x_location =  height - row - 1;
        int y_location = col;
        int size = methods->size(image);

        memcpy(methods->at(rotated_image, x_location, y_location), elem, size);
        
}





/* map_rotate_180: Function that performs the transformation by copying pixels
*                 from one location to another.
*  Expectations: The return value is nothing. It is an UNCHECKED runtime 
*                error for image to be NULL, or for col and row to be out of 
*                bounds.
*  Note: Assertion checks were not included in this function to remove 
*        overheads associated with the actual transformation of the image.
*/
static void map_rotate_180(int col, int row, A2Methods_UArray2 image, 
                                                          void *elem, void *cl)
{
        Mapping_cl *rotation_cl = cl;
 
        A2Methods_UArray2 rotated_image = rotation_cl->image;
        A2Methods_T methods = rotation_cl->methods;
        int height = methods->height(image);
        int width = methods->width(image);

        int y_location = height - row - 1;
        int x_location = width  - col - 1;
        int size = methods->size(image);

        memcpy(methods->at(rotated_image, x_location, y_location), elem, size);
        
}






/* map_rotate_270: Function that performs the transformation by copying pixels
*                  from one location to another.
*  Expectations:   The return value is nothing. It is an UNCHECKED runtime 
*                  error for image to be NULL, or for col and row to be out of 
*                  bounds.
*  Note: Assertion checks were not included in this function to remove 
*        overheads associated with the actual transformation of the image.
*/
static void map_rotate_270(int col, int row, A2Methods_UArray2 image,   
                                                          void *elem, void *cl)
{
        Mapping_cl *rotation_cl = cl;
 
        A2Methods_UArray2 rotated_image = rotation_cl->image;
        A2Methods_T methods = rotation_cl->methods;

        int width = methods->width(image);

        int y_location = width - col - 1;
        int x_location = row;
        int size = methods->size(image);
        
        memcpy(methods->at(rotated_image, x_location, y_location), elem, size);
}




/* map_flip_horizontal: Function that performs the transformation by copying 
*                       pixels from one location to another.
*  Expectations: The return value is nothing. It is an UNCHECKED runtime 
*                error for image to be NULL, or for col and row to be out of 
*                bounds.
*  Note: Assertion checks were not included in this function to remove 
*        overheads associated with the actual transformation of the image.
*/
static void map_flip_horizontal(int col, int row, A2Methods_UArray2 image,
                                                        void *elem, void *cl)
{
        Mapping_cl *rotation_cl = cl;
 
        A2Methods_UArray2 rotated_image = rotation_cl->image;
        A2Methods_T methods = rotation_cl->methods;

        int width = methods->width(image);

        int y_location = row;
        int x_location = width - col - 1;
        int size = methods->size(image);
        
        memcpy(methods->at(rotated_image, x_location, y_location), elem, size);
}


/* map_flip_vertical: Function that performs the transformation by 
*                     copying pixels from one location to another.
*  Expectations:      The return value is nothing. It is an UNCHECKED runtime 
*                     error for image to be NULL, or for col and row to be out
                      of bounds.
*  Note: Assertion checks were not included in this function to remove 
*        overheads associated with the actual transformation of the image.
*/
static void map_flip_vertical(int col, int row, A2Methods_UArray2 image, 
                                                        void *elem, void *cl)
{
        Mapping_cl *rotation_cl = cl;
 
        A2Methods_UArray2 rotated_image = rotation_cl->image;
        A2Methods_T methods = rotation_cl->methods;

        int height = methods->height(image);

        int y_location = height - row - 1;
        int x_location = col;
        int size = methods->size(image);
        
        memcpy(methods->at(rotated_image, x_location, y_location), elem, size);
}




/* map_transpose: Function that performs the transformation by 
*                 copying pixels from one location to another.
*  Expectations:  The return value is nothing. It is an UNCHECKED runtime 
*                 error for image to be NULL, or for col and row to be out
                  of bounds.
*  Note: Assertion checks were not included in this function to remove 
*        overheads associated with the actual transformation of the image.
*/
static void map_transpose(int col, int row, A2Methods_UArray2 image, 
                                                        void *elem, void *cl)
{
        Mapping_cl *rotation_cl = cl;
 
        A2Methods_UArray2 rotated_image = rotation_cl->image;
        A2Methods_T methods = rotation_cl->methods;

        int y_location = col;
        int x_location = row;
        int size = methods->size(image);
        
        memcpy(methods->at(rotated_image, x_location, y_location), elem, size);
}





/* rotate_0:     Rotates the original image by zero degrees
*  Expectations: It is a checked runtime error for original to be NULL. 
*                The return value is the array rotated by 0 degrees.
*/
A2Methods_UArray2 rotate_0(A2Methods_UArray2 original)
{
        assert(original != NULL);

        return original;
}



/* rotate_90:     Rotates the original image by 90 degrees clockwise
*  Expectations: It is a checked runtime error for original to be NULL. 
                 It is also a checked runtime error for methods and map to 
                 be NULL.
*                The return value is the array rotated by 90 degrees.
*/
A2Methods_UArray2 rotate_90(A2Methods_UArray2 original, A2Methods_T methods, 
                                                         A2Methods_mapfun *map)
{
        assert(original != NULL && methods != NULL && map != NULL);

        int width = methods->width(original);
        int height = methods->height(original);
        int size = methods->size(original);
        int blocksize = methods->blocksize(original);

        A2Methods_UArray2 rotated_image;
        rotated_image = methods->new_with_blocksize(height, width, size, 
                                                                    blocksize);

        Mapping_cl rotation_cl = {rotated_image, methods};
 
        rotate_image(original, rotation_cl, &map_rotate_90, map, width, height,
                                                 "clockwise 90 degrees", size);

        assert(rotated_image != NULL);
        return rotated_image;
}




/* rotate_180:   Rotates the original image by 180 degrees
*  Expectations: It is a checked runtime error for original to be NULL. 
*                It is also a checked runtime error for methods and map to
*                be NULL.
*                The return value is the array rotated by 180 degrees clockwise
*/
 A2Methods_UArray2 rotate_180(A2Methods_UArray2 original, A2Methods_T methods, 
                                                         A2Methods_mapfun *map)
{
        assert(original != NULL && methods != NULL && map != NULL);

        int width = methods->width(original);
        int height = methods->height(original);
        int size = methods->size(original);
        int blocksize = methods->blocksize(original);

        A2Methods_UArray2 rotated_image;
        rotated_image = methods->new_with_blocksize(width, height, size, 
                                                                    blocksize);

        Mapping_cl rotation_cl = {rotated_image, methods};

        rotate_image(original, rotation_cl, &map_rotate_180, map, width, 
                                                  height, "180 degrees", size);

        assert(rotated_image != NULL);
        return rotated_image;        
}







/* rotate_270:   Rotates the original image by 270 degrees
*  Expectations: It is a checked runtime error for original to be NULL. 
*                It is also a checked runtime error for methods and map to
*                be NULL.
*                The return value is the array rotated by 270 degrees clockwise
*/    
A2Methods_UArray2 rotate_270(A2Methods_UArray2 original, A2Methods_T methods, 
                                                         A2Methods_mapfun *map)
{
        assert(original != NULL && methods != NULL && map != NULL);

        int width = methods->width(original);
        int height = methods->height(original);
        int size = methods->size(original);
        int blocksize = methods->blocksize(original);
        
        A2Methods_UArray2 rotated_image;
        rotated_image = methods->new_with_blocksize(height, width, size, 
                                                                    blocksize);
   
        Mapping_cl rotation_cl = {rotated_image, methods};

        rotate_image(original, rotation_cl, &map_rotate_270, map, width, 
                                        height, "clockwise 270 degrees", size);

        assert(rotated_image != NULL);
        return rotated_image;
}




/* flip_horizontal:  Reflects the original image about the vertical axis
*  Expectations: It is a checked runtime error for original to be NULL. 
                 It is also a checked runtime error for methods and map to
*                be NULL.
*                The return value is the array flipped horizontally.
*/
A2Methods_UArray2 flip_horizontal(A2Methods_UArray2 original, 
                                  A2Methods_T methods, 
                                  A2Methods_mapfun *map)
{
        assert(original != NULL && methods != NULL && map != NULL);
        
        int width = methods->width(original);
        int height = methods->height(original);
        int size = methods->size(original);
        int blocksize = methods->blocksize(original);
        
        A2Methods_UArray2 rotated_image;
        rotated_image = methods->new_with_blocksize(width, height, size, 
                                                                    blocksize);
   
        Mapping_cl rotation_cl = {rotated_image, methods};

        rotate_image(original, rotation_cl, &map_flip_horizontal, map, width, 
                                              height, "flip horizontal", size);

        assert(rotated_image != NULL);
        return rotated_image;
}


/* flip_vertical: Reflects the original image about the horizontal axis
*  Expectations:  It is a checked runtime error for original to be NULL. 
*                 It is also a checked runtime error for methods and map to
*                 be NULL.
*                 The return value is the array flipped vertically
*/
A2Methods_UArray2 flip_vertical(A2Methods_UArray2 original, 
                                A2Methods_T methods, 
                                A2Methods_mapfun *map)
{
        assert(original != NULL && methods != NULL && map != NULL);

        int width = methods->width(original);
        int height = methods->height(original);
        int size = methods->size(original);
        int blocksize = methods->blocksize(original);
        
        A2Methods_UArray2 rotated_image;
        rotated_image = methods->new_with_blocksize(width, height, size, 
                                                                    blocksize);
   
        Mapping_cl rotation_cl = {rotated_image, methods};

        rotate_image(original, rotation_cl, &map_flip_vertical, map, width, 
                                                height, "flip vertical", size);

        assert(rotated_image != NULL);
        return rotated_image;
}


/* transpose:    Transposes the image across UL-to-LR axis
*  Expectations: It is a checked runtime error for original to be NULL. 
*                It is also a checked runtime error for methods and map to
*                be NULL.
*                The return value is the array transposed across UL-to-LR axis
*/
A2Methods_UArray2 transpose(A2Methods_UArray2 original, A2Methods_T methods, 
                                                         A2Methods_mapfun *map)
{
        assert(original != NULL && methods != NULL && map != NULL);

        A2Methods_UArray2 rotated_image;

        int width = methods->width(original);
        int height = methods->height(original);
        int size = methods->size(original);
        int blocksize = methods->blocksize(original);
        
        rotated_image = methods->new_with_blocksize(height, width, size, 
                                                                    blocksize);
   
        Mapping_cl rotation_cl = {rotated_image, methods};

        rotate_image(original, rotation_cl, &map_transpose, map, width, height,
                                                            "transpose", size);

        assert(rotated_image != NULL);
        return rotated_image;
}