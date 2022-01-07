/*
~~~~OVERVIEW~~~~
Language designed similarly to 65(c)02 Assembly, with some personal design choices instead.
Should technically be Turing complete, although that's a whole different debate.

~~~~PLANS~~~~

6 registers:
X 
Y
Z 
A
B
C

Each register is initialized to 0 upon starting

Memory/RAM --- 	a variable amount of space for variables, defined during runtime, before code is ran
Defaults to 1K slots as of now, will be variable later


CHK flag --- Holds the value of the Comparison instructions, allows one to see the output of the comparison


Math Instructions
AD* -- ADd value to *, argument can be register or int
SB* -- SuBtract value from *, argument can be register or int
IN* -- INcrement * by 1, takes no arguments

Comparison Instructions
CP* -- ComPare to *. Argument, which is value comparing, can be an int or a register
GT* -- Greater Than *. Argument comparing can be an int or a register
LT* -- Less than *. Argument comparing can be an int or a register
IFT -- IF the comparison instruction last used was True, run the next line. Otherwise skip it

Memory Instructions
ST* -- STore register * into memory address given.
LD* -- LoaD value from memory address given into *.

Input/Output Instructions
INP -- take user INPut and store it into given memory spot
OUT -- Outputs the value given to terminal

Control Flow Instructions
# --- says it is a comment, don't run anything on this line, must be a singular # followed by a space, eg: "# This is a comment"
## is invalid, as is "#This is a comment"
LBL -- Create a LaBeL to JMP to.
JMP -- JuMP to the given LaBeL
BEQ -- Branch if EQual to. Branches to the label if the CHK flag is set
BNE -- Branch if Not Equal to. Branches to the label if the CHK flag is NOT set


Pre-Processor Directives
!print --- when OUT prints the first argument, which is a number, it will instead replace that with the string given as the second argument
!input --- will output the string given when INP is ran, allowing one to tell the player what to do
^^^ Might be allowed anywhere, pretty much allowing on-the-fly changing, so that more complex apps can be made.
*/

// Includes here
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>

// Including my own files
#include "include/instructions.h"

// Weird hackery, don't question it, it stops the errors.
#define nop 

// Defining errors here, makes it much easier to read and deal with than just numbers
#define incorrectArgCount 1

// Define all memory here
int xReg = 0;
int yReg = 0;
int zReg = 0;
int aReg = 0;
int bReg = 0;
int cReg = 0;
bool CHK = false;
const int memSize = 1000;
int memory[memSize] = { };
std::map<std::string, int> Labels;
std::map<int, std::string> PrintDirectiveValues;
std::string currentInpString = "-";


// Define non-user variables
std::fstream programFile;
std::string line;
std::vector<std::string> lineCode;
std::vector<std::string> fileVector;
int tempNum;

char n;

bool debugMode = false;
bool foundPrintDirective = false;


