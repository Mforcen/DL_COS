#ifndef VMBUILTIN_H_INCLUDED
#define VMBUILTIN_H_INCLUDED

#include <cstdint>
#include <cmath>
#include "printf.h"
#include "io.h"

#include "stm32f1xx_hal.h"
#include "pin_defs.h"

enum TypeVal : uint8_t
{
	CHAR,
	INT,
	UINT,
	FLOAT,
	ARR,
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

uint32_t _sin(float* rv, float rad);
uint32_t _cos(float* rv, float rad);
uint32_t _tan(float* rv, float rad);
uint32_t _asin(float* rv, float rad);
uint32_t _acos(float* rv, float rad);
uint32_t _atan(float* rv, float rad);
uint32_t _sinh(float* rv, float rad);
uint32_t _cosh(float* rv, float rad);
uint32_t _tanh(float* rv, float rad);
uint32_t _asinh(float* rv, float rad);
uint32_t _acosh(float* rv, float rad);
uint32_t _atanh(float* rv, float rad);

uint32_t _sqrt(float* rv, float val);
uint32_t _exp(float* rv, float val);

uint32_t _print(void* rv, char* fmt);

uint32_t _getAdc(uint32_t* rv, int port, int chan);

uint32_t _setPin(void* rv, int pin, int level);


extern BuiltinFunc builtinFuncs[17];
void init_builtinFuncs();


#endif // VMBUILTIN_H_INCLUDED
