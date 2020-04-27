#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <cstdint>
#include <cstdlib>

int getADC(int port, int chan);
int getRawADC(int port, int chan);
int open(char* path, int oflag);
int read(int fd, void* buf, size_t count);
int poll(int fd);
int write(int fd, const void* buf, size_t count);
int close(int fd);

#endif // IO_H_INCLUDED
