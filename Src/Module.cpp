#include "Module.h"

namespace FwLogger
{
	Module::Module(const char* name, char binid, uint8_t major, uint8_t minor, int numParam, const char** modParam, const uint8_t* modParamSizes) : bin_id(binid), majorVersion(major), minorVersion(minor)
	{
		for(int i = 0; i < 16; ++i) _name[i] = 0;
		for(int i = 0; i < 15; ++i) _name[i] = name[i];
		_modules[getModuleNumber()] = this;
		_numParam = numParam;
		_ptrParam = modParam;
		_ptrParamSize = modParamSizes;
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

	bool Module::isModule(void* ptr)
	{
		for(int i = 0; i < 16; ++i)
		{
			if(_modules[i] == nullptr) return false;
			if(_modules[i] == ptr) return true;
		}
		return false;
	}

	Module* Module::getModule(int modnum)
	{
		return _modules[modnum];
	}

	Module* Module::getModuleById(char bin_id)
	{
		for(int i = 0; i < 16; ++i)
		{
			if(_modules[i] == nullptr) return nullptr;
			if(_modules[i]->bin_id == bin_id) return _modules[i];
		}
		return nullptr;
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

	int Module::getNumParam()
	{
		return _numParam;
	}

	const char** Module::getParamList()
	{
		return _ptrParam;
	}

	const uint8_t* Module::getParamSizes()
	{
		return _ptrParamSize;
	}

	int Module::setParam(const char* name, int value)
	{
		return -1;
	}

	int Module::getParam(const char* name, int* dst)
	{
		return -1;
	}

	int Module::getStatus(uint8_t* buf)
	{
		return 0;
	}

	int Module::bin_eval(uint8_t* buf, int buflen, uint8_t* outbuf)
	{
		return 0;
	}
}
