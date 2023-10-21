#include <stdio.h>
#include "Calc.h"
#include <string.h>

//#define DEBUGONA
struct JMP {
    const char* name;
    int addres;
};

static const size_t maxnummark = 256; 
static JMP arrmark[maxnummark] = {};
static size_t nummark = 0;
static int numundefmarks = 0;

static int search_mark_addres(const char *inhel);
static int add_mark(const char* input, const size_t curnum);
static int jmp_func(int buf[], size_t *curnum, FILE* ASM, FILE *Log);
static void dump_buf(const int *buf, const size_t numcommand);
static void create_arr();

int assemb(const char *ASMfilename, const char *Bytecodefilename, const char *Logfilename) {
    printf("The assembler has started performing its tasks\n");

    if (numundefmarks == 0)
        create_arr();

    numundefmarks = 0;

    FILE *ASM = fopen(ASMfilename, "r");
    FILE *Bytecode = fopen(Bytecodefilename, "wb");//еереместить куда надо
    FILE *Log = fopen(Logfilename, "w");//библиотека логирования

    char input[MAX_COMMAND_SIZE] = {};
    char inhel[MAX_COMMAND_SIZE] = {}; // naming, onegin lib
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

    fprintf(Log, "VERSION: %d\n", VERSION); // LOG("VERSIN %d", version)
    fprintf(Log, "numcommand = %d\n", numcommand);

    size_t curnum = 2;
    
    check = fscanf(ASM, "%s", input);

    fprintf(Log, "\t Name          Code       Addres\n");
    fprintf(Log, "\tin ASM        for VM     in Bytecode buf\n");


    while ((check != EOF) && (check == 1) && (curnum < numcommand + 2)) {
        fprintf(Log, "\t%5s ", input);
        if (!strcmp(input, "HLT")) {
            //printf("HLT curnum = %d\n", curnum);
            buf[curnum++] = HLT;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", HLT, curnum - 1);
            

        } else if (!strcmp(input, "push")) {
            int value = 0;

            if (fscanf(ASM, "%d", &value) != 1) {
                if (fscanf(ASM, "%s", inhel) != 1) {
                    printf("ERROR: check != 1 at push\n");

                } else {
                    fprintf(Log, "%s ", inhel);
                    if (!strcmp(inhel, "rax")) {
                        buf[curnum++] = Pushr, 
                        buf[curnum++] = 1;
                        fprintf(Log, "\t\t%d %d \t\t%lu %lu\n", Pushr, 1, curnum - 2, curnum - 1);

                    } else if (!strcmp(inhel, "rbx")) {
                        buf[curnum++] = Pushr;
                        buf[curnum++] = 2;
                        fprintf(Log, "\t\t%d %d \t\t%lu %lu\n", Pushr, 2, curnum - 2, curnum - 1);

                    } else if (!strcmp(inhel, "rcx")) {
                        buf[curnum++] = Pushr;
                        buf[curnum++] = 3;
                        fprintf(Log, "\t\t%d %d \t\t%lu %lu\n", Pushr, 3, curnum - 2, curnum - 1);

                    } else if (!strcmp(inhel, "rdx")) {
                        buf[curnum++] = Pushr;
                        buf[curnum++] = 4;
                        fprintf(Log, "\t\t%d %d \t\t%lu %lu\n", Pushr, 4, curnum - 2, curnum - 1);

                    } else {
                        printf("Undefined command after push\n");
                    }
                }

            } else {
                buf[curnum++] = Push;
                buf[curnum++] = value;
                fprintf(Log, "%d \t\t%d %d \t\t%lu %lu\n", value, Push, value, curnum - 2, curnum - 1);
            }
        
        } else if (!strcmp(input, "pop")) {
            if (fscanf(ASM, "%s", inhel) != 1) {
                printf("ERROR: check != 1 at pop\n");

            } else {
                fprintf(Log, "%s ", inhel);
                if (!strcmp(inhel, "rax")) {
                    buf[curnum++] = Popr;
                    buf[curnum++] = 1;
                    fprintf(Log, "\t\t%d %d \t\t%lu %lu\n", Popr, 1, curnum - 2, curnum - 1);

                } else if (!strcmp(inhel, "rbx")) {
                    buf[curnum++] = Popr;
                    buf[curnum++] = 2;
                    fprintf(Log, "\t\t%d %d \t\t%lu %lu\n", Popr, 2, curnum - 2, curnum - 1);

                } else if (!strcmp(inhel, "rcx")) {
                    buf[curnum++] = Popr;
                    buf[curnum++] = 3;
                    fprintf(Log, "\t\t%d %d \t\t%lu %lu\n", Popr, 3, curnum - 2, curnum - 1);

                } else if (!strcmp(inhel, "rdx")) {
                    buf[curnum++] = Popr;
                    buf[curnum++] = 4;
                    fprintf(Log, "\t\t%d %d \t\t%lu %lu\n", Popr, 4, curnum - 2, curnum - 1);

                } else {
                    //printf("Just a pop...\n");
                    buf[curnum++] = Pop;
                    fprintf(Log, "\t\t%d \t\t\t%lu\n", Pop, curnum - 1);
                }
            }

        } else if (!strcmp(input, "div")) {
            buf[curnum++] = Div;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", Div, curnum - 1);

        } else if (!strcmp(input, "sub")) {
            buf[curnum++] = Sub;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", Sub, curnum - 1);

        } else if (!strcmp(input, "out")) {
            buf[curnum++] = Out;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", Out, curnum - 1);

        } else if (!strcmp(input, "add")) {
            buf[curnum++] = Add;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", Add, curnum - 1);

        } else if (!strcmp(input, "mul")) {
            buf[curnum++] = Mul;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", Mul, curnum - 1);

        } else if (!strcmp(input, "sqrt")) {
            buf[curnum++] = Sqrt;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", Sqrt, curnum - 1);

        } else if (!strcmp(input, "sin")) {
            buf[curnum++] = Sin;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", Sin, curnum - 1);

        } else if (!strcmp(input, "cos")) {
            buf[curnum++] = Cos;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", Cos, curnum - 1);

        } else if (!strcmp(input, "in")) {
            buf[curnum++] = In;
            fprintf(Log, "     \t\t%d \t\t\t%lu\n", In, curnum - 1);

        } else if (!strcmp(input, "jmp")) {
            buf[curnum++] = Jmp;
            jmp_func(buf, &curnum, ASM, Log);
            
        } else if (!strcmp(input, "ja")) {
            buf[curnum++] = Ja;
            jmp_func(buf, &curnum, ASM, Log);
            
        } else if (!strcmp(input, "jae")) {
            buf[curnum++] = Jae;
            jmp_func(buf, &curnum, ASM, Log);
            
        } else if (!strcmp(input, "jb")) {
            buf[curnum++] = Jb;
            //printf("curnum b = %lu\n", curnum);
            jmp_func(buf, &curnum, ASM, Log);
            //printf("curnum a = %lu\n", curnum);
            
        } else if (!strcmp(input, "jbe")) {
            buf[curnum++] = Jbe;
            jmp_func(buf, &curnum, ASM, Log);
            
        } else if (!strcmp(input, "je")) {
            buf[curnum++] = Je;
            jmp_func(buf, &curnum, ASM, Log);
            
        } else if (!strcmp(input, "jne")) {
            buf[curnum++] = Jne;
            jmp_func(buf, &curnum, ASM, Log);
            
        } else if (input[0] == ':') {
            add_mark(input, curnum);
            fprintf(Log, "     \t\t - \t\t\t - \n");
            //curnum++;

        } else {
            buf[curnum++] = EOF;
            printf("Undefined command\n");
            printf("check = %d\n", check);
            printf("input = %s\n", input);
            printf("curnum = %lu\n", curnum);
        }
 
        check = fscanf(ASM, "%s", input);
    }

    //printf("%lu\n", fwrite(buf, sizeof(int), numcommand + 2, Bytecode));
    fwrite(buf, sizeof(int), numcommand + 2, Bytecode);
    fclose(Bytecode);
    fclose(ASM);

#ifdef DEBUGONA
    dump_buf(buf, numcommand);
#endif

    if (numundefmarks > 0) {
        printf("Next assemb\n");
        return assemb(ASMfilename, Bytecodefilename, Logfilename);
    }

    printf("The assembler has completed its tasks\n");

    return 0;
}

