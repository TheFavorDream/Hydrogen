#include "../include/Xenon.h"
#include <cstdarg>

namespace Xenon
{

#ifdef XE_LOG_LEVEL1 
	Xenon::Log::SetLogLevel(LV1);
#endif
#ifdef XE_LOG_LEVEL2
	Xenon::Log::SetLogLevel(LV2);
#endif	
#ifdef XE_LOG_LEVEL3 
	Xenon::Log::SetLogLevel(LV3);
#endif

	std::queue<Log::LogMessage> Log::s_Logs;
	LogLevel					Log::s_Level = LV3;
	uint64_t				    Log::s_LogCount = 0;

	uint64_t Log::GetLogCount()
	{
		return s_LogCount;
	}


	Log::LogMessage Log::GetLastLog()
	{
#ifndef XE_SHUTUP
		if (!s_LogCount)
			return LogMessage();

		LogMessage log = s_Logs.front();
		s_Logs.pop();
		s_LogCount -= 1;
		return log;
#else
		return LogMessage();
#endif // !XE_SHUTUP
	}

	void Log::SetLogLevel(LogLevel pLevel)
	{
		s_Level = pLevel;
	}

	void Log::PushLog(LogKind pKind, XE_STRING pString, ...)
	{
#ifndef XE_SHUTUP

		if (pKind > s_Level)
			return;


		XE_STRING Kind;
		switch (pKind)
		{
		case LOG_INFO:
			Kind = "[XE_INFO]";
			break;
		case LOG_WARNING:
			Kind = "[XE_WARNING]";
			break;
		case LOG_ERROR:
			Kind = "[XE_ERROR]";
			break;

		}

		va_list Args;
		va_start(Args, pString);
		XE_STRING Res = "";
		for (int i = 0; i < pString.size(); ++i)
		{
			if (pString[i] == '%' && i < pString.size() - 1)
			{
				char SpecialChar = pString[i + 1];

				switch (SpecialChar)
				{
				case 'i':
					Res += std::to_string(va_arg(Args, int));
					i++;
					break;
				case 'f':
					Res += std::to_string(va_arg(Args, double));
					i++;
					break;
				case 'c':
					Res += static_cast<char>(va_arg(Args, char));
					i++;
					break;
				case 's':
					Res += std::string(va_arg(Args, const char*));
					i++;
					break;
				}

			}
			else
			{
				Res += pString[i];
			}
		}

		va_end(Args);
		s_LogCount += 1;


		s_Logs.push({Kind+Res, pKind});
#endif
	}




};