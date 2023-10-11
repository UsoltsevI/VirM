#include "Stack.h"

#ifdef DEBUGON
static int create_check_stack      (struct struc_stack *stk);
static int create_stk_c            (struct struc_stack *stk, const char *name_stk);
static int search_stack_c          (struct struc_stack *stk, const char *func, 
                                    const int line, int creating);
    
static int hash_stack_name         (const char *name);
static int compare_stack_with_stk_c(struct struc_stack *stk, const int check);
static int stack_dump_stk_c        (struct struc_stack *stk, const int check);
static int stack_copy_to_stk_c     (struct struc_stack *stk, const int check);
static int create_stk_c_array      (struct struc_stack *stk, const int check);
static int create_stk_c_name       (struct struc_stack *stk, const int check);

static struct struc_stack *stk_c = {};
static int *stk_c_name = {};
static size_t num_stk_c = 0;

#define SEARCH_STACK_C(stk, creating) \
    search_stack_c(stk, __func__, __LINE__, creating);

#endif

#ifndef DEBUGON

int stack_dump(struct struc_stack *stk, const char *stk_name, int line, const char *file, const char *func) {
    if (stk == NULL) {
        printf("At stack_dump_d stk == NULL!!!\n");
        return -1;
    } else {
        FILE *err_file = fopen("ERRORS.txt", "w");

        if (err_file == NULL) {
            printf("fopen err_file failed at stack_dump\n");
            return -1;
        }

        fprintf(err_file, "Stack %s at the file %s at the function %s at the line %d:\n\t{\n", stk_name, file, func, line);
        fprintf(err_file, "\t capacity = %lu\n",     stk->capacity);
        fprintf(err_file, "\t a        = %lu\n",     stk->a);
        fprintf(err_file, "\t data     = %lu\n",     (unsigned long) stk->data);
        fprintf(err_file, "\t data[%lu]:\n\t\t{\n",  (unsigned long) &stk->data);

        if (stk->data != NULL) {
            for (size_t i = 0; i < stk->capacity; i++)
                fprintf(err_file, "\t\t *[%lu] = %d\n", i, stk->data[i]);

            fprintf(err_file, "\t\t}\n\t}\n\n");
        }
        fclose(err_file);
    }

    return 0;
}

#else

int stack_dump(struct struc_stack *stk, const char *stk_name, int line, const char *file, const char *func) {
    if (stk == NULL) {
        printf("At stack_dump_d stk == NULL!!!\n");
        return -1;
    } else if (stk->is_dtor == 1) {
        printf("stack %s has already destructed\n", stk_name); 
    
    } 

    else {
        fprintf(stk->err_file, "Stack %s at the file %s at the function %s at the line %d:\n\t{\n", stk_name, file, func, line);
        fprintf(stk->err_file, "\tis_ctor = %d\n", stk->is_ctor);
        fprintf(stk->err_file, "\tis_dtor = %d\n", stk->is_dtor);
        fprintf(stk->err_file, "\t struct_stack %d, defined in file %s in func %s at line %d:\n",
                stk->name_stk, stk->def_file_name, stk->def_func_name, stk->def_line);
        fprintf(stk->err_file, "\t capacity = %lu\n",     stk->capacity);
        fprintf(stk->err_file, "\t a        = %lu\n",     stk->a);
        fprintf(stk->err_file, "\t data     = %lu\n",     (unsigned long) stk->data);
        fprintf(stk->err_file, "\t data[%lu]:\n\t\t{\n",  (unsigned long) &stk->data);

        if (stk->data != NULL) {
            for (size_t i = 0; i < stk->capacity; i++)
                fprintf(stk->err_file, "\t\t *[%lu] = %d\n", i, stk->data[i]);

            fprintf(stk->err_file, "\t\t}\n\t}\n\n");
        }

        int check = SEARCH_STACK_C(stk, 0);

        return stack_dump_stk_c(stk, check);

    }
    return 0;
}

