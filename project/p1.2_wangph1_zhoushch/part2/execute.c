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
    } /* raise error*/
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

    For unrecognised instructions you should use raise_machine_code_error().
    For undefined ecalld calls you should use raise_undefined_ecall_error().
    During access operations, the alignment is checked according to the corresponding instruction.
    Raise error when an error occurs and then continue with the next instruction without interrupting or exiting the program.
 */

void execute(Instruction inst, Processor *p, Byte *memory)
{
    /* printf("%#x\n", p->PC); */

    switch (inst.opcode.opcode)
    { 
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    case 0x33: /* rtype's opcode */
        execute_rtype(inst, p);
        break;
    case 0x13: /* itype's opcode wxcept load, ecall, jalr*/
        execute_itype_except_load(inst, p);
        break;
    case 0x73: /* ecall's opcode */
        execute_ecall(p, memory);
        break;
    case 0x63: /* branch's opcode (SB type) */
        execute_branch(inst, p);
        break;
    case 0x03: /* load's opcode */
        execute_load(inst, p, memory);
        break;
    case 0x23: /* store's opcode (stype) */
        execute_store(inst, p, memory);
        break;
    case 0x6f: /* jal's opcode (ujtype) */
        execute_jal(inst, p);
        break;
    case 0x67: /* jalr's opcode */
        execute_jalr(inst, p);
        break;
    case 0x17: /* auipc's opcode (utype) */
        execute_utype(inst, p);
        break;
    case 0x37: /* lui's opcode (utype) */
        execute_utype(inst, p);
        break;
    default: /* undefined opcode */
        raise_machine_code_error(inst);
        p->PC += 4;
        break;
    }
}

void execute_rtype(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
    unsigned int func3 = inst.rtype.funct3; /* func3 of the machine code */
    unsigned int func7 = inst.rtype.funct7; /* func7 of the machine code */
    unsigned int rd = inst.rtype.rd;       /* rd of the machine code */
    unsigned int rs1 = inst.rtype.rs1;     /* rs1 of the machine code */
    unsigned int rs2 = inst.rtype.rs2;     /* rs2 of the machine code */

    if (func3 == 0x0 && func7 == 0x00) /* add */
    {
        p->reg[rd] = p->reg[rs1] + p->reg[rs2];
        /*if(rd == 11)
            printf("add %u %u %d x0 = %u, a0 = %u, a1 = %u\n", rd, rs1, rs2,p->reg[rs1],p->reg[rs2], p->reg[rd]);*/
    }
    else if (func3 == 0x0 && func7 == 0x01) /* mul */
    {
        /* (R[rs1] * R[rs2]) [31:0] */
        /*int64_t ans = (int64_t)p->reg[rs1] * (int64_t)p->reg[rs2];
        p->reg[rd] = (Word)(ans & (uint64_t)0x00000000ffffffff);*/
        p->reg[rd] = (int)((int)p->reg[rs1] * (int)p->reg[rs2]);
    }
    else if (func3 == 0x0 && func7 == 0x20) /* sub */
    {
        p->reg[rd] = p->reg[rs1] - p->reg[rs2];
    }
    else if (func3 == 0x1 && func7 == 0x00) /* sll */
    {
        p->reg[rd] = p->reg[rs1] << p->reg[rs2];
    }
    else if (func3 == 0x1 && func7 == 0x01) /* mulh */
    {
        /* (R[rs1] * R[rs2]) [63:32] */
        p->reg[rd] = (unsigned int)(((long)(int)p->reg[rs1] * (long)(int)p->reg[rs2]) >> 32);
    }
    else if (func3 == 0x2 && func7 == 0x00) /* slt */
    {
        p->reg[rd] = (int)p->reg[rs1] < (int)p->reg[rs2];
    }
    else if (func3 == 0x3 && func7 == 0x00) /* sltu */
    {
        p->reg[rd] = (unsigned int)p->reg[rs1] < (unsigned int)p->reg[rs2];
    }
    else if (func3 == 0x4 && func7 == 0x00) /* xor */
    {
        p->reg[rd] = p->reg[rs1] ^ p->reg[rs2];
    }
    else if (func3 == 0x4 && func7 == 0x01) /* div */
    {
        p->reg[rd] = (int)p->reg[rs1] / (int)p->reg[rs2];
    }
    else if (func3 == 0x5 && func7 == 0x00) /* srl */
    {
        /* >> is arithmatic shift right */
        p->reg[rd] = (unsigned int)p->reg[rs1] >> p->reg[rs2];
    }
    else if (func3 == 0x5 && func7 == 0x20) /* sra */
    {
        /* >> is arithmatic shift right */
        p->reg[rd] = (unsigned int)p->reg[rs1] >> p->reg[rs2];
        p->reg[rd] = bitSigner(p->reg[rd], 32 - p->reg[rs2]);
    }
    else if (func3 == 0x6 && func7 == 0x00) /* or */
    {
        p->reg[rd] = p->reg[rs1] | p->reg[rs2];
    }
    else if (func3 == 0x6 && func7 == 0x01) /* rem */
    {
        p->reg[rd] = (int)p->reg[rs1] % (int)p->reg[rs2];
    }
    else if (func3 == 0x7 && func7 == 0x00) /* and */
    {
        p->reg[rd] = p->reg[rs1] & p->reg[rs2];
    }
    else /* unrecognised instructions */
    {
        raise_machine_code_error(inst);
    }

    if(rd == 0) /* rd is zero*/
    {
        p->reg[rd] = 0; /* x0 cannot be modified */
    }

    p->PC += 4; /* move to the next instruction*/
    return; /* return */
}

