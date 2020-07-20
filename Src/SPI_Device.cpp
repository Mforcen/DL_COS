#include "SPI_Device.h"

namespace FwLogger
{
	SPI_Device::SPI_Device(SPI_HandleTypeDef* hspi)
	{
		_hspi = hspi;
		_locker = nullptr;
		_status = Idle;
	}

	void SPI_Device::CpltCallback()
	{
		_status = Locked;
		_locker->ISR();
	}

	int SPI_Device::startWrite(uint8_t *pdata, uint16_t len)
	{
		if(_status != Locked) return -1;
		if(_locker != this) return -2;

		_status = Transmitting;
		HAL_StatusTypeDef status = HAL_SPI_Transmit_DMA(_hspi, pdata, len);
		if(status != HAL_OK) return -status;

		return len;
	}

	int SPI_Device::startRead(uint8_t* buf, uint16_t len)
	{
		if(_status != Locked) return -1; // está hasiendo otra cosa
		if(_locker != this) return -2; // está en locked organizado por otro módulo

		_status = Receiving;
		return -HAL_SPI_Receive_DMA(_hspi, buf, len);
	}

	int SPI_Device::startTxRx(uint8_t* txbuf, uint8_t* rxbuf, uint16_t len)
	{
		if(_status != Locked) return -1;
		if(_locker != this) return -1;

		_status = TxRx;
		return -HAL_SPI_TransmitReceive_DMA(_hspi, txbuf, rxbuf, len);
	}

	int SPI_Device::try_lock()
	{
		if(_locker != nullptr) //Si no es nullptr, puede ser this o no
			if(_locker != this)	return -1; //y si no es this, pues este objeto no tiene control sobre el periférico

		_locker = this;
		_status = Locked;

		return 0;
	}

	int SPI_Device::unlock()
	{
		if(_status != Locked) return -1;
		if(_locker != this) return -2;

		_locker = nullptr;
		_status = Idle;

		return 0;
	}


	SPI_Device* SPI_Device::_locker = nullptr;
	SPI_Device::SPIStatus SPI_Device::_status = Idle;
}