static int stack_dump_stk_c(struct struc_stack *stk, const int check) {
    if (check < 0) {
        fprintf(stk->err_file, "stk_c not founded...\n");
    } 

    fprintf(stk->err_file, "Stk_c has following parameters:\n");
    fprintf(stk->err_file, "\t{\n");
    fprintf(stk->err_file, "\t stk_c[%d].is_ctor = %d\n",      check,  stk_c[check].is_ctor);
    fprintf(stk->err_file, "\t stk_c[%d].is_dtor = %d\n",      check,  stk_c[check].is_dtor);
    fprintf(stk->err_file, "\t stk_c[%d].capacity = %lu\n",    check,  stk_c[check].capacity);
    fprintf(stk->err_file, "\t stk_c[%d].a = %lu\n",           check,  stk_c[check].a);
    fprintf(stk->err_file, "\t stk_c[%d].addres_data = %lu\n", check,  (unsigned long) stk_c[check].addres_data);
    fprintf(stk->err_file, "\t stk_c[%d].data = %lu\n\t\t{\n", check,  (unsigned long) stk_c[check].data);

    if (stk_c[check].data == NULL) {
        return -1;
    }

    for (size_t i = 0; i < stk->capacity; i++) 
        fprintf(stk->err_file, "\t\t stk_c[%d].data[%lu] = %d\n", check, i, stk_c[check].data[i]);

    fprintf(stk->err_file, "\t\t}\n\t}\n\n");

    return 0;
}

#endif

int stack_ok(struct struc_stack *stk) {
    int result = 0;

    if (stk == NULL)
        return -1;

    if (stk->data == NULL)      return 1;
    if (stk->a > stk->capacity) result += 1;

#ifdef DEBUGON
        if (stk->is_dtor == 1) return result + 2;
        if (stk->is_ctor == 0) return result + 3;

        int check = SEARCH_STACK_C(stk, 0);

        int subresult =  0;

        if (check >= 0)
            subresult = compare_stack_with_stk_c(stk, check);

        if (subresult == -1)
            return -1;

        result += subresult;
#endif

    return result;
}

int stack_push(struct struc_stack *stk, TypeElem value) {
#ifdef DEBUGON
    if (stack_ok(stk)) {
        STACK_DUMP(stk);
        return -1;
    }
#endif

    if (stk->a >= stk->capacity) {
        if (stk->capacity <= 0)
            stk->capacity = 1;

        stk->capacity = stk->capacity * 2;

        TypeElem *helper = (TypeElem *) realloc(stk->data, stk->capacity * sizeof(TypeElem));

#ifdef DEBUGON
        if (helper == NULL) {
            fprintf(stk->err_file, "realloc failed if file %s in func %s at line %d\n",
                    __FILE__, __func__, __LINE__);
            return -1;
        }
#endif
        stk->data = helper;
    }

    stk->data[stk->a++] = value;

#ifdef DEBUGON
    int check = SEARCH_STACK_C(stk, 0);
    stack_copy_to_stk_c(stk, check);
#endif

    return 0;
}

TypeElem stack_pop(struct struc_stack *stk) {
#ifdef DEBUGON
    if (stack_ok(stk)) {
        STACK_DUMP(stk);
        return -1;
    }
#endif

    if (stk->a < stk->capacity / 2) {
        stk->capacity = stk->capacity / 2;

        TypeElem *helper = (TypeElem *) realloc(stk->data, sizeof(TypeElem) * stk->capacity);

#ifdef DEBUGON
        if (helper == NULL) {
            fprintf(stk->err_file, "realloc failed if file %s in func %s at line %d\n",
                    __FILE__, __func__, __LINE__);
            return -1;
        }
#endif

        stk->data = helper;
    }

    TypeElem result = 0;

    if (stk->a > 0) {
        result = stk->data[--stk->a];
        stk->data[stk->a] = 0;

    } 
#ifdef DEBUGON   
    else {
        fprintf(stk->err_file, "You are trying to pop from empty stack!!!\n");
    }

    int check = SEARCH_STACK_C(stk, 0);
    stack_copy_to_stk_c(stk, check);
#endif

    return result;
}

