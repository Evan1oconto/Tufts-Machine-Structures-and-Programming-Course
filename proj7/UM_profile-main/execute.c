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

/* constant declarations */
#define OP_SIZE  4
#define OP_LSB   28
#define WORD_L   32
#define REG_SIZE 3
#define VAL_SIZE 25
#define BYTE     8

#define MAX      4294967296

/* global variable for the registers here */
uint32_t r[8] = {0};

/* enum used for readability for the switch statement */
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, MSEG, UMSEG, OUT, IN, LOADP, LV
} Um_opcode;

typedef struct Stack {
        uint32_t *removed_segs;
        int size;
        int length;
} *Stack;


typedef struct Seg {
        uint32_t *words;
        int length;
} *Seg;


typedef struct Seg_handler {
        Seg *memory;
        Stack unmapped;
        int counter;
        int length;
        int size;
} *Seg_handler;



static inline Seg new_segment(int length)
{
        Seg segment = malloc(sizeof(*segment));
        segment->length = length;
        segment->words = malloc(length * sizeof(uint32_t));
        

        for (int i = 0; i < length; i++) {
                segment->words[i] = 0;
        }
        return segment;
}

static inline void free_segment(Seg segment)
{
        assert(segment != NULL);
        free(segment->words);
        free(segment);
}


static inline Stack stack_new()
{
        Stack unmapped = malloc(sizeof(*unmapped));
        unmapped->removed_segs = malloc(10 * sizeof(uint32_t));
        unmapped->size = 10;
        unmapped->length = 0;
        return unmapped;
}


static inline uint32_t stack_pop(Stack unmapped) 
{
        unmapped->length = (unmapped->length - 1);
        return (unmapped->removed_segs)[unmapped->length];
}


static inline void stack_push(Stack unmapped, uint32_t seg)
{
        int len = unmapped->length;
        int size = unmapped->size;
        if (len == size) {
                unmapped->size = unmapped->size * 2;
                size = unmapped->size;
                uint32_t *temp = malloc(sizeof(uint32_t) * size);

                for (int i = 0; i < len; i++) {
                        temp[i] = (unmapped->removed_segs)[i];
                }
                free(unmapped->removed_segs);
                unmapped->removed_segs = temp;
                
        }
        (unmapped->removed_segs)[len] = seg;
        (unmapped->length)++;
}


static inline void stack_free(Stack *unmapped)
{
        free((*unmapped)->removed_segs);
        free(*unmapped);
}




static inline void free_memory(Seg_handler *mem)
{
        assert(mem != NULL);
        
        int len = (*mem)->length;
        for(int i = 0; i < len; i++) {
                free_segment(((*mem)->memory)[i]);
        }
        
        stack_free(&(*mem)->unmapped);
        free((*mem)->memory);
        free(*mem);
}


static inline uint32_t append_segment(Seg_handler mem, Seg segment)
{
        assert(mem != NULL);

        uint32_t index;

        /* checks if stack holding the umapped values is empty */
        if ((mem->unmapped->length) == 0) {
                int len = mem->length;
                int size = mem->size;
                if (len == size) {
                        mem->size = mem->size * 2;
                        size = mem->size;
                        Seg *temp = malloc(sizeof(Seg) * size);

                        for (int i = 0; i < len; i++) {
                                temp[i] = (mem->memory)[i];
                        }
                        free(mem->memory);
                        mem->memory = temp;
                }
                
                (mem->memory)[len] = segment;
                mem->length++;
                index = mem->length - 1;
        
        /* if the stack holding the umapped values isn't empty, it will have
           the segment mapped to the most recent unmapped segment specified */
        } else {
                index = stack_pop(mem->unmapped);
                free_segment((mem->memory)[index]);
                (mem->memory)[index] = segment;
              
        }

        return index;
}

static inline void rem_segment(Seg_handler mem, uint32_t index)
{
        assert(mem != NULL);
        stack_push(mem->unmapped, index);
}

/* get_length
*   Arguments: file pointer
*   Return: none
*   Description: gets the legnth of the file so the Segment length can be 
*                initialized beforehand 
*   Assumptions: number of chars is divisible by 4 and fseek is valid
*/
static inline int get_length(FILE *fp)
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






