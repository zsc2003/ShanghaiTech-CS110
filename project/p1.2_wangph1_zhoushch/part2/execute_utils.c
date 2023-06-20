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
    int sign = field & (1 << (size - 1)); /* get the first valid number */
    if(sign == 0) /* do not need extension */
        return (int)field;
    return (int)field - (1 << size); /* use overflow to extend sign bit */
}

/* Return the imm from instruction, for details, please refer to I type instruction formats.
   Remember that the offsets should return the offset in BYTES.

    piazza@235:
    This is just telling you that the following functions like get_branch_offset(), you should return offsets in bytes.
    Not means the return data type is bytes.  Just means the unit of offset is bytes.
 */
int get_imm_operand(Instruction instruction)
{
    /* YOUR CODE HERE */
    return bitSigner((unsigned int)instruction.itype.imm, 12);
}
/* Return the offset from instruction, for details, please refer to SB type instruction formats.
   Remember that the offsets should return the offset in BYTES.
 */
int get_branch_offset(Instruction instruction)
{
    /* YOUR CODE HERE */
    unsigned int imm5 = instruction.sbtype.imm5;
    unsigned int imm7 = instruction.sbtype.imm7;
    unsigned int imm =  (imm5 & 0x1E)        /* imm[4:1] */
                     + ((imm7 & 0x3F) << 5)  /* imm[10:5] */
                     + ((imm5 & 0x01) << 11) /* imm[11] */
                     + ((imm7 & 0x40) << 6); /* imm[12] ( >> 6 << 12)*/
    return bitSigner(imm, 13);
}

/* Return the offset from instruction, for details, please refer to UJ type instruction formats.
   Remember that the offsets should return the offset in BYTES.
 */
int get_jump_offset(Instruction instruction)
{
    /* YOUR CODE HERE */
    unsigned int imm20 = instruction.ujtype.imm;
    unsigned int imm = ((imm20 & 0x7FE00) >> 8)  /* imm[10:1] ( >> 9 << 1)*/
                     + ((imm20 & 0x00100) << 3)  /* imm[11] ( >> 8 << 11)*/
                     + ((imm20 & 0x000FF) << 12) /* imm[19:12] */
                     + ((imm20 & 0x80000) << 1); /* imm[20] ( >> 19 << 20)*/
    return bitSigner(imm, 21);
}

/* Return the offset from instruction, for details, please refer to S type instruction formats.
   Remember that the offsets should return the offset in BYTES.
 */
int get_store_offset(Instruction instruction)
{
    /* YOUR CODE HERE */
    unsigned int imm5 = instruction.stype.imm5;
    unsigned int imm7 = instruction.stype.imm7;
    unsigned int imm =  (imm5 & 0x1F)        /* imm[4:0] */
                     + ((imm7 & 0x7F) << 5); /* imm[11:5] */
    return bitSigner(imm, 12);
}