void execute_itype_except_load(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
    unsigned int funct3 = inst.itype.funct3; /* funct3 of the machine code */
    unsigned int rd = inst.itype.rd;       /* rd of the machine code */
    unsigned int rs1 = inst.itype.rs1;     /* rs1 of the machine code */
    int imm = get_imm_operand(inst); /* imm of the machine code */
    unsigned int func7 = (unsigned int)imm >> 5;


    if(funct3 == 0x0) /* addi*/
    {   
        /*printf("imm = %u\n", imm);
        printf("a2 = %u\n", p->reg[rs1]);
        printf("a0 = %u\n", p->reg[10]);*/
        p->reg[rd] = p->reg[rs1] + imm;
        /*if(rd == 11)
            printf("addi %u %u %d %u\n", rd, rs1, imm, p->reg[rd]);*/
    }
    else if(funct3 == 0x1 && func7 == 0x00) /* slli */
    {
        imm &= 0x1f;
        p->reg[rd] = p->reg[rs1] << imm;
    }
    else if(funct3 == 0x2) /* slti */
    {
        p->reg[rd] = (int)p->reg[rs1] < (int)imm;
    }
    else if(funct3 == 0x3) /* sltiu */
    {
        p->reg[rd] = (unsigned int)p->reg[rs1] < (unsigned int)imm;
    }
    else if(funct3 == 0x4) /* xori */
    {
        p->reg[rd] = p->reg[rs1] ^ imm;
    }
    else if(funct3 == 0x5 && func7 == 0x00) /* srli */
    {
        /* >> is arithmatic shift right */
        imm &= 0x1f;
        p->reg[rd] = (unsigned int)p->reg[rs1] >> imm;
        
    }
    else if(funct3 == 0x5 && func7 == 0x20) /* srai */
    {
        /* >> is arithmatic shift right */
        /* imm -= 0x20; */
        imm &= 0x1f;
        p->reg[rd] = (unsigned int)p->reg[rs1] >> imm;
        p->reg[rd] = bitSigner(p->reg[rd], 32 - imm);

    }
    else if(funct3 == 0x6) /* ori */
    {
        p->reg[rd] = p->reg[rs1] | imm;
    }
    else if(funct3 == 0x7) /* andi */
    {
        p->reg[rd] = p->reg[rs1] & imm;
    }
    else /* unrecognised instructions */
    {
        raise_machine_code_error(inst);
    }

    if(rd == 0) /* rd is zero*/
    {
        p->reg[rd] = 0; /* x0 cannot be modified */
    }

    p->PC += 4; /* move to the next instruction*/
    return; /* return */
}

