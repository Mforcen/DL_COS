#ifndef ETSDB_PAGES_H
#define ETSDB_PAGES_H

#include "eTSDB_Utils.hpp"

namespace FwLogger
{
	namespace eTSDB
	{
		class Driver;

		enum PageType : uint8_t
		{
			Deleted = 0,
			HeaderType,
			DataType,
			FileType,
			EmptyType = 0xff
		};

		class Page // a Page object is valid only if _page_idx is other than 0, because 0 is the index for the root page
		{
		public:
			uint32_t getPageIdx() { return _page_idx; }
			PageType getType() { return _type; }
			uint16_t getObjectIdx() { return _object_idx; }

			PageAccessMode getAccessMode() { return _page_mode; }

			int getSize();
			uint8_t* getName();

			virtual int getTypeSize();

			virtual int serialize(uint8_t* buf);
			virtual int deserialize(uint8_t* buf);

			static void setAllocator(Allocator<128>* alloc){_alloc = alloc; }
			void* operator new(std::size_t size) { return _alloc->Allocate(size, 1); }
			void operator delete(void *ptr) { _alloc->Deallocate(ptr); }

		protected:
			Page(uint32_t page_idx, PageType type, uint16_t object_idx) : _page_idx(page_idx), _type(type), _object_idx(object_idx), _page_mode(PageAccessMode::PageEmpty) {}
			Page(uint32_t page_idx, PageType type, uint16_t object_idx, PageAccessMode am) : _page_idx(page_idx), _type(type), _object_idx(object_idx), _page_mode(am) {}
			uint16_t _page_idx;
			PageType _type;
			uint16_t _object_idx;
			uint8_t _name[16];
			static Allocator<128>* _alloc;
			PageAccessMode _page_mode;

		private:
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
			int getNumEntries();

			void copy(DataPage* dp);
		protected:

		private:
			DataPage(uint16_t page_idx, uint16_t object_idx, Date block_date, HeaderPage* header);

			static const uint8_t _header_span = 10;
			static const uint8_t _starter_magic[1];
			static const uint8_t _ender_magic[1];

			uint16_t _rowIdx; ///< This helps to find the writing address of the block
			uint16_t _rowWidth;
			uint8_t _period;

			Date _block_date; ///< This stores the first measure date, used to check the measures
			HeaderPage* _header; // Will be stored at page as block_idx of header page

			friend class Driver;
		};

		struct ColumnHeader
		{
			Format format;
			uint8_t name[16];
		};

		struct HeaderInitializer
		{
			vector<ColumnHeader> cols;
			uint8_t period;
		};

		class HeaderPage : public Page
		{
		public:

			HeaderPage();
			HeaderPage(HeaderPage* hp);
			~HeaderPage();

			uint8_t getNumColumn();
			uint8_t* getColumnName(uint8_t colIdx);
			Format getColumnFormat(uint8_t colIdx);
			uint16_t getColumnStride();
			int8_t getColumnIdx(int8_t span);

			uint8_t getPeriod();

			bool checkFormat(Row& row);
			void getFormat(Row& row);

			int getTypeSize();
			int serialize(uint8_t* dst);
			int deserialize(uint8_t* dst);

			void copy(HeaderPage* hp);
			void move(HeaderPage* hp);

		protected:

		private:
			HeaderPage(uint16_t page_idx, uint16_t object_idx);

			uint16_t _header_spacing; ///< This variable indicates how many space uses the header, up to 304 bytes
			uint16_t _data_idx; ///< This variable indicates where the next data index could be stored
			uint16_t _data_stride; ///< This variable holds the space that a row needs to be fully stored. It's only calculated once based on the format array
			uint8_t _period; ///< This allows to know how many times there will be between samples

			vector <ColumnHeader> _cols;

			DataPage* _currDP;

			friend class Driver;
		};

		class FilePage: public Page
		{
			public:
			FilePage();
			~FilePage();

			int getTypeSize();
			int serialize(uint8_t* dst);

			bool getDataReady();
			uint8_t* getDataPage();
			bool freeDataPage();
			uint16_t getFileSize();
			uint16_t getBytesRead();

			void copy(FilePage* fp);
		protected:

		private:
			FilePage(uint16_t page_idx, uint16_t object_idx);
			uint8_t _page_state; // msb (1-0) -> primer página o no, (111-000) escrito o vacío, (1111-0000) escrito o eliminado
			uint16_t _file_size; // se debe recorrer el conjunto de todas las páginas enlazadas
			uint16_t _next_page;
			uint8_t *_databuf;
			uint8_t _data_status;
			uint8_t _read_status;
			uint16_t _read_bytes;
			uint8_t _data_idx;

			friend class Driver;
		};
	}
}

#endif // ETSDB_BLOCK_H
