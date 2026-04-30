#pragma once

#include "Common.h"
#include <vector>
#include "Log/Log.h"

#ifdef TEST
	#define HYD 
	#define ASSERT(x)  
#endif

namespace Hydrogen
{


	typedef uintptr_t Id;

	template <typename ResourceType>
	class ResourcePool
	{
	public:

		static ResourceType* New();
		static uint32        Delete(ResourceType* pRes);

		static uint32 GetAllocations();

	private:

		inline static uint32 s_Allocated=0;

	public:

		 ResourcePool() = default;
		~ResourcePool();

		ResourcePool(ResourcePool&& pOther);
		ResourcePool(const ResourcePool& pOther) = delete;

		ResourcePool& operator=(ResourcePool&& pOther);
		ResourcePool& operator=(const ResourcePool& pOther) = delete;
		

		bool Exists(Id pId);

		Id      Push(ResourceType* pResource);
		Id		Push(Wraper<ResourceType>& pResource);
		uint32  Pop(Id* pId);

		ResourceType& GetResource(Id pID);
		ResourceType& operator[](Id pID);

		const std::unordered_map<Id, int32>::const_iterator begin() const { return m_IdTable.begin(); }
		const std::unordered_map<Id, int32>::const_iterator end()   const { return   m_IdTable.end(); }


		uint32 Clear();

		inline const std::unordered_map<Id, int32>& GetTable() noexcept { return m_IdTable; }

	private:
		std::unordered_map<Id, int32> m_IdTable;
	};



	template<typename ResourceType>
	inline ResourceType* ResourcePool<ResourceType>::New()
	{
		s_Allocated += 1;
		return new ResourceType;
	}

	template<typename ResourceType>
	inline uint32 ResourcePool<ResourceType>::Delete(ResourceType* pRes)
	{
		if (pRes == nullptr)
			return -1;

		s_Allocated -= 1;
		delete pRes;
		return 0;
	}

	template<typename ResourceType>
	inline uint32 ResourcePool<ResourceType>::GetAllocations(){
		return s_Allocated;
	}

	template<typename ResourceType>
	inline ResourcePool<ResourceType>::~ResourcePool()
	{
		Clear();
	}

	template<typename ResourceType>
	inline ResourcePool<ResourceType>::ResourcePool(ResourcePool&& pOther)
	{
		m_IdTable = std::move(pOther.m_IdTable);
		pOther.m_IdTable.clear();
	}

	template<typename ResourceType>
	inline ResourcePool<ResourceType>& ResourcePool<ResourceType>::operator=(ResourcePool<ResourceType>&& pOther)
	{
		if (this != &pOther)
		{
			m_IdTable = std::move(pOther.m_IdTable);
			pOther.m_IdTable.clear();
		}
		return *this;
	}
	template<typename ResourceType>
	inline bool ResourcePool<ResourceType>::Exists(Id pId)
	{
		return (m_IdTable.find(pId) != m_IdTable.end());
	}
	//------------------Creation & Destruction-----------------------------------
	template<typename ResourceType>
	inline Id ResourcePool<ResourceType>::Push(ResourceType* pResource)
	{
		Id ID = reinterpret_cast<Id>(pResource);
		m_IdTable.emplace(std::pair<Id, int32>(ID, 0));
		return ID;
	}

	template<typename ResourceType>
	inline Id ResourcePool<ResourceType>::Push(Wraper<ResourceType>& pResource)
	{
		Id ID = reinterpret_cast<Id>(pResource.Ptr);
		pResource.Set(nullptr);
		m_IdTable.emplace(std::pair<Id, int32>(ID, 0));
		return ID;
	}

	template<typename ResourceType>
	inline uint32 ResourcePool<ResourceType>::Pop(Id* pId)
	{

		if (m_IdTable.find(*pId) == m_IdTable.end())
		{
			//Id doesn't exist
			return HYD_INVALID_VALUE;
		}

		m_IdTable[*pId] = -1;
		Delete(reinterpret_cast<ResourceType*>(*pId));
		*pId = 0;
		return HYD_OK;
	}

	template<typename ResourceType>
	inline  ResourceType& ResourcePool<ResourceType>::GetResource(Id pID)
	{
		
		ASSERT(m_IdTable.find(pID) == m_IdTable.end(), "Invalid Resource");
		ASSERT(m_IdTable[pID] == -1, "Invalid ID");
		m_IdTable[pID] += 1;
		return *(reinterpret_cast<ResourceType*>(pID));
	}

	template<typename ResourceType>
	inline ResourceType& ResourcePool<ResourceType>::operator[](Id pID)
	{
		return GetResource(pID);
	}

//-------------------------------------------------------------------------

	template<typename ResourceType>
	inline uint32 ResourcePool<ResourceType>::Clear()
	{
		for (auto& Id : m_IdTable)
		{
			if (Id.first != -1)
				Delete(reinterpret_cast<ResourceType*>(Id.first));
		}
		m_IdTable.clear();
		return HYD_OK;
	}


};