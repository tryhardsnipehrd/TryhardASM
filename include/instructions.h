#ifndef INSTRUCTION_HEADER
#define INSTRUCTION_HEADER

#include <string>

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
	BEQ,
	BNE,

	// Directives
	PRINT,
	INPUT,

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

instructions parseInstruction(std::string const& instruction);

registers parseRegisters(std::string const& letter);

#endif
