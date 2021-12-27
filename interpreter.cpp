/*
~~~~OVERVIEW~~~~
No Strings, chars, etc
only integers, so negative is allowed


~~~~PLANS~~~~
Simple instructions, similar to 6502 ASM

6 registers:
X 
Y
Z 
A
B
C

Memory/RAM --- 	a variable amount of space for variables, defined during runtime, before code is ran
Defaults to 1K slots as of now, will be variable later

Technically a 7th register, really just a flag
CHK --- Holds the value of the Comparison instructions, allows one to use it if they really want


Math Instructions
AD* -- ADd value to *, can be register or int
SB* -- SuBtract value from *, can be register or int
IN* -- INcrement * by 1

Comparison Instructions
CP* -- ComPare to *, must be a register. Value comparing can be an int or a register
GT* -- Greater Than *, must be a register. Value comparing can be an int or a register
LT* -- Less than *, must be a register. Value comparing can be an int or a register
IFT -- IF the comparison instruction last used was True, run the next line. Otherwise skip it

Memory Instructions
ST* -- STore * into memory address given
LD* -- LoaD value from different areas into *

Input/Output Instructions
INP -- take user INPut and store it into given memory spot
OUT -- Outputs the value given to terminal

Control Flow Instructions
# says it is a comment, don't run anything on this line
LBL -- Create a LaBeL to JMP to
JMP -- JuMP to the noted Label
JSR -- Jump to SubRoutine. Go to the Label, returns at RSR. MUST HAVE A RSR TO RETURN FROM
RSR -- When to return from the JSR, all JSR Labels must have one
BEQ -- Branch if EQual to. Branches to the label if the CHK flag is set
BNE -- Branch if Not Equal to. Branches to the label if the CHK flag is NOT set

*/

// Includes here
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>

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


// Define non-user variables
std::fstream programFile;
std::string line;
std::vector<std::string> lineCode;
std::vector<std::string> fileVector;
int tempNum;

char n;

bool debugMode = false;

// The actual instructions that the user can use
enum instructions {
	
	// Math Functions
	ADA,
	ADB,
	ADC,
	ADX,
	ADY,
	ADZ,
	SBA,
	SBB,
	SBC,
	SBX,
	SBY,
	SBZ,
	INA,
	INB,
	INC,
	INX,
	INY,
	INZ,
	
	// Comparison Instructions
	CPA,
	CPB,
	CPC,
	CPX,
	CPY,
	CPZ,
	GTA,
	GTB,
	GTC,
	GTX,
	GTY,
	GTZ,
	LTA,
	LTB,
	LTC,
	LTX,
	LTY,
	LTZ,
	IFT,
	
	// Memory Instructions
	STA,
	STB,
	STC,
	STX,
	STY,
	STZ,
	LDA,
	LDB,
	LDC,
	LDX,
	LDY,
	LDZ,
	
	// I/O Instructions
	INP,
	OUT,
	
	// Control Flow Instructions
	comment,
	LBL,
	JMP,
	JSR,
	RSR,
	BEQ,
	BNE,
	InvalidIns
};

enum registers {
	A,
	B,
	C,
	X,
	Y,
	Z,
	NotARegister
};

