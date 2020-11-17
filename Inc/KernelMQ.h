#ifndef KERNELMQ_INCLUDED
#define KERNELMQ_INCLUDED

#include "mem_structs.hpp"

namespace FwLogger
{
	enum class Operation : uint8_t
	{
		None,
		Eval,
		BinEval,

		OpenFile,
		DownloadFile,
		Upload,
		Close,
		ReadNext,
		ListFiles,

		OpenHeader,
		ListTables,
		ReadTable,

		GetPage,
		SaveRow,
		//TestAppend,
		ModemEval,

		LoadProgram
	};

	struct Task
	{
		Operation op;
		int fd; // file_d or sock_d
		uint16_t counter;
		void* buf;
		uint8_t name_buf[16];
	};

	extern circular_buffer<16, Task> msgQueue;
}

#endif // KERNELMQ_INCLUDED
