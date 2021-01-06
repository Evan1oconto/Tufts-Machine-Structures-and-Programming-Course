/*

 * execute.h
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         11/20/2020
 * Assignment:   A Universal Machine

 * Interface for decoding and executing the words passed in by the .um file.  

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bitpack.h"
#include "seq.h"
#include "segment.h"
#include "Handler.h"


#ifndef EXECUTE_H
#define EXECUTE_H


void decode(uint32_t word, Seg_handler mem);

void execute(Seg_handler mem);

void cond_move(uint32_t a, uint32_t b, uint32_t c);

void add(uint32_t a, uint32_t b, uint32_t c);

void mult(uint32_t a, uint32_t b, uint32_t c);

void division(uint32_t a, uint32_t b, uint32_t c);

void nand(uint32_t a, uint32_t b, uint32_t c);

void halt(Seg_handler mem);

void seg_load(Seg_handler mem, uint32_t a, uint32_t b, uint32_t c);

void seg_store(Seg_handler mem, uint32_t a, uint32_t b, uint32_t c);

void map_seg(Seg_handler mem, uint32_t b, uint32_t c);

void unmap_seg(Seg_handler mem, uint32_t c);

void input(FILE *fp, uint32_t c);

void output(uint32_t c);

void load_prog(Seg_handler mem, uint32_t b, uint32_t c);


#endif 