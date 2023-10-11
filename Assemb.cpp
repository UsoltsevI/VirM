#include <stdio.h>
#include "Calc.h"
#include <string.h>

int assemb(const char *ASMfilename, const char *Bytecodefilename) {
    printf("The assembler has started performing its tasks\n");

    FILE *ASM = fopen(ASMfilename, "r");
    FILE *Bytecode = fopen(Bytecodefilename, "w");

    char input[MAX_COMMAND_SIZE] = {};
    char inhel[MAX_COMMAND_SIZE] = {};
    int check = fscanf(ASM, "%s", input);

    while ((check != EOF) && (check == 1)) {
        if (!strcmp(input, "HLT")) {
            fprintf(Bytecode, "%d", HLT);

        } else if (!strcmp(input, "push")) {
            int value = 0;
            check = fscanf(ASM, "%d", &value);

            if (check != 1) {
                check = fscanf(ASM, "%s", inhel);

                if (check != 1) {
                    printf("ERROR: check != 1 at push\n");

                } else {
                    if (!strcmp(inhel, "rax")) {
                        fprintf(Bytecode, "%d %d\n", Pushr, 1);

                    } else if (!strcmp(inhel, "rbx")) {
                        fprintf(Bytecode, "%d %d\n", Pushr, 2);

                    } else if (!strcmp(inhel, "rcx")) {
                        fprintf(Bytecode, "%d %d\n", Pushr, 3);

                    } else if (!strcmp(inhel, "rdx")) {
                        fprintf(Bytecode, "%d %d\n", Pushr, 4);

                    } else {
                        printf("Undefined command after push\n");
                    }
                }

            } else {
                fprintf(Bytecode, "%d %d\n", Push, value);
            }
        
        } else if (!strcmp(input, "pop")) {
            check = fscanf(ASM, "%s", inhel);

            if (check != 1) {
                printf("ERROR: check != 1 at pop\n");
            } else {
                if (!strcmp(inhel, "rax")) {
                    fprintf(Bytecode, "%d %d\n", Popr, 1);

                } else if (!strcmp(inhel, "rbx")) {
                    fprintf(Bytecode, "%d %d\n", Popr, 2);

                } else if (!strcmp(inhel, "rcx")) {
                    fprintf(Bytecode, "%d %d\n", Popr, 3);

                } else if (!strcmp(inhel, "rdx")) {
                    fprintf(Bytecode, "%d %d\n", Popr, 4);

                } else {
                    printf("Just a pop...\n");
                    fprintf(Bytecode, "%d\n", Pop);
                }
            }

        } else if (!strcmp(input, "div")) {
            fprintf(Bytecode, "%d\n", Div);

        } else if (!strcmp(input, "sub")) {
            fprintf(Bytecode, "%d\n", Sub);

        } else if (!strcmp(input, "out")) {
            fprintf(Bytecode, "%d\n", Out);

        } else if (!strcmp(input, "add")) {
            fprintf(Bytecode, "%d\n", Add);

        } else if (!strcmp(input, "mul")) {
            fprintf(Bytecode, "%d\n", Mul);

        } else if (!strcmp(input, "sqrt")) {
            fprintf(Bytecode, "%d\n", Sqrt);

        } else if (!strcmp(input, "sin")) {
            fprintf(Bytecode, "%d\n", Sin);

        } else if (!strcmp(input, "cos")) {
            fprintf(Bytecode, "%d\n", Cos);

        } else if (!strcmp(input, "in")) {
            fprintf(Bytecode, "%d\n", In);

        } else {
            fprintf(Bytecode, "%d", EOF);
            printf("Undefined command\n");
            printf("check = %d\n", check);
            printf("input = %s\n", input);
        }
 
        check = fscanf(ASM, "%s", input);
    }

    fclose(Bytecode);
    fclose(ASM);
    printf("The assembler has completed its tasks\n");

    return 0;
}
