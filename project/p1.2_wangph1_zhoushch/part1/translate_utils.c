/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021. Modified by Lei Jia 
   to be part of the project in Feb 2023.
*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "translate_utils.h"

void write_inst_string(FILE *output, const char *name, char **args,
                       int num_args)
{
    int i;

    fprintf(output, "%s", name);
    for (i = 0; i < num_args; i++)
    {
        fprintf(output, " %s", args[i]);
    }
    fprintf(output, "\n");
}


void write_inst_rtype(FILE *output, const char *name, int rd, int rs1, int rs2)
{
    /* e.g. add rd, rs1, rs2 */
    fprintf(output, "%s x%d x%d x%d\n", name, rd, rs1, rs2);
}

void write_inst_stype(FILE *output, const char *name, int rs1, int rs2, int offset)
{
    /* e.g. sb rs1, offset(rs2) */
    fprintf(output, "%s x%d %d(x%d)\n", name, rs1, offset, rs2);
}

void write_inst_sbtype(FILE *output, const char *name, int rs1, int rs2, int offset)
{   
    /* e.g. beq rs1, rs2, offset */
    fprintf(output, "%s x%d x%d %d\n", name, rs1, rs2, offset);
}

void write_inst_utype(FILE *output, const char *name, int rd, int offset)
{
    /* e.g. auipc rd, offset */
    fprintf(output, "%s x%d %d\n", name, rd, offset);
}

void write_inst_ecall(FILE *output, const char *name)
{   
    /* e.g. ecall */
    if (strcmp(name, "ecall") == 0)
        fprintf(output, "ecall\n");
}

void write_inst_itype(FILE *output, const char *name, int rd, int rs1, int imm)
{
    /* e.g. addi rd, rs1, imm */
    fprintf(output, "%s x%d x%d %d\n", name, rd, rs1, imm);
}

void write_inst_hex(FILE *output, uint32_t instruction)
{
    fprintf(output, "%08x\n", instruction);
}

int is_valid_label(const char *str)
{
    int first = 1;
    if (!str)
    {
        return 0;
    }
    /* read until the label is ended */
    while (*str)
    {
        if (first)
        {
            if (!isalpha((int)*str) && *str != '_')
            {
                return 0; /* does not start with letter or underscore */
            }
            else
            {
                first = 0;
            }
        }
        else if (!isalnum((int)*str) && *str != '_')
        {
           
            return 0; /* subsequent characters not alphanumeric */
        }
        str++;
    }
    return first ? 0 : 1; /* empty string is invalid  */
}

/* Translate the input string into a signed number. The number is then
   checked to be within the correct range (note bounds are INCLUSIVE)
   ie. NUM is valid if LOWER_BOUND <= NUM <= UPPER_BOUND.

   The input may be in either positive or negative, and be in either
   decimal or hexadecimal format. It is also possible that the input is not
   a valid number. Fortunately, the library function strtol() can take
   care of all that (with a little bit of work from your side of course).
   Please read the documentation for strtol() carefully. Do not use strtoul()
   or any other variants.

   You should store the result into the location that OUTPUT points to. The
   function returns 0 if the execution proceeded without errors, or -1 if an
   error occurred.
 */
int translate_num(long int *output, const char *str, long int lower_bound,
                  long int upper_bound)
{

    /* YOUR CODE HERE */
    /* use strol to convert */
    char *end_ptr;
    /* return error if bound is exceeded */
    if (!str || !output || (lower_bound > upper_bound)) {
        return -1;
    }
    /* Convert to long integer */
    *output = strtol(str, &end_ptr, 0);
    /* Check bound */
    if ((*output > upper_bound) || (*output < lower_bound)) {
        return -1;
    }
    /* Check if the string is valid */
    if (*end_ptr != '\0') {
        return -1;
    }
    return 0;
}

int translate_num_bits(long int *output, const char *str, int bits)
{
    char *end_ptr;
    long res;

    /* Obtain the bound of n-bit integer */
    const unsigned long usgn_upper_bound = USNG_UPPER_BOUND(bits);
    const long upper_bound = SNG_UPPER_BOUND(bits);
    const long lower_bound = SNG_LOWER_BOUND(bits);

    /* return error if bound is exceeded */
    if (!str || !output || bits > 64)
    {
        return -1;
    }

    /* Convert to long integer */
    res = strtol(str, &end_ptr, 0);
    if (*end_ptr != '\0')
    {
        return -1;
    }

    /* Regarding the return value:
       0 indicates the string is interpreted as decimal
       1 indicates the string is interpreted as hexadecimal
    */
    if (strlen(str) > 2 && str[0] == '0' && str[1] == 'x')
    {
        /* Hex */
        if ((unsigned long)res > usgn_upper_bound || res < 0)
        {
            return -1;
        }
        /* Map unsigned hexadecimal to signed interval */
        sign_extension(&res, res, bits);
        *output = res;
        return 1;
    }
    else
    {
        /* Decimal */
        /* Check bound */
        if (res > upper_bound || res < lower_bound)
        {
            return -1;
        }
        *output = res;
        return 0;
    }
}

int sign_extension(long int *output, long input, int bits)
{
    /* check input: the input should never be larger than the unsigned bound */
    /* Regarding return value:
       0 indicates the input is positive, no need to extend
       1 indicates extension happened
       -1 the given unsigned integer falles out of range
       */
    if (0 <= input && (unsigned long)input <= USNG_UPPER_BOUND(bits))
    {
        if (input > SNG_UPPER_BOUND(bits))
        {
            /* map unsigned integer to signed integer
               there's a simple law present in the following expression  */
            *output = input - USNG_UPPER_BOUND(bits) - 1;
            return 1;
        }
        else
        {
            /* the integer falls on the positive interval */
            *output = input;
            return 0;
        }
    }
    return -1;
}

