/*
 * memory_seg.c
 * Daniel M Rivers, Saurav Acharya
 * 
 * memory_seg.c implements each universal machine instruction as a function. 
 * The functions take the registers and memory segments in the um and the word
 * to parse, which provides register values, with the exception of 
 * Op_load_value, which provides the value to load into a register.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "seq.h"
#include "memory.h"
#include "operate.h"
#include "bitpack.h"

void Op_cond_move(MState um, uint32_t word)
{
        int rA = Bitpack_getu(word, 3, 6);
        int rB = Bitpack_getu(word, 3, 3);
        int rC = Bitpack_getu(word, 3, 0);

        if(um->r[rC] != 0) {
                um->r[rA] = um->r[rB]; 
        }
}

void Op_segment_load(MState um, uint32_t word)
{
        int rA = Bitpack_getu(word, 3, 6);
        int rB = Bitpack_getu(word, 3, 3);
        int rC = Bitpack_getu(word, 3, 0);

        um->r[rA] = Mem_get(um->memory, um->r[rB], um->r[rC]);
}

void Op_segment_store(MState um, uint32_t word)
{
        int rA = Bitpack_getu(word, 3, 6);
        int rB = Bitpack_getu(word, 3, 3);
        int rC = Bitpack_getu(word, 3, 0);

        Mem_set(um->memory, um->r[rA], um->r[rB], um->r[rC]);
}

void Op_add(MState um, uint32_t word)
{
        int rA = Bitpack_getu(word, 3, 6);
        int rB = Bitpack_getu(word, 3, 3);
        int rC = Bitpack_getu(word, 3, 0);
        um->r[rA] = um->r[rB] + um->r[rC];

}

void Op_multiply(MState um, uint32_t word)
{
        int rA = Bitpack_getu(word, 3, 6);
        int rB = Bitpack_getu(word, 3, 3);
        int rC = Bitpack_getu(word, 3, 0);
        um->r[rA] = um->r[rB] * um->r[rC];
}

void Op_divide(MState um, uint32_t word)
{
        int rA = Bitpack_getu(word, 3, 6);
        int rB = Bitpack_getu(word, 3, 3);
        int rC = Bitpack_getu(word, 3, 0);
        um->r[rA] = um->r[rB] / um->r[rC];
}

void Op_nand(MState um, uint32_t word)
{
        int rA = Bitpack_getu(word, 3, 6);
        int rB = Bitpack_getu(word, 3, 3);
        int rC = Bitpack_getu(word, 3, 0);
        um->r[rA] = ~(um->r[rB] & um->r[rC]);
}

void Op_map_segment(MState um, uint32_t word)
{
        int rB = Bitpack_getu(word, 3, 3);
        int rC = Bitpack_getu(word, 3, 0);
        
        um->r[rB] = Mem_map_segment(um->memory, um->r[rC]);
}
void Op_unmap_segment(MState um, uint32_t word)
{
        int rC = Bitpack_getu(word, 3, 0);
        
        Mem_unmap_segment(um->memory, um->r[rC]);
}


void Op_output(MState um, uint32_t word)
{
        int rC = Bitpack_getu(word, 3, 0);
        fputc((char)um->r[rC], stdout);
}

void Op_input(MState um, uint32_t word)
{
        int rC = Bitpack_getu(word, 3, 0);
        
        int input = fgetc(stdin);
        um->r[rC] = (uint32_t)input;
}

void Op_load_program(MState um, uint32_t word)
{
        int rB = Bitpack_getu(word, 3, 3);
        int rC = Bitpack_getu(word, 3, 0);
        
        Mem_load_program(um->memory, um->r[rB]);
        um->prog_counter = um->r[rC];
}

void Op_load_value(MState um, uint32_t word)
{
        int rA = Bitpack_getu(word, 3, 25);
        uint32_t value = Bitpack_getu(word, 25, 0);
        um->r[rA] = value;
}


