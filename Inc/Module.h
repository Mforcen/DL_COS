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


namespace FwLogger
{
    class Module
    {
	private:
    	static Module* _modules[16];
    	uint8_t _name[16];
	public:
    	Module(const char* name, char bin_id, uint8_t major, uint8_t minor);
		static Module* getModule(int modnum);
		static uint8_t getModuleNumber();
		uint8_t* getName();
		virtual bool loop() = 0; // return false means that no work is left to do until something

		const uint8_t majorVersion, minorVersion;
		const char bin_id;

		virtual int getConfigSize();
		virtual int getConfig(uint8_t* buf);
		virtual int setConfig(uint8_t* buf);

		virtual int getStatus(uint8_t* buf);

		virtual int bin_eval(uint8_t* buf, int buflen, uint8_t* outbuf);
    };
}

#endif // MODULE_INCLUDED
