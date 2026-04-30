#include "ModelPool.h"
#include "HydPch.h"


namespace Hydrogen
{



	uint32 ModelPool::InitPool()
	{
		Log::SetInfo("Model Pool: Initialized");
		return HYD_OK;
	}

	uint32 ModelPool::ShutdownPool()
	{
		
		Log::SetInfo("Model Pool: Shutdown");
		return m_Models.Clear();
	}

//Single Operation

	Id ModelPool::PushModel(Model** pModel)
	{
		return m_Models.Push(*pModel);
		*pModel = nullptr;
	}


	uint32 ModelPool::PopModel(Id* pId)
	{
		return m_Models.Pop(pId);
	}

	uint32 ModelPool::BakeTransform(const Transformation& pTransform)
	{
		for (auto& model : m_Models) 
		{
			m_Models.GetResource(model.first).BakeTransform(pTransform);
		}
		return HYD_OK;
	}

	uint32 ModelPool::RenderModels()
	{
		for (auto& model : m_Models)
		{
			m_Models.GetResource(model.first).RenderModel();
		}
		return HYD_OK;
	}

	const std::unordered_map<Id, int32>::const_iterator ModelPool::begin() const
	{
		return m_Models.begin(); 
	}

	const std::unordered_map<Id, int32>::const_iterator ModelPool::end() const
	{
		return m_Models.end();
	}

	Model& ModelPool::operator[](Id pID)
	{
		return m_Models.GetResource(pID);
	}



//Bulk Operations:

	std::vector<Id> ModelPool::PushModels(std::vector<Model**>& pModel)
	{
		std::vector<Id> Models;
		for (auto& model : pModel)
		{
			Models.push_back(PushModel(model));
		}
		return Models;
	}

	uint32 ModelPool::PopModels(std::vector<Id>& pIds)
	{
		for (auto& id : pIds)
		{
			PopModel(&id);
			id = 0;
		}
		return HYD_OK;
	}

	Model& ModelPool::GetModel(Id pModel)
	{
		return m_Models.GetResource(pModel);
	}






};