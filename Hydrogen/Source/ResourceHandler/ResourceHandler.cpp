#include "ResourceHandler.h"

namespace Hydrogen
{

	LinkedList<Resource>         ResourceHandler::m_ResoucePool;
	std::unordered_map<Id, bool> ResourceHandler::m_IdTable;

	uint32 ResourceHandler::InitHandler()
	{
		Log::SetInfo("Resource Handler: Ready");
		return HYD_OK;
	}

	uint32 ResourceHandler::ShutdownHandler()
	{
		Log::SetInfo("Resource Handler: Shutdown");
		ClearAll();
		return HYD_OK;
	}



	Id ResourceHandler::PushResource(Resource* pResource)
	{
		if (pResource == nullptr)
			return 0;

		m_ResoucePool.AddTail(pResource);
		Id ID = reinterpret_cast<Id>(pResource);
		m_IdTable[ID] = true;
		return ID;
	}

	Id ResourceHandler::PopResource(Id* pResource)
	{
		if (pResource == nullptr || *pResource == 0)
			return 0;

		Resource* resource = GetResource(*pResource);
		m_ResoucePool.Remove(&resource);

		*pResource = 0;
		return *pResource;
	}

	Resource* ResourceHandler::GetResource(Id pResource)
	{
		if (pResource == 0)
			return nullptr;

		Resource* resource = reinterpret_cast<Resource*>(pResource);
		return resource;
	}

	uint32 ResourceHandler::ClearAll()
	{
		uint32 ErrorCode = m_ResoucePool.Clear();
		for (auto &pair : m_IdTable)
		{
			pair.second = false;
		}
		return ErrorCode;
	}


	bool ResourceHandler::IsEmpty()
	{
		return (m_ResoucePool.GetSize() == 0)? true : false;
	}

};