int stack_dtor(struct struc_stack *stk) {
#ifdef DEBUGON
    if (stack_ok(stk)) {
        STACK_DUMP(stk);
        return -1;
    }
#endif

    if (stk == NULL) {
        printf("stk == NULL at stack_dtor!!!\n");
        return -1;
    }

    free(stk->data);
    stk->data     = NULL;
    stk->capacity = 0;
    stk->a        = 0;

#ifdef DEBUGON
    stk->def_line      = 0;
    stk->def_file_name = NULL;
    stk->def_func_name = NULL;
    stk->err_file_name = NULL;
    stk->err_file      = NULL;
    stk->is_dtor       = 1;

    int check = SEARCH_STACK_C(stk, 0);

    if (check > -1) {
        free (stk_c[check].data);
        stk_c[check].data          = NULL;
        stk_c[check].def_file_name = NULL;
        stk_c[check].def_func_name = NULL;
        stk_c[check].err_file_name = NULL;
        stk_c[check].err_file      = NULL;
        stk_c[check].def_line = 0;
        stk_c[check].data     = 0;
        stk_c[check].capacity = 0;
        stk_c[check].a        = 0;
        stk_c[check].is_dtor  = 1;
    }
#endif

    return 0;
}

int stack_copy(struct struc_stack *stk, struct struc_stack *stk_cpy) {
#ifdef DEBUGON
    if (stack_ok(stk)) {
        STACK_DUMP(stk);
        return -1;
    }
#endif

    stk_cpy->a        = stk->a;
    stk_cpy->capacity = stk->capacity;

    stk_cpy->data  = (TypeElem *) calloc(stk_cpy->capacity, sizeof(TypeElem));

#ifdef DEBUGON
    if (stk_cpy->data == NULL) {
        fprintf(stk_cpy->err_file, "Reallocation failed in func %s on line %d\n",
            __func__, __LINE__);
        return -1;
    }
#endif

    for (size_t i = 0; i < stk_cpy->capacity; i++)
        stk_cpy->data[i] = stk->data[i];

#ifdef DEBUGON 
    int check = SEARCH_STACK_C(stk_cpy, 0);
    stack_copy_to_stk_c(stk_cpy, check);
#endif

    return 0;
}

int stack_ctor(struct struc_stack *stk, const char *err_file_name, 
                    const size_t beg_capacity, const char *name_stk, 
                    const int line, const char *file, const char *func) {

    if (stk == NULL) {
        printf("At stack_ctor_d stk == NULL!!!\n");
        return -1;

    } else {
#ifdef DEBUGON
        stk->name_stk = hash_stack_name(name_stk);
        stk->def_line = line;
        stk->def_file_name = file;
        stk->def_func_name = func;
        stk->err_file = fopen(err_file_name, "w"); 
#endif
        stk->data = (TypeElem *) calloc(beg_capacity, sizeof(TypeElem));

        if (stk->data == NULL)
            return -1;

        stk->capacity = beg_capacity; 
        stk->a = 0;

    }

#ifdef DEBUGON  
    stk->is_ctor = 1;
    stk->is_dtor = 0;
    return create_stk_c(stk, name_stk);
#endif

    return 0;
}

#ifdef DEBUGON
static int create_stk_c(struct struc_stack *stk, const char *name_stk) {

    if (stk == NULL) {
        STACK_DUMP(stk);
        return -1;
    } 

    int check = SEARCH_STACK_C(stk, 1)

    if (check != -1) {
        check = num_stk_c++;
        create_stk_c_name(stk, check);
        create_stk_c_array(stk, check);

    } else {
        return -1;
    }

    return 0;
}

static int create_stk_c_name(struct struc_stack *stk, const int check) {
    assert(stk != NULL);
    assert(check >= 0);
    assert(check < num_stk_c);

    int *helper = (int *) realloc(stk_c_name, check * sizeof(int));

    if (helper == NULL) {
        fprintf(stk->err_file, "reallocation failed in func %s on line %d\n", 
        __func__, __LINE__);
        return -1;
    } 

    stk_c_name = helper;
    stk_c_name[check] = stk->name_stk;

    return 0;
}

