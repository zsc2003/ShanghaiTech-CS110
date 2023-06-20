#include <stdio.h>
#include <stdlib.h>
#include "execute_utils.h"


/* You may find implementing this function helpful */

/* Signed extension of a number to 32 bits. 
   size is the number of valid bits of field.
   e.g. field=0x8a, size=8, return 0xFFFFFF8a.
        field=0x7a, size=8, return 0x0000007a.
 */
int bitSigner(unsigned int field, unsigned int size)
{
    /* YOUR CODE HERE */
}

/* Return the imm from instruction, for details, please refer to I type instruction formats.
   Remember that the offsets should return the offset in BYTES.
 */
int get_imm_operand(Instruction instruction)
{
    /* YOUR CODE HERE */
}
/* Return the offset from instruction, for details, please refer to SB type instruction formats.
   Remember that the offsets should return the offset in BYTES.
 */
int get_branch_offset(Instruction instruction)
{
    /* YOUR CODE HERE */
}

/* Return the offset from instruction, for details, please refer to UJ type instruction formats.
   Remember that the offsets should return the offset in BYTES.
 */
int get_jump_offset(Instruction instruction)
{
    /* YOUR CODE HERE */
}

/* Return the offset from instruction, for details, please refer to S type instruction formats.
   Remember that the offsets should return the offset in BYTES.
 */
int get_store_offset(Instruction instruction)
{
    /* YOUR CODE HERE */
}
