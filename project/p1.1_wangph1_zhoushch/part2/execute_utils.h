#ifndef EXECUTE_UTILS_H
#define EXECUTE_UTILS_H

#include "../types.h"
#include "../utils.h"

#define RTYPE_FORMAT "%s\tx%d, x%d, x%d\n"
#define ITYPE_FORMAT "%s\tx%d, x%d, %d\n"
#define MEM_FORMAT "%s\tx%d, %d(x%d)\n"
#define UTYPE_FORMAT "%s\tx%d, %d\n"
#define JAL_FORMAT "jal\tx%d, %d\n"
#define BRANCH_FORMAT "%s\tx%d, x%d, %d\n"
#define ECALL_FORMAT "ecall\n"

/* You may find implementing this function helpful */
int bitSigner(unsigned, unsigned);
int get_imm_operand(Instruction);
int get_branch_offset(Instruction);
int get_jump_offset(Instruction);
int get_store_offset(Instruction);

#endif