/* You may need to use exit_program() */
void execute_ecall(Processor *p, Byte *memory)
{
    /* YOUR CODE HERE */
    unsigned int a0 = p->reg[10]; /* a0 is x10 */
    unsigned int a1 = p->reg[11]; /* a1 is x11 */
    if(a0 == 1) /* print value of a1 as an integer */
    {
        write_to_log("%d", a1);
    }
    else if(a0 == 4) /* print string at adress a1 */
    {
        char c;
        while(1) /* loop */
        {
            /* tranlate word into byte */
            /* c = load(memory, a1, LENGTH_BYTE, 0); */
            /*
            c = 0xff & load(memory, a1, LENGTH_BYTE, 0);
            c &= 0xff; *//* get the first byte */

            c = load(memory, a1, LENGTH_WORD, 0);
            if(c == '\0') /* print until the '\0' */
            {
                break;
            }
            write_to_log("%c", c); /* print the character*/
            ++a1; /* move to the next character */
        }
    }
    else if(a0 == 10) /* exit or end of code indicator */
    {
        exit_program(p);
    }
    else if(a0 == 11) /* print the value of a1 as a characeter */
    {
        write_to_log("%c", a1);
    }
    else /* undefined ecall */
    {
        raise_undefined_ecall_error(a0);
    }

    p->PC += 4; /* move to the next instruction*/
    return; /* return */
}

void execute_branch(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
    unsigned int funct3 = inst.sbtype.funct3; /* funct3 of the machine code */
    unsigned int rs1 = inst.sbtype.rs1;     /* rs1 of the machine code */
    unsigned int rs2 = inst.sbtype.rs2;     /* rs2 of the machine code */
    int imm = get_branch_offset(inst); /* imm of the machine code */

    if(funct3 == 0x0) /* beq */
    {
        if(p->reg[rs1] == p->reg[rs2])
        {
            p->PC += imm; /* goto the pc + offset*/
        }
        else
        {
            p->PC += 4; /* goto next instruction*/
        }
    }
    else if(funct3 == 0x1) /* bne */
    {
        if(p->reg[rs1] != p->reg[rs2])
        {
            p->PC += imm;  /* goto the pc + offset*/
        }
        else
        {
            p->PC += 4; /* goto next instruction*/
        }
    }
    else if(funct3 == 0x4) /* blt */
    {
        if((int)p->reg[rs1] < (int)p->reg[rs2])
        {
            p->PC += imm; /* goto the pc + offset*/
        }
        else
        {
            p->PC += 4; /* goto next instruction*/
        }
    }
    else if(funct3 == 0x5) /* bge */
    {
        if((int)p->reg[rs1] >= (int)p->reg[rs2])
        {
            p->PC += imm; /* goto the pc + offset*/
        }
        else
        {
            p->PC += 4; /* goto next instruction*/
        }
    }
    else if(funct3 == 0x6) /* bltu */
    {
        if((unsigned int)p->reg[rs1] < (unsigned int)p->reg[rs2])
        {
            p->PC += imm; /* goto the pc + offset*/
        }
        else
        {
            p->PC += 4; /* goto next instruction*/
        }
    }
    else if(funct3 == 0x7) /* bgeu */
    {
        if((unsigned int)p->reg[rs1] >= (unsigned int)p->reg[rs2])
        {
            p->PC += imm; /* goto the pc + offset*/
        }
        else
        {
            p->PC += 4; /* goto next instruction*/
        }
    }
    else /* unrecognised instructions */
    {
        raise_machine_code_error(inst);
        p->PC += 4; /* goto next instruction*/
    }

    return; /* return */
}

void execute_load(Instruction inst, Processor *p, Byte *memory)
{
    /* YOUR CODE HERE */
    unsigned int funct3 = inst.itype.funct3; /* funct3 of the machine code */
    unsigned int rd = inst.itype.rd;       /* rd of the machine code */
    unsigned int rs1 = inst.itype.rs1;     /* rs1 of the machine code */
    int imm = get_imm_operand(inst); /* imm of the machine code */

    if(funct3 == 0x0) /* lb */
    {
        p->reg[rd] = load(memory, p->reg[rs1] + imm, LENGTH_BYTE, 1); /* load the byte */
        p->reg[rd] = bitSigner(p->reg[rd], 8); /* a byte is 8 bits */
    }
    else if(funct3 == 0x1) /* lh */
    {
        p->reg[rd] = load(memory, p->reg[rs1] + imm, LENGTH_HALF_WORD, 1); /* load the bytes */
        p->reg[rd] = bitSigner(p->reg[rd], 16); /* a halfword is 16 bits */
    }
    else if(funct3 == 0x2) /* lw */
    {

        p->reg[rd] = load(memory, p->reg[rs1] + imm, LENGTH_WORD, 1);  /* load the bytes */
        /**_rd = load(memory, p->reg[rs1] + imm, LENGTH_WORD, 1);*/

        /*if(rd == 10)
            printf("a0 = %u\n", p->reg[rd]);*/

        /*p->reg[rd] = bitSigner(p->reg[rd], 32);  */ /* a fword is 32 bits */

        /*if(rd == 10)
            printf("a0 = %u\n", p->reg[rd]);*/
        
        /*printf("lw x[rd] = %u, x[rs1] = %u, imm = %d\n", p->reg[rd], p->reg[rs1], imm);*/
    }
    else if(funct3 == 0x4) /* lbu */
    {
        p->reg[rd] = load(memory, p->reg[rs1] + imm, LENGTH_BYTE, 1); /* load the byte */
    }
    else if(funct3 == 0x5) /* lhu */
    {
        p->reg[rd] = load(memory, p->reg[rs1] + imm, LENGTH_HALF_WORD, 1); /* load the bytes */
    }
    else /* unrecognised instructions */
    {
        raise_machine_code_error(inst);
    }

    p->PC += 4; /* move to the next instruction*/
    return; /* return */
}

