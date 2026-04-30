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



void* operator new(size_t pSize);
void  operator delete(void* pPtr, size_t pSize);
void  operator delete[](void* pPtr, size_t pSize);


namespace Hydrogen
{

	/*
		Wraper holds and manages a heap allocated Object and makes sure it doesn't get leaked
	*/

	template <typename T>
	struct Wraper
	{
		T* Ptr = nullptr;

		HYD Wraper()
			: Ptr(nullptr)
		{
		}

		HYD Wraper(T* pPtr) 
			: Ptr(pPtr)
		{

		}
	
		HYD Wraper(const Wraper& pOther) = delete;
		HYD Wraper& operator=(const Wraper& pOther) = delete;
	
		//Move
		HYD Wraper(Wraper&& pOther)
		{
			Ptr = pOther.Ptr;
			pOther.Ptr = nullptr;
		}
	
		HYD Wraper& operator=(Wraper&& pOther)
		{
			Ptr = pOther.Ptr;
			pOther.Ptr = nullptr;
			return *this;
		}
	
		HYD ~Wraper()
		{
			delete Ptr;
		}

		HYD void Delete()
		{
			delete Ptr;
		}
	
		//Ptr Setting:
		HYD void Set(T* pPtr)
		{
			Ptr = pPtr;
		}
	
		HYD void operator=(T* pPtr)
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



		HYD static void*	  AllocateMemory(uint64 pSizeInBytes);

		template <class Ty>
		static Ty* AllocateArray(uint64 pSize)
		{
			return new Ty[pSize];
		}

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