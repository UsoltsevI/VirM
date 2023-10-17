#include <stdio.h>
#include <math.h>
#include "Calc.h"
#include "Stack.h"

int pocess(const char *Bytecodefilename, const char *Outputfilename) {
    FILE *Outputfile = fopen(Outputfilename, "w");

    struct SPU spu = {};
    SPUctor(&spu, "ERRSPU.txt", "ERRORS.txt", 1, 1);
    SPUreadbytecode(&spu, "Bytecode.txt");
    //SPUdump(&spu);

    if (spu.CS[spu.IP] != VERSION) {
        printf("Byrecode has format not for this verion of program\n");
        return -1;
    }

    spu.IP += 2;
    int value1 = 0, value2 = 0;
    int input  = spu.CS[spu.IP];

    while (spu.IP < spu.CS_capacity) {
        switch(input) {
            case HLT: 
                return 0;
                break;

            case Push: 
                spu.IP++;
                stack_push(&spu.stk, spu.CS[spu.IP]);
                break;
            
            case Pushr:
                spu.IP++;

                if (spu.CS[spu.IP] == 1) {
                    stack_push(&spu.stk, spu.rax);

                } else if (spu.CS[spu.IP] == 2) {
                    stack_push(&spu.stk, spu.rbx);

                } else if (spu.CS[spu.IP] == 3) {
                    stack_push(&spu.stk, spu.rcx);

                } else if (spu.CS[spu.IP] == 4) {
                    stack_push(&spu.stk, spu.rdx);

                } else {
                    printf("Undefined command after Pushr\n");
                    return -1;
                }

                break;

            case Pop:
                stack_pop(&spu.stk);
                break;
            
            case Popr:
                spu.IP++;

                if (spu.CS[spu.IP] == 1) {
                    spu.rax = stack_pop(&spu.stk);

                } else if (spu.CS[spu.IP] == 2) {
                    spu.rbx = stack_pop(&spu.stk);

                } else if (spu.CS[spu.IP] == 3) {
                    spu.rcx = stack_pop(&spu.stk);

                } else if (spu.CS[spu.IP] == 4) {
                    spu.rdx = stack_pop(&spu.stk);

                } else {
                    printf("Undefined command after Popr\n");
                    return -1;
                }

                break;

            case Div:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                stack_push(&spu.stk, value2 / value1);
                break;
            
            case Sub:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                stack_push(&spu.stk, value2 - value1);
                break;

            case Out:
                fprintf(Outputfile, "%d", stack_pop(&spu.stk));
                break;

            case Add:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                stack_push(&spu.stk, value1 + value2);
                break;
            
            case Mul:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                stack_push(&spu.stk, value1 * value2);
                break;
            
            case Sqrt:
                value1 = stack_pop(&spu.stk);
                stack_push(&spu.stk, (int) sqrt(value1));
                break;
            
            case Sin:
                value1 = stack_pop(&spu.stk);
                stack_push(&spu.stk, (int) 100 * sin(value1));
                break;
            
            case Cos:
                value1 = stack_pop(&spu.stk);
                stack_push(&spu.stk, (int) 100 * cos(value1));
                break;

            case In:
                printf("Enter a value:\n");
                
                if (scanf("%d", &value1) == 1) {
                    stack_push(&spu.stk, value1);

                } else {
                    printf("Failed scanf at In\n");
                    return -1;
                }

                break;

            default:
                printf("Undefined command\n");
                printf("input = %d\n", input);
                break;
        }
        spu.IP++;
        input = spu.CS[spu.IP];
    }

    fclose(Outputfile);

    SPUdtor(&spu);

    printf("Processor has complited his tasks)\n");

    return 0;
}
