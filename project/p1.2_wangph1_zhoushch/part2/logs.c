#include <stdlib.h>
#include <string.h>
#include "logs.h"
#include "execute_utils.h"


/*******************************
 * Helper Functions
 *******************************/

/* Creates a new empty Logs and returns a pointer to that Logs. 
   If memory allocation fails, you should call allocation_failed().
 */
Logs *create_logs() {
    /* YOUR CODE HERE */
    Logs *logs = malloc(sizeof(Logs));
    if (logs == NULL) { /* failed to allocate */
        allocation_failed();
    }
    logs->size = 0; /* initialize*/
    logs->capacity = 1;
    /* malloc for log */
    logs->log = malloc(sizeof(Log));
    if (logs->log == NULL) { /* faild to malloc*/
        allocation_failed();
    }
    return logs;
}

/* Frees the given Logs and all associated memory. */
void free_logs(Logs *logs) {
    /* YOUR CODE HERE */
    if (logs->log != NULL) 
        free(logs->log);
    if (logs != NULL)     
        free(logs); /* free memory*/
}

/* Record the current status and save in a logs component.
   Think about what information needs to be recorded.
   Logs can resize to fit an arbitrary number of entries. 
   (so you should use dynamic memory allocation).
 */
void record(Instruction inst, Processor *cpu, Byte *mem, Logs *logs)
{
    /* YOUR CODE HERE */
    /* define i for loop */
    int i = 0;
    if (logs->size == logs->capacity) {
        /* resize */
        logs->capacity *= 2;
        logs->log = realloc(logs->log, logs->capacity * sizeof(Log));
        /* check if realloc failed */
        if (logs->log == NULL) {
            free(logs);
            allocation_failed();
        }
    }
    /* copy register */
    for (i = 0; i < 32; i++) {
        logs->log[logs->size].cpu.reg[i] = cpu->reg[i];
    }
    /* copy PC */
    logs->log[logs->size].cpu.PC = cpu->PC;
    /* copy stat */
    logs->log[logs->size].cpu.stat = cpu->stat;
    /* when use s type, store the mem for future roll back */
    if (inst.stype.opcode == 0x23) {
        logs->log[logs->size].ifstype = 1; /* 1 for s type */
        logs->log[logs->size].value = load(mem, cpu->reg[inst.stype.rs1] + get_store_offset(inst), 4, 0); /* load value */
        logs->log[logs->size].memadd = cpu->reg[inst.stype.rs1] + get_store_offset(inst); /* load mem addr */
    } else {
        /* else do not load value */
        logs->log[logs->size].ifstype = 0;
    }
    logs->size++;
    /* default return */
    return;
}

/* Back to the previous state recorded in a logs component.
   if success, return 0.
   if fail, for example, no previous state, return -1.
 */
int undo(Processor *cpu, Byte *mem, Logs *logs) {
    /* YOUR CODE HERE */
    /* define i for loop */
    int i = 0;
    /* detect if null */
    if (logs == NULL || cpu == NULL || mem == NULL) {
        return -1;
    }
    /* check if logs is empty */
    if (logs->size == 0) {
        return -1;
    }
    /* copy register */
    for (i = 0; i < 32; i++) {
        cpu->reg[i] = logs->log[logs->size - 1].cpu.reg[i];
    }
    /* copy PC */
    cpu->PC = logs->log[logs->size - 1].cpu.PC;
    /* copy stat */
    cpu->stat = logs->log[logs->size - 1].cpu.stat;
    /* when use s type, store the mem for future roll back */
    if (logs->log[logs->size - 1].ifstype == 1) {
        store(mem, logs->log[logs->size - 1].memadd, 4, logs->log[logs->size - 1].value, 0);
    }
    /* decrease size */
    logs->size--;
    /* default return */
    return 0;
}

/* Add function definition here if you need */