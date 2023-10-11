all: Proc.o Stack.o MainCalc.o Disassemb.o Assemb.o SPU.o
	 g++ Proc.o Stack.o MainCalc.o Disassemb.o Assemb.o SPU.o -o A.exe

Proc.o: Proc.cpp
	g++ Proc.cpp -c

Stack.o: Stack.cpp
	g++ Stack.cpp -c

MainCalc.o: MainCalc.cpp
	g++ MainCalc.cpp -c

Disassemb.o: Disassemb.cpp
	g++ Disassemb.cpp -c 

Assemb.o: Assemb.cpp
	g++ Assemb.cpp -c 

SPU.o: SPU.cpp 
	g++ SPU.cpp -c

clean: 
	rm -f Proc.o Stack.o MainCalc.o Disassemb.o Assemb.o SPU.o

cleanapp:
	rm -f A.exe

cleanout:
	rm -f Out.txt

cleanerr:
	rm -f ERRORS.txt ERR_SPU.txt
