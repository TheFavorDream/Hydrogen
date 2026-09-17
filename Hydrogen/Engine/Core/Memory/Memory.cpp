#include "Memory.h"
#include "Log/Log.h"

#include <cstddef>
#include <iostream>


namespace Hydrogen
{


	 Buffer::Buffer():
	  m_Ptr(nullptr), m_ByteLength(0)
	 {}

	 Buffer::Buffer(Ptr<uint8> pData, uint64 pByteLength):
	  m_Ptr(pData), m_ByteLength(pByteLength)
	 {}

	 Buffer::Buffer(uint64 pByteLength)
	  : m_Ptr(nullptr), m_ByteLength(pByteLength)
	 {
		 m_Ptr = (uint8*)Memory::AllocateMemory(m_ByteLength);
		 memset(m_Ptr, 0, m_ByteLength);
	 }

	 Buffer::~Buffer()
	 {
		 if (m_Ptr)
		 {
			 delete[] m_Ptr;
			 m_ByteLength = 0;
		 }
	 }

	 Buffer::Buffer(const Buffer& pOther):
		 m_ByteLength(pOther.m_ByteLength), m_Ptr(nullptr)
	 {

		 if (m_ByteLength != 0)
		 {
			m_Ptr = (uint8*)Memory::AllocateMemory(m_ByteLength);
			memcpy((void*)m_Ptr, (void*)pOther.m_Ptr, m_ByteLength);
		 }
	 }

	 Buffer::Buffer(Buffer&& pOther)
	 {
		 m_Ptr		  = pOther.m_Ptr;
		 m_ByteLength = pOther.m_ByteLength;

		 pOther.m_Ptr		 = nullptr;
		 pOther.m_ByteLength = 0;
	 }
	 
	 Buffer& Buffer::operator=(const Buffer& pOther) 
	 {
		 if (&pOther == this)
			 return *this;

		 if (pOther.m_ByteLength == 0)
			 return *this;

		 m_ByteLength = pOther.m_ByteLength;

		 m_Ptr = (uint8*)Memory::AllocateMemory(m_ByteLength);
		 memcpy((void*)m_Ptr, (void*)pOther.m_Ptr, m_ByteLength);

		return *this;
	 }

	 Buffer& Buffer::operator=(Buffer&& pOther)
	 {
		 if (&pOther == this)
			 return *this;

		 m_Ptr		  = pOther.m_Ptr;
		 m_ByteLength = pOther.m_ByteLength;

		 pOther.m_Ptr		 = nullptr;
		 pOther.m_ByteLength = 0;
		return *this;
	 }


	Ptr<uint8> Buffer::operator[](uint64 pOffset) const 
	{
		ASSERT(pOffset < m_ByteLength, "Out of Buffer Bound");
		return (m_Ptr + pOffset);
	}

	Ptr<uint8> Buffer::GetData(uint64 pOffset) const 
	{
		ASSERT(pOffset < m_ByteLength, "Out of Buffer Bound");
		return (m_Ptr + pOffset);
	}


	//TODO: Write this
	Buffer Buffer::SubBuffer(uint64 pLength, uint64 pOffset)
	{
		return Buffer();
	}

	void Buffer::Free()
	{
		if (m_Ptr)
		{
			delete[] m_Ptr;
			m_ByteLength = 0;
		}
	}

	void Buffer::Reset() //Resets the m_Ptr & m_ByteLength. Does NOT deallocate anything!
	{
		m_Ptr 		 = nullptr; 
		m_ByteLength = 0;
	}









	uint32 Memory::s_AllocatedNum  = 0;
	uint32 Memory::s_AllocatedSize = 0;

	void Memory::CheckAllocation()
	{
		//ASSERT(s_AllocatedNum != 0, "Not All Memory has been Freed!");
		//Log::SetWarning(Log::FmtStr("Remaining Memory:%i KB", s_AllocatedSize/1024));
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
		//uint64 AvailMem = GetAvailableMemory();


		return (void*)(new uint8[pSizeInBytes]);
	}

	uint32 Memory::FreeMemory(void* pPtr)
	{
		delete[] pPtr;
		return HYD_OK;
	}


	uint64 Memory::GetAvailableMemory()
	{

#ifdef HYD_WINDOWS

		MEMORYSTATUSEX MemInfo;
		MemInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&MemInfo);

		return  MemInfo.ullAvailPhys;
#endif

#ifdef HYD_LINUX

		

#endif

return 0; //if we got here, we're coocked.
	}


};



//void* operator new(size_t pSize)
//{
//	Hydrogen::Memory::NewAllocation(pSize);
//	return malloc(pSize);
//}
//
//void operator delete(void* pPtr, size_t pSize)
//{
//	if (pPtr == nullptr)
//		return;
//	Hydrogen::Memory::DeAllocation(pSize);
//	free(pPtr);
//}
//
//void operator delete[](void* pPtr, size_t pSize)
//{
//	if (pPtr == nullptr)
//		return;
//	//Hydrogen::Memory::DeAllocation(pSize);
//	free(pPtr);
//}
