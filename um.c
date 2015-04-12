/* um.c
 * Saurav Acharya, Daniel Morgan Rivers
 * March 24, 2015
 * Homework #6: Universal Machine
 *
 * um.c loads a file containing machine instructions in the um language, and
 * executes that file, simulating the actual operation of a virtual machine.
 * 
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "except.h"
#include "bitpack.h"
#include "memory.h"
#include "operate.h"

void initRegisters(MState um);
FILE *open_file(int argc, char *argv[]);
Seq_T parse_program(FILE *fp);
void executeZeroSeg(MState um);
void call_op(MState um, uint32_t word, int opCode, int *halt_now);


int main(int argc, char *argv[])
{
        
        /* The machine is represented by the mState um struct. */
        struct mState um_struct;
        MState um = &um_struct;
        initRegisters(um);
        
        /* Get the file and load instructions into the zero segment. */
        FILE *fp = open_file(argc, argv);
        Seq_T zeroSeg = parse_program(fp);
        um->memory = Mem_new_zeroSeg(zeroSeg);
        
        executeZeroSeg(um);
        
        Mem_free_all(um->memory);
        fclose(fp);
        return EXIT_SUCCESS;
}

/*
 * Initialize all registers to zero 
 */
void initRegisters(MState um)
{
        for (int i = 0; i < 8; i++)
                um->r[i] = 0;
}

/* Read the name of the file from the terminal and return a pointer to it. */
FILE *open_file(int argc, char *argv[])
{ 
        if (argc != 2) {
                fprintf(stderr, "Error: too %s program arguments.",
                                (argc > 2) ? "many" : "few");
                exit(EXIT_FAILURE);
        }
        
        /* Open the file for reading. */
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL) {
                fprintf(stderr, 
                        "%s: %s\n",
                        argv[1], "Could not open file for reading");
                exit(1);
        }
        return fp;
}

/* 
 * Access each word in the input file and stores into the Seq_T called zeroSeg. 
 */
Seq_T parse_program(FILE *fp)
{
        Seq_T zeroSeg = Seq_new(32);    /* Hint is 32 elements in the Seq_T. */

        int hit_eof = 0;
        while (hit_eof != 1) {
                uint32_t *new_word = (uint32_t *)malloc(sizeof(uint32_t));
                *new_word = 0;
                
                /* Put together each word, 4 bytes at a time. */
                for (int shift = 24; shift >= 0; shift -= 8) {
                        uint32_t byte = fgetc(fp);
                        if ((int)byte == EOF) hit_eof = 1;
                        *new_word += ((uint32_t)byte) << shift;
                }
                /* Add the word to the end of the sequence. */
                Seq_addhi(zeroSeg, new_word); 
        }
        
        return zeroSeg;
}

void executeZeroSeg(MState um)
{
        int halt_now = 0; /* Halt is false until set to true. */
        um->prog_counter = 0;

        /* Execute code while the halt instruction hasn't been called. */
        while (halt_now != 1)
        {
                uint32_t word = Mem_get(um->memory, 0, um->prog_counter++);
                int opCode = Bitpack_getu(word, 4, 28);
                
                call_op(um, word, opCode, &halt_now);             
        }
}

/* Execute the instruction based on it's operation code. */
void call_op(MState um, uint32_t word, int opCode, int *halt_now)
{
        switch (opCode) {
                case 0:   Op_cond_move(um, word);        return;
                case 1:   Op_segment_load(um, word);     return;
                case 2:   Op_segment_store(um, word);    return;
                case 3:   Op_add(um, word);              return;
                case 4:   Op_multiply(um, word);         return;
                case 5:   Op_divide(um, word);           return;
                case 6:   Op_nand(um, word);             return;
                /* If halt is called, break program execution. */
                case 7:   *halt_now = 1;                 return;
                case 8:   Op_map_segment(um, word);      return;
                case 9:   Op_unmap_segment(um, word);    return;
                case 10:  Op_output(um, word);           return;
                case 11:  Op_input(um, word);            return;
                case 12:  Op_load_program(um, word);     return;
                case 13:  Op_load_value(um, word);       return;
                /* Do not execute any operations if the opcode is unknown. */
                default:                                 return;
        }
}

