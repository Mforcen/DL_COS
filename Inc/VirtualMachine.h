#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "opcode.h"
#include "VMBuiltin.h"
#include "eTSDB.hpp"
#include "Module.h"

uint16_t getUS(); // funcion del timer

union Arg
{
	float fval;
	int32_t ival;
	uint32_t uival;
	char buf[4];
	intptr_t ptr_val;
};

struct cFuncCall
{
	int nArgs;
	Arg args[8];
	intptr_t func_addr;
};

class VirtualMachine
{
	public:
		VirtualMachine();

		void setProgram(uint8_t* program, uint32_t addr, uint32_t length); // el program tiene una cabecera y de momento es lo que va a hacer
        void setStackSize(uint32_t stackSize);
        void enable(bool enable = true);

        int HeaderFD;

        void loop();

        bool cycle(); // ejecuta una instrucción de la máquina virtual

	protected:

	private:
		uint8_t m_ram[4096]; // de momento
		uint32_t m_stackPointer = 0, m_stackSize; // van dentro de ram, al inicio, después del stacksize va el programa
		uint32_t m_returnAddr, m_localVarAddr, m_argAddr; // esto sería el stackFrame

		bool m_enabled;

		uint32_t m_programCounter;
		bool m_execBuiltin;
		cFuncCall m_currBuiltin;

		void prepareBuiltin();
        void callBuiltin();

		template<typename T>
		void pack(uint32_t addr, T val); // esto es en ram, indicando la memoria
		template<typename T>
		T unpack(uint32_t addr);

		template<typename T>
		void push(T val); // esto es pack, unpack pero solo en el stack y gestiona el stackpointer

		template<typename T>
		T pop();

		//void cycle(); // ejecuta una instrucción de la máquina virtual
};

#endif // VIRTUALMACHINE_H
