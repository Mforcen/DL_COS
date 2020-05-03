#include "io.h"

#include "FwLogger.h"

using namespace FwLogger;

int getRawADC(int port, int chan)
{
	return OS::get().get_adc_val(6*port+chan);
}

int getADC(int port, int chan)
{
	return getRawADC(port, chan) * 3300 / 4096;
}

int open(char* path, int oflag)
{
	return OS::get().open(path, oflag);
}

int read(int fd, void* buf, size_t count)
{
	return OS::get().read(fd, buf, count);
}

int poll(int fd)
{
	return OS::get().poll(fd);
}

int write(int fd, const void* buf, size_t count)
{
	return OS::get().write(fd, buf, count);
}

int close(int fd)
{
	return OS::get().close(fd);
}
