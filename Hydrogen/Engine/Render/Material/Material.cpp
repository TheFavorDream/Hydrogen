#include "Material.h"
#include "HydPch.h"



namespace Hydrogen
{



	Material::Material()
	{
		m_TexturePool.Init();
	}

	Material::~Material()
	{
		m_TexturePool.Shutdown();
	}

	Material::Material(Material&& pOther)
	{
		m_TexturePool  = std::move(pOther.m_TexturePool);
		m_BaseColor    = std::move(pOther.m_BaseColor);
		m_MetallicMap  = std::move(pOther.m_MetallicMap);
		m_NormalMap    = std::move(pOther.m_NormalMap);
		m_MatallicnessFactor = pOther.m_MatallicnessFactor;
		
	}

	Material& Material::operator=(Material&& pOther)
	{
		if (this == &pOther)
			return *this;

		m_TexturePool = std::move(pOther.m_TexturePool);
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



	uint32 Material::SetBaseColorTexture(Texture2D& pBaseColor)
	{
		m_BaseColor = m_TexturePool.PushObject(std::move(pBaseColor));
		return HYD_OK;
	}

	uint32 Material::SetNormalMapTexture(Texture2D&   pNormalMap)
	{
		m_NormalMap = m_TexturePool.PushObject(std::move(pNormalMap));
		return HYD_OK;

	}

	uint32 Material::SetMetallicTexture(Texture2D&   pMetallic)
	{
		m_MetallicMap = m_TexturePool.PushObject(std::move(pMetallic));
		return HYD_OK;
	}

	uint32 Material::SetEmissiveTexture(Texture2D&   pEmissive)
	{
		m_EmissiveMap = m_TexturePool.PushObject(std::move(pEmissive));
		return HYD_OK;
	}

	uint32 Material::SetOcclusionTexture(Texture2D& pOcclusion)
	{
		m_OcclusionMap = m_TexturePool.PushObject(std::move(pOcclusion));
		return HYD_OK;
	}



	uint32 Material::Bind(const Shader& pShader)
	{
		pShader.SetUniformInt1("material.BaseColor", 0);
		pShader.SetUniformInt1("material.NormalMap", 1);
		pShader.SetUniformInt1("material.Metalic",   2);

		pShader.SetUniformFloat1("material.Metallicness", m_MatallicnessFactor);
		pShader.SetUniformFloat1("material.Roughness",   m_RoughnessFactor);
		pShader.SetUniformFloat3("material.BaseColorFactor", m_BaseColorFactor.X, m_BaseColorFactor.Y, m_BaseColorFactor.Z);

		m_BaseColor->Bind(0);
		m_NormalMap->Bind(1);
		m_MetallicMap->Bind(2);

		return HYD_OK;
	}

	uint32 Material::Unbind()
	{
		m_BaseColor->Unbind(0);
		m_NormalMap->Unbind(1);
		m_MetallicMap->Unbind(2);
		return HYD_OK;
	}

};