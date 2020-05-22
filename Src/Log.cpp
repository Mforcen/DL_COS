#include "FwLogger.h"
#include "Log.h"

namespace FwLogger
{
	int Log::_fd_sink = 0;
	Log::LogLevel Log::_log_level = Log::LevelError;


	void Log::Verbose(const char* fmt, ...)
	{
		if(LevelVerbose <= _log_level)
		{
			va_list args;
			va_start(args, fmt);
			Write(fmt, args);
			va_end(args);
		}
	}

	void Log::Info(const char* fmt, ...)
	{
		if(LevelInfo <= _log_level)
		{
			va_list args;
			va_start(args, fmt);
			Write(fmt, args);
			va_end(args);
		}
	}

	void Log::Warning(const char* fmt, ...)
	{
		if(LevelWarning <= _log_level)
		{
			va_list args;
			va_start(args, fmt);
			Write(fmt, args);
			va_end(args);
		}
	}

	void Log::Error(const char* fmt, ...)
	{
		if(LevelError <= _log_level)
		{
			va_list args;
			va_start(args, fmt);
			Write(fmt, args);
			va_end(args);
		}
	}

	void Log::setLogLevel(LogLevel level)
	{
		_log_level = level;
	}

	void Log::Write(const char* fmt, va_list args)
	{
		char buf[128];
		int buflen = vsnprintf_(buf, 128, fmt, args);
		if(buflen < 0) return;
		OS::get().write(0, buf, buflen);

		if(_fd_sink != 0)
			OS::get().write(_fd_sink, buf, buflen);
	}

	Log::Log()
	{

	}
}
