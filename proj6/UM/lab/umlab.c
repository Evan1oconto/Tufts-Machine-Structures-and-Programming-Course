/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.
 *
 * Any additional functions and unit tests written for the lab go
 * here.
 *
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;




/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        //uint32_t val = Um_opcode[op];
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, op);
        word = Bitpack_newu(word, 3, 0, rc);
        word = Bitpack_newu(word, 3, 3, rb);
        word = Bitpack_newu(word, 3, 6, ra);

        return word;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, 13);
        word = Bitpack_newu(word, 3, 25, ra);
        word = Bitpack_newu(word, 25, 0, val);
        return word;
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;
/* Wrapper functions for each of the instructions */

static inline Um_instruction cmove(Um_register a, Um_register b, Um_register c)
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction seg_load(Um_register a, Um_register b, 
                                      Um_register c)
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction seg_store(Um_register a, Um_register b, 
                                       Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c)
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction mult(Um_register a, Um_register b, Um_register c)
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction div(Um_register a, Um_register b, Um_register c)
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c)
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction halt(void)
{
        return three_register(HALT, 0, 0, 0);
}

static inline Um_instruction map_seg(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap_seg(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction loadProg(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}



/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }

}


/* Unit tests for the UM */

void build_cmove_test(Seq_T stream)
{
        append(stream, loadval(r1, 'c'));
        append(stream, loadval(r2, 'd'));
        append(stream, loadval(r3, 0));
        append(stream, cmove(r1, r2, r3));
        append(stream, output(r1));

        append(stream, loadval(r1, 'c'));
        append(stream, loadval(r2, 'd'));
        append(stream, loadval(r3, 5));
        append(stream, cmove(r1, r2, r3));
        append(stream, output(r1));
        
        append(stream, loadval(r4, '\n'));
        append(stream, output(r4));

        append(stream, halt());
}


void build_seg_load_store_test(Seq_T stream) {
        append(stream, loadval(r2, 3));
        append(stream, loadval(r3, 33));
        append(stream, map_seg(r2, r3));

        for(int i = 0; i < 10; i++) {
                append(stream, loadval(r3, i));
                append(stream, loadval(r1, 50+i));
                append(stream, seg_store(r2, r3, r1));
        }

        for(int i = 0; i < 10; i++) {
                append(stream, loadval(r3, i));
                append(stream, seg_load(r1, r2, r3));
                append(stream, output(r1));
        }

        append(stream, loadval(r4, '\n'));
        append(stream, output(r4));

        append(stream, halt());
}

void build_unmap_test(Seq_T stream) {
        append(stream, loadval(r3, 5));
        append(stream, loadval(r1, 65));
        
        for (int i = 1; i < 10; i++) {
                append(stream, loadval(r0, i));
                append(stream, map_seg(r0, r3));
                append(stream, loadval(r4, 0));
                append(stream, seg_store(r0, r4, r1));
        }
        
        for(int i = 1; i < 9; i++) {
                append(stream, loadval(r0, i));
                append(stream, unmap_seg(r0));
        }
        
        append(stream, loadval(r1, 50));
        for (int i = 1; i < 9; i++) {
                append(stream, loadval(r0, i));
                append(stream, map_seg(r0, r3));
                append(stream, loadval(r4, 0));
                append(stream, seg_store(r0, r4, r1));
        }

        append(stream, loadval(r6, 9));
        append(stream, seg_load(r7, r6, r4));
        append(stream, output(r7));

        append(stream, loadval(r0, 3));
        append(stream, loadval(r4, 0));
        append(stream, seg_load(r1, r0, r4));
        append(stream, output(r1));
       
        append(stream, loadval(r6, '\n'));
        append(stream, output(r6));

        append(stream, halt());
}


void build_input_test(Seq_T stream)
{
        append(stream, loadval(r0, 1));
        append(stream, loadval(r1, 10));
        append(stream, map_seg(r0, r1));

        for(int i = 0; i < 10; i++) {
                append(stream, input(r2));
                append(stream, loadval(r1, i));
                append(stream, seg_store(r0, r1, r2));
        }

        for(int i = 0; i < 10; i++) {
                append(stream, loadval(r1, i));
                append(stream, seg_load(r2, r0, r1));
                append(stream, output(r2));
        }

        append(stream, loadval(r2, '\n'));
        append(stream, output(r2));
        append(stream, halt());

}

