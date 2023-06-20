#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mem.h"


/*******************************
 * Helper Functions
 *******************************/

/* Call this function in the following cases:
        1. Attempts to access a memory location that it is not allowed to access.
        2. Attempts to access a memory location in a way that is not allowed.
 */
static void raise_segmentation_fault(Address addr) {
    write_to_log("Error - segmentation fault at address %08x\n", addr);
}

/* Call this function if a data access is performed to an address 
   that is not aligned for the size of the access.
 */
static void raise_alignment_fault(Address addr) {
    write_to_log("Error - alignment fault at address %08x\n", addr);
}

/* This function is helpful to your alignment check.
 */
int check_alignment(Address address, Alignment alignment) {
    if (alignment == LENGTH_BYTE)
    {
        return 1;
    }
    /* HALF_WORD: The last bit is not used. */
    else if (alignment == LENGTH_HALF_WORD)
    {
        return address % 2 == 0;
    }
    /* WORD: The last two bits are not used. */
    else if (alignment == LENGTH_WORD)
    {
        return address % 4 == 0;
    }
    else {
        return 0;
    }
}

/* To store the data in the corresponding address in memory, 
   please note the following points:
        1. When check_align==1, check if the memory address is aligned
         with the data type. You need to use raise_alignment_fault().
        2. Think about when you need to use raise_segmentation_fault().
        3. Please perform an address conversion before actually store to memory.

    piazza@236:
    Byte* memory is "physical memory" that you need to store value in.

    Caveat:
    Raise segmentation fault when accessing an unmapped memory location and attempting to store in text segment.
    For an incorrect load you should raise the corresponding error and return a value of 0.
    For an incorrect store you should raise the corresponding error and return.
    Do not interrupt or exit the program.
 */

void store(Byte *memory, Address address, Alignment alignment, Word value,
           int check_align)
{
    /* YOUR CODE HERE */
    /* define physical addr */
    Byte *physical_memory;
    /* physical address */
    Address physical_address;
    /* check null memory */
    if (memory == NULL)
    {
        return;
    }
    /* 0x10000000 - 0x7FFFFFFC: Data map to 0x40000 to 0x80000 */
    if (address >= DATA_BASE && (address - DATA_BASE) < (MEMORY_STACK - MEMORY_DATA))
    {
        physical_memory = memory + address - DATA_BASE + MEMORY_DATA;
        physical_address = address - DATA_BASE + MEMORY_DATA;
    }
    /* 0x7FFFFFFC: stack map to 0x100000 to 0x80000 */
    else if (address <= STACK_BASE && (STACK_BASE - (long)address) <= (MEMORY_SIZE - 4 - MEMORY_STACK))
    {
        physical_memory = memory + MEMORY_SIZE - 4 - (STACK_BASE - address);
        physical_address = MEMORY_SIZE - 4 - (STACK_BASE - address);
    }
    /* check if out of bound in physical mem */
    else
    {
        raise_segmentation_fault(address);
        return;
    }
    /* check alignment */
    if (check_align == 1 && !check_alignment(address, alignment))
    {
        raise_alignment_fault(physical_address);
        return;
    }
    /* store */
    if (alignment == LENGTH_BYTE)
    {
        physical_memory[0] = value;
    }
    /* type half */
    else if (alignment == LENGTH_HALF_WORD)
    {
        physical_memory[0] = value;
        physical_memory[1] = value >> 8;
    }
    /* type word */
    else if (alignment == LENGTH_WORD)
    {
        physical_memory[0] = value;
        physical_memory[1] = value >> 8;
        physical_memory[2] = value >> 16;
        physical_memory[3] = value >> 24;
    }
    /* return */
    return;
}

/* To load the data in the corresponding address in memory, 
   please note the following points:
        1. When check_align==1, check if the memory address is aligned
         with the data type. You need to use raise_alignment_fault().
        2. Think about when you need to use raise_segmentation_fault().
        3. Please perform an address conversion before actually load from memory.
 */
Word load(Byte *memory, Address address, Alignment alignment, int check_align)
{
    /* YOUR CODE HERE */
    /* define physical addr */
    Byte *physical_memory;
    /* physical address */
    Address physical_address;
    /* check null memory */
    if (memory == NULL)
    {
        return 0;
    }
    /* first detect if out of bound in virtual mem */
    /* map text to memory */
    /* address always bigger than textbase */
    if ((address - TEXT_BASE) < (MEMORY_DATA - MEMORY_TEXT))
    {
        physical_memory = memory + address - TEXT_BASE + MEMORY_TEXT;
        physical_address = address - TEXT_BASE + MEMORY_TEXT;
    }
    /* map data to memory */
    else if ((address >= DATA_BASE) && ((address - DATA_BASE) < (MEMORY_STACK - MEMORY_DATA)))
    {
        physical_memory = memory + address - DATA_BASE + MEMORY_DATA;
        physical_address = address - DATA_BASE + MEMORY_DATA;
    }
    /* map stack to memory */
    else if ((address <= STACK_BASE) && ((STACK_BASE - (long)address) <= (MEMORY_SIZE - 4 - MEMORY_STACK)))
    {
        physical_memory = memory + MEMORY_SIZE - 4 - (STACK_BASE - (long)address);
        physical_address = MEMORY_SIZE - 4 - (STACK_BASE - (long)address);
    }
    /* check if out of bound in physical mem */
    else
    {
        raise_segmentation_fault(address);
        return 0;
    }
    /* check alignment */
    if (check_align == 1 && !check_alignment(address, alignment))
    {
        raise_alignment_fault(physical_address);
        return 0;
    }
    /* type byte */
    if (alignment == LENGTH_BYTE)
    {
        return physical_memory[0];
    }
    /* type half */
    else if (alignment == LENGTH_HALF_WORD)
    {
        return physical_memory[0] | (physical_memory[1] << 8);
    }
    /* type word */
    else if (alignment == LENGTH_WORD)
    {
        return physical_memory[0] | (physical_memory[1] << 8) | (physical_memory[2] << 16) | (physical_memory[3] << 24);
    }
    /* return */
    return 0;
}