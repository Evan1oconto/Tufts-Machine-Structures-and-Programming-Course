// /*
//  * Handler.h
//  * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
//  * Date:         11/20/2020
//  * Assignment:   A Universal Machine
//  * Interface for initializing and handling the memory in our program.  
// */

// #include <stdio.h>
// #include <stdint.h>
// #include <stdlib.h>

#ifndef HANDLER_H
#define HANDLER_H


// typedef struct Stack {
//         uint32_t *removed_segs;
//         int size;
//         int length;
// } *Stack;


// typedef struct Seg {
//         uint32_t *words;
//         int length;
// } *Seg;


// typedef struct Seg_handler {
//         Seg *memory;
//         Stack unmapped;
//         int counter;
//         int length;
//         int size;
// } *Seg_handler;

// //void stack_free(Stack *unmapped);
// //void stack_push(Stack unmapped, uint32_t word);
// //uint32_t stack_pop(Stack unmapped);
// Stack stack_new();

// //Seg_handler initialize(FILE *fp);

// void free_memory(Seg_handler *mem);

// //uint32_t append_segment(Seg_handler memory, Seq_T segment);
// uint32_t append_segment(Seg_handler mem, Seg segment);

// void rem_segment(Seg_handler mem, uint32_t index);

// int get_length(FILE *fp);

// //void extend_mem_size(Seg_handler mem);

#endif 