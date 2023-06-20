/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021. Modified by Lei Jia 
   to be part of the project in Feb 2023.
*/

#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "../types.h"
#include "../utils.h"

/* READ ME - see documentation in translate.c */
int write_static_data(FILE *output, Byte *buf);

/* READ ME - see documentation in translate.c */
unsigned write_original_code(FILE *output, const char *name, char **args,
                           int num_args);

/* IMPLEMENT ME - see documentation in translate.c */
int translate_inst(FILE *basic_code, FILE *machine_code, const char *name, char **args, 
                   size_t num_args, uint32_t addr, SymbolTable *symtbl);

/* IMPLEMENT ME - see documentation in translate.c */
int write_instruction(FILE *basic_code, FILE *machine_code, const char *name, char **args, 
                      size_t num_args, uint32_t addr, SymbolTable *symtbl);

/* Helper functions. You may want to IMPLEMENT US. */

int write_rtype(FILE *basic_code, FILE *machine_code, const char *name, uint8_t opcode, 
                uint8_t funct3, uint8_t funct7, char **args, size_t num_args);

int write_itype(FILE *basic_code, FILE *machine_code, const char *name, uint8_t opcode, 
                uint8_t funct3, char **args, size_t num_args);

int write_stype(FILE *basic_code, FILE *machine_code, const char *name, uint8_t opcode, 
                uint8_t funct3, char **args, size_t num_args);

int write_sbtype(FILE *basic_code, FILE *machine_code, const char *name, uint8_t opcode, 
                uint8_t funct3, char **args, size_t num_args, uint32_t addr, SymbolTable *symbol);

int write_utype(FILE *basic_code, FILE *machine_code, const char *name, uint8_t opcode,  
                char **args, size_t num_args);

int write_ujtype(FILE *basic_code, FILE *machine_code, const char *name, uint8_t opcode,
                 char **args, size_t num_args, uint32_t addr, SymbolTable *symbol);

#endif
