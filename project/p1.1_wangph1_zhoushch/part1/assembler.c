/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021. Modified by Lei Jia 
   to be part of the project in Feb 2023.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "tables.h"
#include "translate.h"
#include "translate_utils.h"

#define MAX_ARGS 3
#define BUF_SIZE 1024
const char *IGNORE_CHARS = " \f\n\r\t\v,()";

/*******************************
 * Helper Functions
 *******************************/

/* you should not be calling this function yourself. */
static void raise_label_error(uint32_t input_line, const char *label)
{
    write_to_log("Error - invalid label at line %d: %s\n", input_line, label);
}

/* call this function if more than MAX_ARGS arguments are found while parsing
   arguments.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.

   EXTRA_ARG should contain the first extra argument encountered.
 */
static void raise_extra_argument_error(uint32_t input_line,
                                       const char *extra_arg)
{
    write_to_log("Error - extra argument at line %d: %s\n", input_line,
                 extra_arg);
}

/* You should call this function if write_original_code() or translate_inst()
   returns 0.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.
 */
static void raise_instruction_error(uint32_t input_line, const char *name,
                                    char **args, int num_args)
{

    write_to_log("Error - invalid instruction at line %d: ", input_line);
    log_inst(name, args, num_args);
}

/* Truncates the string at the first occurrence of the '#' character. */
static void skip_comments(char *str)
{
    char *comment_start = strchr(str, '#');
    if (comment_start)
    {
        *comment_start = '\0';
    }
}

/* Reads STR and determines whether it is a label (ends in ':'), and if so,
   whether it is a valid label, and then tries to add it to the symbol table.

   INPUT_LINE is which line of the input file we are currently processing. Note
   that the first line is line 1 and that empty lines are included in this
   count.

   BYTE_OFFSET is the offset of the NEXT instruction (should it exist).

   Four scenarios can happen:
    1. STR is not a label (does not end in ':'). Returns 0.
    2. STR ends in ':', but is not a valid label. Returns -1.
    3a. STR ends in ':' and is a valid label. Addition to symbol table fails.
        Returns -1.
    3b. STR ends in ':' and is a valid label. Addition to symbol table succeeds.
        Returns 1.
 */
static int add_if_label(uint32_t input_line, char *str, uint32_t byte_offset,
                        SymbolTable *symtbl, int check_align)
{
    
    size_t len = strlen(str);
    if (str[len - 1] == ':')
    {
        str[len - 1] = '\0';
        if (is_valid_label(str))
        {
            /* add to table and return number of tokens, otherwise return -1 */
            if (add_to_table(symtbl, str, byte_offset, check_align) == 0)
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            /* we raise error if no valid label is found */
            raise_label_error(input_line, str);
            return -1;
        }
    }
    else
    {
        return 0;
    }
}

/*******************************
 * Implement the Following
 *******************************/

/* First pass of the assembler. You should implement pass_two() first.

   This function should read each line, strip all comments, scan for labels,
   and pass instructions to write_original_code(), pass data to write_static_data(). 
   The symbol table and data segment should also been built and written to 
   specified file. The input file may or may not be valid. Here are some guidelines:

   1. Only one of ".data" or ".text" may be present per line. It must be the 
      first token present. Once you see a ".data" or ".text", the rest part
      will be treat as data or text segment until the next ".data" or ".text"
      Appears. Ignore the rest token of this line. Default to text segment 
      if not indicated.
    
   2. For data segment, we only considering ".word", ".half", ".Byte" types.
      These types must be the first token of a line. The rest of the tokens 
      in this line will be stored as variables of that type in the data segment.
      You can assume that these numbers are legal with their types.

   3. Only one label may be present per line. It must be the first token
      present. Once you see a label, regardless of whether it is a valid label
      or invalid label, You can treat the NEXT token in this line as the 
      beginning of an instruction or a static data.

   4. If the first token is not a label, treat it as the name of an
      instruction. DO NOT try to check it is a valid instruction in this pass.
   
   5. Everything after the instruction name in the same line should be treated 
      as arguments to that instruction. If there are more than MAX_ARGS 
      arguments, call raise_extra_argument_error() and pass in the first extra 
      argument. Do not write that instruction to the original file (eg. don't call
      write_original_code())
   
   6. Only one instruction should be present per line. You do not need to do
       anything extra to detect this - it should be handled by guideline 5.
   
   8. A line containing only a label is valid. The address of the label should
      be the address of the next instruction or static data regardless of 
      whether there is a next instruction or static data or not.
   
   9. If an instruction contains an immediate, you should output it AS IS.
   
   10. Comments should always be skipped before any further process.

   11. Note that the starting address of the text segment is defined as TEXT_BASE
   in "../types.", and the starting address of the data segment is defined as 
   DATA_BASE in "../types.".

   12. Think about how you should use write_original_code() and write_static_data().
   
   Just like in pass_two(), if the function encounters an error it should NOT
   exit, but process the entire file and return -1. If no errors were
   encountered, it should return 0.
 */

