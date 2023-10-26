#include "Calc.h"
#include <stdio.h>

int SPUctor(struct SPU *spu, const char *spu_err_file_name, const char *err_file_name, size_t beg_CS_capacity, size_t beg_stk_capacity) {
    if (spu == NULL) {
        printf("SPU == NULL at SPUctor\n");
        return -1;
    }

    if (spu->is_ctor == 1) {
        printf("spu has already created\n");
        return -1;
    }

    if (spu->is_dtor == 1) {
        printf("spu has already destroyed\n");
        return -1;
    }

    if (STACK_CTOR(&spu->stk, err_file_name, beg_stk_capacity)) {
        printf("ERROR at SPUctor: STACK_CTOR != 0\n");
        return -1;
    }

    for (int i = 0; i < num_reg; i++) 
        spu->reg[i] = 0;

    for (int i = 0; i < ram_capacity; i++)
        spu->RAM[i] = 0;

    spu->IP  = 0;
    spu->spu_err_file_name = spu_err_file_name;

    spu->CS_capacity = beg_CS_capacity;
    spu->CS = (int*) calloc(beg_CS_capacity, sizeof(int));
    
    if (spu->CS == NULL) {
        printf("Failed callocation spu->CS at SPUctor\n");
        return -1;
    }

    spu->is_ctor = 1;

    return 0;
}

int SPUdtor(struct SPU *spu) {
    if (spu == NULL) {
        printf("SPU == NULL at SPUdtor\n");
        return -1;
    }

    if (spu->is_ctor == 0) {
        printf("spu has not created\n");
        return -1;
    }

    if (spu->is_dtor == 1) {
        printf("spu has already destroyed\n");
        return -1;
    }

    if (stack_dtor(&spu->stk)) {
        printf("ERROR at SPUdtor: stack_dtor != 0\n");
        return -1;
    }

    free(spu->CS);

    for (int i = 0; i < num_reg; i++) 
        spu->reg[i] = 0;
    
    for (int i = 0; i < ram_capacity; i++)
        spu->RAM[i] = 0;

    spu->IP  = 0;

    spu->is_dtor = 1;

    return 0;
}

int SPUver(struct SPU *spu) {
    if (spu == NULL) {
        printf("spu == NULL at SPUver\n");
        return -1;
    }

    if (spu->is_ctor != 1) {
        printf("spu has not created\n");
        return -1;
    }

    if (spu->is_dtor != 0) {
        printf("spu has already destroyed\n");
        return -1;
    }

    if (spu->CS == NULL) {
        printf("spu->CS == NULL\n");
        return -1;
    }

    if (spu->spu_err_file_name == NULL) {
        printf("spu->spu_err_file_name == NULL at SPUvar\n");
        return -1;
    }

    if (spu->CS_capacity < spu->IP) {
        printf("spu->CS_capacity < spu->IP\n");
        return -1;
    }

    return 0;
}

int SPUdump(struct SPU *spu, FILE* err_file) {
    if (SPUver(spu)) {
        printf("SPUver != 0 at SPUdump\n");
        return -1;
    }

    if (err_file == NULL) {
        printf("fopen failed at SPUdump\n");
        return -1;
    }

    if (STACK_DUMP(&spu->stk)) {
        fprintf(err_file, "STACK_DUMP != 0 at SPUdump\n");

    } else {
        fprintf(err_file, "spu->stk dumped in correcting file\n");
    }

    fprintf(err_file, "is_ctor = %d\n", (int) spu->is_ctor);
    fprintf(err_file, "is_dtor = %d\n", (int) spu->is_dtor);
    fprintf(err_file, "rax = %d\n", (int) spu->reg[0]);
    fprintf(err_file, "rbx = %d\n", (int) spu->reg[1]);
    fprintf(err_file, "rcx = %d\n", (int) spu->reg[2]);
    fprintf(err_file, "rdx = %d\n", (int) spu->reg[3]);
    fprintf(err_file, "IP  = %d\n", (int) spu->IP );
    fprintf(err_file, "CS_capacity  = %d\n", (int) spu->CS_capacity);
    fprintf(err_file, "CS  = %lu\n", (unsigned long) spu->CS );

    for (size_t i = 0; i < spu->CS_capacity; i++)
        fprintf(err_file, "%6.2lu ", i);

    fprintf(err_file, "\n");

    for (size_t i = 0; i < spu->CS_capacity; i++)
        fprintf(err_file, "%6d ", spu->CS[i]);

    fprintf(err_file, "\n");

    for (size_t i = 1; i < (spu->IP + 1) * 7 - 1; i++) 
        fprintf(err_file, " ");

    fprintf(err_file, "^IP = %d\n", (int) spu->IP);

    fprintf(err_file, "RAM: \n");

    for (int i = 0; i < ram_capacity; i++)
        fprintf(err_file, "%.3d | ", i);

    fprintf(err_file, "\n");

    /*for (int i = 0; i < ram_capacity * 5; i++)
        fprintf(err_file, "-");

    fprintf(err_file, "\n");*/

    for (int i = 0; i < ram_capacity; i++)
        fprintf(err_file, "%.3d | ", spu->RAM[i]);
    
    fprintf(err_file, "\n\n\n");

    return 0;
}

