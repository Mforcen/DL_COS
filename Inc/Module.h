#ifndef MODULE_H
#define MODULE_H

#include <cstdint>

#define LORA_BINID 		'L'
#define NETWORK_BINID 	'N'
#define FLASH_BINID 	'F'
#define ETSDB_BINID 	'T'
#define SDI12_BINID 	'S'
#define SSD1306_BINID 	'D'
#define VM_BINID 		'V'
#define DIGITAL_BINID	'I'

#define DeclModuleParam(x) #x
#define RetModuleParam(name, x, value) if(strcmp(name, #x)==0) *value = static_cast<int>(x)
#define SetModuleParam(name, x, type, value) if(strcmp(name, #x)==0) x = static_cast<type>(value)

namespace FwLogger
{
    class Module
    {
	private:
    	static Module* _modules[16];
    	uint8_t _name[16];
		int _numParam;
		const char** _ptrParam;
		const uint8_t* _ptrParamSize;
	public:
    	Module(const char* name, char bin_id, uint8_t major, uint8_t minor, int numParam = 0, const char** modParams=nullptr, const uint8_t* modParamSizes=nullptr);
		static Module* getModule(int modnum);
		static Module* getModuleById(char bin_id);
		static uint8_t getModuleNumber();
		uint8_t* getName();
		static bool isModule(void* ptr);

		virtual bool loop() = 0; // return false means that no work is left to do until something

		const uint8_t majorVersion, minorVersion;
		const char bin_id;

		virtual int getConfigSize();
		virtual int getConfig(uint8_t* buf);
		virtual int setConfig(uint8_t* buf);

		int getNumParam();
		const char** getParamList();
		const uint8_t* getParamSizes();
		virtual int setParam(const char* name, int value);
		virtual int getParam(const char* name, int* dst);

		virtual int getStatus(uint8_t* buf);

		virtual int bin_eval(uint8_t* buf, int buflen, uint8_t* outbuf);
    };
}

#endif // MODULE_INCLUDED
