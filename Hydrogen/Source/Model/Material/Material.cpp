#include "Material.h"

namespace Hydrogen
{
	Material::~Material()
	{

	}

	Material::Material(Material && pOther)
	{
		m_BaseColorTexture        = pOther.m_BaseColorTexture;
		m_MetalicRoughnessTexture = pOther.m_MetalicRoughnessTexture;
		
		m_Name				= pOther.m_Name;
		m_MetalicnessFactor = pOther.m_MetalicnessFactor;
		m_RoughnessFactor   = pOther.m_RoughnessFactor;
		m_BaseColorFactor   = pOther.m_BaseColorFactor;
	
	}

	Material & Material::operator=(Material && pOther)
	{
		if (this != &pOther)
		{
			m_BaseColorTexture        = pOther.m_BaseColorTexture;
			m_MetalicRoughnessTexture = pOther.m_MetalicRoughnessTexture;

			m_Name				= pOther.m_Name;
			m_MetalicnessFactor = pOther.m_MetalicnessFactor;
			m_RoughnessFactor	= pOther.m_RoughnessFactor;
			m_BaseColorFactor	= pOther.m_BaseColorFactor;
		}
		return *this;
	}
	void Material::BindBaseColor(uint32 Slot) const
	{
		TextureHandler::BindTexture(m_BaseColorTexture, Slot);
	}
	
	void Material::BindMetalicColor(uint32 Slot) const
	{
		TextureHandler::BindTexture(m_MetalicRoughnessTexture, Slot);

	}
	
	void Material::UnbindBaseColor() const
	{
		TextureHandler::UnbindTexture(m_BaseColorTexture);

	}

	void Material::UnbindMetalicColor() const
	{
		TextureHandler::UnbindTexture(m_MetalicRoughnessTexture);

	}

};