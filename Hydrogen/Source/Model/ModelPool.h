#pragma once

#include "Common.h"
#include "Geometry/Model.h"
#include "Core/ResourcePool.h"
#include "Log/Log.h"

#include <unordered_map>

namespace Hydrogen
{

	class ModelPool
	{
	public:

		//Init & Shutdown
		HYD uint32 InitPool();
		HYD uint32 ShutdownPool();

		//Single Data operations

		HYD Id		PushModel(Model** pModel);
		HYD uint32  PopModel(Id*  pId);


		HYD uint32 BakeTransform(const Transformation& pTransform);

		HYD uint32 RenderModels();

		HYD const std::unordered_map<Id, int32>::const_iterator begin() const;
		HYD const std::unordered_map<Id, int32>::const_iterator end() const;


		//Bulk Operations:
		HYD std::vector<Id> PushModels(std::vector<Model**>& pModel);
		HYD uint32			PopModels(std::vector<Id>& pIds);

		HYD Model& GetModel(Id pModel);
		HYD Model& operator[](Id pID);


	private:
		ResourcePool<Model>            m_Models;
	};


};