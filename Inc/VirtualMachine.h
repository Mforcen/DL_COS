#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "opcode.h"
#include "VMBuiltin.h"
#include "mem_structs.hpp"
#include "Module.h"

uint16_t getUS(); // funcion del timer

struct cFuncCall
{
	int nArgs;
	Arg args[8];
	intptr_t func_addr;
};

struct Symbol
{
	Symbol()
	{
		for(int i = 0; i < 16; ++i) name[i] = 0;
		type = TypeVal::UNKNOWN;
		ptr = reinterpret_cast<uintptr_t>(nullptr);
	}

	Symbol(uint8_t* name_val, TypeVal type_val, uintptr_t ptr_val)
	{
		for(int i = 0; i < 16; ++i) name[i] = name_val[i];
		ptr = ptr_val;
		type = type_val;
	}

	Symbol(const Symbol& s)
	{
		for(int i = 0; i < 16; ++i) name[i] = s.name[i];
		ptr = s.ptr;
		type = s.type;
	}

	uint8_t name[16];
	TypeVal type;
	uintptr_t ptr;
};

class VirtualMachine : public FwLogger::Module
{
	public:
		VirtualMachine();

		void setProgram(const uint8_t* program, uint32_t addr, uint32_t length); // el program tiene una cabecera y de momento es lo que va a hacer
        void setStackSize(uint32_t stackSize);
        void enable(bool enable = true);
        bool getEnabled();

        int HeaderFD;
        int tablePeriod;

        bool loop();
        void reset();
        void resumeExec(); //wakes the machine up when the RTC alarm fires up
        void pauseExec();
        void clear();

        const uint8_t* getPrgName();

        int getNextAlarm(int now_alarm);

        bool cycle(); // ejecuta una instrucción de la máquina virtual
        uint8_t* getTableAddress();
        bool getSaveFlag();
        void ackSaveFlag();
        bool getWaitFlag(); // it returns true when the alarm should be activated at getNextAlarm

        int getConfigSize();
        int getConfig(uint8_t* buf);
        int setConfig(uint8_t* buf);

        int getStatus(uint8_t* buf);

        void addSymbol(uint8_t* name, uintptr_t addr, TypeVal type);
        void setSymbol(int idx, uint8_t* name, uintptr_t addr, TypeVal type);
        void setSymbolName(int idx, uint8_t* name);
        void setSymbolAddr(int idx, uint32_t addr);
        void setSymbolType(int idx, TypeVal type);
        Symbol getSymbol(int idx);
		void resizeSymbolTable(std::size_t entries);
		std::size_t getSymbolTableSize();

		struct Context
		{
			uint32_t stackPointer;
			uint32_t returnAddr;
			uint32_t localVarAddr;
			uint32_t argAddr;
			uint32_t programCounter;
		};

		Context getCurrentContext();

        int bin_eval(uint8_t* buf, int buflen, uint8_t* outbuf);
	protected:

	private:
		uint8_t m_ram[4096]; // de momento
		uint32_t m_stackPointer = 0, m_stackSize; // van dentro de ram, al inicio, después del stacksize va el programa
		uint32_t m_returnAddr, m_localVarAddr, m_argAddr; // esto sería el stackFrame

		uint8_t m_prgName[16];

		bool m_saveTable, m_startTableWait;
		uint8_t* m_tableAddress;

		bool m_enabled;

		uint32_t m_programCounter;
		bool m_execBuiltin;
		cFuncCall m_currBuiltin;

		uint32_t m_delayStart, m_delayTime;
		bool m_delayEnabled;

		bool m_waitTable;

		void prepareBuiltin();
        int callBuiltin();

		template<typename T>
		void pack(uint32_t addr, T val); // esto es en ram, indicando la memoria
		template<typename T>
		T unpack(uint32_t addr);

		template<typename T>
		void push(T val); // esto es pack, unpack pero solo en el stack y gestiona el stackpointer

		template<typename T>
		T pop();

		vector<Symbol> m_symbolTable;
		//void cycle(); // ejecuta una instrucción de la máquina virtual
};

#endif // VIRTUALMACHINE_H