void execute_store(Instruction inst, Processor *p, Byte *memory)
{
    /* YOUR CODE HERE */
    
    unsigned int funct3 = inst.stype.funct3; /* funct3 of the machine code */
    unsigned int rs1 = inst.stype.rs1;     /* rs1 of the machine code */
    unsigned int rs2 = inst.stype.rs2;     /* rs2 of the machine code */
    int imm = get_store_offset(inst); /* imm of the machine code */

    if(funct3 == 0x0) /* sb */
    {
        /* Mem[R[rs1] + offset] <- R[rs2][7:0] */
        store(memory, p->reg[rs1] + imm, LENGTH_BYTE, (p->reg[rs2] & 0xff), 1); /* store the byte */
    }
    else if(funct3 == 0x1) /* sh */
    {
        /* Mem[R[rs1] + offset] <- R[rs2][15:0] */
        store(memory, p->reg[rs1] + imm, LENGTH_HALF_WORD, (p->reg[rs2] & 0xffff), 1); /* store the halfword */
    }
    else if(funct3 == 0x2) /* sw */
    {
        /* Mem[R[rs1] + offset] <- R[rs2] */
        /*store(memory, p->reg[rs1] + imm, LENGTH_WORD, ((int)p->reg[rs2] & 0xffffffffu), 1);*/ /* store the word */
        /* f("sw x[rs2] = %d, x[rs1] = %u, imm = %d\n", p->reg[rs2], p->reg[rs1], imm); */
        store(memory, p->reg[rs1] + imm, LENGTH_WORD, p->reg[rs2], 1); /* store the word */
    }
    else /* unrecognised instructions */
    {
        raise_machine_code_error(inst);
    }

    p->PC += 4; /* move to the next instruction*/
    return; /* return */
}

void execute_jal(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
    unsigned int rd = inst.ujtype.rd; /* rd of the machine code */
    int imm = get_jump_offset(inst); /* imm of the machine code */
    
    p->reg[rd] = p->PC + 4; /* R[rd] <- pc + 4*/
    p->PC += imm; /* goto the pc + offset*/

    return; /* return */
}

void execute_jalr(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
    unsigned int rd = inst.itype.rd; /* rd of the machine code */
    unsigned int rs1 = inst.itype.rs1; /* rs1 of the machine code */
    int imm = get_imm_operand(inst); /* imm of the machine code */

    p->reg[rd] = p->PC + 4; /* R[rd] <- pc + 4*/
    p->PC = p->reg[rs1] + imm; /* goto the pc + offset*/

    if(rd == 0) /* rd is zero*/
    {
        p->reg[rd] = 0; /* x0 cannot be modified*/
    }

    return; /* return */
}

void execute_utype(Instruction inst, Processor *p)
{
    /* YOUR CODE HERE */
    unsigned int opcode = inst.utype.opcode; /* opcode of the machine code */
    unsigned int rd = inst.utype.rd; /* rd of the machine code */
    int imm = inst.utype.imm << 12; /* imm of the machine code */

    if(opcode == 0x17) /* auipc */
    {
        p->reg[rd] = p->PC + imm; /* R[rd] <- imm*/
    }
    else if(opcode == 0x37) /* lui */
    {
        p->reg[rd] = imm; /* R[rd] <- pc + imm*/
    }
    else /* unrecognised instructions */
    {
        raise_machine_code_error(inst);
    }

    if(rd == 0) /* rd is zero*/
    {
        p->reg[rd] = 0; /* x0 cannot be modified*/
    }

    p->PC += 4; /* move to the next instruction*/
    return; /* return */
}