#include "Xenon/include/Xenon.h"
#include "Material.h"
#include "HydPch.h"
#include "Render/Renderer.h"
#include "Image.h"

namespace Hydrogen
{

	Material Material::CreateMaterialGLTF(const Xenon::Material& pMaterial, std::unordered_map<uint64, Instance<Texture2D>>& pTextureTable)
	{
		Material NewMaterial;

		if (pMaterial.HasBaseColor())
		{
			NewMaterial.m_BaseColor = pTextureTable[pMaterial.GetBaseColor().value()];
		}

		if (pMaterial.HasNormalMap())
		{
			NewMaterial.m_NormalMap = pTextureTable[pMaterial.GetNormalMap().value()];
		}

		return std::move(NewMaterial);
	}




	Material::Material()
	{
		
	}

	Material::~Material()
	{
		
	}



	Material::Material(Material&& pOther)
	{
		m_BaseColor    = std::move(pOther.m_BaseColor);
		m_MetallicMap  = std::move(pOther.m_MetallicMap);
		m_NormalMap    = std::move(pOther.m_NormalMap);
		m_MatallicnessFactor = pOther.m_MatallicnessFactor;
		
	}

	Material& Material::operator=(Material&& pOther)
	{
		if (this == &pOther)
			return *this;

		m_BaseColor   = std::move(pOther.m_BaseColor);
		m_MetallicMap = std::move(pOther.m_MetallicMap);
		m_NormalMap   = std::move(pOther.m_NormalMap);
	
		return *this;
	}



	uint32 Material::SetMetallicnessFactor(float pMetalicnessFactor)
	{
		m_MatallicnessFactor = pMetalicnessFactor;
		return HYD_OK;
	}

	uint32 Material::SetRoughnessFactor(float pRoughnessFactor)
	{
		m_RoughnessFactor = pRoughnessFactor;
		return HYD_OK;
	}

	/*
	uint32 Material::Bind(Instance<Program>  pShader)
	{
		//pShader.SetUniformInt1("material.BaseColor", 0);
		//pShader.SetUniformInt1("material.NormalMap", 1);
		//pShader.SetUniformInt1("material.Metalic",   2);
		//
		//pShader.SetUniformFloat1("material.Metallicness", m_MatallicnessFactor);
		//pShader.SetUniformFloat1("material.Roughness",   m_RoughnessFactor);
		//pShader.SetUniformFloat3("material.BaseColorFactor", m_BaseColorFactor.X, m_BaseColorFactor.Y, m_BaseColorFactor.Z);

		if (!m_BaseColor.IsNull())
			m_BaseColor->Bind(0);
		if (!m_NormalMap.IsNull())
			m_NormalMap->Bind(1);
		//m_MetallicMap->Bind(2);

		return HYD_OK;
	}
*/
	uint32 Material::Unbind()
	{
		if (!m_BaseColor.IsNull())
			m_BaseColor->Unbind(0);
		if (!m_NormalMap.IsNull())
			m_NormalMap->Unbind(1);
		//m_MetallicMap->Unbind(2);
		return HYD_OK;
	}

};