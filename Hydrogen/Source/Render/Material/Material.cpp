#include "Material.h"
#include "HydPch.h"



namespace Hydrogen
{



	Material::Material()
	{
		m_TexturePool.InitPool();
	}

	Material::~Material()
	{
		m_TexturePool.ShutdownPool();
	}

	Material::Material(Material&& pOther)
	{
		m_TexturePool = std::move(pOther.m_TexturePool);
		m_BaseColor   = pOther.m_BaseColor;
		m_MetallicMap  = pOther.m_MetallicMap;
		m_NormalMap   = pOther.m_NormalMap;

		pOther.m_TexturePool.ShutdownPool();
		pOther.m_BaseColor  = 0;
		pOther.m_MetallicMap = 0;
		pOther.m_NormalMap  = 0;

	}

	Material& Material::operator=(Material&& pOther)
	{
		if (this != &pOther)
		{
			m_TexturePool = std::move(pOther.m_TexturePool);
			m_BaseColor = pOther.m_BaseColor;
			m_MetallicMap = pOther.m_MetallicMap;
			m_NormalMap = pOther.m_NormalMap;

			pOther.m_TexturePool.ShutdownPool();
			pOther.m_BaseColor  = 0;
			pOther.m_MetallicMap = 0;
			pOther.m_NormalMap  = 0;
		}
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



	uint32 Material::SetBaseColorTexture(Texture** pBaseColor)
	{
		if (pBaseColor == nullptr)
			return HYD_INVALID_VALUE;
		if (*pBaseColor == nullptr)
			return HYD_INVALID_VALUE;

		m_BaseColor = m_TexturePool.PushTexture(pBaseColor);
		return HYD_OK;
	}

	uint32 Material::SetNormalMapTexture(Texture** pNormalMap)
	{
		if (pNormalMap == nullptr)
			return HYD_INVALID_VALUE;
		if (*pNormalMap == nullptr)
			return HYD_INVALID_VALUE;

		m_NormalMap = m_TexturePool.PushTexture(pNormalMap);
	}

	uint32 Material::SetMetallicTexture(Texture** pMetallic)
	{
		if (pMetallic == nullptr)
			return HYD_INVALID_VALUE;
		if (*pMetallic == nullptr)
			return HYD_INVALID_VALUE;

		m_MetallicMap = m_TexturePool.PushTexture(pMetallic);
	}

	uint32 Material::Bind(const Shader& pShader)
	{
		pShader.SetUniformInt1("material.BaseColor", 0);
		pShader.SetUniformInt1("material.NormalMap", 1);
		pShader.SetUniformInt1("material.Metalic",   2);

		pShader.SetUniformFloat1("material.Metallicness", m_MatallicnessFactor);
		pShader.SetUniformFloat1("material.Roughness",   m_RoughnessFactor);

		m_TexturePool.BindTexture(m_BaseColor,  0);
		m_TexturePool.BindTexture(m_NormalMap,  1);
		m_TexturePool.BindTexture(m_MetallicMap, 2);


		return HYD_OK;
	}

	uint32 Material::Unbind()
	{
		m_TexturePool.UnbindTexture(m_BaseColor,  0);
		m_TexturePool.UnbindTexture(m_NormalMap,  1);
		m_TexturePool.UnbindTexture(m_MetallicMap, 2);
		return HYD_OK;
	}

};