#ifndef ETSDB_DRIVER_INCLUDED
#define ETSDB_DRIVER_INCLUDED

#include "SPIFlash.h"

#include "eTSDB_Utils.hpp"
#include "eTSDB_Pages.hpp"

/**
  * Embedded TimeSeries Database
  *
  * Pages: a page is defined as a space of 4KB of memory, laying it down from 0x---000 to 0x---fff. Each page can be erased individually, but the idea is to
  * keep them marked as deleted as long as possible, in order to reduce the wearing of the flash. Each page contains a byte in its address 0, which allow the
  * driver to know the type of the page. In this case, the types are root page, header page, and data page. Then, two bytes are written, which will be used to
  * know which table has the ownership.
  *
  * Root page: this page will serve as an index page, containing some useful information about the file system itself.
  * The first bytes will be a magic number, containing the version of the file system, if needed, alongside other metadata information of the
  * whole device (up to 128 bytes)
  * After 128 bytes, 4 bytes will be used for each table in memory. The first two bytes will indicate the table id, and the last two bytes will
  * indicate the page used to store the table header.
  *
  * Header page: this page will hold some information about the tables. This will include the name of the table, the columns stored and its format,
  * up to 16 different columns. In this case, the strings will be zero-ended, and the column count will be finished when an Invalid format is found.
  * After this, an array of the several data blocks related with this header will be found until the end of the block. The minimum amount of blocks spaces
  * available in the header will be 1902 blocks, which is bigger than the expected 4MB size of the flash.
  *
  * Data page: this page will hold the data itself. The data page also has a header, indicating the block index of the header page. It also stores the date of
  * the beginning of the block, which is quite important since the date of the measures on the block are calculated using the position.
  *
  * The driver will be written in a asynchronous fashion, so, the methods only configure the state machine to execute the operation. The return value of the
  * functions will indicate if the driver is busy doing other things. After the operation is configured, a polling will be required in order to get the results.
  */

namespace eTSDB
{
	class Driver : public SPIFlash
	{
	public:
		Driver(uint32_t offsetAddress, uint32_t size, SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin);

		/**
		  * Create, find and delete header pages
		  */
		RetValue createHeader(uint8_t* name, uint8_t period, uint8_t colLen, Format* formats, uint8_t* colNames[16]);
		RetValue findHeader(uint8_t* name);
		RetValue deleteHeader(HeaderPage& header);

		/**
		  * Create, find and delete data pages
		  */
		RetValue createDataPage(HeaderPage& header, Date& date);
		RetValue findDataPage(HeaderPage& header, uint16_t headerIdx);
		RetValue deleteDataPage(DataPage& dp);

		/**
		  * Read and write values
		  */
		RetValue readValue(DataPage& dp, uint16_t meas_idx);
		RetValue appendValue(DataPage& dp, Row& row);

		void flashModuleISR(); // this advances the fsm, will be called when the flash operation is finished

		enum class State : uint8_t
		{
			Nop = 0,

			Find, //operations
			Write,
			Read,
			FindString, //finds against _opBuf in a uint8_t* fashion
			ReadValue,

			StoreIndexInPage, //generic actions
			WritePageTableIndexInRoot,
			WritePageHeader,
			ReadPageHeader,

			HeaderPageReadDataIndex,// a lo mejor no hace falta si lo pongo en read

			DataPageFindEmptyInHeader,
			DataPageWriteInHeader, //DataPage actions
			DataPageReadHeader,
			DataPageFindEmptyBody,

			ReadDataBody,
			Full = 0x80,
			FreePage,
			Wait = 0xff
		};

		State getState();

		Page* getPage(); // this allow to have the pending page, but it should be copied since the memory buffer will be reused
		Row* getDataRow();

		int getError(); //returns a value indicating an error

		void reset();

	protected:

	private:
		Allocator _alloc;
		uint8_t _alloc_buf[4128];

		State _states[16];
		int _stateIdx;

		Page* _page;
		Row* _row;

		uint32_t _offset, _size;
		int _error;

		uint32_t _opAddr; ///< Indicates where the next read/write operation should be done
		uint32_t _opIdx; ///< In case of iterating procceses, this will indicate the iteration number
		uint32_t _opLen; ///< This variable will indicate the length of the current operation (read/write or max iteration)
		uint16_t _opFind; ///< This variable will hold the value that the driver is looking for in search operations
		uint16_t _opFindMax; ///< This variable will hold the maximum value that appeared during the search operation (used in table creation)
		void* _opBuf; ///< This variable will hold the buffer of the current operation (read/write or iteration)

		/**
		  * Private methods
		  */
		RetValue findEmptyPage();
		void step();
		RetValue prepareCom();
	};
}


#endif // ETSDB_DRIVER_INCLUDED
