#ifndef MODULE_INCLUDED
#define MODULE_INCLUDED

#include <cstdint>

namespace FwLogger
{
    class Module
    {
    	static const char* _names[16];
    	static uint8_t _nameIdx;
	public:
    	Module(const char* name);
		static const char** getNames();
		static uint8_t getModuleNumber();
    };
}

#endif // MODULE_INCLUDED
