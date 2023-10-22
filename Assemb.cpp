#include <stdio.h>
#include "Calc.h"
#include <string.h>
#include "Log.h"
#define DEBUGONA

struct JMP {
    char name[MAX_COMMAND_SIZE];
    int addres;
};

static const size_t maxnummark = 256; 
static JMP arrmark[maxnummark] = {};
static size_t nummark = 0;
static int numundefmarks = 0;

static int search_mark(const char *inhel);
static int add_mark(const char* input, const size_t curnum);
static int jmp_func(int buf[], size_t *curnum, FILE* ASM);
static void dump_buf(const int *buf, const size_t numcommand);
static void create_arr();
static void put_pop_or_push(const char *input, int buf[], size_t *curnum, FILE* ASM);
static void write_bytecode(const int *buf, const char *Bytecodefilename, const size_t numcommand);

int assembly(const char *ASMfilename, const char *Bytecodefilename, const char *Logfilename) {
    printf("The assembler has started performing its tasks\n");

    if (numundefmarks == 0) {
        create_arr();
        LOG_O;
    }

    numundefmarks = 0;

    FILE *ASM = fopen(ASMfilename, "r");

    char input[MAX_COMMAND_SIZE] = {};
    char inhel[MAX_COMMAND_SIZE] = {}; // naming, onegin lib
    int version = -1, numcommand = -1;
    int check = fscanf(ASM, "%s", input);
    check = fscanf(ASM, "%d", &version);
    
    if (strcmp(input, SIGNATURE)) {
        printf("Incorrect ASM signature\n");
        printf("right signature: %s\n", SIGNATURE);
        printf("Entered signature: %s\n", input);
        return -1;
    }

    if (version != VERSION) {
        printf("Incorrect ASM version\n");
        printf("Right version: %d\n", VERSION);
        printf("Entered version: %d\n", version);
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

    LOG_T;

    size_t curnum = 2;
    
    check = fscanf(ASM, "%s", input);

    while ((check != EOF) && (check == 1) && (curnum < numcommand + 2)) {
        LOG_1(input);

        if (!strcmp(input, "HLT")) {
            buf[curnum++] = HLT;
            LOG_2 (HLT);
            
        } else if (!strcmp(input, "push")) {
            put_pop_or_push(input, buf, &curnum, ASM);
        
        } else if (!strcmp(input, "pop")) {
            put_pop_or_push(input, buf, &curnum, ASM);
        }

        //code generation for several commands
#define DEF_CMD(name, number)           \
    else if (!strcmp(input, name)) {    \
        buf[curnum++] = number;         \
        LOG_2(number);                  \
    }
        #include "assembhelp.cpp"

#undef DEF_CMD 

       //code generation for jmp
#define DEF_JMP(name, number)               \
    else if (!strcmp(input, name)) {        \
        buf[curnum++] = number;             \
        jmp_func(buf, &curnum, ASM);        \
    }

        #include "jmphelp.cpp"

#undef DEF_JMP 

        else if (input[0] == ':') {
            //printf("\nmark\n");
            add_mark(input, curnum);
            LOG_M(input);
            //printf("\n");

        } else {
            buf[curnum++] = EOF;
            printf("Undefined command\n");
            printf("check = %d\n", check);
            printf("input = %s\n", input);
            printf("curnum = %lu\n", curnum);
        }
 
        check = fscanf(ASM, "%s", input);
    }

    fclose(ASM);

    write_bytecode(buf, Bytecodefilename, numcommand);

#ifdef DEBUGONA
    dump_buf(buf, numcommand);
#endif

    if (numundefmarks > 0) {
        printf("Next assemb\n");
        LOG_N;
        NUM_ASSEMBLY++;
        return assembly(ASMfilename, Bytecodefilename, Logfilename);
    }
    
    printf("The assembler has completed its tasks\n");

    LOG_C;

    return 0;
}

static void write_bytecode(const int *buf, const char *Bytecodefilename, const size_t numcommand) {
    FILE *Bytecode = fopen(Bytecodefilename, "wb");
    fwrite(buf, sizeof(int), numcommand + 2, Bytecode);
    fclose(Bytecode);
}

static int search_mark(const char *inhel) {
    if (inhel == NULL) {
        printf("inhel == NULL at search_mark_addres\n");
        return -1;
    }
    
    for (size_t i = 0; i < nummark; i++)    
        if (!strcmp(arrmark[i].name, inhel)) 
            return i;

    if (nummark >= maxnummark - 1) {
        printf("The end of the marks buffer\n");
        return -1;
    }

    return nummark++;
}

static int add_mark(const char* input, const size_t curnum) {
    int check = search_mark(input);

    if (check == -1) {
        printf("ERROR: search_mark_addres returned -1\n");
        return -1;
    }

    strcpy(arrmark[check].name, input);
    arrmark[check].addres = curnum;

    return 0;
}

static int jmp_func(int buf[], size_t *curnum, FILE *ASM) {
    char inhel[MAX_COMMAND_SIZE] = {};

    if (fscanf(ASM, "%s", inhel) != 1) {
        printf("Failed fscanf after Jmp");
        return -1;

    } else {
        fprintf(Log, "%s ", inhel);
        int check = search_mark(inhel);

        if (arrmark[check].addres == -1) 
            numundefmarks++;

        buf[*curnum] = arrmark[check].addres;
        *curnum += 1;

        LOG_J;
    }
    
    return 0;
}

static void dump_buf(const int *buf, const size_t numcommand) {
    printf("buf: { \n");

    for (size_t i = 0; i < numcommand + 2; i++)
        printf("%.3lu ", i);
    
    printf("\n");

    for (size_t i = 0; i < numcommand + 2; i++)
        printf("%3d ", buf[i]);

    printf("\n}\n");
}

static void create_arr() {
    for (size_t i = 0; i < maxnummark; i++)
        arrmark[i].addres = -1;
}

static void put_pop_or_push(const char *input, int buf[], size_t *curnum, FILE* ASM) {
    int Enum = 0;

    if (!strcmp(input, "pop")) {
        Enum = Pop;

    } else if (!strcmp(input, "push")) {
        Enum = Push;

    } else {
        printf("Undefined command in put_pop_or_push\n");
        printf("input = %s\n", input);
    }

    int value = 0;
    char inhel[MAX_COMMAND_SIZE] = {};

    if (fscanf(ASM, "%d", &value) != 1) {
        if (fscanf(ASM, "%s", inhel) != 1) {
            printf("ERROR: check != 1 at push\n");

        } else {
            if (!strcmp(input, "pop")) {
                Enum = Popr;

            } else if (!strcmp(input, "push")) {
                Enum = Pushr;
            } 

            fprintf(Log, "%s ", inhel);
            if (!strcmp(inhel, "rax")) {
                buf[(*curnum)++] = Enum, 
                buf[(*curnum)++] = 1;
                LOG_4X (Enum, 1);

            } else if (!strcmp(inhel, "rbx")) {
                buf[(*curnum)++] = Enum;
                buf[(*curnum)++] = 2;
                LOG_4X (Enum, 2);

            } else if (!strcmp(inhel, "rcx")) {
                buf[(*curnum)++] = Enum;
                buf[(*curnum)++] = 3;
                LOG_4X (Enum, 3);

            } else if (!strcmp(inhel, "rdx")) {
                buf[(*curnum)++] = Enum;
                buf[(*curnum)++] = 4;
                LOG_4X (Enum, 4);

            } else {
                printf("Undefined command after %s\n", input);
            }
        }
    } else if (Enum == Push) {
        buf[(*curnum)++] = Enum;
        buf[(*curnum)++] = value;
        fprintf(Log, "%d \t\t%d %d \t\t%lu %lu\n", value, Enum, value, *curnum - 2, *curnum - 1);

    } else if (Enum == Pop) {
        buf[(*curnum)++] = Pop;
        LOG_2X(Pop);
    }
}