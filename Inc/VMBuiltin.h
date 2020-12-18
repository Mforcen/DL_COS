#ifndef VMBUILTIN_H_INCLUDED
#define VMBUILTIN_H_INCLUDED

#include <cstdint>
#include <cmath>
#include "printf.h"
#include "io.h"

#include "stm32f1xx_hal.h"
#include "pin_defs.h"

union Arg
{
	float fval;
	int32_t ival;
	uint32_t uival;
	char buf[4];
	intptr_t ptr_val;
	char* str;
	uint8_t* ustr;
};

enum TypeVal : uint8_t
{
	UNKNOWN = 0,
	CHAR,
	INT,
	UINT,
	FLOAT,
	PTR,
	VOID
};

struct Value
{
	TypeVal typeVal;
	union
	{
		float fval;
		int32_t ival;
		uint32_t uival;
		char buf[4];
		uintptr_t ptr_val;
	} numdata;
};

struct BuiltinFunc
{
    uint8_t nArgs;
    TypeVal argTypes[8];
    TypeVal retValue;
    intptr_t func_ptr;
};

// TODO (forcen#1#): Define the remaining builtin functions for accessing all the hardware interfaces
// TODO (forcen#1#12/03/20): Add some functions like: ...
//-Get battery voltage

int32_t _sin(float* rv, Arg rad);
int32_t _cos(float* rv, Arg rad);
int32_t _tan(float* rv, Arg rad);
int32_t _asin(float* rv, Arg rad);
int32_t _acos(float* rv, Arg rad);
int32_t _atan(float* rv, Arg rad);
int32_t _sinh(float* rv, Arg rad);
int32_t _cosh(float* rv, Arg rad);
int32_t _tanh(float* rv, Arg rad);
int32_t _asinh(float* rv, Arg rad);
int32_t _acosh(float* rv, Arg rad);
int32_t _atanh(float* rv, Arg rad);

int32_t _sqrt(float* rv, Arg val);
int32_t _exp(float* rv, Arg val);

int32_t _print(void* rv, Arg fmt);

int32_t _getAdc(uint32_t* rv, Arg chan);

int32_t _SDI12_ReadSingleMeasurement(void* rv, Arg addr, Arg dst, Arg count, Arg additional);
int32_t _digitalRead(int* rv, Arg ch);
int32_t _digitalWrite(void* rv, Arg ch, Arg value);
int32_t _pulseRead(int* rv, Arg ch);


extern BuiltinFunc builtinFuncs[31];
void init_builtinFuncs();


#endif // VMBUILTIN_H_INCLUDED
