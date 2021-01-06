/*

 * Handler.h
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         11/20/2020
 * Assignment:   A Universal Machine

 * Interface for initializing and handling the memory in our program.  

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "seq.h"
#include "stack.h"

#ifndef HANDLER_H
#define HANDLER_H


typedef struct Seg_handler {
        Seq_T memory;
        Stack_T unmapped;
        int counter;
} *Seg_handler;

Seg_handler initialize(FILE *fp);

void free_memory(Seg_handler *mem);

uint32_t append_segment(Seg_handler memory, Seq_T segment);

void rem_segment(Seg_handler mem, uint32_t index);

int get_length(FILE *fp);


#endif 