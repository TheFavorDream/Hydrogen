/*

	Global Resource Handler:
	Global Resource Handler is a system designed to store objects that needs to be shared globally.
	Every object has an unique ID associate with it.
*/


#pragma once

#include "Common.h"
#include "Log/Log.h"
#include "Misc/LinkedList.h"

namespace Hydrogen
{

	typedef uintptr_t Id;
	
	
	//Base class for every resource
	class Resource
	{
	public:
		virtual ~Resource() = default;

	public:
		Resource* Prev = nullptr;
		Resource* Next = nullptr;
	};
	
	
	class ResourceHandler
	{
	public:

		
		HYD static uint32 InitHandler();
		HYD static uint32 ShutdownHandler();

		HYD static Id		 PushResource(Resource* pResource);
		HYD static Id		 PopResource(Id* pResource);
		HYD static Resource* GetResource(Id pResource);

		HYD static uint32 ClearAll();

		HYD static bool IsEmpty();

	private:
		HYD static LinkedList<Resource> m_ResoucePool;
		HYD static std::unordered_map<Id, bool> m_IdTable;
	};

};