#ifndef EMULATOR_H
#define EMULATOR_H

#include "../types.h"
#include "../utils.h"

#include <stdio.h>

typedef struct
{
    Register reg[32];   /* 32 registers */
    Register PC;        /* program counter */
    int stat;           /* 0:idle, 1:work */
} Processor;

typedef struct
{
    Processor cpu;
    Byte *memory;       
    Address end;        /* program end address */
    void *logs;         /* logs component */
} Emulator;

Emulator *create_emulator(FILE *text, FILE *data);
int free_emulator(Emulator *emulator);

int run  (Emulator *emulator, int mode);
int step (Emulator *emulator, int mode);
int prev (Emulator *emulator);
int trace(Emulator *emulator);
int dump (Emulator *emulator);

#endif