instructions parseInstruction(std::string const& instruction) {
	
	// Math Instructions
	if (instruction == "ADA") return ADA;
	if (instruction == "ADB") return ADB;
	if (instruction == "ADC") return ADC;
	if (instruction == "ADX") return ADX;
	if (instruction == "ADY") return ADY;
	if (instruction == "ADZ") return ADZ;
	if (instruction == "SBA") return SBA;
	if (instruction == "SBB") return SBB;
	if (instruction == "SBC") return SBC;
	if (instruction == "SBX") return SBX;
	if (instruction == "SBY") return SBY;
	if (instruction == "SBZ") return SBZ;
	if (instruction == "INA") return INA;
	if (instruction == "INB") return INB;
	if (instruction == "INC") return INC;
	if (instruction == "INX") return INX;
	if (instruction == "INY") return INY;
	if (instruction == "INZ") return INZ;
	
	// Comparison Instructions
	if (instruction == "CPA") return CPA;
	if (instruction == "CPB") return CPB;
	if (instruction == "CPC") return CPC;
	if (instruction == "CPX") return CPX;
	if (instruction == "CPY") return CPY;
	if (instruction == "CPZ") return CPZ;
	if (instruction == "GTA") return GTA;
	if (instruction == "GTB") return GTB;
	if (instruction == "GTC") return GTC;
	if (instruction == "GTX") return GTX;
	if (instruction == "GTY") return GTY;
	if (instruction == "GTZ") return GTZ;
	if (instruction == "LTA") return LTA;
	if (instruction == "LTB") return LTB;
	if (instruction == "LTC") return LTC;
	if (instruction == "LTX") return LTX;
	if (instruction == "LTY") return LTY;
	if (instruction == "LTZ") return LTZ;
	if (instruction == "IFT") return IFT;
	
	// Memory Instructions
	if (instruction == "STA") return STA;
	if (instruction == "STB") return STB;
	if (instruction == "STC") return STC;
	if (instruction == "STX") return STX;
	if (instruction == "STY") return STY;
	if (instruction == "STZ") return STZ;
	if (instruction == "LDA") return LDA;
	if (instruction == "LDB") return LDB;
	if (instruction == "LDC") return LDC;
	if (instruction == "LDX") return LDX;
	if (instruction == "LDY") return LDY;
	if (instruction == "LDZ") return LDZ;
	
	// Input/Output Instructions
	if (instruction == "INP") return INP;
	if (instruction == "OUT") return OUT;
	
	// Control Flow Instructions
	if (instruction == "#") return comment;
	if (instruction == "LBL") return LBL;
	if (instruction == "JMP") return JMP;
	if (instruction == "JSR") return JSR;
	if (instruction == "RSR") return RSR;
	if (instruction == "BEQ") return BEQ;
	if (instruction == "BNE") return BNE;
	return InvalidIns;
};

