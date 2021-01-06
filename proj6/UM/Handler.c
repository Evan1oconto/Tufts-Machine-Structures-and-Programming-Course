/*

 * Handler.c
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         11/20/2020
 * Assignment:   A Universal Machine

 * Implementation for initializing and handling the memory in our program.

*/
#include "Handler.h"
#include "segment.h"
#include "assert.h"

#define BYTE 8

/* initialize
*   Arguments: file
*   Return: seg_handler representing memory
*   Description: Initialize a Sequence of Sequences and read in the codewords
*                into memory.
*   Assumptions: the registers are valid
*/
Seg_handler initialize(FILE *fp)
{

        /* initialize struct */
        int hint = 2;
        Seg_handler mem = malloc(sizeof(*mem));
        mem->memory = Seq_new(hint);
        mem->unmapped = Stack_new();
        mem->counter = 0;
        
        /* get length of segment populating */
        int length = get_length(fp);
        Seq_T segment_0 = Seq_new(length);

        /* read words into memory */
        uint32_t byte = 0;
        uint32_t word = 0;

        byte = getc(fp);
        while (byte != (uint32_t) EOF) {
                word = 0;

                for (int i = 3; i >= 0; i--) {
                        word = (uint32_t)Bitpack_newu(word, BYTE, BYTE * i,
                                                      byte);
                        byte = getc(fp);
                }

                add_word(segment_0, word);
                word = 0;
        }

        Seq_addlo(mem->memory, (void *)segment_0);

        return mem;
}


/* free_memory
*   Arguments: a pointer to the memory struct
*   Return: none
*   Description: frees all the memory related to the Seg_handler struct
*   Assumptions: mem is not equal to null
*/
void free_memory(Seg_handler *mem)
{
        assert(mem != NULL);
        
        for(int i = 0; i < Seq_length((*mem)->memory); i++){
                free_segment(Seq_get((*mem)->memory, i));
        }
        
        Stack_free(&(*mem)->unmapped);
        Seq_free(&(*mem)->memory);
        free(*mem);
}


/* append_segment
*   Arguments: The memory struct and a segment to be added
*   Return: A uint32_t representing the index of the newly appended segment
*   Description: Adds a segment to a certain position in the memory struct.
*                The position which the segment is added to will either be an 
*                unmapped segment's position or added to the end of the memory
*                sequence.  
*   Assumptions: mem is not null
*/
uint32_t append_segment(Seg_handler mem, Seq_T segment)
{
        assert(mem != NULL);

        uint32_t index;

        /* checks if stack holding the umapped values is empty */
        if (Stack_empty(mem->unmapped) == 1) {
                Seq_addhi(mem->memory, (void *) segment);
                index = (uint32_t)Seq_length(mem->memory) - 1;
        
        /* if the stack holding the umapped values isn't empty, it will have
           the segment mapped to the most recent unmapped segment specified */
        } else {
                index = (uintptr_t) Stack_pop(mem->unmapped);
                free_segment(Seq_put(mem->memory, index, (void *) segment));
        }

        return index;
}


/* rem_segment
*   Arguments: The memory struct and the index of the unmapped segment
*   Return: none
*   Description: it will push the index of an unmapped segment onto 
*                the unmapped segment stack (found in the memory struct)
*   Assumptions: mem is not null
*/
void rem_segment(Seg_handler mem, uint32_t index)
{
        assert(mem != NULL);
        Stack_push(mem->unmapped, (void *) (uintptr_t) index);
}

/* get_length
*   Arguments: file pointer
*   Return: none
*   Description: gets the legnth of the file so the Segment length can be 
*                initialized beforehand 
*   Assumptions: number of chars is divisible by 4 and fseek is valid
*/
int get_length(FILE *fp)
{
        assert(fseek(fp, 0, SEEK_END) != -1);
        int length = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if (length % 4 != 0 || length == 0) {
                fprintf(stderr, "ERROR: Invalid file content\n");
		exit(EXIT_FAILURE);
        }
        return (length/4);
}