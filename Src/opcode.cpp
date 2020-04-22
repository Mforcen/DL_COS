#include "opcode.h"

uint32_t simpleHashName(const char* name)
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

bool GetOpString(Opcode code, char* dest)
{
    bool out = true;
    switch (code)
    {
        case Opcode::LITERAL1:
            strcpy(dest, "LITERAL1");
            break;
		case Opcode::LITERAL4:
            strcpy(dest, "LITERAL4");
            break;
        case Opcode::LITERAL1_ARRAY:
            strcpy(dest, "LITERAL1_ARRAY");
            break;
        case Opcode::LITERAL4_ARRAY:
            strcpy(dest, "LITERAL4_ARRAY");
            break;
        case Opcode::LOAD1:
            strcpy(dest, "LOAD1");
            break;
        case Opcode::LOAD4:
            strcpy(dest, "LOAD4");
            break;
        case Opcode::LOAD1_ARRAY:
            strcpy(dest, "LOAD1_ARRAY");
            break;
        case Opcode::LOAD4_ARRAY:
            strcpy(dest, "LOAD4_ARRAY");
            break;
        case Opcode::STORE1:
            strcpy(dest, "STORE1");
            break;
        case Opcode::STORE4:
            strcpy(dest, "STORE4");
            break;
        case Opcode::STORE1_ARRAY:
            strcpy(dest, "STORE1_ARRAY");
            break;
        case Opcode::STORE4_ARRAY:
            strcpy(dest, "STORE4_ARRAY");
            break;
        case Opcode::LOAD1_LCL:
            strcpy(dest, "LOAD1_LCL");
            break;
        case Opcode::LOAD4_LCL:
            strcpy(dest, "LOAD4_LCL");
            break;
        case Opcode::LOAD1_ARRAY_LCL:
            strcpy(dest, "LOAD1_ARRAY_LCL");
            break;
        case Opcode::LOAD4_ARRAY_LCL:
            strcpy(dest, "LOAD4_ARRAY_LCL");
            break;
        case Opcode::STORE1_LCL:
            strcpy(dest, "STORE1_LCL");
            break;
        case Opcode::STORE4_LCL:
            strcpy(dest, "STORE4_LCL");
            break;
        case Opcode::STORE1_ARRAY_LCL:
            strcpy(dest, "STORE1_ARRAY_LCL");
            break;
        case Opcode::STORE4_ARRAY_LCL:
            strcpy(dest, "STORE4_ARRAY_LCL");
            break;
        case Opcode::LOAD1_ARG:
            strcpy(dest, "LOAD1_ARG");
            break;
        case Opcode::LOAD4_ARG:
            strcpy(dest, "LOAD4_ARG");
            break;
        case Opcode::LOAD1_ARRAY_ARG:
            strcpy(dest, "LOAD1_ARRAY_ARG");
            break;
        case Opcode::LOAD4_ARRAY_ARG:
            strcpy(dest, "LOAD4_ARRAY_ARG");
            break;
        case Opcode::STORE1_ARG:
            strcpy(dest, "STORE1_ARG");
            break;
        case Opcode::STORE4_ARG:
            strcpy(dest, "STORE4_ARG");
            break;
        case Opcode::STORE1_ARRAY_ARG:
            strcpy(dest, "STORE1_ARRAY_ARG");
            break;
        case Opcode::STORE4_ARRAY_ARG:
            strcpy(dest, "STORE4_ARRAY_ARG");
            break;
		case Opcode::POP1:
			strcpy(dest, "POP1");
			break;
		case Opcode::POP4:
			strcpy(dest, "POP4");
			break;
		case Opcode::CLONE1:
			strcpy(dest, "CLONE1");
			break;
		case Opcode::CLONE4:
			strcpy(dest, "CLONE4");
			break;
        case Opcode::ALLOC:
            strcpy(dest, "ALLOC");
            break;
        case Opcode::FREE:
            strcpy(dest, "FREE");
            break;
        case Opcode::ADD:
            strcpy(dest, "ADD");
            break;
        case Opcode::SUB:
            strcpy(dest, "SUB");
            break;
		case Opcode::MUL:
			strcpy(dest, "MUL");
			break;
		case Opcode::DIV:
			strcpy(dest, "DIV");
			break;
		case Opcode::FADD:
            strcpy(dest, "FADD");
            break;
        case Opcode::FSUB:
            strcpy(dest, "FSUB");
            break;
		case Opcode::FMUL:
			strcpy(dest, "FMUL");
			break;
		case Opcode::FDIV:
			strcpy(dest, "FDIV");
			break;
		case Opcode::MOD:
			strcpy(dest, "MOD");
			break;
		case Opcode::DEC_S:
			strcpy(dest, "DEC_S");
			break;
		case Opcode::INC_S:
			strcpy(dest, "INC_S");
        case Opcode::LESS:
            strcpy(dest, "LESS");
            break;
        case Opcode::GREATER:
            strcpy(dest, "GREATER");
            break;
        case Opcode::NOT:
            strcpy(dest, "NOT");
            break;
        case Opcode::EQUALS:
            strcpy(dest, "EQUALS");
            break;
		case Opcode::FLESS:
            strcpy(dest, "FLESS");
            break;
        case Opcode::FGREATER:
            strcpy(dest, "FGREATER");
            break;
		case Opcode::FNOT:
            strcpy(dest, "FNOT");
            break;
        case Opcode::FEQUALS:
            strcpy(dest, "FEQUALS");
            break;
        case Opcode::JMP:
            strcpy(dest, "JMP");
            break;
        case Opcode::JMP_IF:
            strcpy(dest, "JMP_IF");
            break;
		case Opcode::JMP_SZ:
			strcpy(dest, "JMP_SZ");
			break;
        case Opcode::CALL:
            strcpy(dest, "CALL");
            break;
        case Opcode::RETURN:
            strcpy(dest, "RETURN");
            break;
		case Opcode::NOP:
			strcpy(dest, "NOP");
			break;
        default:
            out = false;
            break;
    }
    return out;
};

