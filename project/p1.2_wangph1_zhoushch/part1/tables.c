/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021. Modified by Lei Jia 
   to be part of the project in Feb 2023.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"

const int SYMBOLTBL_NON_UNIQUE = 0;
const int SYMBOLTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

void addr_alignment_incorrect()
{
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char *name)
{
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_sym(FILE *output, uint32_t addr, const char *name)
{
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time.
   If memory allocation fails, you should call allocation_failed().
   Mode will be either SYMBOLTBL_NON_UNIQUE or SYMBOLTBL_UNIQUE_NAME. You will
   need to store this value for use during add_to_table().
 */
SymbolTable *create_table(int mode)
{
    /* YOUR CODE HERE */
    SymbolTable *table = calloc(1, sizeof(SymbolTable));
    if (table == NULL) 
    {
        allocation_failed();
    }
    table->tbl = NULL;
    table->len = 0;
    table->cap = 0;
    table->mode = mode;
    return table;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable *table)
{
    /* YOUR CODE HERE */

    /* free the SymbolTable */
    size_t i;
    if(table == NULL) {
        return;
    }
    for(i = 0; i < table->len ;i++)
        free(table->tbl[i].name);
    free(table->tbl);
    free(table);
    /* end of the function */
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE. 
   1. ADDR is given as the byte offset from the first instruction. 
   2. The SymbolTable must be able to resize itself as more elements are added. 

   3. Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   4. If ADDR is not word-aligned and check_align==1, you should call 
   addr_alignment_incorrect() and return -1. 

   5. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists 
   in the table, you should call name_already_exists() and return -1. 

   6.If memory allocation fails, you should call allocation_failed(). 

   Otherwise, you should store the symbol name and address and return 0. 
 */
int add_to_table(SymbolTable *table, const char *name, uint32_t addr, int check_align)
{
    /* YOUR CODE HERE */
    /* check if the address is word-aligned */
    uint32_t i;
    /* check if null table */
    if (table == NULL || name == NULL) {
        return -1;
    }
    if ((check_align == 1) && (addr % 4 != 0)) {
        addr_alignment_incorrect();
        return -1;
    }
    /* check if the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists in the table */
    if (table->mode == SYMBOLTBL_UNIQUE_NAME) {
        /* check if the symbol is already in the table */
        for (i = 0; i < table->len; i++) {
            /* if the symbol is already in the table, return -1 */
            if (strcmp(table->tbl[i].name, name) == 0) {
                name_already_exists(name);
                return -1;
            }
            /* end of the for loop */
        }
    }

    /* check if the table needs to be resized */
    if (table->len == table->cap) {
        /* double the capacity of the table */
        if(table->cap == 0)
            table->cap = 1;
        else
            table->cap *= 2;
        
        if(table->tbl == NULL)
        {
            table->tbl = (Symbol *)malloc(sizeof(Symbol));
        }
        else
        {
           /* realloc the SymbolTable */
            table->tbl = (Symbol *)realloc(table->tbl, table->cap * sizeof(Symbol));
        }
        /* check if realloc fails */
        if (table->tbl == NULL) {
            allocation_failed();
            return -1;
        }
    }
    /* store the symbol name and address */
    /* printf("table, %d\n", table->len); */
    table->tbl[table->len].name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(table->tbl[table->len].name, name);

    table->tbl[table->len].addr = addr;
    table->len++;
    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable *table, const char *name)
{
    /* YOUR CODE HERE */
    /* check if the symbol is present in the table */
    uint32_t i;
    /* check if null table */
    if (table == NULL) {
        return -1;
    }
    for (i = 0; i < table->len; i++) {
        /* if the symbol is present in the table, return its address */
        if (strcmp(table->tbl[i].name, name) == 0) {
            return table->tbl[i].addr;
        }
    }
    /* if the symbol is not present in the table, return -1 */
    return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_sym() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable *table, FILE *output)
{
    /* YOUR CODE HERE */
    /* write the SymbolTable to OUTPUT */
    uint32_t i;
    for (i = 0; i < table->len; i++) {
        write_sym(output, table->tbl[i].addr, table->tbl[i].name);
    }
    /* end of the function */
}

/* DO NOT MODIFY THIS LINE BELOW */
/* ===================================================================== */
/* Read the symbol table file written by the `write_table` function, and
   restore the symbol table.
 */
SymbolTable *create_table_from_file(int mode, FILE *file)
{
    SymbolTable *symtbl = create_table(mode);
    char buf[1024];
    while (fgets(buf, sizeof(buf), file))
    {
        char *name, *addr_str, *end_ptr;
        uint32_t addr;
        addr_str = strtok(buf, "\t");
        /* create table based on these tokens */
        addr = strtol(addr_str, &end_ptr, 0);
        name = strtok(NULL, "\n");
        /* add the name to table, one by one */
        add_to_table(symtbl, name, addr, 0);
    }
    return symtbl;
}