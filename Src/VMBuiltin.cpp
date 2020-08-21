
#include "VMBuiltin.h"

uint32_t _print(void* rv, char* fmt)
{
	printf_(fmt);
	printf_("\n");
    return 0;
}

uint32_t _sin(float* rv, float rad)
{
	*rv = sin(rad);
	return 0;
}

uint32_t _cos(float* rv, float rad)
{
	*rv = cos(rad);
	return 1;
}

uint32_t _tan(float* rv, float rad)
{
	*rv = tan(rad);
	return 2;
}

uint32_t _asin(float* rv, float rad)
{
	*rv = asin(rad);
	return 0;
}

uint32_t _acos(float* rv, float rad)
{
	*rv = acos(rad);
	return 0;
}

uint32_t _atan(float* rv, float rad)
{
	*rv = atan(rad);
	return 0;
}

uint32_t _sinh(float* rv, float rad)
{
	*rv = sinh(rad);
	return 0;
}

uint32_t _cosh(float* rv, float rad)
{
	*rv = cosh(rad);
	return 0;
}

uint32_t _tanh(float* rv, float rad)
{
	*rv = tanh(rad);
	return 0;
}

uint32_t _asinh(float* rv, float rad)
{
	*rv = asinh(rad);
	return 0;
}

uint32_t _acosh(float* rv, float rad)
{
	*rv = acosh(rad);
	return 0;
}

uint32_t _atanh(float* rv, float rad)
{
	*rv = atanh(rad);
	return 0;
}

uint32_t _sqrt(float* rv, float val)
{
	*rv = sqrt(val);
	return 0;
}

uint32_t _exp(float* rv, float val)
{
	*rv = exp(val);
	return 0;
}

uint32_t _getAdc(uint32_t* rv, int port, int chan)
{
	*rv = getADC(port, chan);
	return 0;
}

uint32_t _setPin(void* rv, int pin, int level)
{
	GPIO_PinState pinlevel;
	if(level)
		pinlevel = GPIO_PIN_SET;
	else
		pinlevel = GPIO_PIN_RESET;
    switch(pin)
    {
	case 0:
		HAL_GPIO_WritePin(IO0, pinlevel);
		break;
	case 1:
		HAL_GPIO_WritePin(IO1, pinlevel);
		break;
	case 2:
		HAL_GPIO_WritePin(IO2, pinlevel);
		break;
	case 3:
		HAL_GPIO_WritePin(IO3, pinlevel);
		break;
	case 4:
		HAL_GPIO_WritePin(IO4, pinlevel);
		break;
	case 5:
		HAL_GPIO_WritePin(IO5, pinlevel);
		break;
    }
    return 0;
}

uint32_t _SDI12_ReadSingleMeasurement(void* rv, int addr, float* dst, int count)
{
	return SDI12_SingleMeasurementRead(addr, dst, count);
}

BuiltinFunc builtinFuncs[18];

void init_builtinFuncs()
{
	builtinFuncs[0].nArgs = 1;
	builtinFuncs[0].argTypes[0] = FLOAT;
	builtinFuncs[0].retValue = FLOAT;
	builtinFuncs[0].func_ptr = (intptr_t)_sin;

	builtinFuncs[1].nArgs = 1;
	builtinFuncs[1].argTypes[0] = FLOAT;
	builtinFuncs[1].retValue = FLOAT;
	builtinFuncs[1].func_ptr = (intptr_t)_cos;

	builtinFuncs[2].nArgs = 1;
	builtinFuncs[2].argTypes[0] = FLOAT;
	builtinFuncs[2].retValue = FLOAT;
	builtinFuncs[2].func_ptr = (intptr_t)_tan;

	builtinFuncs[3].nArgs = 1;
	builtinFuncs[3].argTypes[0] = FLOAT;
	builtinFuncs[3].retValue = FLOAT;
	builtinFuncs[3].func_ptr = (intptr_t)_asin;

	builtinFuncs[4].nArgs = 1;
	builtinFuncs[4].argTypes[0] = FLOAT;
	builtinFuncs[4].retValue = FLOAT;
	builtinFuncs[4].func_ptr = (intptr_t)_acos;

	builtinFuncs[5].nArgs = 1;
	builtinFuncs[5].argTypes[0] = FLOAT;
	builtinFuncs[5].retValue = FLOAT;
	builtinFuncs[5].func_ptr = (intptr_t)_atan;

	builtinFuncs[6].nArgs = 1;
	builtinFuncs[6].argTypes[0] = FLOAT;
	builtinFuncs[6].retValue = FLOAT;
	builtinFuncs[6].func_ptr = (intptr_t)_sinh;

	builtinFuncs[7].nArgs = 1;
	builtinFuncs[7].argTypes[0] = FLOAT;
	builtinFuncs[7].retValue = FLOAT;
	builtinFuncs[7].func_ptr = (intptr_t)_cosh;

	builtinFuncs[8].nArgs = 1;
	builtinFuncs[8].argTypes[0] = FLOAT;
	builtinFuncs[8].retValue = FLOAT;
	builtinFuncs[8].func_ptr = (intptr_t)_tanh;

	builtinFuncs[9].nArgs = 1;
	builtinFuncs[9].argTypes[0] = FLOAT;
	builtinFuncs[9].retValue = FLOAT;
	builtinFuncs[9].func_ptr = (intptr_t)_asinh;

	builtinFuncs[10].nArgs = 1;
	builtinFuncs[10].argTypes[0] = FLOAT;
	builtinFuncs[10].retValue = FLOAT;
	builtinFuncs[10].func_ptr = (intptr_t)_acosh;

	builtinFuncs[11].nArgs = 1;
	builtinFuncs[11].argTypes[0] = FLOAT;
	builtinFuncs[11].retValue = FLOAT;
	builtinFuncs[11].func_ptr = (intptr_t)_atanh;

	builtinFuncs[12].nArgs = 1;
	builtinFuncs[12].argTypes[0] = FLOAT;
	builtinFuncs[12].retValue = FLOAT;
	builtinFuncs[12].func_ptr = (intptr_t)_sqrt;

	builtinFuncs[13].nArgs = 1;
	builtinFuncs[13].argTypes[0] = FLOAT;
	builtinFuncs[13].retValue = FLOAT;
	builtinFuncs[13].func_ptr = (intptr_t)_exp;

	builtinFuncs[14].nArgs = 1;
	builtinFuncs[14].retValue = VOID;
	builtinFuncs[14].argTypes[0] = ARR;
	builtinFuncs[14].func_ptr = (intptr_t)_print;

	builtinFuncs[15].nArgs = 2;
	builtinFuncs[15].retValue = INT;
	builtinFuncs[15].argTypes[0] = INT;
	builtinFuncs[15].argTypes[1] = INT;
	builtinFuncs[15].func_ptr = (intptr_t)_getAdc;

	builtinFuncs[16].nArgs = 2;
	builtinFuncs[16].retValue = VOID;
	builtinFuncs[16].argTypes[0] = INT;
	builtinFuncs[16].argTypes[1] = INT;
	builtinFuncs[16].func_ptr = (intptr_t)_setPin;

	builtinFuncs[17].nArgs = 3;
	builtinFuncs[17].retValue = VOID;
	builtinFuncs[17].argTypes[0] = INT;
	builtinFuncs[17].argTypes[1] = ARR;
	builtinFuncs[17].argTypes[2] = INT;
	builtinFuncs[17].func_ptr = (intptr_t)_SDI12_ReadSingleMeasurement;
}


