#include "Model.h"

namespace Hydrogen
{
	Model::Model()
	{
		SetupModel("UnamedModel", {});
	}

	Model::Model(const std::string & pName)
	{
		SetupModel(pName, {});
	}

	Model::Model(const std::string & pName, const std::vector<Mesh>& pMeshes)
	{
		SetupModel(pName, pMeshes);
	}

	Model::Model(const std::string & pName, const std::vector<Mesh>& pMeshes, const Matrix & pTransform)
	{
		SetupModel(pName, pMeshes);
		//SetTransform(pTransform);
	}

	Model::~Model()
	{
		DestroyModel();
	}

	Model::Model(const Model & pOther)
	{
		m_Transform = std::move(pOther.m_Transform);
		m_Meshes	= std::move(pOther.m_Meshes);
		m_Name		= pOther.m_Name;
	}

	Model::Model(Model && pOther)
	{
		m_Transform = std::move(pOther.m_Transform);
		m_Meshes    = std::move(pOther.m_Meshes);
		m_Name      = pOther.m_Name;

		pOther.m_Name.clear();
		pOther.m_Meshes.clear();
	}


	uint32 Model::SetupModel(const std::string & pName, const std::vector<Mesh>& pMeshes)
	{
		uint32 ErrorCode = HYD_OK;
		ErrorCode = SetName(pName);
		m_Meshes = std::move(pMeshes);

		return ErrorCode;
	}

	uint32 Model::DestroyModel()
	{
		m_Meshes.clear();
		return HYD_OK;
	}


	uint32 Model::SetTransform(const glm::mat4 & pTransform)
	{
		m_Transform = pTransform;
		return HYD_OK;
	}
	uint32 Model::SetTransform(Vec3 pScale, Vec3 pTranslate, Vec4 pRotate)
	{
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


	uint32 Model::RenderModel(const Shader & pShader)
	{
		for (auto &i : m_Meshes)
		{
			i.Render(pShader, &m_Transform, this);
		}
		return HYD_OK;
	}


	const Mesh & Model::GetMeshByIndex(uint32 pIndex) noexcept
	{
		return m_Meshes[pIndex];
	}

	const Mesh & Model::GetMeshByName(std::string pName)
	{
		return m_Meshes[0];
	}
};