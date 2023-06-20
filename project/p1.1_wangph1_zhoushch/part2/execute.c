#include <stdio.h>
#include "execute.h"
#include "execute_utils.h"
#include "mem.h"


/*******************************
 * Helper Functions
 *******************************/

/* Function handle with error machine code. You should skip the error 
   machine code after call this funcion
*/
void raise_machine_code_error(Instruction instruction){
    write_to_log("Invalid Machine Code: 0x%08x\n", instruction.bits);
}

/* Function handle with undefined ecall parameter.
*/
void raise_undefined_ecall_error(Register a0){
    write_to_log("Undefined Ecall Number: %d\n", a0);
}

/* Function to exit current program */
void exit_program(Processor *p) {
    if (p->stat==1) {
        /* set cpu state unwork */
        p->stat=0;
        write_to_log("Exited with error code 0.\n");
    }
    else {
        write_to_log("Error: program already exit.\n");
    }
}

/* 
    In this section you need to complete execute instruction. This should modify 
    the appropriate registers, make any necessary calls to memory, and updatge the program
    counter to refer You will find the functions in execute_utils.h very useful. So it's 
    better to finish execute_utils.c first. The following provides you with a general 
    framework. You can follow this framework or add your own functions, but please 
    don't change declaration or feature of execute(). Case execute() is the entry point
    to the execution phase. The execute_*() is specific implementations for each 
    instruction type. You had better read types.h carefully. Think about the usage 
    of the union defined in types.h
 */

void execute(Instruction inst, Processor *p, Byte *memory)
{
    switch (inst.opcode.opcode)
    { 
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */

    default: /* undefined opcode */
        raise_machine_code_error(inst);
        p->PC += 4;
        break;
    }
}

void execute_rtype(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
}

void execute_itype_except_load(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
}

/* You may need to use exit_program() */
void execute_ecall(Processor *p, Byte *memory)
{
    /* YOUR CODE HERE */
}

void execute_branch(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
}

void execute_load(Instruction inst, Processor *p, Byte *memory)
{
    /* YOUR CODE HERE */
}

void execute_store(Instruction inst, Processor *p, Byte *memory)
{
    /* YOUR CODE HERE */
}

void execute_jal(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
}

void execute_jalr(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
}

void execute_utype(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
}