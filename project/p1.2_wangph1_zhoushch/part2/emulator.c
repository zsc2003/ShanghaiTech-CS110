#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"
#include "execute.h"
#include "logs.h"
#include "mem.h"


#define MAX_SIZE 50

/* WARNING: DO NOT CHANGE THIS FILE.
   YOU PROBABLY DON'T EVEN NEED TO LOOK AT IT... */

int run(Emulator *emulator, int mode)
{
    if (emulator->cpu.stat!=1 || emulator->cpu.PC >= emulator->end)
        return -1;
    
    for (;;)
    {
        if(step(emulator, mode)==-1)
            return 0;
    }

    return 0;
}

int step(Emulator *emulator, int mode)
{
    Instruction instruction;
    int i,j,k;

    /* check cpu stat and current PC*/
    if (emulator->cpu.stat!=1 || emulator->cpu.PC >= emulator->end)
        return -1;
    
    /* enforce $0 being hard-wired to 0 */
    emulator->cpu.reg[0] = 0;

    /* fetch an instruction */
    instruction.bits = load(emulator->memory, emulator->cpu.PC, LENGTH_WORD, 1);

    /* recode an undo log of instruction */
    record(instruction, &(emulator->cpu), emulator->memory, emulator->logs);

    /* execute an instruction */
    execute(instruction, &(emulator->cpu), emulator->memory);

    /* enforce $0 being hard-wired to 0 */
    emulator->cpu.reg[0] = 0;

    /* trace in mode 1 */
    if(mode==1){
        for(i=0;i<8;i++){
            for(j=0;j<4;j++){
                k=4*i+j;
                if(k<10)
                    write_to_log(" x%d=0x%08x ",k,emulator->cpu.reg[k]);
                else
                    write_to_log("x%d=0x%08x ",k,emulator->cpu.reg[k]);
            }
            write_to_log("\n");
        }
        write_to_log("\n");
    }

    /* check next PC */
    if (emulator->cpu.PC >= emulator->end && emulator->cpu.stat==1){
        emulator->cpu.stat=0;
        write_to_log("Exited with error code %d.\n", emulator->cpu.reg[10]);
    }

    return 0;
}

int prev(Emulator *emulator)
{
    if (undo(&(emulator->cpu), emulator->memory, emulator->logs)==-1)
        return -1;
    emulator->cpu.stat=1;
    return 0;
}

int trace(Emulator *emulator)
{
    return run(emulator, 1);
}

int dump(Emulator *emulator)
{
    Address pc = TEXT_BASE;
    Instruction instruction;
    while(pc < emulator->end){
        instruction.bits = load(emulator->memory, pc, LENGTH_WORD, 1);
        write_to_log("%08x\n", instruction.bits);
        pc+=4;
    }
    return 0;
}

void load_program(Emulator *emulator, FILE *text, FILE *data)
{
    char line[MAX_SIZE];
    int instruction, offset = 0;

    /* load text */
    if (text)
    {
        rewind(text);
        while (fgets(line, MAX_SIZE, text) != NULL)
        {
            instruction = (int32_t)strtol(line, NULL, 16);
            emulator->memory[MEMORY_TEXT + offset] = instruction & 0xFF;
            emulator->memory[MEMORY_TEXT + offset + 1] =
                (instruction >> 8) & 0xFF;
            emulator->memory[MEMORY_TEXT + offset + 2] =
                (instruction >> 16) & 0xFF;
            emulator->memory[MEMORY_TEXT + offset + 3] =
                (instruction >> 24) & 0xFF;
            offset += 4;
        }

        /* record the end address of the program */
        emulator->end = MEMORY_TEXT + offset;
    }
    
    instruction = 0;
    offset = 0;

    /* load data */
    if (data)
    {
        rewind(data);
        while (fgets(line, MAX_SIZE, data) != NULL)
        {
            instruction = (int32_t)strtol(line, NULL, 16);
            emulator->memory[MEMORY_DATA + offset] = instruction & 0xFF;
            emulator->memory[MEMORY_DATA + offset + 1] =
                (instruction >> 8) & 0xFF;
            emulator->memory[MEMORY_DATA + offset + 2] =
                (instruction >> 16) & 0xFF;
            emulator->memory[MEMORY_DATA + offset + 3] =
                (instruction >> 24) & 0xFF;
            offset += 4;
        }
    }
}

Emulator *create_emulator(FILE *text, FILE *data)
{
    Emulator *emulator;
    emulator = (Emulator *)calloc(1, sizeof(Emulator));
    if(emulator==NULL)
        return NULL;

    emulator->memory = (Byte *)calloc(MEMORY_SIZE, sizeof(Byte));
    if((emulator->memory)==NULL) {
        free(emulator);
        return NULL;
    }

    emulator->logs = create_logs();
    
    load_program(emulator, text, data);
    /* set stack pointer */
    emulator->cpu.reg[2]=STACK_BASE;
    /* set global pointer */
    emulator->cpu.reg[3]=DATA_BASE;
    emulator->cpu.stat=1;
    return emulator;
}

int free_emulator(Emulator *emulator)
{
    free_logs(emulator->logs);
    free(emulator->memory);
    free(emulator);
    return 0;
}
