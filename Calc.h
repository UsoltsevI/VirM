#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED

#include "Stack.h"

#define MAX_COMMAND_SIZE 5

enum {
    HLT  = -1,
    Push = 1,
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
    Popr = 43,
    Pushr = 33
};

struct SPU {
    struct struc_stack stk;
    TypeElem rax;
    TypeElem rbx;
    TypeElem rcx;
    TypeElem rdx;
    int *CS;
    size_t IP;
    size_t CS_capacity;
    const char *spu_err_file_name;
    bool is_ctor;
    bool is_dtor;
};

int assemb(const char *ASMfilename, const char *Bytecodefilename);

int disassemb(const char *ASMfilename, const char *Bytecodefilename);

int pocess(const char *Bytecodefilename, const char *Outputfilename);

int SPUctor(struct SPU *spu, const char *spu_err_file_name, const char *err_file_name, size_t beg_CS_capacity, size_t beg_stk_capacity);
int SPUdtor(struct SPU *spu);
int SPUver (struct SPU *spu);
int SPUdump(struct SPU *spu);
int SPUreadbytecode(struct SPU *spu, const char *bytecode_file_name);

#endif //CALC_H_INCLUDED