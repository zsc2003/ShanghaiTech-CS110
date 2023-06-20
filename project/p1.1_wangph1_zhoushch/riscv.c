/* DO NOT CHANGE THIS FILE! */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "riscv.h"
#include "part2/mem.h"

static void print_usage_and_exit()
{
    printf("Usage:\n");
    printf("    riscv <input_file> [-d <data_file>] [-s <symbol_table>] [-o <original_file>] [-b <basic_file>] [-t <text_file>] [-l <log_file>]\n");
    printf("    riscv assembler     <input_file> [-d <data_file>] [-s <symbol_table>] [-o <original_file>] [-b <basic_file>] [-t <text_file>] [-l <log_file>]\n");
    printf("    riscv assembler-p1  <input_file> [-d <data_file>] [-s <symbol_table>] [-o <original_file>] [-l <log_file>]\n");
    printf("    riscv assembler-p2  <symbol_table> <original_file> [-b <basic_file>] [-t <text_file>] [-l <log_file>]\n");
    printf("    riscv emulator      <text_file> <data_file> [-l <log_file>]\n");
    printf("Description:\n");
    printf("    input_file: Input of RISC-V assembly code.\n");
    printf("    data_file: Target of saving data.\n");
    printf("    symbol_table: Target of saving symbol table.\n");
    printf("    original_file: Target of saving original code. Note that original code is like in Venus, comments are strpped and data segment is ommitted.\n");
    printf("    basic_file: Target of saving basic code.  Note that basic code is like in Venus.  Pseudo-instructions are translated.\n");
    printf("    text_file: Target of saving finalized text segment, this file should be in hexadecimal.\n");
    exit(0);
}

