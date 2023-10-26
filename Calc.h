#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED

#include "Stack.h"
#include "ReadStrings.h"
#include "VirPng.h"

#define MAX_COMMAND_SIZE 5
#define SIGNATURE "ASMU"
#define VERSION 113
const unsigned num_reg = 4;
const unsigned ram_capacity = 32;
const unsigned video_ram_capacity = 360 * 240; //640 * 480; //720 * 576;

enum {
    HLT  = -1,
    Push = 1, // push rax + 10; push rax[10]
    Pop  = 2,
    Div  = 3,
    Sub  = 4,
    Out  = 5,
    Add  = 6,
    Mul  = 7,
    Sqrt = 8,
    Sin  = 9,
    Cos  = 10,
    In   = 11,
    Jmp  = 44,
    Ja   = 45,
    Jae  = 46,
    Jb   = 47,
    Jbe  = 48,
    Je   = 50,
    Jne  = 51,
    Popr = 41,  //all function number with elements after it 40 more than usual
    Pushr = 42,
    Pushm = 61, //all function numbers for working with ram 60 more than usual
    Popm  = 62,
    Pushv = 71, //all function numbers for working with video 70 more than usual
    Popv  = 72,
    Outv = 75,
};

struct SPU {
    struct struc_stack stk;
    TypeElem reg[num_reg];
    TypeElem RAM[ram_capacity];
    png_byte video_ram[video_ram_capacity];
    int *CS;
    size_t IP;
    size_t CS_capacity;
    const char *spu_err_file_name;
    bool is_ctor;
    bool is_dtor;
};

// asssembly, ..., process
int assembly(const char *ASMfilename, const char *Bytecodefilename, const char *Logfilename);
int check_signature(const char * signature);
int check_version(const int version);
int hash_file_name(const struct string s);
char *unhash_file_name(int hash);

//int disassemb(const char *ASMfilename, const char *Bytecodefilename);

int process(const char *Bytecodefilename, const char *Outputfilename);

int SPUctor(struct SPU *spu, const char *spu_err_file_name, const char *err_file_name, size_t beg_CS_capacity, size_t beg_stk_capacity);
int SPUdtor(struct SPU *spu);
int SPUver (struct SPU *spu);
int SPUdump(struct SPU *spu, FILE* err_file);
int SPUreadbytecode(struct SPU *spu, const char *bytecode_file_name);
int SPUreadbytecodebin(struct SPU *spu, const char *bytecode_file_name);
int SPUwritebytecodeastxt(struct SPU spu, const char *bytecode_txt_file_name);

#endif //CALC_H_INCLUDED