#ifndef SPIFLASH_H
#define SPIFLASH_H

#include "SPI_Device.h"

uint16_t getUS();

class SPIFlash : public SPI_Device
{
	public:
		SPIFlash(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin);
		int writePage(uint8_t* data, uint8_t len, uint32_t addr);
		int readPage(uint8_t len, uint32_t addr);
		int eraseSector(uint32_t addr);
		int eraseChip();
		int jedecId(uint8_t* data);
		int powerDown();
		int wakeUp();

		void CpltCallbackModule();
		void pollNextAction();

		int available();
		int peek();
		int pop();
	protected:

	private:
		GPIO_TypeDef* _gpio;
		uint16_t _pin;

		enum NextAction
		{
			None,
			WENDelay,
			WriteEnable,
			WriteStart,
			Write,
			WaitBusy,
			Read,
			Erase
		} _action;

		uint8_t _rx_status[2];
		uint8_t _tx_status[2];

		uint8_t tx_buffer[260];
		int16_t tx_size;

		uint8_t rx_buffer[260];
		uint16_t rx_idx;
		uint16_t rx_size;

		uint16_t _last_us; // para añadir un poco de espera por si hace falta para que a la memoria le de tiempo de hacer cosas
		uint32_t _last_write_ms;
		uint16_t _max_delay_time;

		int writeEnable(); // añadir bsy check
		int writeDisable();

		void readStatus(int statusreg);
		void writeStatus(uint16_t status);
};

#endif // SPIFLASH_H