static int create_stk_c_array(struct struc_stack *stk, const int check) {
    assert(stk != NULL);
    assert(check >= 0);
    assert(check < num_stk_c);

    struct struc_stack *helper = (struc_stack *) realloc(stk_c, num_stk_c * sizeof(struc_stack));

    if (helper == NULL) {
        fprintf(stk->err_file, "reallocation failed in func %s on line %d \n", 
        __func__, __LINE__);
        return -1;
    } 

    stk_c = helper;

    stack_copy_to_stk_c(stk, check);

    return 0;
}

static int compare_stack_with_stk_c(struct struc_stack *stk, const int check) {
    assert(stk != NULL);
    assert(check >= 0);
    assert(check < num_stk_c);

    int result = 0;

    if (stk_c[check].is_ctor       != stk->is_ctor)       result++;
    if (stk_c[check].is_dtor       != stk->is_dtor)       result++;
    if (stk_c[check].name_stk      != stk->name_stk)      result++;
    if (stk_c[check].def_line      != stk->def_line)      result++;
    if (stk_c[check].def_file_name != stk->def_file_name) result++;
    if (stk_c[check].def_func_name != stk->def_func_name) result++;
    if (stk_c[check].a             != stk->a)             result++;
    if (stk_c[check].capacity      != stk->capacity)      result++;
    if (stk_c[check].addres_data   != stk->data)          result++;
    if (stk_c[check].err_file_name != stk->err_file_name) result++;
    if (stk_c[check].err_file      != stk->err_file)      result++;

    for (size_t i = 0; i < stk_c[check].capacity; i++)
        if (stk_c[check].data[i] != stk->data[i])
            result++;

    return result;
}

static int stack_copy_to_stk_c(struct struc_stack *stk, const int check) {
    assert(stk != NULL);
    assert(check >= 0);
    assert(check < num_stk_c);

    stk_c[check].is_ctor       = stk->is_ctor;         
    stk_c[check].is_dtor       = stk->is_dtor;          
    stk_c[check].name_stk      = stk->name_stk;         
    stk_c[check].def_line      = stk->def_line;        
    stk_c[check].def_file_name = stk->def_file_name;   
    stk_c[check].def_func_name = stk->def_func_name;    
    stk_c[check].a             = stk->a;                      
    stk_c[check].addres_data   = stk->data;        
    stk_c[check].err_file_name = stk->err_file_name;   
    stk_c[check].err_file      = stk->err_file;        

    if (stk_c[check].capacity != stk->capacity) {
        stk_c[check].capacity = stk->capacity;
        
        if (stk->a == 0) {
            stk_c[check].data = (TypeElem *) calloc(stk_c[check].capacity, sizeof(TypeElem));

        } else {
            stk_c[check].data = (TypeElem *) realloc(stk_c[check].data, stk_c[check].capacity * sizeof(TypeElem));
        }

        if (stk_c[check].data == NULL) {
            fprintf(stk->err_file, "reallocation failed in func %s on line %d\b", 
                    __func__, __LINE__);
            return -1;
        } 
    }

    for (size_t i = 0; i < stk_c[check].capacity; i++)
        stk_c[check].data[i] = stk->data[i];

    return 0;
}

static int hash_stack_name(const char *name) {
    assert(name != NULL);

    size_t i = 0;
    int result = 5381;

    if (*(name + i) == '&')
        i++;

    while ((*(name + i) != '\0') && (i < 10)) {
        result += (int) (*(name + i)) * 33;
        i++;
    }

    return result;
}

static int search_stack_c(struct struc_stack *stk, const char *func, 
                            const int line, const int creating) {

    if (stk == NULL) {
        printf("stk == NULL at search_stack_c, called by command from func %s on line %d\n", func, line);
        return -1;
    }

    for (size_t check = 0; check < num_stk_c; check++) {
        if (stk->name_stk == stk_c_name[check]) {
            if (creating == 1) 
                fprintf(stk->err_file, "stk_c founded but not createded func %s on line %d\n", func, line);

            return check;
        }
    }

    if (creating != 1) {
        fprintf(stk->err_file, "stk_c not founded at func %s on line %d\n", func, line);
        return -1;
    }

    return 0;
}

#endif
