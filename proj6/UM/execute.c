/*

 * execute.c
 * Authors:      Edward Alf (ealf01), Evan Loconto (elocon01)
 * Date:         11/20/2020
 * Assignment:   A Universal Machine

 * Implementation for decoding and executing the words passed in by the
 * .um file.  

*/

#include "execute.h"
#include "assert.h"

/* constant decllarations */
#define OP_SIZE  4
#define OP_LSB   28
#define REG_SIZE 3
#define VAL_SIZE 25
#define MAX      4294967296

/* global variable for the registers here */
uint32_t r[8] = {0};

/* enum used for readability for the switch statement */
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, MSEG, UMSEG, OUT, IN, LOADP, LV
} Um_opcode;


/* decode
*   Arguments: The Seg_handler struct, and the uint32_t codeword
*   Return: none
*   Description: takes a codeword, decodes it and the calls the appropriate 
*                operation based on the operation the codeword represents.
*   Assumptions: the word is valid and the Seg_handler struct is not null.
*/
void decode(uint32_t word, Seg_handler mem)
{
        uint32_t operation = Bitpack_getu(word, OP_SIZE, OP_LSB);
        assert(operation <= 13);

        mem->counter++;

        /* since load value is a special case, has its own conditional */
        if (operation == LV) {
                uint32_t reg_lsb = OP_LSB - REG_SIZE;
                uint32_t reg_a = Bitpack_getu(word, REG_SIZE, reg_lsb);

                uint32_t value = Bitpack_getu(word, VAL_SIZE, 0);

                r[reg_a] = value;

        /* else statement for the 12 other operations */
        } else {
                uint32_t reg_c = Bitpack_getu(word, REG_SIZE, 0);
                uint32_t reg_b = Bitpack_getu(word, REG_SIZE, REG_SIZE);
                uint32_t reg_a = Bitpack_getu(word, REG_SIZE, 2 * REG_SIZE);
 
                assert(reg_a < 8);
                assert(reg_b < 8);
                assert(reg_c < 8);
        
                /* switch statement calls necessary function based
                   on the operation that was read from the word     */
                switch( operation )
                {
                        case  CMOV :
                                cond_move(reg_a, reg_b, reg_c);
                                break;
                        case  SLOAD :
                                seg_load(mem, reg_a, reg_b, reg_c);
                                break;
                        case  SSTORE :
                                seg_store(mem, reg_a, reg_b, reg_c);
                                break;
                        case  ADD :
                                add(reg_a, reg_b, reg_c);
                                break;
                        case  MUL :
                                mult(reg_a, reg_b, reg_c);
                                break;
                        case  DIV :
                                division(reg_a, reg_b, reg_c);
                                break;
                        case  NAND :
                                nand(reg_a, reg_b, reg_c);
                                break;
                        case  HALT :
                                halt(mem);
                                break;
                        case  MSEG :
                                map_seg(mem, reg_b, reg_c);
                                break;
                        case  UMSEG :
                                unmap_seg(mem, reg_c);
                                break;
                        case OUT :
                                output(reg_c);
                                break;
                        case IN : 
                                input(stdin, reg_c);
                                break;
                        case LOADP :
                                load_prog(mem, reg_b, reg_c);
                                break;
                }

        }
}




/* execute
*   Arguments: The Seg_handler struct
*   Return: none
*   Description: Takes the program memory (via Seg_handler struct) and 
*                ultimately executes the program by calling the decode
                 function on each point specified by the counter.
*   Assumptions: the mem is not null.
*/
void execute(Seg_handler mem) 
{
        assert(mem != NULL);
        uint32_t seg_i = 0;
        int word_pos = mem->counter;
        while (1) {
                decode(get_word(Seq_get(mem->memory, seg_i), word_pos), mem);
                word_pos = mem->counter;
        }


}

/* cond_move
*   Arguments: three uint32_t's representing the register numbers
*   Return: none
*   Description: Performs the conditional move operation by moving the value 
*                in register B to register A if register C is not equal to 0.
*   Assumptions: the registers are valid
*/
void cond_move(uint32_t a, uint32_t b, uint32_t c)
{
        
        if (r[c] != 0){
            r[a] = r[b];
        }
}


/* add
*   Arguments: three uint32_t's representing the register numbers
*   Return: none
*   Description: Performs the add operation by adding together register B and 
*                register C and modding it by 2^32 and placing it in 
*                register A.
*   Assumptions: the registers are valid
*/
void add(uint32_t a, uint32_t b, uint32_t c)
{
        r[a] = (r[b] + r[c]) % MAX;        
}


