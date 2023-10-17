#include <stdio.h>
#include "Calc.h"
#include <string.h>

int assemb(const char *ASMfilename, const char *Bytecodefilename) {
    printf("The assembler has started performing its tasks\n");

    FILE *ASM = fopen(ASMfilename, "r");
    FILE *Bytecode = fopen(Bytecodefilename, "wb");

    char input[MAX_COMMAND_SIZE] = {};
    char inhel[MAX_COMMAND_SIZE] = {};
    int version = -1, numcommand = -1;
    int check = fscanf(ASM, "%d", &version);
    
    if (version != VERSION) {
        printf("Incorrect ASM version\n");
        return -1;
    }

    check = fscanf(ASM, "%d", &numcommand);
    
    int *buf = (int *) calloc(numcommand + 2, sizeof(int));

    if (buf == NULL) {
        printf("Failed callocation for buf at assemb\n");
        return -1;
    }

    buf[0] = VERSION;
    buf[1] = numcommand;

    size_t curnum = 2;
    
    check = fscanf(ASM, "%s", input);

    while ((check != EOF) && (check == 1) && (curnum < numcommand)) {
        if (!strcmp(input, "HLT")) {
            buf[curnum++] = HLT;

        } else if (!strcmp(input, "push")) {
            int value = 0;
            check = fscanf(ASM, "%d", &value);

            if (check != 1) {
                check = fscanf(ASM, "%s", inhel);

                if (check != 1) {
                    printf("ERROR: check != 1 at push\n");

                } else {
                    if (!strcmp(inhel, "rax")) {
                        buf[curnum++] = Pushr, 
                        buf[curnum++] = 1;

                    } else if (!strcmp(inhel, "rbx")) {
                        buf[curnum++] = Pushr;
                        buf[curnum++] = 2;

                    } else if (!strcmp(inhel, "rcx")) {
                        buf[curnum++] = Pushr;
                        buf[curnum++] = 3;

                    } else if (!strcmp(inhel, "rdx")) {
                        buf[curnum++] = Pushr;
                        buf[curnum++] = 4;

                    } else {
                        printf("Undefined command after push\n");
                    }
                }

            } else {
                buf[curnum++] = Push;
                buf[curnum++] = value;
            }
        
        } else if (!strcmp(input, "pop")) {
            check = fscanf(ASM, "%s", inhel);

            if (check != 1) {
                printf("ERROR: check != 1 at pop\n");
            } else {
                if (!strcmp(inhel, "rax")) {
                    buf[curnum++] = Popr;
                    buf[curnum++] = 1;

                } else if (!strcmp(inhel, "rbx")) {
                    buf[curnum++] = Popr;
                    buf[curnum++] = 2;

                } else if (!strcmp(inhel, "rcx")) {
                    buf[curnum++] = Popr;
                    buf[curnum++] = 3;

                } else if (!strcmp(inhel, "rdx")) {
                    buf[curnum++] = Popr;
                    buf[curnum++] = 4;

                } else {
                    //printf("Just a pop...\n");
                    buf[curnum++] = Pop;
                }
            }

        } else if (!strcmp(input, "div")) {
            buf[curnum++] = Div;

        } else if (!strcmp(input, "sub")) {
            buf[curnum++] = Sub;

        } else if (!strcmp(input, "out")) {
            buf[curnum++] = Out;;

        } else if (!strcmp(input, "add")) {
            buf[curnum++] = Add;

        } else if (!strcmp(input, "mul")) {
            buf[curnum++] = Mul;

        } else if (!strcmp(input, "sqrt")) {
            buf[curnum++] = Sqrt;

        } else if (!strcmp(input, "sin")) {
            buf[curnum++] = Sin;

        } else if (!strcmp(input, "cos")) {
            buf[curnum++] = Cos;

        } else if (!strcmp(input, "in")) {
            buf[curnum++] = In;

        } else {
            buf[curnum++] = EOF;
            printf("Undefined command\n");
            printf("check = %d\n", check);
            printf("input = %s\n", input);
        }
 
        check = fscanf(ASM, "%s", input);
    }

    fwrite(buf, sizeof(int), numcommand, Bytecode);
    fclose(Bytecode);
    fclose(ASM);
    printf("The assembler has completed its tasks\n");

    return 0;
}