/* execute
*   Arguments: The Seg_handler struct
*   Return: none
*   Description: Takes the program memory (via Seg_handler struct) and 
*                ultimately executes the program by calling the decode
                 function on each point specified by the counter.
*   Assumptions: the mem is not null.
*/
void execute(FILE *fp) 
{
                
        Seg_handler mem = malloc(sizeof(*mem));
        mem->length = 1;
        mem->size = 4;
        mem->counter = 0;
        mem->unmapped = stack_new();
        mem->memory = malloc(mem->size * sizeof(Seg));

        
        /* get length of segment populating */
        int length = get_length(fp);
        Seg segment_0 = new_segment(length);

        /* read words into memory */
        uint32_t byte = 0;
        uint32_t word = 0;
        int index = 0;

        byte = getc(fp);
        while (byte != (uint32_t) EOF) {
                word = 0;

                for (int i = 3; i >= 0; i--) {
                        uint32_t mask1 = (((uint64_t)1 << 32) - 1);
                        mask1 = mask1 << (BYTE + BYTE * i);
                        uint32_t mask2 = (((uint64_t)1 << 32) - 1);
                        mask2 = mask2 >> (32 - BYTE * i);
                        uint32_t mask3 = (mask2 | mask1);

                        uint32_t prototype = (mask3 & word);
                        uint32_t translate_val = byte << (BYTE * i);
                        uint32_t translate = (translate_val | prototype);

                        word = translate;
                        
                        byte = getc(fp);
                }
                segment_0->words[index] = word;
                index++;
                word = 0;
        }

        (mem->memory)[0] = segment_0;
        
        assert(mem != NULL);
        uint32_t seg_i = 0;
        int word_pos = mem->counter;
        fclose(fp);

        while (1) {
                
                uint32_t word = (((mem->memory)[seg_i])->words)[word_pos];
                
                uint32_t operation = word >> OP_LSB;
                assert(operation <= 13);
                
                mem->counter++;

                if (operation == LV) {
                        
                        uint32_t a = word << OP_SIZE;
                        a = a >> (WORD_L - REG_SIZE);

                        uint32_t value = word << (OP_SIZE + REG_SIZE);
                        value = value >> (OP_SIZE + REG_SIZE);

                        r[a] = value;

                /* else statement for the 12 other operations */
                } else {
                        uint32_t c = (word << (WORD_L - 1 * REG_SIZE));
                        c = c >> (WORD_L - REG_SIZE);


                        uint32_t b = word << (WORD_L - 2 * REG_SIZE);
                        b = b >> (WORD_L - REG_SIZE);
                        
                        uint32_t a = word << (WORD_L - 3 * REG_SIZE);
                        a = a >> (WORD_L - REG_SIZE);

                        assert(a < 8);
                        assert(b < 8);
                        assert(c < 8);
                
                        Seg segment;
                        uint32_t user_in;
                        /* switch statement calls necessary function based
                        on the operation that was read from the word     */
                        switch( operation )
                        {
                                case  CMOV : 
                                        if (r[c] != 0){
                                                r[a] = r[b];
                                        }
                                        break;
                                case  SLOAD : 
                                        segment = (mem->memory)[r[b]];
                                        r[a] = (segment->words)[r[c]];
                                        break;
                                case  SSTORE : 
                                        segment = (mem->memory)[r[a]];
                                        segment->words[r[b]] = r[c];
                                        break;
                                case  ADD : 
                                        r[a] = (r[b] + r[c]) % MAX;
                                        break;
                                case  MUL : 
                                        r[a] = (r[b] * r[c]) % MAX;
                                        break;
                                case  DIV : 
                                        r[a] = (r[b] / r[c]);
                                        break;
                                case  NAND : 
                                        r[a] = ~(r[b] & r[c]);
                                        break;
                                case  HALT : 
                                        free_memory(&mem);
                                        exit(0);
                                        break;
                                case  MSEG : 
                                        segment = new_segment(r[c]);
                                        r[b] = append_segment(mem, segment);
                                        break;
                                case  UMSEG : 
                                        rem_segment(mem, r[c]);
                                        break;
                                case OUT : 
                                        assert(r[c] <= 255);
                                        putchar(r[c]);
                                        break;
                                case IN : 
                                        user_in = getc(stdin);
                                        if (user_in == (uint32_t) EOF) {
                                                user_in = -1;
                                        }
                                        r[c] = user_in;
                                        break;
                                case LOADP : 
                                        mem->counter = r[c];
                                        if (r[b] == 0) {
                                                break;
                                        }
                                        Seg temp = (mem->memory)[r[b]];
                                        int len = temp->length;
                                        Seg new_seg = new_segment(len); 
        
                                        for (int i = 0; i < len; i++){
                                                new_seg->words[i] = 
                                                                temp->words[i];
                                        }
                                        new_seg->length = temp->length;
                                        free_segment((mem->memory)[0]);
                                        (mem->memory)[0] = new_seg;
                                        break;
                        }

                }
                word_pos = mem->counter;
        }


}