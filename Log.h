#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED
#include <stdio.h>

#define DEBUGONL

#ifdef DEBUGONL

static const char* log_file_name = "Log.txt";
static FILE *Log;
int NUM_ASSEMBLY = 0;
int NUM_OUT = 0;
void log_spl();

#define LOG_O Log = fopen(log_file_name, "w")
#define LOG_T   fprintf(Log, "NUM_ASSEMBLY: %d\n", NUM_ASSEMBLY);                           \
                fprintf(Log, "SIGNATURE: %s\n", SIGNATURE);                                 \
                fprintf(Log, "VERSION: %d\n", VERSION);                                     \
                fprintf(Log, "numcommand = %d\n", numcommand);                              \
                fprintf(Log, "numundefmarks = %d\n", numundefmarks);                        \
                fprintf(Log, "    Name              Code             Addres\n");            \
                fprintf(Log, "    in ASM            for VM           in Bytecode buf\n");   \
                fprintf(Log, "-------------------------------------------------------\n");

#define LOG_P(a)        fprintf(Log, "%6s", a);
#define LOG_PN(a)       fprintf(Log, "%6s\n", a); log_spl()
#define LOG_2(a)        fprintf(Log, "%18d %18lu\n", a, curnum - 1); log_spl()
#define LOG_2X(a)       fprintf(Log, "%18d %15lu\n", a, *curnum - 1); log_spl()
#define LOG_2XP(a)      fprintf(Log, "%12d %18lu\n", a, *curnum - 1); log_spl()
#define LOG_4(a, b)     fprintf(Log, "%18d %4d %10lu %lu\n", a, b, curnum - 2, curnum - 1); log_spl()
#define LOG_4X(a, b)    fprintf(Log, "%12d %4d %10lu %lu\n", a, b, *curnum - 2, *curnum - 1); log_spl()
#define LOG_1(a)        fprintf(Log, "%6s ", a);
#define LOG_M(a)        fprintf(Log, "%18c %18c \n", '-', '-'); log_spl()
#define LOG_J           fprintf(Log, "%12d %4d %13lu %lu\n", buf[*curnum - 2], buf[*curnum - 1], *curnum - 2, *curnum - 1); log_spl()
#define LOG_N           fprintf(Log, "\n"); log_spl()
#define LOG_C           fprintf(Log, "\nEND\n"); \
                        fclose(Log);

void log_spl() {
    if ((NUM_OUT % 5) == 0)                                                            
        fprintf(Log, "-------------------------------------------------------\n"); 

    NUM_OUT++;
}    

#else 

#define LOG_O           ;     
#define LOG_T           ;                  
#define LOG_P(a)        ;
#define LOG_2(a)        ;
#define LOG_2X(a)       ;
#define LOG_4(a, b)     ;
#define LOG_4X(a, b)    ;
#define LOG_1(a)        ;
#define LOG_M(a)        ;
#define LOG_J           ;
#define LOG_N           ;
#define LOG_C           ;

#endif 

#endif //LOG_H_INCLUDEDs