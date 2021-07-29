# DL_COS
This project aims to develop a proof of concept of a datalogger with a several tasks operating concurrently in a cooperative way.

Although the name is an acronym for Cooperative Operating System, it is not really an Operating System, since it can't stop the execution of a task.

All the modules are programmed following a state machine structure, with functions that won't block, hence the Cooperative part of the name.

The firmware includes several components. Among them, we can find:

1. A simple stack-based bytecode interpreter. This module is important because it offers two enhancements. In one hand it allows the device to execute programs can be load from flash without reprogram the whole OS. On the other hand, the user does not have to take care about the cooperative behaviour, since the execution of the program is controlled by the module. Finally, this makes the communication quite simple, since a user only needs to call the write function and the OS executes the communications asyncronously, halting the execution of the bytecode program until the communication is finished. This module is intended to use with another piece of software.
2. A SDI12 driver. This manages the communications with several field sensors. This module allows the device to read all the data from the sensors, following the SDI12 specification. This piece of software only needs two timers and a GPIO pin with interruptions enabled. This driver is tested with several SDI12 devices, such as MPS6 sensors-
3. A SPI arbitrer. This makes sure that only one SPI device is in use at once. It is used in the LoRa module and in the SPI Flash module.
4. A logging file system. This allows to write logs to the internal SPI flash memory.
5. A communication driver. This is an effort to write a device communication abstraction model, in order to make an standard API to expose several types of communication devices. It only implements a SIM800L driver in HTTP and SMS mode.

## What this this
This is a Proof of Concept that shows how a datalogger could work, with several modules working concurrently. This is more an academic exercise that gives a glimpse about the integration of several elements working together.
It could be seen also as a reference to implement some modules into your own work.

## What this is not
This is not the best way to make a datalogger. There are several design flaws that I realized when I was developing this project.
For example, writing state machines in C is tedious and makes the code messy and hard to debug. It is true that the code is highly portable, since it allows not to use assembly to make things like coroutines, but the embedded developing is a highly specific activity, so it is worthwhile to do several things in a less portable way.

## License
Copyright 2021 Manuel Forcén Muñoz

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.