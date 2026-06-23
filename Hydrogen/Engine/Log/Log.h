/*

	Created: 2026/1/6 by Pouya Alizadeh aka "The Voltage"
	Hydrogen's Loggin system Manages errors and provides a way to identify and debug engine's internal errors and failures.
	it also provides an interface to know the engin's internal state

*/

#pragma once


#include "Common.h"
#include "Glew/glew.h"

#include "VecMath/Vector/Vectors.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdarg>

namespace Hydrogen
{

	struct LogHolder
	{
		std::string Description;
		
		int32 ErrorCode;
		int32 Line;
		std::string FilePath;
	};

	/*
		Level 0 (LV0): All the logging is off, no errors, no warnings, no info
		Level 1 (LV1): Only Errors will be stored and the rest will be ignored
		Level 2 (LV2): Only Errors & Warnings will be stored
		Level 3 (LV3): Everything 
	*/

	enum LogLevel{LV0=0, LV1, LV2, LV3};
	enum OutColor{WHITE, RED, YELLOW, GREEN, PURPLE};
	class Log
	{
	public:

		
		HYD static void SetLevel(LogLevel pLevel);

		//Format string
		HYD static std::string FmtStr(std::string pString, ...);
		
		
		HYD static void SetInfo(std::string pDescription);
		HYD static void SetInfo(LogHolder pLog);

		HYD static void SetWarning(std::string pDescription, int32 pErrorCode = -1, std::string pFilePath="", int32 pLine=-1);
		HYD static void SetWarning(LogHolder pLog);

		HYD static void SetError(std::string pDescription, int32 pErrorCode = -1, std::string pFilePath = "", int32 pLine= -1);
		HYD static void SetError(LogHolder pLog);

		//Debug prints are not part of loggin. they won't get stored in log files.
		HYD static void DebugPrint(const std::string& pContent);

		HYD static void DisableFile();
		HYD static void EnableFile(std::string pPath="./Debug.txt");
		
		HYD static std::vector<LogHolder>& GetLogs();
		
		HYD static void SetOutputColor(OutColor pColor);
		
		//API specific:
		HYD static void CheckOpenGLErrors(std::string pFile, int32 pLine);



	private:
		HYD static std::string TranslateErrorCode(uint32 ErrorCode);

		//Writes to file and the main output device (normally console)
		HYD static void Output(const std::string& pOutputStr);


	private:
		static std::vector<LogHolder> m_Logs; // Logs are stored here
		static OutColor m_OutputColor;
		static std::ofstream m_File;
		static bool m_FileLogging;
		static LogLevel m_Level;
	};

};

#define GL_CALL(x) x;Hydrogen::Log::CheckOpenGLErrors(__FILE__, __LINE__);

#ifndef TEST
	#define ASSERT(a,s)  if (a!=true) {Hydrogen::Log::SetError(s);__debugbreak();}
#else
	#define ASSERT(a, s) if (a==true) {std::cout << "[Test:Error]" << s << '\n';}
#endif