CC = g++
CFLAGS = 

thl: instructions.o interpreter.o
	@echo Compiling THL.exe 
	@echo This is required
	$(CC) $(CFLAGS)  -o THL instructions.o interpreter.o

instructions.o: instructions.cpp include/instructions.h
	@echo Compiling instructions.cpp into a .o file
	$(CC) $(CFLAGS) -c instructions.cpp

interpreter.o: interpreter.cpp include/instructions.h
	@echo Compiling interpreter.cpp into a .o file
	$(CC) $(CFLAGS) -c interpreter.cpp