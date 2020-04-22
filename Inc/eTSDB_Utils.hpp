#ifndef ETSBD_UTILS_INCLUDED
#define ETSBD_UTILS_INCLUDED

#include <cstdint>
#include <mem_structs.hpp>

/**	Although the driver will be async, this first version will be partially async. A main class driver which handles a FSM
	will be used.
*/

namespace FwLogger
{
	namespace eTSDB
	{
		static const uint16_t PageWidth = 0x1000;

		enum RetValue
		{
			Ok = 0,
			DateError,
			ColError,
			Busy,
			FullBuffer,
			NotFound,
			Invalid,
			OutOfTime,
			Error,
			FullPage,
			FileEnded
		};

		enum class Format : uint8_t
		{
			Invalid = 0,
			Uint8,
			Int8,
			Uint16,
			Int16,
			Uint32,
			Int32,
			Float
		};

		enum class PageAccessMode: uint8_t
		{
			PageEmpty = 0,
			PageRead,
			PageWrite,
			PageWriteUpdate
		};

		uint8_t getFormatWidth(Format format);
		uint32_t getSecondsFromPeriod(uint8_t period);

		struct Value
		{
			union
			{
				float _float;
				uint32_t _uint32;
				int32_t _int32;
				uint8_t bytes[4];
			} data;
			Format format;
		};

		// Binary date for date format: LSB ssssssmm mmmmhhhh hdddddmm mmyyyyyy yyyyyyye MSB

		struct Date
		{
			Date():exists(0xff){}

			Date(uint16_t Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute, uint8_t Second)
			{
				exists = 0;
				year = Year;
				month = Month;
				day = Day;
				hour = Hour;
				minute = Minute;
				second = Second;
			}

			Date(uint8_t* byte_arr)
			{
				deserialize(byte_arr);
			}

			Date(uint64_t secs)
			{
				fromSeconds(secs);
			}

			void serialize(uint8_t* arr)
			{
				if(exists != 0) for(int i = 0; i < 5; ++i) arr[i] = 0xff;
				else
				{
					arr[0] = (second | (minute << 6) ) & 0xff;
					arr[1] = ((minute >> 2) | (hour << 4)) & 0xff;
					arr[2] = ((hour >> 4 )| (day << 1) | (month << 6)) & 0xff;
					arr[3] = ((month >> 2)|(year << 2)) & 0xff;
					arr[4] = (year >> 6) & 0x7f;
				}
			}

			void deserialize(uint8_t* arr)
			{
				second = arr[0] & 0x3f;
				minute = ((arr[1] << 2) & 0x3c) | ((arr[0] >> 6) & 0x03);
				hour = ((arr[2] << 4) & 0x10) | ((arr[1] >> 4) & 0x0f);
				day = (arr[2] >> 1) & 0x1f;
				month = ((arr[2] >> 6) & 0x03) | ((arr[3] << 2) & 0x0c);
				year = ((arr[3] >> 2) & 0x3f) | ((arr[4] << 6) & 0x1fff);
				exists = 0;
			}

			uint64_t seconds() const
			{
				uint64_t retval;
				retval = second + minute*60 + hour*3600 + day*86400;

				for(int i = 0; i < year; ++i)
					retval += ((i%4 == 0 )|( i%400 == 0) ? 366 : 365)*86400;

				uint8_t days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
				if(year % 4 == 0 || year % 400 == 0) days[1] = 29;

				for(uint8_t month_idx = 0; month_idx < month; ++month_idx) retval += days[month_idx] * 86400;
				return retval;
			}

			void fromSeconds(uint64_t seconds)
			{
				exists = 0;

				year = 0;
				int seconds_in_year = 366*86400;
				while(seconds >= seconds_in_year)
				{
					year++;
					seconds -= seconds_in_year;
					seconds_in_year = (year % 4 == 0 ? 31622400 : 31536000);
				}

				month = 0;
				int seconds_in_month = 31*86400;
				uint8_t days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
				if(year % 4 == 0 || year % 400 == 0) days[1] = 29;
				while(seconds >= seconds_in_month)
				{
					seconds-=seconds_in_month;
					month++;
					seconds_in_month = 86400*days[month];
				}

				day = seconds/86400;
				seconds = seconds % 86400;
				hour = seconds/3600;
				seconds %= 3600;
				minute = seconds/60;
				second = seconds % 60;
			}

			Date operator-(Date const& other)
			{
				return Date(seconds()-other.seconds());
			}

			Date operator-(uint64_t const dt)
			{
				return Date(seconds()-dt);
			}

			Date operator+(uint64_t const dt)
			{
				return Date(seconds()+dt);
			}

			bool operator==(Date const& other)
			{
				return ((year==other.year) && (month == other.month) && (day == other.day) && (hour == other.hour) && (minute == other.minute) && (second == other.second));
			}

			uint8_t exists;
			uint16_t year; // upto 8192
			uint8_t month;
			uint8_t day;
			uint8_t hour;
			uint8_t minute;
			uint8_t second;
		};

		struct Row
		{
			Row()
			{
				for(int i = 0; i < 16; ++i) vals[i].format = Format::Invalid;
			}

			Row(Row* row)
			{
				clone(row);
			}

			void clone(Row* row)
			{
				for(int i = 0; i < 16; ++i)
				{
					vals[i].format = row->vals[i].format;
					vals[i].data._uint32 = vals[i].data._uint32;
				}
			}

			void clear()
			{
				for(int i = 0; i < 16; ++i)
					vals[i].format = Format::Invalid;
			}

			Date rowDate;
			Value vals[16];

			int getBufSize()
			{
				int size = 0;
				for(int i = 0; i < 16; ++i) size += getFormatWidth(vals[i].format);
				return size;
			}

			int serialize(uint8_t* buf)
			{
				int buf_idx = 0;
				int val_idx;
				for(val_idx = 0; val_idx < 16; ++val_idx)
				{
					int formatWidth = getFormatWidth(vals[val_idx].format);
					for(int inner_idx = 0; inner_idx < formatWidth; ++inner_idx) buf[buf_idx++] = vals[val_idx].data.bytes[inner_idx];
				}
				return buf_idx;
			}
		};
	}
}

#endif // ETSBD_UTILS_INCLUDED
