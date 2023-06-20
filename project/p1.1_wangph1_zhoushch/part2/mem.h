#ifndef MEM_H
#define MEM_H

#include "../types.h"
#include "../utils.h"

#define MEMORY_SIZE  0x100000           /* 1 MByte of Memory */
#define MEMORY_TEXT  0x000000           /* physical start address of Text */
#define MEMORY_DATA  0x040000           /* physical start address of DATA */
#define MEMORY_STACK 0x080000           /* physical start address of STACK */

void store(Byte *memory, Address address, Alignment alignment, Word value, int check_align);

Word load(Byte *memory, Address address, Alignment alignment, int check_align);

/* Checks that the address is aligned correctly */
int check_alignment(Address address, Alignment alignment);

#endif