#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "opcode.h"
#include "VMBuiltin.h"
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

class VirtualMachine : public FwLogger::Module
{
	public:
		VirtualMachine();

		void setProgram(uint8_t* program, uint32_t addr, uint32_t length); // el program tiene una cabecera y de momento es lo que va a hacer
        void setStackSize(uint32_t stackSize);
        void enable(bool enable = true);
        bool getEnabled();

        int HeaderFD;
        int tablePeriod;

        bool loop();
        void reset();
        void resumeExec(); //wakes the machine up when the RTC alarm fires up

        uint8_t* getPrgName();

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

        int bin_eval(uint8_t* buf, uint8_t* outbuf);

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
