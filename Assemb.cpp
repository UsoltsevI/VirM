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
static int set_p_elem(struct string *asm_data, int buf[], size_t *cur_str, size_t *curnum, const TypeElem Enum);
static int set_p_elem_ram(struct string *asm_data, int buf[], size_t *cur_str, size_t *curnum, const TypeElem Enum);
static void dump_buf(const int *buf, const size_t numcommand);
static void dump_arr_mark(const char *filename);
static void create_arr();
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

#ifdef DEBUGONA
    write_strings(asm_data, num_str, "LLL.txt");
#endif

    size_t cur_str = 0;

    if (check_signature(asm_data[cur_str++].str))
        return -1;

    int version    = 0;
    int numcommand = 0;
    convert_str_to_int(asm_data[cur_str++], &version);
    convert_str_to_int(asm_data[cur_str++], &numcommand);

    if (check_version(version))
        return -1;
    
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

    while ((curnum < numcommand) && (cur_str < num_str) && (asm_data[cur_str].str != NULL)) {
        if (asm_data[cur_str].str[0] == '%') {
            cur_str++;
            continue;
        }
        
        LOG_1(asm_data[cur_str].str);

        if (!str_cmp(asm_data[cur_str], "HLT")) {
            buf[curnum++] = HLT;
            LOG_2 (HLT);
            
        } else if (!str_cmp(asm_data[cur_str], "push")) {
            int Enum = Push;
            if (set_p_elem(asm_data, buf, &cur_str, &curnum, Enum))
                set_p_elem_ram(asm_data, buf, &cur_str, &curnum, Enum);
        
        } else if (!str_cmp(asm_data[cur_str], "pop")) {
            int Enum = Pop;
            if (set_p_elem(asm_data, buf, &cur_str, &curnum, Enum))
                set_p_elem_ram(asm_data, buf, &cur_str, &curnum, Enum);

        } else if (!str_cmp(asm_data[cur_str], "outv")) {
            buf[curnum++] = Outv;
            cur_str++;
            buf[curnum++] = hash_file_name(asm_data[cur_str]);
            LOG_4(buf[curnum - 2], buf[curnum - 1]);
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

#ifdef DEBUGONA
    dump_buf(buf, numcommand);
    dump_arr_mark("mark.txt");
    int zero = 0;
    printf("Enter any key to contimue:\n");
    scanf("%d", &zero);
#endif

    if (numundefmarks > 0) {
        printf("Next assemb\n");
        LOG_N;
        NUM_ASSEMBLY++;
        return assembly(ASMfilename, Bytecodefilename, Logfilename);
    }
    
    printf("The assembler has completed its tasks\n");

    write_bytecode(buf, Bytecodefilename, numcommand);

    clean_strings(&asm_data, &asm_buf);

    LOG_C;

    return 0;
}

int hash_file_name(const struct string s) {
    int hash = 0;

    char *helper = (char *) &hash;

    for (int i = 0; i <= s.len; i++) 
        *(helper + i) = s.str[i];

    return hash; 
}

char *unhash_file_name(int hash) {
    char *filename = (char *) calloc(8, sizeof(char));
    char *helper = (char *) &hash;

    unsigned i = 0;

    for (i = 0; (i < 4) && ('a' <= *(helper + i)) && (*(helper + i) <= 'z'); i++) 
        filename[i] = (unsigned char) *(helper + i);
    
    filename[i++] = '.';
    filename[i++] = 'p';
    filename[i++] = 'n';
    filename[i++] = 'g';

    return filename;
}

static void write_bytecode(const int *buf, const char *Bytecodefilename, const size_t numcommand) {
    FILE *Bytecode = fopen(Bytecodefilename, "wb");
    fwrite(buf, sizeof(int), numcommand, Bytecode);
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

int check_version(const int version) {
    if (version != VERSION) {
        printf("Incorrect ASM version\n");
        printf("Right version: %d\n", VERSION);
        printf("Entered version: %d\n", version);
        return -1;
    }

    return 0;
}

int check_signature(const char *signature) {
    if (strcmp(signature, SIGNATURE)) {
        printf("Incorrect ASM signature\n");
        printf("right signature: %s\n", SIGNATURE);
        printf("Entered signature: %s\n", signature);
        return -1;
    }

    return 0;
}

static int jmp_func(struct string *asm_data, int buf[], size_t *cur_str, size_t *curnum) {
    (*cur_str)++;

    LOG_P(asm_data[*cur_str].str);

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

static void dump_arr_mark(const char *filename) {
    FILE *file = fopen(filename, "w");

    fprintf(file, "arrmark: { \n");

    for (size_t i = 0; i < maxnummark; i++)
        fprintf(file, "%6lu ", i);
    
    fprintf(file, "\n");

    for (size_t i = 0; i < maxnummark; i++)
        fprintf(file, "%6s ", arrmark[i].name);

    fprintf(file, "\n");

    for (size_t i = 0; i < maxnummark; i++)
        fprintf(file, "%6d ", arrmark[i].addres);

    fprintf(file, "\n}\n");

    fclose(file);
}

static void create_arr() {
    for (size_t i = 0; i < maxnummark; i++)
        arrmark[i].addres = -1;
}

static int set_p_elem(struct string *asm_data, int buf[], size_t *cur_str, size_t *curnum, const TypeElem EnumEnt) {
    int value = 0;

    (*cur_str)++;

    if (convert_str_to_int(asm_data[*cur_str], &value)) {
        LOG_P(asm_data[*cur_str].str);
        int Enum = 40 + EnumEnt;

        if (!str_cmp(asm_data[*cur_str], "rax")) {
            buf[(*curnum)++] = Enum;
            buf[(*curnum)++] = 1;
            printf("reg\n");
            LOG_4X (buf[*curnum - 2], buf[*curnum - 1]);

        } else if (!str_cmp(asm_data[*cur_str], "rbx")) {
            buf[(*curnum)++] = Enum;
            buf[(*curnum)++] = 2;
            printf("reg\n");
            LOG_4X (buf[*curnum - 2], buf[*curnum - 1]);

        } else if (!str_cmp(asm_data[*cur_str], "rcx")) {
            buf[(*curnum)++] = Enum;
            buf[(*curnum)++] = 3;
            printf("reg\n");
            LOG_4X (buf[*curnum - 2], buf[*curnum - 1]);

        } else if (!str_cmp(asm_data[*cur_str], "rdx")) {
            buf[(*curnum)++] = Enum;
            buf[(*curnum)++] = 4;
            printf("reg\n");
            LOG_4X (buf[*curnum - 2], buf[*curnum - 1]);

        } else {
            return -1;
        }

    } else {
        buf[(*curnum)++] = EnumEnt;
        buf[(*curnum)++] = value;
        fprintf(Log, "%6d %10d %6d %12lu %lu\n", value, buf[*curnum - 2], buf[*curnum - 1], *curnum - 2, *curnum - 1);
    }

    return 0;
}

static int set_p_elem_ram(struct string *asm_data, int buf[], size_t *cur_str, size_t *curnum, const TypeElem EnumEnt) {
    if (asm_data[*cur_str].str[0] == '[') {
        int Enum = 60 + EnumEnt;
        buf[(*curnum)++] = Enum;
        LOG_2XP (buf[*curnum - 1]);

    } else if (asm_data[*cur_str].str[0] == '(') {
        int Enum = 70 + EnumEnt;
        buf[(*curnum)++] = Enum;
        LOG_2XP (buf[*curnum - 1]);

    } else {
        buf[(*curnum)++] = EnumEnt;
        (*cur_str)--;
        LOG_2XP (buf[*curnum - 1]);
    }

    return 0;
}