// /*
//  * Handler.c
//  * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
//  * Date:         11/20/2020
//  * Assignment:   A Universal Machine
//  * Implementation for initializing and handling the memory in our program.
// */
// #include "Handler.h"
// #include "segment.h"
// #include "assert.h"

// // #define BYTE 8

// // static inline uint32_t stack_pop(Stack unmapped) 
// // {
// //         unmapped->length = (unmapped->length - 1);
// //         return (unmapped->removed_segs)[unmapped->length];
// // }


// // static inline void stack_push(Stack unmapped, uint32_t seg)
// // {
// //         int len = unmapped->length;
// //         int size = unmapped->size;
// //         if (len == size) {
// //                 unmapped->size = unmapped->size * 2;
// //                 size = unmapped->size;
// //                 uint32_t *temp = malloc(sizeof(uint32_t) * size);

// //                 for (int i = 0; i < len; i++) {
// //                         temp[i] = (unmapped->removed_segs)[i];
// //                 }
// //                 free(unmapped->removed_segs);
// //                 unmapped->removed_segs = temp;
                
// //         }
// //         (unmapped->removed_segs)[len] = seg;
// //         (unmapped->length)++;
// // }

// // static inline void stack_free(Stack *unmapped)
// // {
// //         free((*unmapped)->removed_segs);
// //         free(*unmapped);
// // }



// /* free_memory
// *   Arguments: a pointer to the memory struct
// *   Return: none
// *   Description: frees all the memory related to the Seg_handler struct
// *   Assumptions: mem is not equal to null
// */
// void free_memory(Seg_handler *mem)
// {
//         assert(mem != NULL);
        
//         int len = (*mem)->length;
//         for(int i = 0; i < len; i++) {
//                 //free_segment(Seq_get((*mem)->memory, i));
//                 free_segment(((*mem)->memory)[i]);
//         }
        
//         stack_free(&(*mem)->unmapped);
//         //Seq_free(&(*mem)->memory);
//         free((*mem)->memory);
//         free(*mem);
// }

// // static inline void extend_mem_size(Seg_handler mem)
// // {
// //         int len = mem->length;
// //         mem->size = mem->size * 2;
// //         int size = mem->size;
// //         Seg *temp = malloc(sizeof(Seg) * size);

// //         for (int i = 0; i < len; i++) {
// //                 temp[i] = (mem->memory)[i];
// //                 //free_segment((mem->memory)[i]);
// //         }
// //         free(mem->memory);
// //         mem->memory = temp;

// //         // mem->size = mem->size * 2;
// //         // mem->memory = realloc(mem->memory, mem->size * sizeof(Seg));
// // }

// /* append_segment
// *   Arguments: The memory struct and a segment to be added
// *   Return: A uint32_t representing the index of the newly appended segment
// *   Description: Adds a segment to a certain position in the memory struct.
// *                The position which the segment is added to will either be an 
// *                unmapped segment's position or added to the end of the memory
// *                sequence.  
// *   Assumptions: mem is not null
// */

// //uint32_t append_segment(Seg_handler mem, Seq_T segment)
// uint32_t append_segment(Seg_handler mem, Seg segment)
// {
//         assert(mem != NULL);

//         uint32_t index;

//         /* checks if stack holding the umapped values is empty */
//         if ((mem->unmapped->length) == 0) {
//                 int len = mem->length;
//                 int size = mem->size;
//                 if (len == size) {
//                         mem->size = mem->size * 2;
//                         size = mem->size;
//                         Seg *temp = malloc(sizeof(Seg) * size);

//                         for (int i = 0; i < len; i++) {
//                                 temp[i] = (mem->memory)[i];
//                         }
//                         free(mem->memory);
//                         mem->memory = temp;
//                 }
                
//                 //Seq_addhi(mem->memory, (void *) segment);
//                 (mem->memory)[len] = segment;
//                 mem->length++;
//                 index = mem->length - 1;
        
//         /* if the stack holding the umapped values isn't empty, it will have
//            the segment mapped to the most recent unmapped segment specified */
//         } else {
//                 //index = (int)(uintptr_t) Stack_pop(mem->unmapped);
//                 index = stack_pop(mem->unmapped);
//                 free_segment((mem->memory)[index]);
//                 (mem->memory)[index] = segment;
              
//         }

//         return index;
// }


// /* rem_segment
// *   Arguments: The memory struct and the index of the unmapped segment
// *   Return: none
// *   Description: it will push the index of an unmapped segment onto 
// *                the unmapped segment stack (found in the memory struct)
// *   Assumptions: mem is not null
// */
// void rem_segment(Seg_handler mem, uint32_t index)
// {
//         assert(mem != NULL);
//         //Stack_push(mem->unmapped, (void *) (uintptr_t) index);
//         stack_push(mem->unmapped, index);
// }

// /* get_length
// *   Arguments: file pointer
// *   Return: none
// *   Description: gets the legnth of the file so the Segment length can be 
// *                initialized beforehand 
// *   Assumptions: number of chars is divisible by 4 and fseek is valid
// */
// int get_length(FILE *fp)
// {
//         assert(fseek(fp, 0, SEEK_END) != -1);
//         int length = ftell(fp);
//         fseek(fp, 0, SEEK_SET);

//         if (length % 4 != 0 || length == 0) {
//                 fprintf(stderr, "ERROR: Invalid file content\n");
// 		exit(EXIT_FAILURE);
//         }
//         return (length/4);
// }



// Stack stack_new()
// {
//         Stack unmapped = malloc(sizeof(*unmapped));
//         unmapped->removed_segs = malloc(10 * sizeof(uint32_t));
//         unmapped->size = 10;
//         unmapped->length = 0;
//         return unmapped;
// }

