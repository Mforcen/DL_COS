#pragma once

#ifndef __OPCODE_CULEBRA__
#define __OPCODE_CULEBRA__

#include <cstring>
#include <inttypes.h>

constexpr uint32_t constHashName(const char* name)
{
	char c = name[0];
	uint32_t out = 0;
	unsigned int i = 1;
	while(c != '\0')
	{
		out += (unsigned int) c;
		out *= out;
		c = name[i++];
	}
	return (out);
}

uint32_t simpleHashName(const char* name);

enum class Opcode : char
{
	BAD = -1,
	NOP = 0x7f, // yield hara esto
	//Memory Manipulation
	LITERAL1 = 0,
	LITERAL4,
	LITERAL1_ARRAY,
	LITERAL4_ARRAY,

	LOAD1,
	LOAD4,
	LOAD1_ARRAY,
	LOAD4_ARRAY,

	STORE1,
	STORE4,
	STORE1_ARRAY,
	STORE4_ARRAY,

	LOAD1_LCL,
	LOAD4_LCL,
	LOAD1_ARRAY_LCL,
	LOAD4_ARRAY_LCL,

	STORE1_LCL,
	STORE4_LCL,
	STORE1_ARRAY_LCL,
	STORE4_ARRAY_LCL,

	LOAD1_ARG,
	LOAD4_ARG,
	LOAD1_ARRAY_ARG,
	LOAD4_ARRAY_ARG,

	STORE1_ARG,
	STORE4_ARG,
	STORE1_ARRAY_ARG,
	STORE4_ARRAY_ARG,

	POP1 = 28,
	POP4,

	CLONE1,
	CLONE4,

	ALLOC,
	FREE,

	//Arithmetic / Logic
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,

	FADD,
	FSUB,
	FMUL,
	FDIV,

	DEC_S,
	INC_S,

	LESS,
	GREATER,
	NOT,
	EQUALS,

	FLESS,
	FGREATER,
	FNOT,
	FEQUALS,

	//Casting ops
	CHAR2INT,
	INT2FLOAT,

	FLOAT2INT,
	INT2CHAR,

	//Bitwise ops
	BIT_AND,
	BIT_OR,
	BIT_LS,
	BIT_RS,

	//Flow Control
	JMP ,
	JMP_IF,
	JMP_SZ,

	CALL,
	RETURN
};

enum class OpcodeHashed : uint32_t
{
	BAD,
	NOP = constHashName("NOP"),
	//Memory Manipulation
	LITERAL1 = constHashName("LITERAL1"),
	LITERAL4 = constHashName("LITERAL4"),
	LITERAL1_ARRAY = constHashName("LITERAL1_ARRAY"),
	LITERAL4_ARRAY = constHashName("LITERAL4_ARRAY"),

	LOAD1 = constHashName("LOAD1"),
	LOAD4 = constHashName("LOAD4"),
	LOAD1_ARRAY = constHashName("LOAD1_ARRAY"),
	LOAD4_ARRAY = constHashName("LOAD4_ARRAY"),
	STORE1 = constHashName("STORE1"),
	STORE4 = constHashName("STORE4"),
	STORE1_ARRAY = constHashName("STORE1_ARRAY"),
	STORE4_ARRAY = constHashName("STORE4_ARRAY"),
	LOAD1_LCL = constHashName("LOAD1_LCL"),
	LOAD4_LCL = constHashName("LOAD4_LCL"),
	LOAD1_ARRAY_LCL = constHashName("LOAD1_ARRAY_LCL"),
	LOAD4_ARRAY_LCL = constHashName("LOAD4_ARRAY_LCL"),
	STORE1_LCL = constHashName("STORE1_LCL"),
	STORE4_LCL = constHashName("STORE4_LCL"),
	STORE1_ARRAY_LCL = constHashName("STORE1_ARRAY_LCL"),
	STORE4_ARRAY_LCL = constHashName("STORE4_ARRAY_LCL"),
	LOAD1_ARG = constHashName("LOAD1_ARG"),
	LOAD4_ARG = constHashName("LOAD4_ARG"),
	LOAD1_ARRAY_ARG = constHashName("LOAD1_ARRAY_ARG"),
	LOAD4_ARRAY_ARG = constHashName("LOAD4_ARRAY_ARG"),
	STORE1_ARG = constHashName("STORE1_ARG"),
	STORE4_ARG = constHashName("STORE4_ARG"),
	STORE1_ARRAY_ARG = constHashName("STORE1_ARRAY_ARG"),
	STORE4_ARRAY_ARG = constHashName("STORE4_ARRAY_ARG"),

	POP1 = constHashName("POP1"),
	POP4 = constHashName("POP4"),

	CLONE1 = constHashName("CLONE1"),
	CLONE4 = constHashName("CLONE4"),

	ALLOC = constHashName("ALLOC"),
	FREE = constHashName("FREE"),

	//Arithmetic / Logic
	ADD = constHashName("ADD"),
	SUB = constHashName("SUB"),
	MUL = constHashName("MUL"),
	DIV = constHashName("DIV"),
	MOD = constHashName("MOD"),

	FADD = constHashName("FADD"),
	FSUB = constHashName("FSUB"),
	FMUL = constHashName("FMUL"),
	FDIV = constHashName("FDIV"),

	DEC_S = constHashName("DEC_S"),
	INC_S = constHashName("INC_S"),


	LESS = constHashName("LESS"),
	GREATER = constHashName("GREATER"),
	NOT = constHashName("NOT"),
	EQUALS = constHashName("EQUALS"),

	FLESS = constHashName("FLESS"),
	FGREATER = constHashName("FGREATER"),
	FNOT = constHashName("FNOT"),
	FEQUALS = constHashName("FEQUALS"),

	CHAR2INT = constHashName("CHAR2INT"),
	INT2FLOAT = constHashName("INT2FLOAT"),

	FLOAT2INT = constHashName("FLOAT2INT"),
	INT2CHAR = constHashName("INT2CHAR"),

	BIT_AND = constHashName("BIT_AND"),
	BIT_OR = constHashName("BIT_OR"),
	BIT_LS = constHashName("BIT_LS"),
	BIT_RS = constHashName("BIT_RS"),

	//Flow Control
	JMP = constHashName("JMP"),
	JMP_IF = constHashName("JMP_IF"),
	JMPZ = constHashName("JMPZ"),

	CALL = constHashName("CALL"),
	RETURN = constHashName("RETURN"),

	CALL_BUILTIN = constHashName("CALL_BUILTIN"),

	PRINT = constHashName("PRINT"),
	PRINT_INT = constHashName("PRINT_INT")
};

bool GetOpString(Opcode code, char* dest);
Opcode GetStrOp(const char* name);

#endif