int main(int argc,char** argv) {
    int mode = 4;
    FILE *input=NULL, *data=NULL, *symtbl=NULL, *orign=NULL, *basic=NULL, *text=NULL;
    char *log=NULL;
    char *optstring=NULL;
    int optoffset=0;
    int ch;
    char buf[100];
    char *end_ptr;
    Address addr;
    int err = 0;
    int i,j,k;

    Emulator *emulator;

    if(argc < 2)
        print_usage_and_exit();
    
    if (strcmp(argv[1], "assembler") == 0){
        if(argc < 3)
            print_usage_and_exit();
        mode = 0;
        input=fopen(argv[2], "r+");
        optoffset=2;
        optstring="d:s:o:b:t:l:";
    } else if (strcmp(argv[1], "assembler-p1") == 0){
        if(argc < 3)
            print_usage_and_exit();
        mode = 1;
        input=fopen(argv[2], "r+");
        optoffset=2;
        optstring="d:s:o:l:";
    } else if (strcmp(argv[1], "assembler-p2") == 0){
        if(argc < 4)
            print_usage_and_exit();
        mode = 2;
        symtbl=fopen(argv[2], "r+");
        orign=fopen(argv[3], "r+");
        optoffset=3;
        optstring="b:t:l:";
    } else if (strcmp(argv[1], "emulator") == 0){
        if(argc < 4)
            print_usage_and_exit();
        mode = 3;
        text=fopen(argv[2], "r+");
        data=fopen(argv[3], "r+");
        optoffset=3;
        optstring="l:";
    } else {
        mode = 4;
        input=fopen(argv[1], "r+");
        optoffset=1;
        optstring="d:s:o:b:t:l:";
    }

    while((ch=getopt(argc-optoffset, argv+optoffset, optstring)) != -1) {
        switch (ch){
            case 'd':
                data = fopen(optarg, "w+");
                break;
            case 's':
                symtbl = fopen(optarg, "w+");
                break;
            case 'o':
                orign = fopen(optarg, "w+");
                break;
            case 'b':
                basic = fopen(optarg, "w+");
                break;
            case 't':
                text = fopen(optarg, "w+");
                break;
            case 'l':
                log = optarg;
                break;
            case '?':
                print_usage_and_exit();
            default:
                break;
        }
    }

    if(input==NULL)
        input=tmpfile();
        
    if(data==NULL)
        data=tmpfile();
    
    if(symtbl==NULL)
        symtbl=tmpfile();
    
    if(orign==NULL)
        orign=tmpfile();
    
    if(basic==NULL)
        basic=tmpfile();
    
    if(text==NULL)
        text=tmpfile();
    
    set_log_file(log);

    /* Skips assmbler when only emulator is invoked. */
    if (mode != 3){
        err = assemble(mode, input, data, symtbl, orign, basic, text);
    }

    if (mode == 4) {
        rewind(text);
        rewind(basic);
        rewind(orign);
        addr=0;
        write_to_log("PC\t\t%-10s\t\t%-20s\t\t%-20s\n","Machine Code","Basic Code","Original Code");
        while (fgets(buf, sizeof(buf), text) != NULL){
            write_to_log("0x%x\t\t",addr);
            if(buf[strlen(buf)-1]=='\n')
                buf[strlen(buf)-1]='\0';
            write_to_log("0x%-10s\t\t",buf);
            if (fgets(buf, sizeof(buf), basic) != NULL){
                if(buf[strlen(buf)-1]=='\n')
                    buf[strlen(buf)-1]='\0';
                write_to_log("%-20s\t\t",buf);
            }
            else
                write_to_log("%-20s\t\t","");

            if (fgets(buf, sizeof(buf), orign) != NULL){
                if(buf[strlen(buf)-1]=='\n')
                    buf[strlen(buf)-1]='\0';
                write_to_log("%-20s",buf);
            }
            else
                write_to_log("%-20s","");
            write_to_log("\n");
            addr+=4;
        }
    }

        
    if (mode >= 3) { 
        rewind(text);
        rewind(data);
        emulator = create_emulator(text, data);
        for (;;)
        {
            scanf("%s", buf);
            if (strcmp(buf, "run") == 0 || strcmp(buf, "r") == 0)
            {
                if(run(emulator,0)==-1)
                    write_to_log("Program has been executed, please try prev or reset\n");
            }
            else if (strcmp(buf, "step") == 0 || strcmp(buf, "s") == 0)
            {
                if(step(emulator,0)==-1)
                    write_to_log("Program has been executed, please try prev or reset\n");
            }
            else if (strcmp(buf, "prev") == 0 || strcmp(buf, "p") == 0)
            {
                if(prev(emulator)==-1)
                    write_to_log("Program has not yet started, please try run or step\n");
            }
            else if (strcmp(buf, "trace") == 0 || strcmp(buf, "t") == 0)
            {
                if(trace(emulator)==-1)
                    write_to_log("Program has been executed, try prev or reset\n");
            }
            else if (strcmp(buf, "dump") == 0 || strcmp(buf, "d") == 0)
            {
                dump(emulator);
            }
            else if (strcmp(buf, "reset") == 0)
            {
                if(prev(emulator)==-1)
                    write_to_log("Program has not yet started, please try run or step\n");
                free_emulator(emulator);
                rewind(text);
                rewind(data);
                emulator = create_emulator(text, data);
            }
            else if (strcmp(buf, "exit") == 0)
            {
                break;
            }
            else if (strcmp(buf, "print") == 0)
            {
                scanf("%s", buf);
                if (strcmp(buf, "pc") == 0) {
                    write_to_log("0x%x\n", emulator->cpu.PC);
                    continue;
                } else if (strcmp(buf, "reg") == 0) {
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
                    continue;
                }
                addr=strtol(buf, &end_ptr, 0);
                if (*end_ptr != '\0')
                {
                    write_to_log("usage: print <Address>\n");
                    continue;
                }
                write_to_log("%08x\n", load(emulator->memory,addr, LENGTH_WORD, 0));
            }
            else
            {
                write_to_log("try : run, step, prev, dump, trace, reset, exit, print <args>.\n");
            }
        }

        free_emulator(emulator);
    }

    fclose(input);
    fclose(data);
    fclose(symtbl);
    fclose(orign);
    fclose(basic);
    fclose(text);

    return err;
    
}