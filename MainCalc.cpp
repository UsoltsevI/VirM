#include <stdio.h>
#include "Calc.h"

int main() {
    printf("Program started\n");

    const char *ASM = "ASM.txt";
    const char *Bytecode = "Bytecode.bin";
    const char *Outputfile = "Out.txt";
    const char *Log = "Log.txt";

    assemb(ASM, Bytecode, Log);
    //disassemb(ASM, Bytecode);
    pocess(Bytecode, Outputfile);

    printf("Program comlited\n");
    return 0;
}