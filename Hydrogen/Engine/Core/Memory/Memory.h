/*
	Memory.h
	Created by Pouya Alizadeh

	purpose:
		Memory.h aims to manage usage of memory in hydrogen and provide interfaces that can be used by other systems to work with memory.
*/


#pragma once

#include "../../Common.h"
#include <cstddef>


#ifdef TEST 
	#define HYD
#endif // TEST 



//void* operator new(size_t pSize);
//void  operator delete(void* pPtr, size_t pSize);
//void  operator delete[](void* pPtr, size_t pSize);


namespace Hydrogen
{


	template <typename Type>
	using Ptr = Type * ;
	template <typename Type>
	using Ref = Type&;





	/*
		Wraper holds and manages a heap allocated Object and makes sure it doesn't leak
	*/

	template <typename T>
	struct Wraper
	{
		T* m_Ptr = nullptr;

		Wraper()
			: m_Ptr(nullptr)
		{}

		Wraper(T* pPtr) 
			: m_Ptr(pPtr)
		{	}
	
		Wraper(const Wraper& pOther)			= delete;
		Wraper& operator=(const Wraper& pOther) = delete;
	
		//Move
		Wraper(Wraper&& pOther)
		{
			m_Ptr= pOther.m_Ptr;

			pOther.m_Ptr = nullptr;
		}
	
		Wraper& operator=(Wraper&& pOther)
		{
			m_Ptr = pOther.m_Ptr;
			pOther.m_Ptr = nullptr;
			
			return *this;
		}
	
		inline bool operator==(const Wraper& pOther) const {	return (pOther.m_Ptr == m_Ptr); }

		inline bool operator==(T* pOther) const { return (pOther == m_Ptr); }

		~Wraper(){
			delete m_Ptr;
		}

		void Delete(){
			delete m_Ptr;
			m_Ptr = nullptr;
		}
	
		T* operator->() { return m_Ptr; }

		//Ptr Setting:
		void Set(T* pPtr)
		{
			m_Ptr = pPtr;
		}
	
		void operator=(T* pPtr)
		{
			m_Ptr = pPtr;
		}
	
	};
	


	/*
		Buffer Objects: 
			
	*/


	struct Buffer
	{

	public:

		HYD  Buffer(); //Empty Buffer
		HYD  Buffer(Ptr<uint8> pData, uint64 pByteLength);
		HYD  Buffer(uint64 pByteLength); //Preallocate pByteLength size empty buffer
		HYD ~Buffer();

		HYD Buffer(const Buffer& pOther);
		HYD Buffer(Buffer&& pOther);
		
		HYD Buffer& operator=(const Buffer& pOther);
		HYD Buffer& operator=(Buffer&& pOther);

		HYD inline operator void*() const { return (void*)m_Ptr; }

		HYD inline uint8*     AccessPtr() {return m_Ptr;}
		HYD inline Ptr<uint8> GetPtr() const { return m_Ptr; }
		HYD inline uint64     Length() const { return m_ByteLength; }

		HYD Ptr<uint8> operator[](uint64 pOffset) const;
		HYD Ptr<uint8> GetData(uint64 pOffset=0)  const;


		HYD Buffer SubBuffer(uint64 pLength, uint64 pOffset=0);

		HYD void Free();
		HYD void Reset(); //Resets the m_Ptr & m_ByteLength. Does NOT deallocate anything!

	private:
		Ptr<uint8> m_Ptr        = nullptr;
		uint64     m_ByteLength = 0;
	};


	class Memory
	{
	public:


		//Gets Called at the end of the program to check if there are any unallocated memory
		static void CheckAllocation();


		static void NewAllocation(uint32 pSize);
		static void DeAllocation(uint32 pSize);

		template <typename T>
		static Ptr<T> AllocateObjects(uint64 pCounts)
		{
			return new T[pCounts];
		}

		template <typename T>
		static Wraper<T> Allocate()
		{
			return std::move(Wraper<T>(new T()));
		}

		template <typename T>
		static Ptr<T> AllocateRaw()
		{
			return new T();
		}

		HYD static void*	  AllocateMemory(uint64 pSizeInBytes);
		HYD static uint32     FreeMemory(void* pPtr);

		//Getters:
		HYD static inline uint32 GetAllocatedSize() { return s_AllocatedSize; }
		HYD static inline uint32 GetAllocatedNum() {  return s_AllocatedNum; }

		HYD static uint64 GetAvailableMemory();

	private:

		static uint32 s_AllocatedNum;
		static uint32 s_AllocatedSize;
	};


};