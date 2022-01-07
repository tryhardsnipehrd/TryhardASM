#include "include/instructions.h"
#include <string>


registers parseRegisters(std::string const& letter) {
	if (letter == "A") return A;
	if (letter == "B") return B;
	if (letter == "C") return C;
	if (letter == "X") return X;
	if (letter == "Y") return Y;
	if (letter == "Z") return Z;
	return NotARegister;
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
	if (instruction == "BEQ") return BEQ;
	if (instruction == "BNE") return BNE;

	// Pre-Processor Directives
	if (instruction == "!print") return PRINT;
	if (instruction == "!input") return INPUT;

	return InvalidIns;
};