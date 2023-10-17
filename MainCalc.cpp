#include <stdio.h>
#include "Calc.h"

int main() {
    printf("Program started\n");

    const char *ASM = "ASM.txt";
    const char *Bytecode = "Bytecode.bin";
    const char *Outputfile = "Out.txt";

    //assemb(ASM, Bytecode);
    disassemb(ASM, Bytecode);
    pocess(Bytecode, Outputfile);

    printf("Program comlited\n");
    return 0;
}