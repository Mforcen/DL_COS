#include "io.h"

#include "FwLogger.h"

using namespace FwLogger;

int getRawADC(int chan)
{
	return OS::get().get_adc_raw(chan);
}

int getADC(int chan)
{
	return getRawADC(chan) * 3300 / 4096;
}

int getBattery()
{
	return OS::get().get_bat_mv();
}

int getCharging()
{
	return OS::get().getCharging();
}

int open(char* path, int oflag)
{
	return OS::get().open(path, oflag);
}

int read(int fd, void* buf, size_t count)
{
	return OS::get().read(fd, buf, count);
}

int write(int fd, const void* buf, size_t count)
{
	return OS::get().write(fd, buf, count);
}

int close(int fd)
{
	return OS::get().close(fd);
}

int SDI12_SingleMeasurementRead(int addr, float* dst, int count, int additional)
{
	return OS::get().sdi12.singleMeasure(addr, dst, count, additional);
}

int DigitalRead(int ch)
{
	return OS::get().digital.digitalRead(ch);
}

void DigitalWrite(int ch, int value)
{
	OS::get().digital.digitalWrite(ch, value);
}

int PulseCount(int ch)
{
	return OS::get().digital.pulseRead(ch);
}

void sleepFor(int s)
{
	OS::get().sleepFor(s);
}

uint64_t time()
{
	return OS::get().time();
}
