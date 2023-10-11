#include <stdio.h>
#include "Calc.h"

int disassemb(const char *ASMfilename, const char *Bytecodefilename) {
    printf("The disassembler has started performing its tasks\n");
    
    FILE *ASM = fopen(ASMfilename, "w");
    FILE *Bytecode = fopen(Bytecodefilename, "r");

    int input = 0;
    int value = 0;
    int check = fscanf(Bytecode, "%d", &input);

    while ((check != EOF) && (check == 1)) {
        switch (input) {
            case HLT:
                fprintf(ASM, "%s\n", "HLT");
                break;

            case Push:
                check = fscanf(Bytecode, "%d", &value);

                if (check != 1) {
                    printf("ERROR: check != 1 at push\n");
                    return -1;
                }
                printf("Just a push...\n");
                fprintf(ASM, "%s %d\n", "push", value);
                break;

            case Pushr:
                check = fscanf(Bytecode, "%d", &value);

                if (check != 1) {
                    printf("ERROR: check != 1 at Pushr\n");
                    return -1;

                } else if (value == 1){
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
                check = fscanf(Bytecode, "%d", &value);

                if (check != 1) {
                    printf("ERROR: check != 1 at Popr\n");
                    return -1;

                } else if (value == 1){
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
                printf("check = %d\n", check);
                printf("input = %d\n", input);
        }

        check = fscanf(Bytecode, "%d", &input);
    }

    fclose(Bytecode);
    fclose(ASM);

    printf("The disassembler has completed its tasks\n");

    return 0;
}
