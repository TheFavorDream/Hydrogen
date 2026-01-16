/*

	Created: 2026/1/6 by Pouya Alizadeh aka "The Voltage"
	Hydrogen's Loggin system Manages errors and provides a way to identify and debug engine's internal errors and failures.


*/

#pragma once


#include "Common.h"
#include "Glew/glew.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>


namespace Hydrogen
{

	struct LogHolder
	{
		uint32 ErrorCode;
		uint32 Line;
		const char* FilePath;
		const char* OptionalText;
	};

	class Log
	{
	public:

		static void SetError(uint32 pErrorCode,   const char* pFilePath, uint32 pLine, const char* pDescription="");
		
		static void SetInfo(std::string pText);
		static void SetInfo(const char* pText);
		 
		static void EngineDump(const char* pDumpPath="./");

		static void CheckOpenGLErrors(const char* file, uint32 Line);

		static std::vector<LogHolder>& GetLogs();

	private:
		static std::string TranslateErrorCode(uint32 ErrorCode);

	private:
		static std::vector<LogHolder> m_Logs; // Logs are stored here, so when we need it, we can dump it in a file
	};

};