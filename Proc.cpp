#include <stdio.h>
#include <math.h>
#include "Calc.h"
#include "Stack.h"

//#define DEBUGONP

int process(const char *Bytecodefilename, const char *Outputfilename) {
    FILE *Outputfile = fopen(Outputfilename, "w");

    struct SPU spu = {};
    SPUctor(&spu, "ERRSPU.txt", "ERRORS.txt", 1, 1);
    SPUreadbytecodebin(&spu, Bytecodefilename);
    //SPUdump(&spu);

    if (spu.CS[spu.IP] != VERSION) {
        printf("Bytecode has format not for this verion of program\n");
        printf("VERSION = %d\n", VERSION);
        printf("spu.CS[spu.IP] = %d\n", spu.CS[spu.IP]);
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

                if ((0 < spu.CS[spu.IP]) && (spu.CS[spu.IP] <= num_reg)) {
                    stack_push(&spu.stk, spu.reg[spu.CS[spu.IP]]);

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

                if ((0 < spu.CS[spu.IP]) && (spu.CS[spu.IP] <= num_reg)) {
                    spu.reg[spu.CS[spu.IP]] = stack_pop(&spu.stk);

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
                fprintf(Outputfile, "%d ", stack_pop(&spu.stk));
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
            
            case Jmp:
                spu.IP++;

                if (spu.CS[spu.IP] < 1) {
                    printf("ERROR: addres after Jmp < 1\n");
                    return -1;
                }
                //if (spu.CS[spu.IP] - 1 < spu.IP)
                //    printf("Perhaps it is an infinite loop...\n");

                spu.IP = spu.CS[spu.IP] - 1;
                break;

            case Ja:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                spu.IP++;

                if (value2 > value1) {
                    if (spu.CS[spu.IP] < 1) {
                        printf("ERROR: addres after Ja < 1\n");
                        return -1;
                    }

                    spu.IP = spu.CS[spu.IP] - 1;
                }

                break;

            case Jae:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                spu.IP++;

                if (value2 >= value1) {
                    if (spu.CS[spu.IP] < 1) {
                        printf("ERROR: addres after Jae < 1\n");
                        return -1;
                    }

                    spu.IP = spu.CS[spu.IP] - 1;
                }

                break;

            case Jb:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                spu.IP++;

                if (value2 < value1) {
                    if (spu.CS[spu.IP] < 1) {
                        printf("ERROR: addres after Jb < 1\n");
                        return -1;
                    }

                    spu.IP = spu.CS[spu.IP] - 1;
                }

                break;
            
            case Jbe:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                spu.IP++;

                if (value2 <= value1) {
                    if (spu.CS[spu.IP] < 1) {
                        printf("ERROR: addres after Jbe < 1\n");
                        return -1;
                    }

                    spu.IP = spu.CS[spu.IP] - 1;
                }

                break;
            
            case Je:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                spu.IP++;

                if (value2 == value1) {
                    if (spu.CS[spu.IP] < 1) {
                        printf("ERROR: addres after Je < 1\n");
                        return -1;
                    }
                
                    spu.IP = spu.CS[spu.IP] - 1;
                }

                break;
            
            case Jne:
                value1 = stack_pop(&spu.stk);
                value2 = stack_pop(&spu.stk);
                spu.IP++;

                if (value2 != value1) {
                    if (spu.CS[spu.IP] < 1) {
                        printf("ERROR: addres after Jne < 1\n");
                        return -1;
                    }

                    spu.IP = spu.CS[spu.IP] - 1;
                }

                break;

            default:
                printf("Undefined command\n");
                printf("input = %d\n", input);
                break;
        }
        spu.IP++;

#ifdef DEBUGONP
        SPUdump(&spu);
        printf("Press any key ro continue\n");
        int zero = 0;
        scanf("%d", &zero);
#endif

        input = spu.CS[spu.IP];
    }

    fclose(Outputfile);

    SPUdtor(&spu);

    printf("Processor has complited his tasks)\n");

    return 0;
}