int SPUreadbytecode(struct SPU *spu, const char *bytecode_file_name) {
    if (SPUver(spu)) {
        printf("SPUvar != 0 at SPUreadbytecode\n");
        return -1;
    }

    if (bytecode_file_name == NULL) {
        printf("bytecode_file_name == NULL at SPUreadbytecode\n");
        return -1;
    }

    FILE *bytecode = fopen(bytecode_file_name, "r");

    if (bytecode == NULL) {
        printf("foupen bytecode file failed at SPUreadbytecode\n");
        return -1;
    }

    int value = 0;
    size_t number = 0;
    int check = fscanf(bytecode, "%d", &value);

    while (check != EOF) {
        if (number > spu->CS_capacity) {
            if (spu->CS_capacity == 0)
                spu->CS_capacity = 1;

            spu->CS_capacity *= 2;

            spu->CS = (int *) realloc(spu->CS, spu->CS_capacity * sizeof(int));

            if (spu->CS == NULL) {
                printf("Feailed reallocation for spu->CD\n");
                return -1;
            }
        }

        spu->CS[number] = value;

        number++;

        check = fscanf(bytecode, "%d", &value);
    }

    spu->CS[spu->CS_capacity] = 0;

    fclose(bytecode);

    return 0;
}

int SPUreadbytecodebin(struct SPU *spu, const char *bytecode_file_name) {
    if (SPUver(spu)) {
        printf("SPUvar != 0 at SPUreadbytecode\n");
        return -1;
    }

    if (bytecode_file_name == NULL) {
        printf("bytecode_file_name == NULL at SPUreadbytecode\n");
        return -1;
    }

    FILE *bytecode = fopen(bytecode_file_name, "rb");

    if (bytecode == NULL) {
        printf("foupen bytecode file failed at SPUreadbytecode\n");
        return -1;
    }

    fseek(bytecode, EOF, SEEK_END);//addres = fseek(...)

    spu->CS_capacity = ftell(bytecode);
    //printf("spu->CS_capacity = %lu\n", spu->CS_capacity);
    fseek(bytecode, 0, SEEK_SET);

    spu->CS_capacity = spu->CS_capacity / sizeof(int) + 1;

    spu->CS = (int *) calloc(spu->CS_capacity + 1, sizeof(int));

    if (spu->CS == NULL) {
        printf("Failed callocation for spu->CS at SPUreadbytecodebin\n");
        return -1;
    }

    fread(spu->CS, sizeof(int), spu->CS_capacity, bytecode);

    spu->CS[spu->CS_capacity] = 0;

    fclose(bytecode);

    return 0;
}

int SPUwritebytecodeastxt(struct SPU spu, const char *bytecode_txt_file_name) {
    FILE* bytecode_txt = fopen(bytecode_txt_file_name, "w");

    if (!bytecode_txt) return -1;

    for (int i = 0; i < spu.CS_capacity; i++)
        fprintf(bytecode_txt, "%3d   %d\n", i, spu.CS[i]);
    
    fclose(bytecode_txt);

    return 0;
}
