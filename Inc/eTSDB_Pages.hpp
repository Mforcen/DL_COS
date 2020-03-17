#ifndef ETSDB_PAGES_H
#define ETSDB_PAGES_H

#include "eTSDB_Utils.hpp"

namespace eTSDB
{
	class Driver;

	enum PageType : uint8_t
	{
		Deleted = 0,
		HeaderType,
		DataType
	};

	class Page // a Page object is valid only if _page_idx is other than 0, because 0 is the index for the root page
	{
	public:
		uint32_t getPageIdx() { return _page_idx; }
		PageType getType() { return _type; }
		uint16_t getTableIdx() { return _table_idx; }

		int getSize();

		virtual int getTypeSize();

		virtual int serialize(uint8_t* buf);

		static void setAllocator(Allocator* alloc){_alloc = alloc; }
		void* operator new(std::size_t size) { return _alloc->Allocate(size); }
		void operator delete(void *ptr) { _alloc->Deallocate(ptr); }

	protected:
		Page(uint32_t page_idx, PageType type, uint16_t table_idx) : _page_idx(page_idx), _type(type), _table_idx(table_idx) {}
		uint16_t _page_idx;
		PageType _type;
		uint16_t _table_idx;

	private:
		static Allocator* _alloc;

		friend class Driver;
	};

	class HeaderPage;

	class DataPage : public Page
	{
	public:
		DataPage();
		Date getBlockDate();

		int getTypeSize();
		int serialize(uint8_t* dst);

		void copy(DataPage* dp);
	protected:

	private:
		DataPage(uint16_t page_idx, uint16_t table_idx, Date block_date, HeaderPage* header);

		static const uint8_t _header_span = 10;
		static const uint8_t _starter_magic[2];
		static const uint8_t _ender_magic[2];

		uint16_t _rowIdx; ///< This helps to find the writing address of the block
		uint8_t _rowWidth;
		//uint8_t _numCols;
		uint8_t _period;

		Date _block_date; ///< This stores the first measure date, used to check the measures
		HeaderPage* _header; // Will be stored at page as block_idx of header page
		Format _formats[16];

		friend class Driver;
	};


	class HeaderPage : public Page
	{
	public:

		HeaderPage();
		HeaderPage(HeaderPage* hp);

		uint8_t* getName();
		uint8_t getNumColumn();
		uint8_t* getColumnName(uint8_t colIdx);
		Format getColumnFormat(uint8_t colIdx);
		uint8_t getColumnStride();
		int8_t getColumnIdx(int8_t span);

		uint8_t getPeriod();

		int getTypeSize();
		int serialize(uint8_t* dst);
		int deserialize(uint8_t* dst);

		void copy(HeaderPage* hp);

	protected:

	private:
		HeaderPage(uint16_t page_idx, uint16_t table_idx);
		uint8_t _name[16];
		uint8_t _colNames[16][16];
        Format _formats[16];

        uint16_t _header_spacing; ///< This variable indicates how many space uses the header, up to 304 bytes
        uint8_t _data_index; ///< This variable indicates where the next data index could be stored
        uint8_t _data_stride; ///< This variable holds the space that a row needs to be fully stored. It's only calculated once based on the format array
        uint8_t _period; ///< This allows to know how many times there will be between samples

        friend class Driver;
	};
}

#endif // ETSDB_BLOCK_H