registers parseRegisters(std::string const& letter) {
	if (letter == "A") return A;
	if (letter == "B") return B;
	if (letter == "C") return C;
	if (letter == "X") return X;
	if (letter == "Y") return Y;
	if (letter == "Z") return Z;
	return NotARegister;
};

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
AD* Function
Usage: AD<Register> <Integer/Register>
ADds the Integer or value of the Register given to the current Register, negative numbers are allowed
*/
int AD_Star (std::vector<std::string> parseLine, std::string insStr, int expArgs, int *regToUse) {
	if (parseLine.size() != 2){
					std::cout << insStr << " got " << parseLine.size() - 1 << " arguments, and expected " << expArgs << ".\n";
					return -1;
				} else if (parseRegisters(parseLine[1]) == NotARegister) {
					tempNum = std::stoi(parseLine[1]);
					*regToUse += tempNum;
				} else {
					switch (parseRegisters(parseLine[1])) {
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
Usage
SB<Register> <Integer/Register>
SuBtracts the Integer or value of the Register given from the current Register, negative numbers can be the result
*/
int SB_Star (std::vector<std::string> parseLine, std::string insStr, int expArgs, int *regToUse) {
	if (parseLine.size() != 2){
					std::cout << insStr << " got " << parseLine.size() - 1 << " arguments, and expected " << expArgs << ".\n";
					return -1;
				} else if (parseRegisters(parseLine[1]) == NotARegister) {
					tempNum = std::stoi(parseLine[1]);
					*regToUse -= tempNum;
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
INcrements a Register by 1
*/
int IN_Star (std::vector<std::string> parseLine, std::string insStr, int expArgs, int *regToUse) {
	if (parseLine.size() != 1){
					std::cout << insStr << " got " << parseLine.size() - 1 << " arguments, and expected " << expArgs << ".\n";
					return -1;
				} else {*regToUse += 1;}
				
				return 0;
}


/*
CP* Function
Usage CP<Register> <Integer/Register>
ComPares an Integer or value from a Register to the Register specified, makes CHK flag 1 if it is equal, 0 otherwise
*/
int CP_Star (std::vector<std::string> parseLine, std::string insStr, int expArgs, int *regToUse) {
	if (parseLine.size() != 2) {
		std::cout << insStr << " got " << parseLine.size() - 1 << " arguments, and expected " << expArgs << ".\n";
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
int GT_Star (std::vector<std::string> parseLine, std::string insStr, int expArgs, int *regToUse) {
	if (parseLine.size() != 2) {
		std::cout << insStr << " got " << parseLine.size() - 1 << " arguments, and expected " << expArgs << ".\n";
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
int LT_Star (std::vector<std::string> parseLine, std::string insStr, int expArgs, int *regToUse) {
	if (parseLine.size() != 2) {
		std::cout << insStr << " got " << parseLine.size() - 1 << " arguments, and expected " << expArgs << ".\n";
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
STores the value of <Register> into the Memory Address shown. Using the same register twice is allowed, although not different than a nop
*/
int ST_Star (std::vector<std::string> parseLine, std::string insStr, int expArgs, int *regToUse){
	if (parseLine.size() != 2) {
		std::cout << insStr << " got " << parseLine.size() - 1 << " arguments, and expected " << expArgs << ".\n";
		return -1;
	} else {
		memory[stoi(lineCode[1], nullptr, 0)] = *regToUse;
	}
	return 0;
}

/*
LD* Function
Usage: LD<Register> <Register/Integer/Memory>
LoaDs the value from <Register/Integer/Memory> into <Register>
*/
int LD_Star (std::vector<std::string> parseLine, std::string insStr, int expArgs, int *regToUse) {
	if (parseLine.size() != 2) {
		std::cout << insStr << " got " << parseLine.size() - 1 << " arguments, and expected " << expArgs << ".\n";
		return -1;
	} else if (parseRegisters(lineCode[1]) == NotARegister) {
		if (split(lineCode[1], "x").size() == 2){
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
		std::string argvStr = argv[2];
		if (argvStr.compare("-d") == 0){debugMode = true;}
	}
	// Preproccessing the file before executing
	while(getline(programFile, line)) {
		fileVector.push_back(line);
	}
	/*
	Do an initial pass-through of the file, in order to get all the labels
	That is ALL we are doing here, ignoring the rest of them
	*/
	for (int i=0;i<fileVector.size();i++){
		lineCode = split(fileVector[i], " ");
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
				Labels[lineCode[1]] = i;
			}
			
		}
	}
	
	
	for (int i=0;i<fileVector.size();i++){
		lineCode = split(fileVector[i], " ");
		if (lineCode.size() == 0) {continue;}
		switch (parseInstruction(lineCode[0])) {
			case InvalidIns:
				std::cout << lineCode[0] << " is not an instruction, exiting.\n";
				return -1;
			// Math Instructions
			case ADA:
				tempNum = AD_Star(lineCode, "ADA", 1, &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADB:
				tempNum = AD_Star(lineCode, "ADB", 1, &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADC:
				tempNum = AD_Star(lineCode, "ADC", 1, &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADX:
				tempNum = AD_Star(lineCode, "ADX", 1, &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADY:
				tempNum = AD_Star(lineCode, "ADY", 1, &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case ADZ:
				tempNum = AD_Star(lineCode, "ADZ", 1, &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBA:
				tempNum = SB_Star(lineCode, "SBA", 1, &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBB:
				tempNum = SB_Star(lineCode, "SBB", 1, &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBC:
				tempNum = SB_Star(lineCode, "SBC", 1, &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBX:
				tempNum = SB_Star(lineCode, "SBX", 1, &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBY:
				tempNum = SB_Star(lineCode, "SBY", 1, &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case SBZ:
				tempNum = SB_Star(lineCode, "SBZ", 1, &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case INA:
				tempNum = IN_Star(lineCode, "INA", 0, &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case INB:
				tempNum = IN_Star(lineCode, "INB", 0, &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case INC:
				tempNum = IN_Star(lineCode, "INC", 0, &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case INX:
				tempNum = IN_Star(lineCode, "INX", 0, &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case INY:
				tempNum = IN_Star(lineCode, "INY", 0, &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case INZ:
				tempNum = IN_Star(lineCode, "INZ", 0, &zReg);
				if (tempNum == -1) {return -1;}
				break;
			
			// Comparison Instructions
			case CPA:
				tempNum = CP_Star(lineCode, "CPA", 1, &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPB:
				tempNum = CP_Star(lineCode, "CPB", 1, &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPC:
				tempNum = CP_Star(lineCode, "CPC", 1, &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPX:
				tempNum = CP_Star(lineCode, "CPX", 1, &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPY:
				tempNum = CP_Star(lineCode, "CPY", 1, &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case CPZ:
				tempNum = CP_Star(lineCode, "CPZ", 1, &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTA:
				tempNum = GT_Star(lineCode, "GTA", 1, &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTB:
				tempNum = GT_Star(lineCode, "GTB", 1, &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTC:
				tempNum = GT_Star(lineCode, "GTC", 1, &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTX:
				tempNum = GT_Star(lineCode, "GTX", 1, &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTY:
				tempNum = GT_Star(lineCode, "GTAY", 1, &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case GTZ:
				tempNum = GT_Star(lineCode, "GTZ", 1, &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTA:
				tempNum = LT_Star(lineCode, "LTA", 1, &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTB:
				tempNum = LT_Star(lineCode, "LTB", 1, &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTC:
				tempNum = LT_Star(lineCode, "LTC", 1, &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTX:
				tempNum = LT_Star(lineCode, "LTX", 1, &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTY:
				tempNum = LT_Star(lineCode, "LTY", 1, &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case LTZ:
				tempNum = LT_Star(lineCode, "LTZ", 1, &zReg);
				if (tempNum == -1) {return -1;}
				break;
			/*
			IFT Command
			Usage: IFT
			IF CHK is True, execute the next line, otherwise skip it
			*/
			case IFT:
				if (!CHK) {i++;continue;}
				break;
			
			// Memory Instructions
			case LDA:
				tempNum = LD_Star(lineCode, "LDA", 1, &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDB:
				tempNum = LD_Star(lineCode, "LDB", 1, &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDC:
				tempNum = LD_Star(lineCode, "LDC", 1, &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDX:
				tempNum = LD_Star(lineCode, "LDX", 1, &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDY:
				tempNum = LD_Star(lineCode, "LDY", 1, &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case LDZ:
				tempNum = LD_Star(lineCode, "LDZ", 1, &zReg);
				if (tempNum == -1) {return -1;}
				break;
			case STA:
				tempNum = ST_Star(lineCode, "STA", 1, &aReg);
				if (tempNum == -1) {return -1;}
				break;
			case STB:
				tempNum = ST_Star(lineCode, "STB", 1, &bReg);
				if (tempNum == -1) {return -1;}
				break;
			case STC:
				tempNum = ST_Star(lineCode, "STC", 1, &cReg);
				if (tempNum == -1) {return -1;}
				break;
			case STX:
				tempNum = ST_Star(lineCode, "STX", 1, &xReg);
				if (tempNum == -1) {return -1;}
				break;
			case STY:
				tempNum = ST_Star(lineCode, "STY", 1, &yReg);
				if (tempNum == -1) {return -1;}
				break;
			case STZ:
				tempNum = ST_Star(lineCode, "STZ", 1, &zReg);
				if (tempNum == -1) {return -1;}
				break;
				
			// Input/Output Instructions
			case INP:
				
				break;
			case OUT:
				
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
					std::cout << "JMP got " << lineCode.size() - 1 << " arguments, and expected 1.\n";
					return -1;
				} else {
					i = (Labels[lineCode[1]]-1);
				}
				break;
			case JSR:
				break;
			case RSR:
				break;
			/*
			BEQ Command
			Usage: BEQ <Label> 
			Branch to Label if CHK is EQual to 1
			*/
			case BEQ:
				if (lineCode.size() != 2) {
					std::cout << "BEQ got " << lineCode.size() - 1 << " arguments, and expected 1.\n";
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
					std::cout << "BNE got " << lineCode.size() - 1 << " arguments, and expected 1.\n";
					return -1;
				} else {
					if (!CHK) {i=Labels[lineCode[1]]-1;}
				}
				break;
		
			}
			std::cout << lineCode[0] << "\n";
			if (debugMode) {
				std::cout << "A: " << aReg << "\n";
				std::cout << "B: " << bReg << "\n";
				std::cout << "C: " << cReg << "\n";
				std::cout << "X: " << xReg << "\n";
				std::cout << "Y: " << yReg << "\n";
				std::cout << "Z: " << zReg << "\n";
				std::cout << "CHK: " << CHK << "\n";
				system("pause");
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
	
	programFile.close();
	return 0;
}