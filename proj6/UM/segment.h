/*

 * segment.h
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         11/20/2020
 * Assignment:   A Universal Machine

 * Interface for handling the segments in our UM.  

*/


#include <stdio.h>
#include <stdlib.h>
#include "bitpack.h"
#include "seq.h"
#include "Handler.h"


#ifndef SEGMENT_H
#define SEGMENT_H

Seq_T new_segment(int length);

void free_segment(Seq_T segment);

uint32_t get_word(Seq_T segment, uint32_t index);

void add_word(Seq_T segment, uint32_t codeword);

void duplicate_segment(Seg_handler mem, int index);

#endif 
