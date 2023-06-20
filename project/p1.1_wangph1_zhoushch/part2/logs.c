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
}

/* Frees the given Logs and all associated memory. */
void free_logs(Logs *logs) {
    /* YOUR CODE HERE */
}

/* Record the current status and save in a logs component.
   Think about what information needs to be recorded.
   Logs can resize to fit an arbitrary number of entries. 
   (so you should use dynamic memory allocation).
 */
void record(Instruction inst, Processor *cpu, Byte *mem, Logs *logs)
{
    /* YOUR CODE HERE */
}

/* Back to the previous state recorded in a logs component.
   if success, return 0.
   if fail, for example, no previous state, return -1.
 */
int undo(Processor *cpu, Byte *mem, Logs *logs) {
    /* YOUR CODE HERE */
}

/* Add function definition here if you need */