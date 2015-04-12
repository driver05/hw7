/* operate.h
 * 
 * Daniel M Rivers, Saurav Acharya
 * Date Created: 4/5/2015
 *      
 * Interface for operate.c. 
 * 
 * NOTE:
 * rA = register A
 * rB = register B
 * rC = register C
 */

#ifndef OPERATE_INCLUDED
#define OPERATE_INCLUDED


/* MState represents the UM's virtual memory and internal state. */
typedef struct mState {
        uint32_t r[8];
        uint32_t prog_counter;
        Mem_T memory;
} *MState;

/* Operation zero. Moves rB to rA if rC equals zero. */
void Op_cond_move(MState um, uint32_t word);

/* Operation one. Loads segment at rB offset of rC into rA. */
void Op_segment_load(MState um, uint32_t word);

/* Operation two. Stores rC into segment at rA with an offset of rB. */
void Op_segment_store(MState um, uint32_t word);

/* Operation three. Adds rB and rC and stores into rA. */
void Op_add(MState um, uint32_t word);

/* Operation four. Multiplies rB and rC and stores into rA. */
void Op_multiply(MState um, uint32_t word);

/* Operation five. Divides rB by rC and stores the result into rA. */
void Op_divide(MState um, uint32_t word);

/* Operation six. Performs a bitwise nand of the binary values represented by 
 * rB and rC, equivalent to Not[rB And rC]. This is stored into rA. */
void Op_nand(MState um, uint32_t word);

/* halt skipped, see um.c */

/* Operation eight. Creates a segment with rC elements and sets rB to the ID 
 * value of the newly created segment. */
void Op_map_segment(MState um, uint32_t word);

/* Operation nine. Unmap (deallocate and free) the segment with ID rC. */
void Op_unmap_segment(MState um, uint32_t word);

/* Operation ten. Output the ascii character associated with the value in rC. */
void Op_output(MState um, uint32_t word);

/* Operation eleven. Wait for input on the command line, and store it to rC. */
void Op_input(MState um, uint32_t word);

/* Operation twelve. Replace segment zero with the words in segment ID rC*/
void Op_load_program(MState um, uint32_t word);

/* Operation thirteen. Load the value in the last 25 bits of word into rA.*/
void Op_load_value(MState um, uint32_t word);


#endif
