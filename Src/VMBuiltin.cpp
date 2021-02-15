
#include "VMBuiltin.h"

int32_t _print(void* rv, Arg fmt)
{
	printf_(fmt.str);
	printf_("\n");
    return 0;
}

int32_t _sin(float* rv, Arg rad)
{
	*rv = sin(rad.fval);
	return 0;
}

int32_t _cos(float* rv, Arg rad)
{
	*rv = cos(rad.fval);
	return 0;
}

int32_t _tan(float* rv, Arg rad)
{
	*rv = tan(rad.fval);
	return 0;
}

int32_t _asin(float* rv, Arg rad)
{
	*rv = asin(rad.fval);
	return 0;
}

int32_t _acos(float* rv, Arg rad)
{
	*rv = acos(rad.fval);
	return 0;
}

int32_t _atan(float* rv, Arg rad)
{
	*rv = atan(rad.fval);
	return 0;
}

int32_t _sinh(float* rv, Arg rad)
{
	*rv = sinh(rad.fval);
	return 0;
}

int32_t _cosh(float* rv, Arg rad)
{
	*rv = cosh(rad.fval);
	return 0;
}

int32_t _tanh(float* rv, Arg rad)
{
	*rv = tanh(rad.fval);
	return 0;
}

int32_t _asinh(float* rv, Arg rad)
{
	*rv = asinh(rad.fval);
	return 0;
}

int32_t _acosh(float* rv, Arg rad)
{
	*rv = acosh(rad.fval);
	return 0;
}

int32_t _atanh(float* rv, Arg rad)
{
	*rv = atanh(rad.fval);
	return 0;
}

int32_t _sqrt(float* rv, Arg val)
{
	*rv = sqrt(val.fval);
	return 0;
}

int32_t _exp(float* rv, Arg val)
{
	*rv = exp(val.fval);
	return 0;
}

int32_t _getAdc(uint32_t* rv, Arg chan)
{
	*rv = getADC(chan.ival);
	return 0;
}

int32_t _SDI12_ReadSingleMeasurement(void* rv, Arg addr, Arg dst, Arg count, Arg additional)
{
	return SDI12_SingleMeasurementRead(addr.ival, reinterpret_cast<float*>(dst.ptr_val), count.ival, additional.ival);
}

int32_t _digitalRead(int* rv, Arg ch)
{
	*rv = DigitalRead(ch.ival);
	return 0;
}

int32_t _digitalWrite(void* rv, Arg ch, Arg value)
{
	DigitalWrite(ch.ival, value.ival);
	return 0;
}

int32_t _pulseRead(int* rv, Arg ch)
{
	*rv = PulseCount(ch.ival);
	return 0;
}

int32_t _open(int* rv, Arg uri)
{
	*rv = open(uri.str,0);
	return 0;
}

int32_t _read(int* rv, Arg fd, Arg buf, Arg count)
{
	*rv = 0;
	if(*rv == -1) return 1; // no dejar de ejecutra y salir del bucle
	return 0;
}

int32_t _write(int* rv, Arg fd, Arg buf, Arg count)
{
	*rv = write(fd.ival, reinterpret_cast<void*>(buf.ptr_val), count.ival);
	return 0;
}

int32_t _sleepFor(void* rv, Arg s)
{
	sleepFor(s.ival);
	return -1; //dejar de ejecutar y salir del bucle
}

int32_t _strcat(void* rv, Arg dst, Arg src)
{
	int i = 0, j = 0;
	while(dst.str[i] != 0) i++;
	while(src.str[j] != 0) dst.str[i++] = src.str[j++];
	dst.str[i] = 0;
	return 0;
}

int32_t _strlen(int* rv, Arg str)
{
	int len = 0;
	while(str.str[len] != 0)
		len++;
	*rv = len;
	return 0;
}

int32_t _itoa(void* rv, Arg i, Arg str)
{
	sprintf(str.str, "%u\0", i.ival);
	return 0;
}

int32_t _ftoa(void* rv, Arg f, Arg str)
{
	sprintf(str.str, "%.3f\0", f.fval);
	return 0;
}

int32_t _time(int* rv)
{
	*rv = time();
	return 0;
}

int32_t _getBattery(int* rv)
{
	*rv = getBattery();
	return 0;
}

int32_t _getCharging(int* rv)
{
	*rv = getCharging();
	return 0;
}

int32_t _sleepUntil(void* rv, Arg secs)
{
	setAlarm(secs.ival);
	sleep();
	return -1;
}