int pass_one(FILE *input, FILE *original, FILE *symtbl, FILE *data) {
    /* YOUR CODE HERE */
    /* read each line to a buffer */
    char buffer[BUF_SIZE];
    /* processed string */
    char *prostr;
    /* keep track of error */
    int error = 0;
    /* keep track of line number */
    int line = 0;
    /* define table */
    SymbolTable* table = NULL;
    /* define addr */
    /* uint32_t addr = 0; */
    /* number of args */
    int num_args = 0;
    /* define res lines */
    int resline = 0;
    /* define the args char for parsing instruction */
    char *arg_list[MAX_ARGS];
    /* define instruction name */
    char* instruction_name = NULL;
    /* add label res */
    int add_res = 0;
    /* text offset */
    int off_text = TEXT_BASE;
    /* data offset */
    int off_data = DATA_BASE;
    /* mark text segment */
    int text_seg = 1;
    /* mark data segment */
    int data_seg = 0;
    /* translate num */
    long int tra_num = 0;
    /* define the buffer of data 64 in case of overflow */
    uint64_t data_buffer = 0;
    /* track data buffer */
    int data_buffer_track = 0;
    /* determine NULL cases */
    if (input == NULL || original == NULL || symtbl == NULL || data == NULL)
    {
        return -1;
    }
    /* init the table */
    table = create_table(SYMBOLTBL_UNIQUE_NAME);
    /* get each line with loop */
    while (fgets(buffer, BUF_SIZE, input))
    {
        /* increment line */
        line++;
        /* skip comments */
        skip_comments(buffer);
        /* use strtok */
        if ((prostr = strtok(buffer, IGNORE_CHARS)) == NULL)
        {
            /* handle null str */
            continue;
        }
        /* determine if text */
        if (strcmp(prostr, ".text") == 0)
        {
            /* mark text segment */
            text_seg = 1;
            /* mark data segment */
            data_seg = 0;
            /* continut as there is no more info */
            continue;
        }
        if (strcmp(prostr, ".data") == 0)
        {
            /* mark text segment */
            text_seg = 0;
            /* mark data segment */
            data_seg = 1;
            /* continut as there is no more info */
            continue;
        }
        if (text_seg)
        {
            /* deal with label and use align */
            add_res = add_if_label(line, prostr, off_text, table, text_seg);
        }
        /* not text segment */
        else
        {
            /* deal with label and use align */
            add_res = add_if_label(line, prostr, off_data, table, text_seg);
        }
        /* -1 not valid */
        if (add_res == -1)
        {
            /* do not raise */
            error = -1;
            prostr = strtok(NULL, IGNORE_CHARS);
        }
        /* 1 valid */
        if (add_res == 1)
        {
            /* get next token */
            prostr = strtok(NULL, IGNORE_CHARS);
        }
        if (prostr == NULL)
        {
            /* no more token */
            continue;
        }
        /* deal with text segment */
        /* default text segment */
        if (text_seg)
        {
            /* loop for args */
            for (num_args = 0; num_args < MAX_ARGS; num_args++)
            {
                /* by passing NULL to strtok to continue find*/
                if (!(arg_list[num_args] = strtok(NULL, IGNORE_CHARS)))
                {
                    /* no more args find then break */
                    break;
                }
            }
            /* value for instruction name */
            instruction_name = prostr;
            if ((prostr = strtok(NULL, IGNORE_CHARS)) != NULL)
            {
                /* raise error */
                raise_extra_argument_error(line, prostr);
                error = -1;
            }
            else
            {
                /* success then write code */
                resline = write_original_code(original, instruction_name, arg_list, num_args);
                /* no lines fail */
                if (resline == 0)
                {
                    /* raise error */
                    raise_instruction_error(line, instruction_name, arg_list, num_args);
                    error = -1;
                }
                /* update offset */
                off_text += resline * 4;
            }
        }
        /* deal with data segment*/
        if (data_seg)
        {
            /* byte type: single byte */
            if (!strcmp(prostr, ".byte"))
            {
                int need_bits = 0;
                /* while loop to find the next */
                while ((prostr = strtok(NULL, IGNORE_CHARS)))
                {
                    /* translate the number */
                    if (translate_num_bits(&tra_num, prostr, 8) == -1)
                    {
                        /* raise error */
                        error = -1;
                        /* continue to next */
                        continue;
                    }
                    /* get the needed bits */
                    need_bits = 8;
                    /* bitwise to get the bits */
                    tra_num &= USNG_UPPER_BOUND(need_bits);
                    tra_num = tra_num << (data_buffer_track << 3);
                    /* update track */
                    data_buffer_track += 1;
                    /* add bytes */
                    data_buffer = data_buffer + tra_num;
                    /* if buffer is full */
                    if (data_buffer_track >= 4)
                    {
                        /* update buffer track */
                        data_buffer_track -= 4;
                        /* write to data */
                        write_static_data(data, (Byte *)&data_buffer);
                    }
                    /* update offset */
                    off_data += 1;
                }
            }
            /* half type: 2 bytes */
            else if (!strcmp(prostr, ".half"))
            {
                /* needed bits */
                int need_bits = 0;
                /* while loop to find the next */
                while ((prostr = strtok(NULL, IGNORE_CHARS)))
                {
                    /* translate the number */
                    if (translate_num_bits(&tra_num, prostr, 16) == -1)
                    {
                        /* raise error */
                        error = -1;
                        /* continue to next */
                        continue;
                    }
                    /* get the needed bits */
                    need_bits = 16;
                    /* bitwise to get the need bits */
                    tra_num &= USNG_UPPER_BOUND(need_bits);
                    tra_num = tra_num << (data_buffer_track << 3);
                    /* update track */
                    data_buffer_track += 2;
                    /* add bytes */
                    data_buffer = data_buffer + tra_num;
                    /* if buffer is full */
                    if (data_buffer_track >= 4)
                    {
                        /* update buffer track */
                        data_buffer_track -= 4;
                        /* write to data */
                        write_static_data(data, (Byte *)&data_buffer);
                    }
                    /* update offset */
                    off_data += 2;
                }
            }
            /* word type: 4 bytes */
            else if (!strcmp(prostr, ".word"))
            {
                int need_bits = 0;
                /* while loop to find the next */
                while ((prostr = strtok(NULL, IGNORE_CHARS)))
                {
                    /* translate the number */
                    if (translate_num_32(&tra_num, prostr) == -1)
                    {
                        /* raise error */
                        error = -1;
                        /* continue to next */
                        continue;
                    }
                    /* get the needed bits */
                    need_bits = 32;
                    /* bit wise to get the needed bits */
                    tra_num &= USNG_UPPER_BOUND(need_bits);
                    tra_num = tra_num << (data_buffer_track << 3);
                    /* update track */
                    data_buffer_track += 4;
                    /* add bytes */
                    data_buffer = data_buffer + tra_num;
                    /* if buffer is full */
                    if (data_buffer_track >= 4)
                    {
                        /* update buffer track */
                        data_buffer_track -= 4;
                        /* write to data */
                        write_static_data(data, (Byte *)&data_buffer);
                    }
                    /* update offset */
                    off_data += 4;
                }
            }
            /* else raise error */
            else
            {
                /* raise error */
                error = -1;
                continue;
            }
        }
    }
    /* deal with left buffer data without align */
    if (data_buffer_track != 0)
    {
        /* write to data */
        write_static_data(data, (Byte *)&data_buffer);
    }
    /* write to symtbl */
    write_table(table, symtbl);
    /* free the table */
    free_table(table);
    /* detect error */
    return error;
}

