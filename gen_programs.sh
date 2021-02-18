#!/bin/bash
for file in programas/*.cpp
do
	cp $file Src/embeddedProgram.cpp
	touch Src/FwLogger.cpp
	make -j 8
	cp build/FLogger.elf salida/${file:10:-4}.elf
done
