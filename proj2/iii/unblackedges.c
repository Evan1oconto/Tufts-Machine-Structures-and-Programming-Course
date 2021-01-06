/*

 * unblackedges.c
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/28/2020
 * Assignment:   iii

 * Prints to stdout the pbm image in plain pbm format after changing the 
 * black edge pixels to white.

*/

#include <stdio.h>
#include <stdlib.h>
#include "pnmrdr.h"
#include "assert.h"
#include "bit2.h"
#include "seq.h"
#include "filehandler.h"


#define WHITE 0
#define BLACK 1



struct Pixel {
        int col; /* the column position of the pixel */
        int row; /* the row position of the pixel */
};

typedef struct Pixel Pixel;


/*--    Function declarations     --*/
void checkImageProperties(Pnmrdr_T reader);
Pixel *newPixel(int col, int row); 
void removeBlackEdges(Bit2_T BitArray2);
int isEdge(int col, int row, int width, int height);
void findBlackEdges(int col, int row, Bit2_T BitArray2, int bit, void *cl);
void populateQueue(Seq_T queue, Bit2_T *image);
void addToQueue(Pixel *neighbor, Seq_T *queue);
void storeInArray(int i, int j, Bit2_T image, int bit, void *cl);
Bit2_T pbmread(Pnmrdr_T reader);
void storeInArray(int i, int j, Bit2_T image, int bit, void *cl);
void pbmwrite(FILE *outputfp, Bit2_T bitmap);
/*--                              --*/





int main(int argc, char **argv) 
{
        FILE *fp = NULL;

        /* validating the number of command line arguments supplied */
        assert(argc <= 2);
        if (argc == 2) {
                fp = getFileStream(argv[1]);
        } else {
                fp = getFileStream(NULL);
        }

        Pnmrdr_T reader;
        Bit2_T image;
  
        /* read the image data and populate the array with the intensity */
        reader = Pnmrdr_new(fp);
        checkImageProperties(reader);
        image = pbmread(reader);

        closeFileStream(fp);    
        Pnmrdr_free(&reader);

        removeBlackEdges(image);
        pbmwrite(stdout, image);


        Bit2_free(&image);
        return EXIT_SUCCESS;
}






/* checkImageProperties: Checks to make sure the Pbm file is valid
*  Expectations:         The reader is not null, and the file is a portable 
                         bitmap file.
*/
void checkImageProperties(Pnmrdr_T reader)
{
        assert(reader != NULL);

        Pnmrdr_mapdata map_data = Pnmrdr_data(reader);
        assert(map_data.type == Pnmrdr_bit); /* Ensuring that the file is 
                                                  Portable Bitmap */
        assert(map_data.height > 0 && map_data.width > 0);
}






/* newPixel:       Takes in a column and row position and returns the newly 
                   allocated pixel.
*  Expectations:   The output is expected to be a Pixel * that contains 
                   pixel data whose coordinates are the ones supplied in the 
                   arguments.
*/
Pixel *newPixel(int col, int row) 
{
        Pixel *pixel_p = malloc(sizeof(*pixel_p));
        assert(pixel_p != NULL);

        pixel_p->col = col;
        pixel_p->row = row;

        return pixel_p;
}






/* removeBlackEdges: Takes in a the Bit array and calls the appropriate 
                     functions to remove the black edges in the image.
*  Expectations:     Image is expected to be a non-null. It does not return 
                     anything.
*/
void removeBlackEdges(Bit2_T image)
{
        assert(image != NULL);
        
        int width = Bit2_width(image);
        int height = Bit2_height(image);
        Seq_T queue = Seq_new(width * height);
        Bit2_map_row_major(image, &findBlackEdges, &queue);

        /* populates and queue, performs bfs and changes black edge 
           pixels to white in the array */
        populateQueue(queue, &image);
        Seq_free(&queue);
}