// Quick stolen function from https://stackoverflow.com/a/46943631
std::vector<std::string> split(std::string str, std::string token){
    std::vector<std::string> result;
    while(str.size()){
        int index = str.find(token);
        if(index!=std::string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}


/*
This function handles errors, instead of making the constantly, just call this then return -1
This will eventually evolve, but for now just prints
*/
void ERR(int errorToCall, std::string insStr, int expArgs, int parseLineSize, std::string additionalText = "NWDNZ") {
	switch (errorToCall) {
		case incorrectArgCount: // This is to be called when the number of args given is incorrect
			std::cout << insStr << " got " << parseLineSize - 1 << " arguments, and expected " << expArgs << ".\n";
			break;
	}
	if (additionalText != "NWDNZ") {std::cout << additionalText << "\n";}
}



/*
AD* Function
Usage: AD<Register> <Integer/Register>
ADds the Integer or value of the Register given to the current Register, negative numbers are allowed
*/

// This function will be commented, the rest operate on a similar basis. I will only explain things in those that are different.
// Yes the comments are almost too much and obvious, I'd rather too much than too little.
int AD_Star (std::vector<std::string> parseLine, std::string insStr, int *regToUse) {
	if (parseLine.size() != 2){ 
		// Check if there are exactly 2 elements in the line, the AD* and the argument
		// If we did not get that, we will error and give a better output. (I can probably turn this into a function)
		// Essentially say that we expected a different amount of arguments
		ERR(incorrectArgCount, insStr, 2, parseLine.size());
		return -1;
	} else if (parseRegisters(parseLine[1]) == NotARegister) { 
		// See if we got something other than a register, we are assuming it is a number
		tempNum = std::stoi(parseLine[1]); // Grabbing the second element of the vector, which is the argument given
		*regToUse += tempNum; // Add that number to the register given by the command
	} else {
		switch (parseRegisters(parseLine[1])) { 
			// If we actually hit a register, we will then check which one it is
			// This does essentially the the same as above, just redone for each register
			case A:
				*regToUse += aReg;
				break;
			case B:
				*regToUse += bReg;
				break;
			case C:
				*regToUse += cReg;
				break;
			case X:
				*regToUse += xReg;
				break;
			case Y:
				*regToUse += yReg;
				break;
			case Z:
				*regToUse += zReg;
				break;
		}
	}
	return 0;
}

/*
SB* Function
Usage: SB<Register> <Integer/Register>
SuBtracts the Integer or value of the Register given from the current Register, negative numbers can be the result
*/
int SB_Star (std::vector<std::string> parseLine, std::string insStr, int *regToUse) {
	if (parseLine.size() != 2){
		ERR(incorrectArgCount, insStr, 2, parseLine.size());
		return -1;
	} else if (parseRegisters(parseLine[1]) == NotARegister) {
		tempNum = std::stoi(parseLine[1]);
		*regToUse -= tempNum; // As a bi-product of C++ being smart, it will just add a negative number
	} else {
		switch (parseRegisters(parseLine[1])) {
			case A:
				*regToUse -= aReg;
				break;
			case B:
				*regToUse -= bReg;
				break;
			case C:
				*regToUse -= cReg;
				break;
			case X:
				*regToUse -= xReg;
				break;
			case Y:
				*regToUse -= yReg;
				break;
			case Z:
				*regToUse -= zReg;
				break;
		}
	}
	return 0;
}

/*
IN* Function
Usage: IN<Register>
INcrements the Register given by 1
*/
int IN_Star (std::vector<std::string> parseLine, std::string insStr, int *regToUse) {
	if (parseLine.size() != 1){
		ERR(incorrectArgCount, insStr, 1, parseLine.size());
		return -1;
	} else {*regToUse += 1;} // This one literally just adds one to the register given, since it takes no arguments
				
	return 0;
}


/*
CP* Function
Usage CP<Register> <Integer/Register>
ComPares an Integer or value from a Register to the Register specified, makes CHK flag 1 if it is equal, 0 otherwise
*/
int CP_Star (std::vector<std::string> parseLine, std::string insStr, int *regToUse) {
	if (parseLine.size() != 2) {
		ERR(incorrectArgCount, insStr, 2, parseLine.size());
		return -1;
	} else if (parseRegisters(lineCode[1]) == NotARegister) {
		if (*regToUse == stoi(lineCode[1])) {
			CHK = true;
		} else {
			CHK = false;
		}
	} else {
		switch (parseRegisters(lineCode[1])) {
			case A:
				CHK = (*regToUse == aReg);
				// Doing a bit of trickery here to return whether or not the two are the same
				break;
			case B:
				CHK = (*regToUse == bReg);
				break;
			case C:
				CHK = (*regToUse == cReg);
				break;
			case X:
				CHK = (*regToUse == xReg);
				break;
			case Y:
				CHK = (*regToUse == yReg);
				break;
			case Z:
				CHK = (*regToUse == zReg);
				break;
		}
	}
return 0;
}

/*
GT* Function
Usage: GT<Register> <Integer/Register>
See if a given Integer or Register is Greater Than the Register with the command, makes CHK flag 1 if it is, 0 otherwise
*/
int GT_Star (std::vector<std::string> parseLine, std::string insStr, int *regToUse) {
	if (parseLine.size() != 2) {
		ERR(incorrectArgCount, insStr, 2, parseLine.size());
		return -1;
	} else if (parseRegisters(lineCode[1]) == NotARegister) {
		if (*regToUse < stoi(lineCode[1])) {
			CHK = true;
		} else {
			CHK = false;
		}
	} else {
		switch (parseRegisters(lineCode[1])) {
			case A:
				CHK = (*regToUse < aReg);
				break;
			case B:
				CHK = (*regToUse < bReg);
				break;
			case C:
				CHK = (*regToUse < cReg);
				break;
			case X:
				CHK = (*regToUse < xReg);
				break;
			case Y:
				CHK = (*regToUse < yReg);
				break;
			case Z:
				CHK = (*regToUse < zReg);
				break;
		}
	}
	return 0;
}


/*
LT* Function
Usage: LT<Register> <Integer/Register>
See if a given Integer or Register is Less Than the Register with the command, makes CHK flag 1 if it is, 0 otherwise
*/
int LT_Star (std::vector<std::string> parseLine, std::string insStr, int *regToUse) {
	if (parseLine.size() != 2) {
		ERR(incorrectArgCount, insStr, 2, parseLine.size());
		return -1;
	} else if (parseRegisters(lineCode[1]) == NotARegister) {
		CHK = (*regToUse > stoi(lineCode[1]));
	} else {
		switch (parseRegisters(lineCode[1])) {
			case A:
				CHK = (*regToUse > aReg);
				break;
			case B:
				CHK = (*regToUse > bReg);
				break;
			case C:
				CHK = (*regToUse > cReg);
				break;
			case X:
				CHK = (*regToUse > xReg);
				break;
			case Y:
				CHK = (*regToUse > yReg);
				break;
			case Z:
				CHK = (*regToUse > zReg);
				break;
		}
	}
	return 0;
}


/*
ST* Function
Usage: ST<Register> <Memory Address>
STores the value of <Register> into the Memory Address shown.
*/
int ST_Star (std::vector<std::string> parseLine, std::string insStr, int *regToUse){
	if (parseLine.size() != 2) {
		ERR(incorrectArgCount, insStr, 2, parseLine.size());
		return -1;
	} else {
		// This little mess here is just telling it to turn the string that is returned by lineCode into an integer, which is what memroy uses
		memory[stoi(lineCode[1], nullptr, 0)] = *regToUse;
	}
	return 0;
}


/*
LD* Function
Usage: LD<Register> <Register/Integer/Memory>
LoaDs the value from <Register/Integer/Memory> into <Register>
*/
int LD_Star (std::vector<std::string> parseLine, std::string insStr, int *regToUse) {
	if (parseLine.size() != 2) {
		ERR(incorrectArgCount, insStr, 2, parseLine.size());
		return -1;
	} else if (parseRegisters(lineCode[1]) == NotARegister) {
		if (split(lineCode[1], "x").size() == 2){
			// Same hackery as above here
			*regToUse = memory[stoi(lineCode[1], nullptr, 0)];
		} else {
			*regToUse = stoi(lineCode[1], nullptr, 0);
		}
	} else {
		switch (parseRegisters(lineCode[1])) {
			case A:
				*regToUse = aReg;
				break;
			case B:
				*regToUse = bReg;
				break;
			case C:
				*regToUse = cReg;
				break;
			case X:
				*regToUse = xReg;
				break;
			case Y:
				*regToUse = yReg;
				break;
			case Z:
				*regToUse = zReg;
				break;
		}
	}
	
	return 0;
}



int main(int argc, char * argv[]) {
	// Make sure they actually specified a file
	if (argc == 1){
		// This is a kinda horrible way to check, but it at least makes sure we get an argument
		std::cout << "Please specify a file to read from\n";
		return -1;
	}
	
	if (split(argv[1], ".")[1] == "thl") {
		programFile.open(argv[1], std::ios::in);
	} else {
		std::cout << "Error, wrong file format given.\nPlease input a .thl file.\n";
		return -1;
	}
	if (argc >= 3) {
		// There are better ways to do this, but this way works for me and hasn't broken yet
		std::string argvStr = argv[2];
		if (argvStr.compare("-d") == 0){debugMode = true;}
	}
	// Preproccessing the file before executing
	// This just adds the lines from the file into a format we can read and jump without an issue
	while(getline(programFile, line)) {
		fileVector.push_back(line);
	}
	programFile.close(); // Closing the file here in order to save on memory, since people might have large files, this makes it a bit easier
	/*
	Do an initial pass-through of the file, in order to get all the labels
	That is ALL we are doing here, ignoring the rest of the lines
	*/
	for (int i=0;i<fileVector.size();i++){
		lineCode = split(fileVector[i], " ");
		// Ignoring all of the blank lines, since those are correct
		if (lineCode.size() == 0) {continue;}
		if (parseInstruction(lineCode[0]) == LBL) {
			/*
			LBL Command
			Usage: LBL <Label name>
			Creates a LaBeL with the name of <Label name>, allowing one to JMP, BNE, and BEQ to that label
			*/
			if (lineCode.size() != 2) {
				std::cout << "LBL got " << lineCode.size() - 1 << " arguments, and expected 1.\n";
				return -1;
			} else {
				// Labels is just a map of strings with integers as the value, allowing us to use custom names
				Labels[lineCode[1]] = i;
			}
			
		}
	}
	
	// Now we actually start the craziness, after nearly 600 lines
	for (int i=0;i<fileVector.size();i++){
		// Go through each file, splitting it
		lineCode = split(fileVector[i], " ");
		// Like above, if it is blank, we skip it
		if (lineCode.size() == 0) {continue;}
		switch (parseInstruction(lineCode[0])) {
			// Since we currently aren't too worried about performance, we can group them in an easy to read way.
			// Once performance is an issue, we will have to start grouping by most used... which will take research and be a pain
			
			// If the instruction is not in the list, we error since it doesn't know what to do here, and give the line number so they can fix it
			case InvalidIns:
				std::cout << lineCode[0] << " is not an instruction, exiting on line " << i+1 << ".\n";
				return -1;
			// Math Instructions
			case ADA:
				tempNum = AD_Star(lineCode, "ADA", &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADB:
				tempNum = AD_Star(lineCode, "ADB", &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADC:
				tempNum = AD_Star(lineCode, "ADC", &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADX:
				tempNum = AD_Star(lineCode, "ADX", &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADY:
				tempNum = AD_Star(lineCode, "ADY", &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADZ:
				tempNum = AD_Star(lineCode, "ADZ", &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBA:
				tempNum = SB_Star(lineCode, "SBA", &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBB:
				tempNum = SB_Star(lineCode, "SBB", &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBC:
				tempNum = SB_Star(lineCode, "SBC", &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBX:
				tempNum = SB_Star(lineCode, "SBX", &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBY:
				tempNum = SB_Star(lineCode, "SBY", &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBZ:
				tempNum = SB_Star(lineCode, "SBZ", &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case INA:
				tempNum = IN_Star(lineCode, "INA", &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case INB:
				tempNum = IN_Star(lineCode, "INB", &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case INC:
				tempNum = IN_Star(lineCode, "INC", &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case INX:
				tempNum = IN_Star(lineCode, "INX", &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case INY:
				tempNum = IN_Star(lineCode, "INY", &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case INZ:
				tempNum = IN_Star(lineCode, "INZ", &zReg);
				if (tempNum == -1) {return -1;}
				break;
			
			// Comparison Instructions
			case CPA:
				tempNum = CP_Star(lineCode, "CPA", &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPB:
				tempNum = CP_Star(lineCode, "CPB", &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPC:
				tempNum = CP_Star(lineCode, "CPC", &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPX:
				tempNum = CP_Star(lineCode, "CPX", &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPY:
				tempNum = CP_Star(lineCode, "CPY", &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPZ:
				tempNum = CP_Star(lineCode, "CPZ", &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTA:
				tempNum = GT_Star(lineCode, "GTA", &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTB:
				tempNum = GT_Star(lineCode, "GTB", &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTC:
				tempNum = GT_Star(lineCode, "GTC", &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTX:
				tempNum = GT_Star(lineCode, "GTX", &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTY:
				tempNum = GT_Star(lineCode, "GTA", &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTZ:
				tempNum = GT_Star(lineCode, "GTZ", &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTA:
				tempNum = LT_Star(lineCode, "LTA", &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTB:
				tempNum = LT_Star(lineCode, "LTB", &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTC:
				tempNum = LT_Star(lineCode, "LTC", &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTX:
				tempNum = LT_Star(lineCode, "LTX", &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTY:
				tempNum = LT_Star(lineCode, "LTY", &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTZ:
				tempNum = LT_Star(lineCode, "LTZ", &zReg);
				if (tempNum == -1) {return -1;}
				break;
			/*
			IFT Command
			Usage: IFT
			IF CHK is True, execute the next line, otherwise skip it
			*/
			case IFT:
				// This is hyper simple, literally just adding 1 to the counter that keeps track of where in the file we are.
				// This is a super hacky yet effective trick heh
				if (!CHK) {i++;continue;}
				break;
			
			// Memory Instructions
			case LDA:
				tempNum = LD_Star(lineCode, "LDA", &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDB:
				tempNum = LD_Star(lineCode, "LDB", &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDC:
				tempNum = LD_Star(lineCode, "LDC", &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDX:
				tempNum = LD_Star(lineCode, "LDX", &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDY:
				tempNum = LD_Star(lineCode, "LDY", &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDZ:
				tempNum = LD_Star(lineCode, "LDZ", &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case STA:
				tempNum = ST_Star(lineCode, "STA", &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case STB:
				tempNum = ST_Star(lineCode, "STB", &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case STC:
				tempNum = ST_Star(lineCode, "STC", &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case STX:
				tempNum = ST_Star(lineCode, "STX", &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case STY:
				tempNum = ST_Star(lineCode, "STY", &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case STZ:
				tempNum = ST_Star(lineCode, "STZ", &zReg);
				if (tempNum == -1) {return -1;}
				break;
				
			// Input/Output Instructions
			/*
			INP Function
			Usage: INP <Memory Address>
			Takes the INPut from user, and stores it into <Memory Address>
			Note: For some reason I'm not sure of, it will NOT error if the user inputs a character or string
			It will just store a 0 into that memory spot, which is... somehow useful, but I'm not sure how yet
			*/
			case INP:
				if (lineCode.size() != 2) {
					ERR(incorrectArgCount, "INP", 1, lineCode.size());
					return -1;
				} else {
					// Take the input, it will only allow integer numbers, so no decimals or strings...
					std::cout << currentInpString;
					std::cin >> tempNum;
					memory[stoi(lineCode[1], nullptr, 0)] = tempNum;
				}
				break;
				
			case OUT:
				if (lineCode.size() != 2) {
					ERR(incorrectArgCount, "OUT", 1, lineCode.size());
					return -1;
				} else {
					// Check first to see if the number being printed exists in PrintDirectiveValues
					for (auto& elem : PrintDirectiveValues) {
						if (elem.first == memory[stoi(lineCode[1], nullptr, 0)]) {
							foundPrintDirective = true;
							std::cout << elem.second << "\n";
							break;
						}
					} if (!foundPrintDirective) {
						std::cout << memory[stoi(lineCode[1], nullptr, 0)] << "\n";
					}
				}
				foundPrintDirective = false;
				break;
			
			// Control Flow Instructions
			case comment:
				continue;
			/*
			JMP Command
			Usage: JMP <Label Name>
			JuMPs unconditionally to the Label specified, must be a valid label
			*/
			case JMP:
				if (lineCode.size() != 2) {
					ERR(incorrectArgCount, "JMP", 1, lineCode.size());
					return -1;
				} else {
					i = (Labels[lineCode[1]]-1);
					// This is required in Branch instructions because the file will jump one more when it goes, meaning it will skip the line we state
				}
				break;
			/*
			BEQ Command
			Usage: BEQ <Label> 
			Branch to Label if CHK is EQual to 1, or True
			*/
			case BEQ:
				if (lineCode.size() != 2) {
					ERR(incorrectArgCount, "BEQ", 1, lineCode.size());
					return -1;
				} else {
					if (CHK) {i=Labels[lineCode[1]]-1;}
				}
				break;
			/*
			BNE Command
			Usage: BNE <Label>
			Branch to Label if CHK is Not Equal to 1
			*/
			case BNE:
				if (lineCode.size() != 2) {
					ERR(incorrectArgCount, "BNE", 1, lineCode.size());
					return -1;
				} else {
					if (!CHK) {i=Labels[lineCode[1]]-1;}
				}
				break;
		
			/*
				Pre-Processor Directives
				!print changes how print is used
			*/
			
			case PRINT:
				if (lineCode.size() < 3) {
					ERR(incorrectArgCount, "!print", 2, 1, "!print expects a number then a string, these were not given");
					return -1;
				} else {
					PrintDirectiveValues[stoi(lineCode[1], nullptr, 0)] = split(fileVector[i], "\"")[1];
				}
				break;
				
			case INPUT:
				if (lineCode.size() < 2) {
					ERR(incorrectArgCount, "!input", 2, 1);
					return -1;
				} else {
					currentInpString = split(fileVector[i], "\"")[1];
				}
				break;
			}
			
			
			
			if (debugMode) {
				std::cout << lineCode[0] << "\n";
				std::cout << "A: " << aReg << "\n";
				std::cout << "B: " << bReg << "\n";
				std::cout << "C: " << cReg << "\n";
				std::cout << "X: " << xReg << "\n";
				std::cout << "Y: " << yReg << "\n";
				std::cout << "Z: " << zReg << "\n";
				std::cout << "CHK: " << CHK << "\n";
				system("pause"); // This line somehow does exactly what I need with debug mode, I honestly don't know how it works
		}
		
	}
	
	/*
	Memory output
	*/
	std::cout << "Memory after running.\n\n";
	std::cout << "A: " << aReg << "\n";
	std::cout << "B: " << bReg << "\n";
	std::cout << "C: " << cReg << "\n";
	std::cout << "X: " << xReg << "\n";
	std::cout << "Y: " << yReg << "\n";
	std::cout << "Z: " << zReg << "\n";
	std::cout << "CHK: " << CHK << "\n";
	
	if (debugMode) {
		// Function stolen from https://stackoverflow.com/a/14070977
		for(const auto& elem : PrintDirectiveValues)
		{
			std::cout << elem.first << " " << elem.second << "\n";
		}
		std::cout << currentInpString << "\n";
		
	}
	return 0;
}