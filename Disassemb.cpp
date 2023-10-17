#include <stdio.h>
#include "Calc.h"

static int readbytecode(int *buf, const char *Bytecodefilename);

int disassemb(const char *ASMfilename, const char *Bytecodefilename) {
    printf("The disassembler has started performing its tasks\n");
    
    FILE *ASM = fopen(ASMfilename, "w");

    int *buf = {};

    if (readbytecode(buf, Bytecodefilename)) {
        printf("ERROR at readbytecode (from disassemb)\n");
        return -1;
    }
    printf("?\n");
    printf("bug[0] = %d\n", buf[0]);
    if (*buf != VERSION) {
        printf("Incorrect bytecode version\n");
        return -1;
    }

    printf("?\n");
    int numcommand = buf[1] - 2;
    printf("buf[1] = %d\n", buf[1]);
    int input = 0;
    int value = 0;
    size_t curnum = 2;
    input = buf[curnum++];

    while (curnum < numcommand) {
        printf("input = %d\n", input);
        switch (input) {
            case HLT:
                fprintf(ASM, "%s\n", "HLT");
                break;

            case Push:
                value = buf[curnum++];

                //printf("Just a push...\n");
                fprintf(ASM, "%s %d\n", "push", value);
                break;

            case Pushr:
                value = buf[curnum++];

                if (value == 1){
                    fprintf(ASM, "%s %s\n", "push", "rax");

                } else if (value == 2) {
                    fprintf(ASM, "%s %s\n", "push", "rbx");

                } else if (value == 3) {
                    fprintf(ASM, "%s %s\n", "push", "rcx");
                    
                } else if (value == 4) {
                    fprintf(ASM, "%s %s\n", "push", "rdx");
                    
                } else {
                    printf("Undefined command at Pushr\n");
                    return -1;
                }

                break;

            case Pop:
                fprintf(ASM, "%s\n", "pop");
                break;

            case Popr:
                value = buf[curnum++];

                if (value == 1){
                    fprintf(ASM, "%s %s\n", "pop", "rax");

                } else if (value == 2) {
                    fprintf(ASM, "%s %s\n", "pop", "rbx");

                } else if (value == 3) {
                    fprintf(ASM, "%s %s\n", "pop", "rcx");
                    
                } else if (value == 4) {
                    fprintf(ASM, "%s %s\n", "pop", "rdx");
                    
                } else {
                    printf("Undefined command at Popr\n");
                    return -1;
                }

                break;

            case Div:
                fprintf(ASM, "%s\n", "div");
                break;
            
            case Sub:
                fprintf(ASM, "%s\n", "sub");
                break;
            
            case Out:
                fprintf(ASM, "%s\n", "out");
                break;

            case Add:
                fprintf(ASM, "%s\n", "add");
                break;

            case Mul:
                fprintf(ASM, "%s\n", "mul");
                break;

            case Sqrt:
                fprintf(ASM, "%s\n", "sqrt");
                break;

            case Sin:
                fprintf(ASM, "%s\n", "sin");
                break;

            case Cos:
                fprintf(ASM, "%s\n", "cos");
                break;
            
            case In:
                fprintf(ASM, "%s\n", "in");
                break;
            
            default:
                printf("Undefined command\n");
                printf("input = %d\n", input);
        }

        input = buf[curnum++];
    }

    fclose(ASM);

    printf("The disassembler has completed its tasks\n");

    return 0;
}

static int readbytecode(int *buf, const char *Bytecodefilename) {
    FILE *Bytecode = fopen(Bytecodefilename, "rb");

    if (Bytecode == NULL) {
        printf("foupen bytecode file failed at disassemb\n");
        return -1;
    }

    fseek(Bytecode, EOF, SEEK_END);

    size_t bufcapacity = ftell(Bytecode);
    
    bufcapacity = bufcapacity / 4;

    buf = (int *) calloc(bufcapacity + 1, sizeof(int));

    if (buf == NULL) {
        printf("Failed callocation for buf at disassemb\n");
        return -1;
    }

    fseek(Bytecode, 0, SEEK_SET);

    fread(buf, sizeof(int), bufcapacity, Bytecode);

    buf[bufcapacity] = 0;
    buf[0] = VERSION;
    buf[1] = bufcapacity - 2;

    fclose(Bytecode);

    return 0;
}
