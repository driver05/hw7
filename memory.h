/* memory_seg.h
 * 
 * Daniel M Rivers, Saurav Acharya
 * Date Created: 3/28/2015
 *      
 * Interface for memory_seg.c. The memory.h interface represents the
 *         virtual memory used by the Universal Machine. It contains
 *         functions for mapping & unmapping segments, and getting &
 *         setting data into the memory.
 */

#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED

#include <stdint.h>
#include "seq.h"

/* Mem_T represents the virtual memory used by the UM. Implementation hidden.*/
typedef struct Mem_T *Mem_T;

/* Initializes the virtual memory with no mapped segments.
 * Returns:     Mem_T with no mapped segments */
extern Mem_T Mem_new();

/* Initializes virtual memory with a given segment 0.
 * Args:        A Seq_T of uint32_t elements representing words
 * Returns:     Mem_T with the zero segment mapped */
extern Mem_T Mem_new_zeroSeg(Seq_T zeroSeg);

/* Loads a program into the zero segment. 
 * Args:        Mem_T and ID of the segment to be loaded into segment zero */
extern void Mem_load_program(Mem_T memory, int segID);

/* Allocates a segment and initializes it with 0's. 
 * Args:        Mem_T and size of the new segment
 * Returns:     The segment ID of the new segment */
extern uint32_t Mem_map_segment(Mem_T memory, uint32_t nWords);

/* Unmaps the specified segment, freeing the memory used by it. 
 * Args:        Mem_T, segment ID of the segment to be unmapped */
extern void Mem_unmap_segment(Mem_T memory, uint32_t segID);

/* Returns the word at the given segment ID and offset.
 * Args:        Mem_T, segment ID and the offset of the desired word
 * Returns:     The desired word */
extern uint32_t Mem_get(Mem_T memory, uint32_t segID, uint32_t offset);

/* Sets a word in a segment.
 * Args:        Mem_T, segment ID, offset within the segment, the new word */
extern void Mem_set(Mem_T memory, uint32_t segID, uint32_t offset, 
                uint32_t word);

/* Frees all virtual memory used by the UM. 
 * Args:        Mem_T representing the virtual memory to free */
extern void Mem_free_all(Mem_T memory);

#endif
