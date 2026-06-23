/*
	Memory.h
	Created by Pouya Alizadeh

	purpose:
		Memory.h aims to manage usage of memory in hydrogen and provide interfaces that can be used by other systems to work with memory.
*/


#pragma once

#include "Common.h"


#ifdef TEST 
	#define HYD
#endif // TEST 



//void* operator new(size_t pSize);
//void  operator delete(void* pPtr, size_t pSize);
//void  operator delete[](void* pPtr, size_t pSize);


namespace Hydrogen
{

	/*
		Wraper holds and manages a heap allocated Object and makes sure it doesn't leak
	*/

	template <typename T>
	struct Wraper
	{
		T* Ptr = nullptr;

		Wraper()
			: Ptr(nullptr)
		{
		}

		Wraper(T* pPtr) 
			: Ptr(pPtr)
		{

		}
	
		Wraper(const Wraper& pOther) = delete;
		Wraper& operator=(const Wraper& pOther) = delete;
	
		//Move
		Wraper(Wraper&& pOther)
		{
			Ptr = pOther.Ptr;
			pOther.Ptr = nullptr;
		}
	
		Wraper& operator=(Wraper&& pOther)
		{
			Ptr = pOther.Ptr;
			pOther.Ptr = nullptr;
			return *this;
		}
	
		bool operator==(const Wraper& pOther) const
		{
			return (pOther.Ptr == Ptr);
		}

		bool operator==(T* pOther) const
		{
			return (pOther == Ptr);
		}



		~Wraper()
		{
			delete Ptr;
		}

		void Delete()
		{
			delete Ptr;
			Ptr = nullptr;
		}
	
		T* operator->()
		{
			return Ptr;
		}

		//Ptr Setting:
		void Set(T* pPtr)
		{
			Ptr = pPtr;
		}
	
		void operator=(T* pPtr)
		{
			Ptr = pPtr;
		}
	
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