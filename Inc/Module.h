#ifndef MODULE_H
#define MODULE_H

#include <cstdint>

namespace FwLogger
{
    class Module
    {
	private:
    	static Module* _modules[16];
    	uint8_t _name[16];
	public:
    	Module(const char* name);
		static Module* getModule(int modnum);
		static uint8_t getModuleNumber();
		uint8_t* getName();
		virtual bool loop() = 0; // return false means that no work is left to do until something
    };
}

#endif // MODULE_INCLUDED