BuiltinFunc builtinFuncs[32];

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
	builtinFuncs[14].argTypes[0] = PTR;
	builtinFuncs[14].func_ptr = (intptr_t)_print;

	builtinFuncs[15].nArgs = 1;
	builtinFuncs[15].retValue = INT;
	builtinFuncs[15].argTypes[0] = INT;
	builtinFuncs[15].func_ptr = (intptr_t)_getAdc;

	builtinFuncs[16].nArgs = 4;
	builtinFuncs[16].retValue = VOID;
	builtinFuncs[16].argTypes[0] = INT;
	builtinFuncs[16].argTypes[1] = PTR;
	builtinFuncs[16].argTypes[2] = INT;
	builtinFuncs[16].argTypes[3] = INT;
	builtinFuncs[16].func_ptr = (intptr_t)_SDI12_ReadSingleMeasurement;

	builtinFuncs[17].nArgs = 1;
	builtinFuncs[17].retValue = INT;
	builtinFuncs[17].argTypes[0] = INT;
	builtinFuncs[17].func_ptr = (intptr_t)_digitalRead;

	builtinFuncs[18].nArgs = 2;
	builtinFuncs[18].retValue = VOID;
	builtinFuncs[18].argTypes[0] = INT;
	builtinFuncs[18].argTypes[1] = INT;
	builtinFuncs[18].func_ptr = (intptr_t)_digitalWrite;

	builtinFuncs[19].nArgs = 1;
	builtinFuncs[19].retValue = INT;
	builtinFuncs[19].argTypes[0] = INT;
	builtinFuncs[19].func_ptr = (intptr_t)_pulseRead;

	builtinFuncs[20].nArgs = 1;
	builtinFuncs[20].retValue = INT;
	builtinFuncs[20].argTypes[0] = PTR;
	builtinFuncs[20].func_ptr = (intptr_t)_open;

	builtinFuncs[21].nArgs = 3;
	builtinFuncs[21].retValue = INT;
	builtinFuncs[21].argTypes[0] = INT;
	builtinFuncs[21].argTypes[1] = PTR;
	builtinFuncs[21].argTypes[2] = INT;
	builtinFuncs[21].func_ptr = (intptr_t)_read;

	builtinFuncs[22].nArgs = 3;
	builtinFuncs[22].retValue = INT;
	builtinFuncs[22].argTypes[0] = INT;
	builtinFuncs[22].argTypes[1] = PTR;
	builtinFuncs[22].argTypes[2] = INT;
	builtinFuncs[22].func_ptr = (intptr_t)_write;

	builtinFuncs[23].nArgs = 1;
	builtinFuncs[23].retValue = VOID;
	builtinFuncs[23].argTypes[0] = INT;
	builtinFuncs[23].func_ptr = (intptr_t)_sleepFor;

	builtinFuncs[24].nArgs = 2;
	builtinFuncs[24].retValue = VOID;
	builtinFuncs[24].argTypes[0] = PTR;
	builtinFuncs[24].argTypes[1] = PTR;
	builtinFuncs[24].func_ptr = (intptr_t)_strcat;

	builtinFuncs[25].nArgs = 1;
	builtinFuncs[25].retValue = INT;
	builtinFuncs[25].argTypes[0] = PTR;
	builtinFuncs[25].func_ptr = (intptr_t)_strlen;

	builtinFuncs[26].nArgs = 2;
	builtinFuncs[26].retValue = VOID;
	builtinFuncs[26].argTypes[0] = INT;
	builtinFuncs[26].argTypes[1] = PTR;
	builtinFuncs[26].func_ptr = (intptr_t)_itoa;

	builtinFuncs[27].nArgs = 2;
	builtinFuncs[27].retValue = VOID;
	builtinFuncs[27].argTypes[0] = FLOAT;
	builtinFuncs[27].argTypes[1] = PTR;
	builtinFuncs[27].func_ptr = (intptr_t)_ftoa;

	builtinFuncs[28].nArgs = 0;
	builtinFuncs[28].retValue = INT;
	builtinFuncs[28].func_ptr = (intptr_t)_time;

	builtinFuncs[29].nArgs = 0;
	builtinFuncs[29].retValue = INT;
	builtinFuncs[29].func_ptr = (intptr_t)_getBattery;

	builtinFuncs[30].nArgs = 0;
	builtinFuncs[30].retValue = INT;
	builtinFuncs[30].func_ptr = (intptr_t)_getCharging;

	builtinFuncs[31].nArgs = 1;
	builtinFuncs[31].retValue = INT;
	builtinFuncs[31].argTypes[0] = INT;
	builtinFuncs[31].func_ptr = (intptr_t)_sleepUntil;
}