/* populateQueue:  Takes in a queue and populates it with the black edge values
                   from the image
*  Expectations:   The queue and the image are expected to be non-null. Also
                   *image is expected to be non-null.
*/
void populateQueue(Seq_T queue, Bit2_T *image)
{
        assert(queue != NULL && image != NULL && *image != NULL);
        while (Seq_length(queue) != 0) {
                Pixel *pixel_p = (Pixel *)Seq_remlo(queue);
                int col = pixel_p->col;
                int row = pixel_p->row;
                if (Bit2_get(*image, col, row) == BLACK) {
                        /* make it white, add black neighbors to the queue */
                        Bit2_put(*image, pixel_p->col, pixel_p->row, WHITE);

                        if (pixel_p->col - 1 >= 0 && 
                                     Bit2_get(*image, col - 1, row) == BLACK) {
                                Pixel *left_neighbor = newPixel(col - 1, row);
                                addToQueue(left_neighbor, &queue);
                        } 

                        if (pixel_p->col + 1 < Bit2_width(*image) && 
                                     Bit2_get(*image, col + 1, row) == BLACK) {
                                Pixel *right_neighbor = newPixel(col + 1, row);
                                addToQueue(right_neighbor, &queue);
                        } 

                        if (pixel_p->row + 1 < Bit2_height(*image) && 
                                     Bit2_get(*image, col, row + 1) == BLACK) {
                                Pixel *bottom_neighbor = newPixel(col,row + 1);
                                addToQueue(bottom_neighbor, &queue);
                        } 

                        if (pixel_p->row - 1 >= 0 && 
                                     Bit2_get(*image, col, row - 1) == BLACK) {
                                Pixel *top_neighbor = newPixel(col, row - 1);
                                addToQueue(top_neighbor, &queue);
                        } 

                }
                free(pixel_p);
        }
}





/* addToQueue:   Adds a pixel value to the given queue
*  Expectations: The queue and *queue are expected to be non-null. This 
                 function does not return anything as output.
*/
void addToQueue(Pixel *neighbor, Seq_T *queue)
{
        assert(queue != NULL && *queue != NULL);

        Seq_addhi(*queue, (void *)neighbor);
}




/* findBlackEdges: Finds the black edges in the border of the image supplied 
                   (the third parameter) and stores the pixel data in 
                   the sequence (the fifth parameter)
*  Expectations:   The image is expected to be non-null. This does not 
                   return anything.
*/
void findBlackEdges(int col, int row, Bit2_T BitArray2, int bit, void *cl)
{        
        assert(BitArray2 != NULL);

        Seq_T *queue = cl;

        int width = Bit2_width(BitArray2);
        int height = Bit2_height(BitArray2);
        
        if (isEdge(col, row, width, height)) {
                if (bit == BLACK) {
                        Pixel *pixel_p = newPixel(col, row);
                        Seq_addhi(*queue, (void *)pixel_p);
                }
        }

}






/* isEdge:       Checks if the pixel at the given coordinates are at the 
                 edge of the image or not.
*  Expectations: The col, row, width, and height are expected to be within the
                 bounds of the bit array. It returns 1 if the coordinates are
                 at the edge of the image, otherwise returns 0.
*/
int isEdge(int col, int row, int width, int height)
{
        if (col == 0 || row == 0 || 
                        col == (width - 1) || row == (height - 1)) {
                return 1;
        }
        return 0;
}




/* pbmwrite:       Prints to outputfp (the first parameter) the pbm image 
                   stored in bitmap (the second parameter)
*  Expectations:   outputfp is expected to be non-null and bitmap is expected
                   to be non-null. The function does not return anything.
*/
void pbmwrite(FILE *outputfp, Bit2_T bitmap)
{ 
        fprintf(outputfp, "P1\n");
        int col = Bit2_width(bitmap);
        int row = Bit2_height(bitmap);
        fprintf(outputfp, "# Image of width %d and height %d\n", col, row);
        fprintf(outputfp, "%d %d\n", col, row);
        for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                        if (j % 69 == 0 && j != 0) {
                                fprintf(outputfp, "\n");
                        }
                        fprintf(outputfp, "%d", Bit2_get(bitmap, j, i));
                }
                fprintf(outputfp, "\n");
        }
}



/* pbmread:        Stores the bit in the reader (first parameter) to a 
                   Bit2 instance and returns the newly created Bit2 instance.
*  Expectations:   The reader is non-null.
*/
Bit2_T pbmread(Pnmrdr_T reader)
{
        assert(reader != NULL);

        Pnmrdr_mapdata map_data = Pnmrdr_data(reader); 
        Bit2_T image = Bit2_new(map_data.width, map_data.height);
        Bit2_map_row_major(image, &storeInArray, &reader);

        return image;
}


/* storeInArray:   Stores the bit at (col, row) position 
                   in the image (the third parameter) Bit2 instance. 
*  Expectations:   image is expected to be non-null.
*/
void storeInArray(int col, int row, Bit2_T image, int bit, void *cl)
{
        assert(image != NULL);

        (void) bit;
        Pnmrdr_T *reader = cl;

        int intensity = Pnmrdr_get(*reader);
        assert(intensity == BLACK || intensity == WHITE); /* sanity check */
        
        /* store the intensity in the image */
        Bit2_put(image, col, row, intensity);
}
