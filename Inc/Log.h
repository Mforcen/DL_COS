#ifndef LOG_H
#define LOG_H

#include <cstdarg>

namespace FwLogger
{
	class Log
	{
		public:
			enum LogLevel : uint8_t
			{
				LevelError = 0,
				LevelWarning,
				LevelInfo,
				LevelVerbose
			};
			static void Verbose(const char* fmt, ...);
			static void Info(const char* fmt, ...);
			static void Warning(const char* fmt, ...);
			static void Error(const char* fmt, ...);
		protected:
			static void setLogLevel(LogLevel level);
			static LogLevel _log_level;

			static int _fd_sink;

			friend class OS;
		private:
			static void Write(const char* fmt, va_list args);
			Log();
	};
}
#endif // LOG_H
