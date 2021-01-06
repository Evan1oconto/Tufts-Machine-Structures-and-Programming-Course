/*
 * segment.c
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         11/20/2020
 * Assignment:   A Universal Machine
 * Implementation for handling the segments in our UM.  
*/

// #include "segment.h"
// #include "assert.h"


/* new_segment
*   Arguments: length of new segment
*   Return: sequence which represents the segment
*   Description: Create a new Hanson sequence and preallocate with 0's
*   Assumptions: none
*/
// Seg new_segment(int length)
// {

//         Seg segment = malloc(sizeof(*segment));
//         segment->length = length;
//         segment->words = malloc(length * sizeof(uint32_t));

//         for (int i = 0; i < length; i++) {
//                 segment->words[i] = 0;
//         }
//         return segment;
// }

/* free_segment
*   Arguments: sequence of the segment to be freed
*   Return: none
*   Description: frees segment 
*   Assumptions: none
*/
// void free_segment(Seg segment)
// {
//         assert(segment != NULL);
//         free(segment->words);
//         free(segment);
// }



/* duplicate_segment
*   Arguments:  the struct of memory and index of segment in memory
*   Return: none
*   Description: make a copy of a segment and place in 0th position segment
*   Assumptions: none
*/
// void duplicate_segment(Seg_handler mem, int index)
// {
       
//         Seg temp = (mem->memory)[index];
//         int len = temp->length;
//         Seg new_seg = new_segment(len); 
//         //printf("%d\n", len);
//         for (int i = 0; i < len; i++){
//                 new_seg->words[i] = temp->words[i];
//                 //Seq_put(new_seg, i, get_word(temp, i));
                
//         }
//         new_seg->length = temp->length;
//         free_segment((mem->memory)[0]);
        
//         /* place into 0th position */
//         (mem->memory)[0] = new_seg;

// }