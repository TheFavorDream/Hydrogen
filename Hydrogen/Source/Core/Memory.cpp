#include "Memory.h"
#include "Log/Log.h"

#include <iostream>


namespace Hydrogen
{

	uint32 Memory::s_AllocatedNum  = 0;
	uint32 Memory::s_AllocatedSize = 0;

	void Memory::CheckAllocation()
	{
		//ASSERT(s_AllocatedNum != 0, "Not All Memory has been Freed!");
		Log::SetWarning(Log::FmtStr("Remaining Memory:%i KB", s_AllocatedSize/1024));
	}

	void Memory::NewAllocation(uint32 pSize)
	{
		s_AllocatedNum += 1;
		s_AllocatedSize += pSize;
	}

	void Memory::DeAllocation(uint32 pSize)
	{
		s_AllocatedNum -= 1;
		s_AllocatedSize -= pSize;

	}

	void* Memory::AllocateMemory(uint64 pSizeInBytes)
	{
		uint64 AvailMem = GetAvailableMemory();

		//Limit memory allocated size to 4% of available memory:
		ASSERT(uint32((float(pSizeInBytes) / float(AvailMem)) * 100.0f) >= 4, "Memory Limit");

		return (void*)(new uint8[pSizeInBytes]);
	}

	uint32 Memory::FreeMemory(void* pPtr)
	{
		delete[] pPtr;
		return HYD_OK;
	}


	uint64 Memory::GetAvailableMemory()
	{

#ifdef WINDOWS

		MEMORYSTATUSEX MemInfo;
		MemInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&MemInfo);

		return  MemInfo.ullAvailPhys;
#endif

	}


};



void* operator new(size_t pSize)
{
	Hydrogen::Memory::NewAllocation(pSize);
	return malloc(pSize);
}

void operator delete(void* pPtr, size_t pSize)
{
	if (pPtr == nullptr)
		return;
	Hydrogen::Memory::DeAllocation(pSize);
	free(pPtr);
}

void operator delete[](void* pPtr, size_t pSize)
{
	if (pPtr == nullptr)
		return;
	//Hydrogen::Memory::DeAllocation(pSize);
	free(pPtr);
}
