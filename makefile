all: Proc.o Stack.o MainCalc.o Assemb.o SPU.o ReadStrings.o VirPng.o
	 g++ Proc.o Stack.o MainCalc.o Assemb.o SPU.o ReadStrings.o VirPng.o -o A.exe -l png

Proc.o: Proc.cpp
	g++ Proc.cpp -c

Stack.o: Stack.cpp
	g++ Stack.cpp -c

MainCalc.o: MainCalc.cpp
	g++ MainCalc.cpp -c

Assemb.o: Assemb.cpp
	g++ Assemb.cpp -c 

SPU.o: SPU.cpp 
	g++ SPU.cpp -c

ReadStrings.o: ReadStrings.cpp
	g++ ReadStrings.cpp -c

VirPng.o: VirPng.cpp
	g++ VirPng.cpp -c

clean: 
	rm -f Proc.o MainCalc.o Assemb.o

cleanstk:
	rm -f Stack.o

cleanVR:
	rm -f VirPng.o 

cleanr:
	rm -f ReadStrings.o 

cleans:
	rm -f SPU.o 
	
cleanapp:
	rm -f A.exe

cleanout:
	rm -f Out.txt

cleanerr:
	rm -f ERRORS.txt ERR_SPU.txt

cleanas:
	rm -f Assemb.o

cleanproc:
	rm -f Proc.o 