/*
    Rules for translate 12-bit number: in most cases, 12-bit integer can be
    obtained by 32-bit representation or 12-bit representation. Since some
    instructions just take 32-bit integers, but they are limited with 12 bits,
    32-bit representation is also allowd. For example, 0xFFFFFFFF can be
   interpreted as -1 and allowed to be passed into addi. However, if we only
   allow 12-bit representation, 0xFFFFFFFF are seems too long to be fit in imm
   field. Hence, we should consider both cases.
*/
int translate_num_12(long int *output, const char *str)
{
    long res;
    int ret;
    /* check 12-bit representation first */
    if ((ret = translate_num_bits(&res, str, 12)) == -1)
    {
        /* check 32-bit representation then */
        if ((ret = translate_num_32(&res, str)) != -1)
        {
            /* remember to check bound when 32-bit representation */
            if (res < SNG_LOWER_BOUND(12) || res > SNG_UPPER_BOUND(12))
            {
                ret = -1;
            }
        }
    }
    if (ret != -1)
        *output = res;
    return ret;
}

/*
    Rules for translate 20-bit number are quite similar, but you may note that,
    in most cases, instructions with 20-bit imm field usually take unsigned
   integers as arguments. In those cases, DO NOT USE translate_num_20, for it
   only interprets sign integers.
*/
int translate_num_20(long int *output, const char *str)
{
    long res;
    int ret;
    /* check 20-bit representation first */
    if ((ret = translate_num_bits(&res, str, 20)) == -1)
    {
        /* check 32-bit representation then */
        if ((ret = translate_num_32(&res, str)) != -1)
        {
            /* remember to check bound when 32-bit representation */
            if (res < SNG_LOWER_BOUND(20) || res > SNG_UPPER_BOUND(20))
            {
                ret = -1;
            }
        }
    }
    if (ret != -1)
        *output = res;
    return ret;
}

/* Simply translate from 32-bit representation */
int translate_num_32(long int *output, const char *str)
{
    return translate_num_bits(output, str, 32);
}

/* Translates the register name to the corresponding register number. Please
   see the RISC-V Green Sheet for information about register numbers.

   Returns the register number of STR or -1 if the register name is invalid.
 */
int translate_reg(const char *str)
{
    /* YOUR CODE HERE */
    if (!str)
        return -1;
    /* determine the type */
    if (str[0] =='x')
    {
        long int temp = -1;
        translate_num(&temp, str+1, 0, 31);
        return temp;
    }
    /* determine the type */
    if (strcmp(str, "zero") == 0)
        return 0;
    /* determine the type */
    if (strcmp(str, "ra") == 0)
        return 1;
    /* determine the type */
    if (strcmp(str, "sp") == 0)
        return 2;
    /* determine the type */
    if (strcmp(str, "gp") == 0)
        return 3;
    /* determine the type */
    if (strcmp(str, "tp") == 0)
        return 4;
    /* determine the type */
    if (strcmp(str, "t0") == 0)
        return 5;
    /* determine the type */
    if (strcmp(str, "t1") == 0)
        return 6;
    /* determine the type */
    if (strcmp(str, "t2") == 0)
        return 7;
    /* determine the type */
    if (strcmp(str, "s0") == 0)
        return 8;
    /* determine the type */
    if (strcmp(str, "s1") == 0)
        return 9;
    /* determine the type */
    if (strcmp(str, "a0") == 0)
        return 10;
    /* determine the type */
    if (strcmp(str, "a1") == 0)
        return 11;
    /* determine the type */
    if (strcmp(str, "a2") == 0)
        return 12;
    /* determine the type */
    if (strcmp(str, "a3") == 0)
        return 13;
    /* determine the type */
    if (strcmp(str, "a4") == 0)
        return 14;
    /* determine the type */
    if (strcmp(str, "a5") == 0)
        return 15;
    /* determine the type */
    if (strcmp(str, "a6") == 0)
        return 16;
    /* determine the type */
    if (strcmp(str, "a7") == 0)
        return 17;
    /* determine the type */
    if (strcmp(str, "s2") == 0)
        return 18;
    /* determine the type */
    if (strcmp(str, "s3") == 0)
        return 19;
    /* determine the type */
    if (strcmp(str, "s4") == 0)
        return 20;
    /* determine the type */
    if (strcmp(str, "s5") == 0)
        return 21;
    /* determine the type */
    if (strcmp(str, "s6") == 0)
        return 22;
    /* determine the type */
    if (strcmp(str, "s7") == 0)
        return 23;
    /* determine the type */
    if (strcmp(str, "s8") == 0)
        return 24;
    /* determine the type */
    if (strcmp(str, "s9") == 0)
        return 25;
    /* determine the type */
    if (strcmp(str, "s10") == 0)
        return 26;
    /* determine the type */
    if (strcmp(str, "s11") == 0)
        return 27;
    /* determine the type */
    if (strcmp(str, "t3") == 0)
        return 28;
    /* determine the type */
    if (strcmp(str, "t4") == 0)
        return 29;
    /* determine the type */
    if (strcmp(str, "t5") == 0)
        return 30;
    /* determine the type */
    if (strcmp(str, "t6") == 0)
        return 31;
    /* determine the type */
    return -1;
}
