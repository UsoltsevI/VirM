#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define DEBUGON  //detector for debug version

typedef int TypeElem;

struct struc_stack {
    TypeElem *data;
    size_t a;
    size_t capacity;

#ifdef DEBUGON
    FILE *err_file;
    const char *err_file_name;
    int def_line;
    int name_stk;
    const char *def_file_name;
    const char *def_func_name;
    TypeElem *addres_data;
    bool is_dtor;
    bool is_ctor;

#endif
};

int stack_dump(struc_stack *stk, const char *stk_name, int line, const char *file, const char *func);

int stack_ok(struc_stack *stk);

int stack_push(struc_stack *stk, TypeElem value);

TypeElem stack_pop(struc_stack *stk);

int stack_dtor(struc_stack *stk);

int stack_copy(struc_stack *stk, struc_stack *stk_cpy);

int stack_ctor(struct struc_stack *stk, const char *err_file_name, 
                    const size_t beg_capacity, const char *name_stk, 
                    const int line, const char *file, const char *func);


#define STACK_DUMP(stk) stack_dump(stk, #stk "\0", __LINE__, __FILE__, __func__)

#define STACK_CTOR(stk, err_file_name, beg_capacity)       \
    stack_ctor(stk, err_file_name, beg_capacity, #stk "\0", \
                    __LINE__, __FILE__, __func__)

#endif // STACK_H_INCLUDED