static int search_mark_addres(const char *inhel) {
    //printf("inhel = %s\n", inhel);
    for (size_t i = 0; i < nummark; i++) 
        if (strcmp(arrmark[i].name, inhel)) 
            return arrmark[i].addres;

    return -1;
}

static int add_mark(const char* input, const size_t curnum) {
    int check = search_mark_addres(input);

    if (check != -1) {
        arrmark[check].addres = (int) curnum;

    } else {
        arrmark[nummark].name   = input;
        arrmark[nummark].addres = curnum;
        nummark++;
        numundefmarks++;

        if (nummark >= maxnummark) {
            printf("The end of the marks buffer\n");
            return -1;
        }
    }

    return 0;
}

static int jmp_func(int buf[], size_t *curnum, FILE *ASM, FILE *Log) {
    char inhel[MAX_COMMAND_SIZE] = {};

    if (fscanf(ASM, "%s", inhel) != 1) {
        printf("Failed fscanf after Jmp");
        return -1;

    } else {
        fprintf(Log, "%s ", inhel);
        int addres = search_mark_addres(inhel);
        //printf("addres = %d\n", addres);

        if (addres == -1) {
            add_mark(inhel, addres);
            numundefmarks++;
            //printf("numundefmarks++\n");

        } else {
            //printf("buf[*curnum] = %d\n", buf[*curnum]);
            //printf("buf[*curnum - 1] = %d\n", buf[*curnum - 1]);
            buf[*curnum] = addres;
            *curnum += 1;
        }
        fprintf(Log, "\t%d %d \t\t%lu %lu\n", buf[*curnum - 2], buf[*curnum - 1], *curnum - 2, *curnum - 1);
    }

    return 0;
}

static void dump_buf(const int *buf, const size_t numcommand) {
    printf("buf: { \n");

    for (size_t i = 0; i < numcommand + 2; i++) {
        printf("%.3lu ", i);
    }
    printf("\n");
    for (size_t i = 0; i < numcommand + 2; i++){
        printf("%3d ", buf[i]);
    }
    printf("\n}\n");
}

static void create_arr() {
    for (size_t i = 0; i < maxnummark; i++)
        arrmark[i].addres = -1;
}
