/*

 * segment.c
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         11/20/2020
 * Assignment:   A Universal Machine

 * Implementation for handling the segments in our UM.  

*/

#include "segment.h"
#include "assert.h"


/* new_segment
*   Arguments: length of new segment
*   Return: sequence which represents the segment
*   Description: Create a new Hanson sequence and preallocate with 0's
*   Assumptions: none
*/
Seq_T new_segment(int length)
{
        Seq_T segment = Seq_new(length);
        for (int i = 0; i < length; i++) {
                Seq_addhi(segment, (void *) (uintptr_t) 0);
        }
        return segment;
}

/* free_segment
*   Arguments: sequence of the segment to be freed
*   Return: none
*   Description: frees segment 
*   Assumptions: none
*/
void free_segment(Seq_T seg)
{
        assert(seg != NULL);
        Seq_free(&seg);
}

/* get_word
*   Arguments:  a sequence of the segment and index of word
*   Return: the codeword
*   Description: extract the codeword from a segment at a specified index
*   Assumptions: none
*/
uint32_t get_word(Seq_T segment, uint32_t index)
{
        return (uint32_t)(uintptr_t)Seq_get(segment, index);
}

/* add_word
*   Arguments:  a sequence of the segment and the codeword
*   Return: none
*   Description: add a codeword to the end of a segment
*   Assumptions: none
*/
void add_word(Seq_T segment, uint32_t codeword)
{
        Seq_addhi(segment, (void *)(uintptr_t)codeword);
}

/* duplicate_segment
*   Arguments:  the struct of memory and index of segment in memory
*   Return: none
*   Description: make a copy of a segment and place in 0th position segment
*   Assumptions: none
*/
void duplicate_segment(Seg_handler mem, int index)
{
        /* make copy of segment */
        Seq_T temp = Seq_get(mem->memory, index);
        Seq_T new_seg = new_segment(Seq_length(temp));

        for (int i = 0; i < Seq_length(temp); i++){
                Seq_put(new_seg, i, (void *)(uintptr_t)get_word(temp, i));
                
        }

        /* place into 0th position */
        Seq_put(mem->memory, 0, new_seg);

}