/*
	FileSystem.h

	Purpose:
		Provides basic file management functionalities such as Reading, Writing, File Watcher, etc
*/

#pragma once

#include "Common.h"
#include "Log/Log.h"

namespace Hydrogen
{
namespace Internal 
{


	class FileSys
	{
	public:

		HYD static Buffer ReadFile(const std::string& pPath);
		HYD static uint32 WriteFile(const std::string& pPath, Buffer pData);

		HYD static uint64 FileSize(std::ifstream& pFileHandle);

	};
};
};