Opcode GetStrOp(const char* name)
{
    OpcodeHashed hash = OpcodeHashed(simpleHashName(name));
    Opcode out;
    switch (hash)
    {
        case OpcodeHashed::LITERAL1:
            out = Opcode::LITERAL1;
            break;
        case OpcodeHashed::LITERAL4:
            out = Opcode::LITERAL4;
            break;
        case OpcodeHashed::LITERAL1_ARRAY:
            out = Opcode::LITERAL1_ARRAY;
            break;
        case OpcodeHashed::LITERAL4_ARRAY:
            out = Opcode::LITERAL4_ARRAY;
            break;

        case OpcodeHashed::LOAD1:
            out = Opcode::LOAD1;
            break;
        case OpcodeHashed::LOAD4:
            out = Opcode::LOAD4;
            break;
        case OpcodeHashed::LOAD1_ARRAY:
            out = Opcode::LOAD1_ARRAY;
            break;
        case OpcodeHashed::LOAD4_ARRAY:
            out = Opcode::LOAD4_ARRAY;
            break;
        case OpcodeHashed::STORE1:
            out = Opcode::STORE1;
            break;
        case OpcodeHashed::STORE4:
            out = Opcode::STORE4;
            break;
        case OpcodeHashed::STORE1_ARRAY:
            out = Opcode::STORE1_ARRAY;
            break;
        case OpcodeHashed::STORE4_ARRAY:
            out = Opcode::STORE4_ARRAY;
            break;

        case OpcodeHashed::LOAD1_LCL:
            out = Opcode::LOAD1_LCL;
            break;
        case OpcodeHashed::LOAD4_LCL:
            out = Opcode::LOAD4_LCL;
            break;
        case OpcodeHashed::LOAD1_ARRAY_LCL:
            out = Opcode::LOAD1_ARRAY_LCL;
            break;
        case OpcodeHashed::LOAD4_ARRAY_LCL:
            out = Opcode::LOAD4_ARRAY_LCL;
            break;
        case OpcodeHashed::STORE1_LCL:
            out = Opcode::STORE1_LCL;
            break;
        case OpcodeHashed::STORE4_LCL:
            out = Opcode::STORE4_LCL;
            break;
        case OpcodeHashed::STORE1_ARRAY_LCL:
            out = Opcode::STORE1_ARRAY_LCL;
            break;
        case OpcodeHashed::STORE4_ARRAY_LCL:
            out = Opcode::STORE4_ARRAY_LCL;
            break;
        case OpcodeHashed::LOAD1_ARG:
            out = Opcode::LOAD1_ARG;
            break;
        case OpcodeHashed::LOAD4_ARG:
            out = Opcode::LOAD4_ARG;
            break;
        case OpcodeHashed::LOAD1_ARRAY_ARG:
            out = Opcode::LOAD1_ARRAY_ARG;
            break;
        case OpcodeHashed::LOAD4_ARRAY_ARG:
            out = Opcode::LOAD4_ARRAY_ARG;
            break;
		case OpcodeHashed::STORE1_ARG:
			out = Opcode::STORE1_ARG;
			break;
		case OpcodeHashed::STORE4_ARG:
			out = Opcode::STORE4_ARG;
			break;
		case OpcodeHashed::STORE1_ARRAY_ARG:
			out = Opcode::STORE1_ARRAY_ARG;
			break;
		case OpcodeHashed::STORE4_ARRAY_ARG:
			out = Opcode::STORE4_ARRAY_ARG;
			break;

		case OpcodeHashed::POP1:
			out = Opcode::POP1;
			break;
		case OpcodeHashed::POP4:
			out = Opcode::POP4;
			break;
		case OpcodeHashed::CLONE1:
			out = Opcode::CLONE1;
			break;
		case OpcodeHashed::CLONE4:
			out = Opcode::CLONE4;
			break;

        case OpcodeHashed::ALLOC:
            out = Opcode::ALLOC;
            break;
        case OpcodeHashed::FREE:
            out = Opcode::FREE;
            break;

        case OpcodeHashed::ADD:
            out = Opcode::ADD;
            break;
        case OpcodeHashed::SUB:
            out = Opcode::SUB;
            break;
		case OpcodeHashed::MUL:
			out = Opcode::MUL;
			break;
		case OpcodeHashed::DIV:
			out = Opcode::DIV;
			break;
		case OpcodeHashed::MOD:
			out = Opcode::MOD;
			break;

		case OpcodeHashed::FADD:
            out = Opcode::FADD;
            break;
        case OpcodeHashed::FSUB:
            out = Opcode::FSUB;
            break;
		case OpcodeHashed::FMUL:
			out = Opcode::FMUL;
			break;
		case OpcodeHashed::FDIV:
			out = Opcode::FDIV;
			break;

		case OpcodeHashed::DEC_S:
			out = Opcode::DEC_S;
			break;
		case OpcodeHashed::INC_S:
			out = Opcode::INC_S;
			break;

        case OpcodeHashed::LESS:
            out = Opcode::LESS;
            break;
        case OpcodeHashed::GREATER:
            out = Opcode::GREATER;
            break;
        case OpcodeHashed::NOT:
            out = Opcode::NOT;
            break;
        case OpcodeHashed::EQUALS:
            out = Opcode::EQUALS;
            break;

        case OpcodeHashed::FLESS:
            out = Opcode::FLESS;
            break;
        case OpcodeHashed::FGREATER:
            out = Opcode::FGREATER;
            break;
        case OpcodeHashed::FNOT:
            out = Opcode::FNOT;
            break;
        case OpcodeHashed::FEQUALS:
            out = Opcode::FEQUALS;
            break;

		case OpcodeHashed::CHAR2INT:
			out = Opcode::CHAR2INT;
			break;
		case OpcodeHashed::INT2FLOAT:
			out = Opcode::INT2FLOAT;
			break;
		case OpcodeHashed::FLOAT2INT:
			out = Opcode::FLOAT2INT;
			break;
		case OpcodeHashed::INT2CHAR:
			out = Opcode::INT2CHAR;
			break;
        case OpcodeHashed::JMP:
            out = Opcode::JMP;
            break;
        case OpcodeHashed::JMP_IF:
            out = Opcode::JMP_IF;
            break;
        case OpcodeHashed::CALL:
            out = Opcode::CALL;
            break;
        case OpcodeHashed::RETURN:
            out = Opcode::RETURN;
            break;
		case OpcodeHashed::NOP:
			out = Opcode::NOP;
			break;
        default:
            out = Opcode::BAD;
            break;
    }
    return out;
};
