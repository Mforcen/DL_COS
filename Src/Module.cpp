#include "Module.h"

namespace FwLogger
{
	Module::Module(const char* name)
	{
		_names[_nameIdx++] = name;
	}

	const char* Module::_names[16] = {0};
	uint8_t Module::_nameIdx = 0;

	const char** Module::getNames()
	{
		return _names;
	}

	uint8_t Module::getModuleNumber()
	{
		return _nameIdx;
	}
}