/* mult
*   Arguments: three uint32_t's representing the register numbers
*   Return: none
*   Description: Performs multiplication on registers b and c and 
*                stores the result in register a. Mods by 2^32 to
*                fit in uint32
*   Assumptions: the registers are valid
*/
void mult(uint32_t a, uint32_t b, uint32_t c)
{
        r[a] = (r[b] * r[c]) % MAX;
}


/* division
*   Arguments: three uint32_t's representing the register numbers
*   Return: none
*   Description: Performs the division operation by dividing  register B and 
*                register C and placing it in register A.
*   Assumptions: the registers are valid
*/
void division(uint32_t a, uint32_t b, uint32_t c)
{
        r[a] = (r[b] / r[c]);
}


/* nand
*   Arguments: three uint32_t's representing the register numbers
*   Return: none
*   Description: Performs the nand operation by and-ing register B and 
*                register C and then not-ing it and placing it in register A.
*   Assumptions: the registers are valid
*/
void nand(uint32_t a, uint32_t b, uint32_t c)
{
        r[a] = ~(r[b] & r[c]);
}


/* halt
*   Arguments: memory struct
*   Return: none
*   Description: frees memory and quietly exits program
*   Assumptions: 
*/
void halt(Seg_handler mem)
{
        free_memory(&mem);
        exit(0);
}


/* seg_load
*   Arguments: three uint32_t's representing the register numbers and the 
*              memory struct
*   Return: none
*   Description: Performs the segment_load operation by getting the segment 
*                specified by register B and then getting the word specified by
*                register C, and then placing the word in register A.
*   Assumptions: the registers are valid
*/
void seg_load(Seg_handler mem, uint32_t a, uint32_t b, uint32_t c)
{
        Seq_T segment = (Seq_T) Seq_get(mem->memory, r[b]);
        r[a] = get_word(segment, r[c]);
}


/* seg_store
*   Arguments: three uint32_t's representing the register numbers and the 
*              memory struct
*   Return: none
*   Description: Performs the segment_store operation by getting the segment 
*                specified by register A and then getting the word specified by
*                register B, and then placing the value in register C into
*                the spot specified by register A and B.
*   Assumptions: the registers are valid
*/
void seg_store(Seg_handler mem, uint32_t a, uint32_t b, uint32_t c)
{
        Seq_T segment = (Seq_T) Seq_get(mem->memory, r[a]);
        Seq_put(segment, r[b],  (void *) (uintptr_t)r[c]);

}


/* map_seg
*   Arguments: memory struct and uint32_t's for registers b and c
*   Return: none
*   Description: creates a new segment and appends that segment to
*                 correct location in memory 
*   Assumptions: the registers are valid
*/
void map_seg(Seg_handler mem, uint32_t b, uint32_t c) 
{
        Seq_T map_seg = new_segment(r[c]);
        r[b] = append_segment(mem, map_seg);
}

/* unmap_seg
*   Arguments: memory struct and uint32_t's for register c
*   Return: none
*   Description: removes segment from memory by pushing its index 
*                onto the stack 
*   Assumptions: the registers are valid
*/
void unmap_seg(Seg_handler mem, uint32_t c) 
{
        rem_segment(mem, r[c]);
}

/*  input
*   Arguments: file and uint32_t's for register c
*   Return: none
*   Description: reads from an input file and stores codeword into r[c]
*   Assumptions: none
*/
void input(FILE *fp, uint32_t c)
{
        uint32_t user_in = getc(fp);
        if (user_in == (uint32_t) EOF) {
                user_in = -1;
        }
        r[c] = user_in;
}


/* output
*   Arguments: a uint32_t representing the register number
*   Return: none
*   Description: Performs the output operation by outputting the value in 
*                register C. 
*   Assumptions: the register is valid and is within the range 0 - 255
*/
void output(uint32_t c)
{
        assert(r[c] <= 255);
        putchar(r[c]);
}


/* load_prog
*   Arguments: two uint32_t's representing the register numbers and the memory
*              struct
*   Return: none
*   Description: Performs the load_program operation by updating the counter
*                position based on the value in register C, and then
*                duplicates and replaces the 0th segment by the segment 
*                specified by register B.
*   Assumptions: the registers are valid
*/
void load_prog(Seg_handler mem, uint32_t b, uint32_t c)
{
        mem->counter = r[c];

        /* checks if register B is 0, which speeds up the process */
        if (r[b] == 0) {
                return;
        }

        duplicate_segment(mem, r[b]);
}
