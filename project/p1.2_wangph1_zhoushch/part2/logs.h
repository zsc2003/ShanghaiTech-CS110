#ifndef LOGS_H
#define LOGS_H

#include "emulator.h"
#include "mem.h"


/* Complete the following definition of Logs and implement the following
   functions. You are free to add structs or functions, but you must keep
   structure Logs. And you cannot change a function that has already been given.
 */

 /* Add structure definition here if you need */

typedef struct
{
    /* YOUR CODE HERE */
    /* record cpu */
    Processor cpu;
    /* instruction type if s type */
    int ifstype;
    /* value if s type */
    Word value;
    /* record memory add*/
    int memadd;
} Log;

typedef struct
{
    /* YOUR CODE HERE */
    /* log size (numbers) */
    int size;
    /* log capacity (numbers) */
    int capacity;
    /* log array */
    Log *log;
} Logs;

/* IMPLEMENT ME - see documentation in logs.c */
Logs *create_logs();

/* IMPLEMENT ME - see documentation in logs.c */
void free_logs(Logs *logs);

/* IMPLEMENT ME - see documentation in logs.c */
void record(Instruction inst, Processor *cpu, Byte *mem, Logs *logs);

/* IMPLEMENT ME - see documentation in logs.c */
int undo(Processor *cpu, Byte *mem, Logs *logs);

/* Add function declaration here if you need */

#endif