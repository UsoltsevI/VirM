#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED
#include <stdio.h>

#define DEBUGONL

#ifdef DEBUGONL

static const char* log_file_name = "Log.txt";
static FILE *Log;
int NUM_ASSEMBLY = 0;

#define LOG_O Log = fopen(log_file_name, "w")
#define LOG_T   fprintf(Log, "NUM_ASSEMBLY: %d\n", NUM_ASSEMBLY);                   \
                fprintf(Log, "SIGNATURE: %s\n", SIGNATURE);                         \
                fprintf(Log, "VERSION: %d\n", VERSION);                             \
                fprintf(Log, "numcommand = %d\n", numcommand);                      \
                fprintf(Log, "numundefmarks = %d\n", numundefmarks);                  \
                fprintf(Log, "    Name              Code             Addres\n");           \
                fprintf(Log, "    in ASM            for VM           in Bytecode buf\n");   \
                fprintf(Log, "-------------------------------------------------------\n");

#define LOG_P(a)        fprintf(Log, "%6s\n", a)
#define LOG_PN(a)       fprintf(Log, "%6s", a)
#define LOG_2(a)        fprintf(Log, "%18d %15lu\n", a, curnum - 1)
#define LOG_2X(a)       fprintf(Log, "%18d %15lu\n", a, *curnum - 1)
#define LOG_4(a, b)     fprintf(Log, "%12d %4d %10lu %4lu\n", a, b, curnum - 2, curnum - 1)
#define LOG_4X(a, b)    fprintf(Log, "%12d %4d %10lu %4lu\n", a, b, *curnum - 2, *curnum - 1)
#define LOG_1(a)        fprintf(Log, "%6s ", a)
#define LOG_M(a)        fprintf(Log, "%18c %15c \n", '-', '-');
#define LOG_J           fprintf(Log, "%12d %4d %10lu %4lu\n", buf[*curnum - 2], buf[*curnum - 1], *curnum - 2, *curnum - 1);
#define LOG_N           fprintf(Log, "\n");
#define LOG_C           fprintf(Log, "end\n"); \
                        fclose(Log);

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