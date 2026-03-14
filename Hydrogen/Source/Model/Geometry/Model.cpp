#include "Model.h"

namespace Hydrogen
{
	Model::Model(const std::string& pName, const Transform& pTransform )
	{
		m_Name      = pName;
		m_ModelMatrix = CalculateMatrix(pTransform);
	}

	Model::~Model()
	{
		DestroyModel();
	}


	Model::Model(Model&& pOther)
	{
		m_ModelMatrix = std::move(pOther.m_ModelMatrix);
		m_Meshes    = std::move(pOther.m_Meshes);
		m_Name      = std::move(pOther.m_Name);
	}

	Model& Model::operator=(Model&& pOther)
	{
		if (this != &pOther)
		{
			m_ModelMatrix = std::move(pOther.m_ModelMatrix);
			m_Meshes = std::move(pOther.m_Meshes);
			m_Name = std::move(pOther.m_Name);

		}

		return *this;
	}

	uint32 Model::SetupModel(const std::string& pName, const Transform& pTransform, std::vector<Mesh*>& pMeshes)
	{
		uint32 ErrorCode = HYD_OK;
		ErrorCode = SetName(pName);
		ErrorCode = SetTransform(pTransform);

		for (auto& mesh : pMeshes)
		{
			AddMesh(&mesh);
		}

		return ErrorCode;
	}

	uint32 Model::DestroyModel()
	{
		m_Meshes.ShutdownPool();
		return HYD_OK;
	}



	uint32 Model::SetTransform(const Transform& pTransform)
	{
		m_ModelMatrix = m_ModelMatrix*CalculateMatrix(pTransform);
		return HYD_OK;
	}

	uint32 Model::SetTransform(Vec3 pScale, Vec3 pTranslate, Vec4 pRotate)
	{
		return HYD_OK;
	}


	uint32 Model::BakeTransform(Transform& pTransform)
	{
		m_Meshes.BakeTransform(pTransform);
		return HYD_OK;
	}

	uint32 Model::RenderModel()
	{
		m_Meshes.RenderMeshes(m_ModelMatrix);
		return HYD_OK;
	}


	uint32 Model::SetName(std::string pName)
	{
		if (pName.size() == 0)
		{
			Log::SetError("Model name cannot be empty", HYD_INVALID_VALUE, __FILE__, __LINE__);
			return HYD_INVALID_VALUE;
		}
		return HYD_OK;
	}




	Mesh& Model::GetMesh(const std::string& pName)
	{
		return m_Meshes.GetMesh(pName);
	}

	Mesh& Model::GetMesh(const Id pId)
	{
		return m_Meshes.GetMesh(pId);
	}

	Mesh& Model::operator[](const std::string & pName)
	{
		return m_Meshes.GetMesh(pName);
	}

	Mesh& Model::operator[](const Id pId)
	{
		return m_Meshes.GetMesh(pId);
	}

	Id Model::AddMesh(Mesh** pMesh)
	{
		return m_Meshes.PushMesh(pMesh);
	}

	uint32 Model::PopMesh(Id* pMesh)
	{
		return m_Meshes.PopMesh(pMesh);
	}

};