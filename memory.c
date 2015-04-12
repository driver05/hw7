/*
 * memory_seg.c
 * Daniel M Rivers, Saurav Acharya
 * 
 * memory_seg.c implements the memory management of the universal machine. The
 * memory is represented as a struct. 
 * 
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "seq.h"
#include "memory.h"

/*
 * Mem_T represents a set of memory segments, with its implementation visible
 * to the memory.c file only.
 */
struct Mem_T {
        Seq_T segs;
};


static const int HINT = 32;/* Guess for initial segment size & # of segments. */

/*
 * Initializes and allocates a Mem_T with no segments. 
 */
Mem_T Mem_new()
{
        Mem_T memory = malloc(sizeof(struct Mem_T));
        memory->segs = Seq_new(HINT);
        return memory;
}

/* 
 * Initializes and adds a zeroth segment with the given seq_t of segment 
 * elements.
 */
Mem_T Mem_new_zeroSeg(Seq_T zeroSeg)
{
        Mem_T memory = Mem_new();
        Seq_addhi(memory->segs, zeroSeg);
        return memory;
}

/* 
 * Load segment with ID segID into the zero segment. 
 */
void Mem_load_program(Mem_T memory, int segID)
{
        if (segID == 0) return;
        
        Seq_T segment = Seq_get(memory->segs, segID);
        uint32_t nWords = Seq_length(segment);
        
        /* Segment zero is now an empty segment. */
        Seq_T newZeroSeg = Seq_new(HINT);
        
        /* Add each word in segment segID segment zero */
        for (uint32_t i = 0; i < nWords; i++) {
                uint32_t *new_word = (uint32_t *)malloc(sizeof(uint32_t));
                *new_word = *(uint32_t *)Seq_get(segment, i);
                Seq_addhi(newZeroSeg, new_word);
        }
        
        Mem_unmap_segment(memory, 0);        

        /* Put the newly created segment in the zeroth index. */
        Seq_put(memory->segs, 0, newZeroSeg);
}

/*
 * Allocate space for a segment with nWords words and returns the id of the 
 * new segment. 
 */
uint32_t Mem_map_segment(Mem_T memory, uint32_t nWords)
{
        Seq_T segment = Seq_new(HINT);
        for (uint32_t i = 0; i < nWords; i++) {
                uint32_t *new_word = malloc(sizeof(uint32_t));
                *new_word = 0;
                Seq_addhi(segment, new_word);
        }
        
        /* Find a segment that was previously unmapped (i.e. a 'hole') */
        uint32_t segCount = Seq_length(memory->segs);
        for (uint32_t i = 0; i < segCount; i++) {
                if (Seq_get(memory->segs, i) == NULL) {
                        Seq_put(memory->segs, i, segment);
                        return i;
                }
        }

        /* No 'holes' found; expansion needed: */
        Seq_addhi(memory->segs, segment);
        return segCount;
}

/* 
 * Delete a segment from memory.
 */
void Mem_unmap_segment(Mem_T memory, uint32_t segID)
{
        Seq_T segment = Seq_get(memory->segs, segID);
        if (segment == NULL) return;
        
        /* Remove each element in the segment. */
        int nWords = Seq_length(segment);
        for (int i = 0; i < nWords; i++) {
                uint32_t *temp = Seq_remlo(segment);
                if (temp != NULL) free(temp);
        }
        
        Seq_free(&segment);
        Seq_put(memory->segs, segID, NULL);
}

/* 
 * Get and return a word from the segment segID and the location offset within 
 * the segment. 
 */
uint32_t Mem_get(Mem_T memory, uint32_t segID, uint32_t offset)
{
        Seq_T segment = Seq_get(memory->segs, segID);
        return *(uint32_t *)Seq_get(segment, offset);
}

/* 
 * Set the memory at segment segID and word within segment at element ID offset.
 */
void Mem_set(Mem_T memory, uint32_t segID, uint32_t offset, uint32_t word)
{
        Seq_T segment = Seq_get(memory->segs, segID);
        uint32_t *word_in_mem = Seq_get(segment, offset);
        *word_in_mem = word;
}

/*
 * Remove all allocated space containing the memory
 */
void Mem_free_all(Mem_T memory)
{
        if (memory->segs == NULL) return;
        
        int numSegs = Seq_length(memory->segs);
        for (int i = 0; i < numSegs; i++) {
                Mem_unmap_segment(memory, 0);
                Seq_remlo(memory->segs);
        }
        
        if (memory->segs != NULL) Seq_free(&(memory->segs));
        free(memory);
}
