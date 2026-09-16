#include "FileSystem.h"
#include "HydPch.h"

namespace Hydrogen
{

	Buffer FileSys::ReadFile(const std::string & pPath)
	{

		std::ifstream File(pPath, std::ios::binary);
		if (!File.is_open())
		{
			Log::SetError(Log::FmtStr("Unable to Open File: %s", pPath.c_str()));
			return Buffer();
		}

		//Retrive File Size:
		uint64 Size = FileSize(File);

		//Allocate Memeory:
		Buffer Data(Size);

		//Read to Buffer
		File.read((char*)Data.GetPtr(), Size);

		File.close();

		return std::move(Data);
	}

	uint32 FileSys::WriteFile(const std::string & pPath, Buffer pData)
	{
		return HYD_OK;
	}

	uint64 FileSys::FileSize(std::ifstream& pFileHandle)
	{
		pFileHandle.seekg(0, pFileHandle.end);
		uint64 SizeInBytes = pFileHandle.tellg();
		pFileHandle.seekg(0, pFileHandle.beg);
		return SizeInBytes;
	}
	

};