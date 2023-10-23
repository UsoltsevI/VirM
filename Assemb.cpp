#include <stdio.h>
#include "Calc.h"
#include <string.h>
#include "Log.h"

//#define DEBUGONA

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
static int jmp_func(struct string *asm_data, int buf[], size_t *cur_str, size_t *curnum);
static void dump_buf(const int *buf, const size_t numcommand);
static void create_arr();
static void put_pop_or_push(struct string *asm_data, int buf[], size_t *cur_str, size_t *curnum);
static void write_bytecode(const int *buf, const char *Bytecodefilename, const size_t numcommand);

int assembly(const char *ASMfilename, const char *Bytecodefilename, const char *Logfilename) {
    printf("The assembler has started performing its tasks\n");

    static struct string *asm_data = {};
    static char * asm_buf = {};
    static size_t num_str = 0;

    if (numundefmarks == 0) {
        create_arr();
        read_strings(&asm_data, &num_str, &asm_buf, ASMfilename);
        change_str_ending_buf(asm_buf); 
        LOG_O;
    }

    size_t cur_str = 0;

    if (str_cmp(asm_data[cur_str++], SIGNATURE)) {
        printf("Incorrect ASM signature\n");
        printf("right signature: %s\n", SIGNATURE);
        printf("Entered signature: %s\n", asm_data[cur_str - 1].str);
        return -1;
    }

    int version    = 0;
    int numcommand = 0;
    convert_str_to_int(asm_data[cur_str++], &version);
    convert_str_to_int(asm_data[cur_str++], &numcommand);

    if (version != VERSION) {
        printf("Incorrect ASM version\n");
        printf("Right version: %d\n", VERSION);
        printf("Entered version: %d\n", version);
        return -1;
    }
    
    int *buf = (int *) calloc(numcommand + 2, sizeof(int));

    if (buf == NULL) {
        printf("Failed callocation for buf at assemb\n");
        return -1;
    }

    size_t curnum = 0;
    buf[curnum++] = VERSION;
    buf[curnum++] = numcommand;

    LOG_T;

    numundefmarks = 0; 

    while ((curnum < numcommand + 2) && (cur_str < num_str)) {
        LOG_1(asm_data[cur_str].str);

        if (!str_cmp(asm_data[cur_str], "HLT")) {
            buf[curnum++] = HLT;
            LOG_2 (HLT);
            
        } else if (!str_cmp(asm_data[cur_str], "push")) {
            put_pop_or_push(asm_data, buf, &cur_str, &curnum);
        
        } else if (!str_cmp(asm_data[cur_str], "pop")) {
            put_pop_or_push(asm_data, buf, &cur_str, &curnum);
        }

        //code generation for several commands
#define DEF_CMD(name, number)                       \
    else if (!str_cmp(asm_data[cur_str], name)) {   \
        buf[curnum++] = number;                     \
        LOG_2(number);                              \
    }
        #include "assembhelp.cpp"
#undef DEF_CMD 

       //code generation for jmp
#define DEF_JMP(name, number)                       \
    else if (!str_cmp(asm_data[cur_str], name)) {   \
        buf[curnum++] = number;                     \
        jmp_func(asm_data, buf, &cur_str, &curnum); \
    }
        #include "jmphelp.cpp"
#undef DEF_JMP 

        else if (asm_data[cur_str].str[0] == ':') {
            add_mark(asm_data[cur_str].str, curnum);
            LOG_M(asm_data[cur_str].str);

        } else {
            buf[curnum++] = EOF;
            printf("Undefined command\n");
            printf("input = %s\n", asm_data[cur_str].str);
            printf("curnum = %lu\n", curnum);
        }
 
        cur_str++;
    }

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

    clean_strings(&asm_data, &asm_buf);

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

static int jmp_func(struct string *asm_data, int buf[], size_t *cur_str, size_t *curnum) {
    (*cur_str)++;

    LOG_PN(asm_data[*cur_str].str);

    int check = search_mark(asm_data[*cur_str].str);

    if (arrmark[check].addres == -1) 
        numundefmarks++;

    buf[*curnum] = arrmark[check].addres;
    (*curnum)++;

    LOG_J;
    
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

static void put_pop_or_push(struct string *asm_data, int buf[], size_t *cur_str, size_t *curnum) {
    int Enum = 0;

    if (!str_cmp(asm_data[*cur_str], "pop")) {
        Enum = Pop;

    } else if (!str_cmp(asm_data[*cur_str], "push")) {
        Enum = Push;

    } else {
        printf("Undefined command in put_pop_or_push\n");
        printf("input = %s\n", asm_data[*cur_str].str);
    }

    (*cur_str)++;

    int value = 0;

    if (convert_str_to_int(asm_data[*cur_str], &value) == -1) {
        if (Enum == Pop) {
            Enum = Popr;

        } else if (Enum == Push) {
            Enum = Pushr;
        } 

        LOG_PN(asm_data[*cur_str].str);
        if (!str_cmp(asm_data[*cur_str], "rax")) {
            buf[(*curnum)++] = Enum, 
            buf[(*curnum)++] = 1;
            LOG_4X (Enum, 1);

        } else if (!str_cmp(asm_data[*cur_str], "rbx")) {
            buf[(*curnum)++] = Enum;
            buf[(*curnum)++] = 2;
            LOG_4X (Enum, 2);

        } else if (!str_cmp(asm_data[*cur_str], "rcx")) {
            buf[(*curnum)++] = Enum;
            buf[(*curnum)++] = 3;
            LOG_4X (Enum, 3);

        } else if (!str_cmp(asm_data[*cur_str], "rdx")) {
            buf[(*curnum)++] = Enum;
            buf[(*curnum)++] = 4;
            LOG_4X (Enum, 4);

        } else {
            printf("Undefined command after %s\n", asm_data[*cur_str].str);
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