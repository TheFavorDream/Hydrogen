#include "Log.h"
#include "HydPch.h"

namespace Hydrogen
{

	std::vector<LogHolder> Log::m_Logs;
	LogLevel Log::m_Level = LV3;
	bool Log::m_FileLogging = true;
	std::ofstream Log::m_File;
	OutColor Log::m_OutputColor = WHITE;



	void Log::CheckXenonErrors(const Xenon::Log::LogMessage& pMessage)
	{
		switch (pMessage.Kind)
		{
		case Xenon::LOG_INFO:
			SetInfo(pMessage.Message);
			break;
		case Xenon::LOG_WARNING:
			SetWarning(pMessage.Message);
			break;
		case Xenon::LOG_ERROR:
			SetError(pMessage.Message);
			break;
		}
	}

	std::vector<LogHolder>& Log::GetLogs()
	{
		return m_Logs;
	}

	void Log::SetOutputColor(OutColor pColor)
	{
		m_OutputColor = pColor;
	}

	std::string Log::TranslateErrorCode(uint32 ErrorCode)
	{

		switch (ErrorCode)
		{
			case HYD_INVALID_FILE:
				return "HYD_INVALID_FILE";
			case HYD_INVALID_PATH:
				return "HYD_INVALID_PATH";
			case HYD_INVALID_GLTF:
				return "HYD_INVALID_GLTF";
			case HYD_INVALID_BIN :
				return "HYD_INVALID_BIN";
			case HYD_INVALID_GLB :
				return "HYD_INVALID_GLB";
			case HYD_INVALID_API :
				return "HYD_INVALID_API";
			case HYD_OPENGL_VERTEX_BUFFER_FAILED :
				return "HYD_OPENGL_VERTEX_BUFFER_FAILED";
			case HYD_GLEW_FAILED  :
				return "HYD_GLEW_FAILED";
			case HYD_CORRUPTED_GLTF :
				return "HYD_CORRUPTED_GLTF";
			case HYD_CORRUPTED_GLB  :
				return "HYD_CORRUPTED_GLB";
			case HYD_URI_FAILED :
				return "HYD_URI_FAILED";
			case HYD_INVALID_COUNT:
				return "HYD_INVALID_COUNT";
			case HYD_INVALID_TARGET:
				return "HYD_INVALID_TARGET";
			case HYD_WINDOW_FAILD:
				return "HYD_WINDOW_FAILD";
			case HYD_FAILED_TEXTURE_CREATION:
				return "HYD_FAILED_TEXTURE_CREATION";
			case HYD_INVALID_VALUE:
				return "HYD_INVALID_VALUE";
			case HYD_UI_INVALID_WINDOW:
				return "HYD_UI_INVALID_WINDOW";
			case HYD_SHADER_FAILED:
				return "HYD_SHADER_FAILED";
			case HYD_IMAGE_FAILED:
				return "HYD_IMAGE_FAILED";

			
		}

		return "Undefined Error Code";
	}

	void Log::Output(const std::string & pOutputStr)
	{
		m_File << pOutputStr;


		//Set Color:

		switch (m_OutputColor)
		{
		case RED:
			std::cout << "\x1b[31;20m";
			break;
		case YELLOW:
			std::cout << "\x1b[33;20m";
			break;
		case WHITE:
			std::cout << "\x1b[37;20m";
			break;
		case GREEN:
			std::cout << "\x1b[32;20m";
			break;
			case PURPLE:
			std::cout << "\x1b[35;20m";
			break;
		}
		
		std::cout << pOutputStr;
	}



	void Log::SetLevel(LogLevel pLevel)
	{
		m_Level = pLevel;
	}


	std::string Log::FmtStr(std::string pString, ...)
	{
		va_list Args;
		va_start(Args, pString);
		std::string Res = "";
		for (int i = 0; i < pString.size(); ++i)
		{
			if (pString[i] == '%' && i < pString.size()-1)
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
		return  Res;
	}


	void Log::SetInfo(std::string pDescription)
	{
		SetInfo({ pDescription, HYD_OK, -1, ""});
	}

	void Log::SetInfo(LogHolder pLog)
	{
		if (m_Level < LV3)
			return;

		m_Logs.push_back(pLog);
		std::string OutputStr = "[Info]" + pLog.Description + "\n";
		
		SetOutputColor(WHITE);
		Output(OutputStr);
	}

	void Log::SetWarning(std::string pDescription, int32 pErrorCode, std::string pFilePath, int32 pLine)
	{
		SetWarning({ pDescription, pErrorCode, pLine, pFilePath });
	}

	void Log::SetWarning(LogHolder pLog)
	{
		if (m_Level < LV2)
			return;

		m_Logs.push_back(pLog);
		std::string OutputStr = "[Warning]" + ((pLog.ErrorCode != -1) ? TranslateErrorCode(pLog.ErrorCode) + ":\t" : "")
		    + pLog.Description + ((pLog.FilePath.size() != 0) ? "=>File:" + pLog.FilePath : "")
			+ ((pLog.Line != -1) ? "=>Line:" + std::to_string(pLog.Line) : "") + "\n";

		SetOutputColor(YELLOW);
		Output(OutputStr);
	}

	void Log::SetError(std::string pDescription, int32 pErrorCode, std::string pFilePath, int32 pLine)
	{
		SetError({pDescription, pErrorCode, pLine, pFilePath});
	}

	void Log::SetError(LogHolder pLog)
	{
		if (m_Level < LV1)
			return;

		m_Logs.push_back(pLog);
		std::string OutputStr = "[Error]" + ((pLog.ErrorCode != -1) ? TranslateErrorCode(pLog.ErrorCode) + ":\t" : "")
		    + pLog.Description + ((pLog.FilePath.size() != 0) ? "=>File:" + pLog.FilePath : "")
			+ ((pLog.Line != -1) ? "=>Line:" + std::to_string(pLog.Line) : "") + "\n";

		SetOutputColor(RED);
		Output(OutputStr);
	}


	void Log::DebugPrint(const std::string & pContent)
	{
		SetOutputColor(GREEN);
		std::cout << pContent << '\n';
	}

	void Log::DisableFile()
	{
		m_File.close();
		m_FileLogging = false;
	}

	void Log::EnableFile(std::string pPath)
	{
		if (m_Level == LV0)
			return;

		m_File.open(pPath);
		m_FileLogging = true;
	}
};