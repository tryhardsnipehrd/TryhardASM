CXX = g++
CXXFLAGS = 
RM = rm

thl: interpreter.o instructions.o
	@echo Compiling THL.exe 
	@echo This is required
	$(CXX) -o thl $^

%.o: %.cpp
	@echo Compiling $< into a .o file
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) -f *.o thl