/* Second pass of the assembler.

   This function should read an original file and the corresponding symbol table
   file, translates it into basic code and machine code. You may assume:
    1. The input file contains no comments
    2. The input file contains no labels
    3. The input file contains at maximum one instruction per line
    4. All instructions have at maximum MAX_ARGS arguments
    5. The symbol table file is well formatted
    6. The symbol table file contains all the symbol required for translation
    7. The line may contain only one "-" which is placeholder you can ignore.
   If an error is reached, DO NOT EXIT the function. Keep translating the rest of
   the document, and at the end, return -1. Return 0 if no errors were encountered. */


int pass_two(FILE *original, FILE *symtbl, FILE *basic, FILE *machine) {

    /* YOUR CODE HERE */
    /* read each line to a buffer */
    char buffer[BUF_SIZE];
    /* keep track of error */
    int error = 0;
    /* keep track of line number */
    uint32_t line = 0;
    /* define res lines */
    int resline = 0;
    /* define table */
    SymbolTable* table = NULL;
    /* define addr */
    uint32_t addr = TEXT_BASE;
    /* define instruction name */
    char* instruction_name = NULL;
    /* number of args */
    int num_args = 0;
    /* define the args char for parsing instruction */
    char *arg_list[MAX_ARGS];
    /* determine special cases */
    if (original == NULL || symtbl == NULL || basic == NULL || machine == NULL)
    {
        return -1;
    }
    /* initialize table */
    table = create_table_from_file(SYMBOLTBL_UNIQUE_NAME, symtbl);
    /* loop */
    while (fgets(buffer, BUF_SIZE, original))
    {
        /* line++ */
        line++;
        /* if buffer is empty or - then continue */
        if (strlen(buffer) == 0 || buffer[0] == '-')
        {
            /* if buffer is empty or - then continue */
            continue;
        }
        instruction_name = strtok(buffer, IGNORE_CHARS);
        if ((instruction_name) == NULL)
        {
            /* if no instruction name then continue */
            continue;
        }
        /* if no instruction name then continue also no extra arg */
        for (num_args = 0; num_args < MAX_ARGS; num_args++)
        {
            /* by passing NULL to strtok to continue find*/
            if (!(arg_list[num_args] = strtok(NULL, IGNORE_CHARS)))
            {
                /* no more args find then break */
                break;
            }
        }
        /* consider 3 args */
        if (num_args == 3)
        {
            /* mark for if swap args */
            int if_swap = 0;
            /* temp var for swap */
            char* swap;
            /* consider special cases */
            if (!strcmp(instruction_name, "lb"))
            {
                /* args */
                if_swap = 1;
            }
            /* consider special cases */
            if (!strcmp(instruction_name, "lh"))
            {
                /*args*/
                if_swap = 1;
            }
            /* consider special cases */
            if (!strcmp(instruction_name, "lw"))
            {
                /*args*/
                if_swap = 1;
            }
            /* consider special cases */
            if (!strcmp(instruction_name, "lbu"))
            {
                /*args*/
                if_swap = 1;
            }
            /* consider special cases */
            if (!strcmp(instruction_name, "lhu"))
            {
                /*args*/
                if_swap = 1;
            }
            /* if swap then swap */
            if (if_swap)
            {
                /* legacy method for swap */
                swap = arg_list[1];
                arg_list[1] = arg_list[2];
                arg_list[2] = swap;
            }
        }
        /* translate the instruction */
        resline = translate_inst(basic, machine, instruction_name, arg_list, num_args, addr, table);
        if (resline == 0)
        {
            /* error then raise error */
            error = -1;
            raise_instruction_error(line, instruction_name, arg_list, num_args);
        }
        else
        {
            /* else addr directly += 4 */
            addr += resline << 2;
        }
    }
    /* free the table */
    free_table(table);
    /* detect error */
    return error;
}


/*******************************
 * Do Not Modify Code Below
 *******************************/


/* Runs the two-pass assembler. Most of the actual work is done in pass_one()
   and pass_two().
 */
int assemble(int mode, FILE *input, FILE *data, FILE *symtbl, FILE *orgin, FILE *basic, FILE *text)
{
    int err = 0;

    /* Execute pass one if mode two is not specified */
    if (mode != 2)
    {
        rewind(input);
        if (pass_one(input, orgin, symtbl, data) != 0)
        {
            err = 1;
        }
    }

    /* Execute pass two if mode one is not specified */
    if (mode != 1)
    {
        rewind(orgin);
        rewind(symtbl);
        if (pass_two(orgin, symtbl, basic, text) != 0)
        {
            err = 1;
        }
    }

    /* Error handling */
    if (err) {
        write_to_log("One or more errors encountered during assembly operation.\n");
    } else {
        write_to_log("Assembly operation completed successfully!\n");
    }

    return err;
}