void build_add_test(Seq_T stream)
{
        append(stream, loadval(r0, 52));
        append(stream, loadval(r1, 7));
        append(stream, add(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 6));
        append(stream, loadval(r1, 99));
        append(stream, add(r2, r1, r0));
        append(stream, output(r2)); 

        append(stream, loadval(r0, 73));
        append(stream, loadval(r1, 40));
        append(stream, add(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 33));
        append(stream, loadval(r1, 50));
        append(stream, add(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 75));
        append(stream, loadval(r1, 33));
        append(stream, add(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 111));
        append(stream, loadval(r1, 0));
        append(stream, add(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 1));
        append(stream, loadval(r1, 9));
        append(stream, add(r2, r1, r0));
        append(stream, output(r2));

        append(stream, halt());
}

void build_mult_test(Seq_T stream)
{
        append(stream, loadval(r0, 1));
        append(stream, loadval(r1, 99));
        append(stream, mult(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 35));
        append(stream, loadval(r1, 3));
        append(stream, mult(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 18));
        append(stream, loadval(r1, 6));
        append(stream, mult(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 2));
        append(stream, loadval(r1, 54));
        append(stream, mult(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 10));
        append(stream, loadval(r1, 10));
        append(stream, mult(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 4));
        append(stream, loadval(r1, 21));
        append(stream, mult(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 2));
        append(stream, loadval(r1, 5));
        append(stream, mult(r2, r1, r0));
        append(stream, output(r2));

        append(stream, halt());
}

void build_divide_test(Seq_T stream)
{
        append(stream, loadval(r0, 2));
        append(stream, loadval(r1, 200));
        append(stream, div(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 2));
        append(stream, loadval(r1, 222));
        append(stream, div(r2, r1, r0));
        append(stream, output(r2));


        append(stream, loadval(r0, 1));
        append(stream, loadval(r1, 99));
        append(stream, div(r2, r1, r0));
        append(stream, output(r2));


        append(stream, loadval(r0, 10));
        append(stream, loadval(r1, 800));
        append(stream, div(r2, r1, r0));
        append(stream, output(r2));

        append(stream, loadval(r0, 2));
        append(stream, loadval(r1, 20));
        append(stream, div(r2, r1, r0));
        append(stream, output(r2));

        append(stream, halt());
}

void build_nand_test(Seq_T stream)
{
        append(stream, loadval(r0, 0));
        append(stream, loadval(r1, 64));
        append(stream, nand(r0, r0, r0));
        append(stream, nand(r1, r1, r1));
        append(stream, nand(r3, r0, r1));
        append(stream, output(r3));

        append(stream, loadval(r4, '\n'));
        append(stream, output(r4));

        append(stream, halt());
}

void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());

        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}

void build_load_val_test(Seq_T stream)
{

        append(stream, loadval(r1, 'G'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'o'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'o'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
        append(stream, halt());
}


void build_print_six(Seq_T stream)
{
        append(stream, loadval(r1, 1));
        append(stream, loadval(r2, 66));
        append(stream, loadval(r3, 6));
        append(stream, add(r1, r2, r3));
        append(stream, output(r1));

        append(stream, loadval(r4, '\n'));
        append(stream, output(r4));

        append(stream, halt());
}





void build_map_seg_test(Seq_T stream)
{
        append(stream, loadval(r2, 1));
        append(stream, loadval(r3, 8));
        append(stream, map_seg(r2, r3));

        append(stream, loadval(r4, 2));
        append(stream, loadval(r5, 22));
        append(stream, map_seg(r4, r5));

        append(stream, loadval(r6, 12));
        append(stream, loadval(r7, 1000));
        append(stream, map_seg(r6, r7));

        append(stream, loadval(r4, 2));
        append(stream, loadval(r5, 98));
        append(stream, map_seg(r4, r5));
        
        append(stream, loadval(r2, 1));
        append(stream, loadval(r3, 1008));
        append(stream, map_seg(r2, r3));

        append(stream, halt());
}


void build_load_prog_test(Seq_T stream) 
{

        append(stream, loadval(r1, 50));
        append(stream, loadval(r0, 0));
        append(stream, loadval(r5, 5));
        append(stream, loadProg(r0, r5));
        append(stream, output(r1));
        append(stream, halt());
        append(stream, loadval(r5,4)); 
        append(stream, loadProg(r0, r5));  

        append(stream, loadval(r4, 10));
        append(stream, loadval(r0, 2));
        append(stream, loadval(r1, 1));
        
        append(stream, map_seg(r1, r0));  
        append(stream, loadval(r2, 0));
   
    
}