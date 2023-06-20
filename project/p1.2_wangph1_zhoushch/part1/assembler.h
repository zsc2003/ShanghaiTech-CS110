/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021. Modified by Lei Jia 
   to be part of the project in Feb 2023.
*/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../types.h"
#include "../utils.h"

/*******************************
 * Do Not Modify Code Below
 *******************************/

int assemble(int mode, FILE *input, FILE *data, FILE *symtbl, FILE *orgin, FILE *basic, FILE *machine);

#endif