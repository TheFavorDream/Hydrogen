#include "MeshPool.h"
#include "HydPch.h"


namespace Hydrogen
{

	uint32 MeshPool::InitPool()
	{

		return HYD_OK;
	}

	uint32 MeshPool::ShutdownPool()
	{
		m_MeshePool.Clear();
		return HYD_OK;
	}


//Mesh Creation:

	Id MeshPool::CreateMesh(const std::string& pName, std::vector<Primitive>&& pPrimitives)
	{
		Mesh* NewMesh = ResourcePool<Mesh>::New();
		NewMesh->SetMesh(pName, std::move(pPrimitives));
		return m_MeshePool.Push(NewMesh);
	}

	Id MeshPool::PushMesh(Mesh** pMesh)
	{
		Id New = m_MeshePool.Push(*pMesh);
		*pMesh = nullptr;

		return New;
	}

	uint32 MeshPool::PopMesh(Id* pId)
	{
		return m_MeshePool.Pop(pId);
	}

	uint32 MeshPool::BakeTransform(const Transformation& pTransform)
	{
		for (auto& mesh : m_MeshePool.GetTable())
		{
			m_MeshePool[mesh.first].SetTransform( pTransform * m_MeshePool[mesh.first].GetTransform());
		}
		return HYD_OK;
	}

	uint32 MeshPool::RenderMeshes(Transformation& pTransform)
	{

		for (auto &meshID : m_MeshePool.GetTable())
		{
			m_MeshePool.GetResource(meshID.first).Render(pTransform);
		}
		return HYD_OK;
	}



//Get Methods:
	Mesh& MeshPool::GetMesh(const std::string& pName) noexcept
	{
		
		ASSERT(m_MeshTable.find(pName) == m_MeshTable.end(), "Invalid Mesh Name");
		return m_MeshePool.GetResource(m_MeshTable[pName]);
	}

	Mesh& MeshPool::GetMesh(const Id pId) noexcept
	{
		return m_MeshePool.GetResource(pId);
	}

	const std::unordered_map<Id, int32>::const_iterator MeshPool::begin() const
	{
		return m_MeshePool.begin();
	}

	const std::unordered_map<Id, int32>::const_iterator MeshPool::end() const
	{
		return m_MeshePool.end();
	}


	Mesh& MeshPool::operator[](const std::string& pName)
	{
		return GetMesh(pName);
	}

	Mesh& MeshPool::operator[](const Id pId)
	{
		return GetMesh(pId);
	}



	MeshPool::MeshPool(MeshPool&& pOther)
	{
		m_MeshePool = std::move(pOther.m_MeshePool);
		m_MeshTable = std::move(pOther.m_MeshTable);

		pOther.m_MeshePool.Clear();
		pOther.m_MeshTable.clear();
	}

	MeshPool& MeshPool::operator=(MeshPool&& pOther)
	{
		if (this != &pOther)
		{
			m_MeshePool = std::move(pOther.m_MeshePool);
			m_MeshTable = std::move(pOther.m_MeshTable);

			pOther.m_MeshePool.Clear();
			pOther.m_MeshTable.clear();
		}
		return *this;
	}


};