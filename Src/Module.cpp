#include "Module.h"

namespace FwLogger
{
	Module::Module(const char* name, char binid, uint8_t major, uint8_t minor) : bin_id(binid), majorVersion(major), minorVersion(minor)
	{
		for(int i = 0; i < 16; ++i) _name[i] = 0;
		for(int i = 0; i < 15; ++i) _name[i] = name[i];
		_modules[getModuleNumber()] = this;
	}

	Module* Module::_modules[16] = {nullptr};

	uint8_t Module::getModuleNumber()
	{
		for(int i = 0; i < 16; ++i)
		{
			if(_modules[i] == nullptr) return i;
		}
		return 16;
	}

	uint8_t* Module::getName()
	{
		return _name;
	}

	Module* Module::getModule(int modnum)
	{
		return _modules[modnum];
	}

	int Module::getConfigSize()
	{
		return 0;
	}

	int Module::getConfig(uint8_t* buf)
	{
		return getConfigSize();
	}

	int Module::setConfig(uint8_t* buf)
	{
		return 0;
	}

	int Module::getStatus(uint8_t* buf)
	{
		return 0;
	}

	int Module::bin_eval(uint8_t* buf, uint8_t* outbuf)
	{
		return 0;
	}
}
