#include "Material.h"

namespace Hydrogen
{
	Material::~Material()
	{
		m_BaseColor.DestroyTexture();
		m_MetalicRoughness.DestroyTexture();
	}

	Material::Material(Material && pOther)
	{
		m_BaseColor        = std::move(pOther.m_BaseColor);
		m_MetalicRoughness = std::move(pOther.m_MetalicRoughness);
		
		m_Name				= pOther.m_Name;
		m_MetalicnessFactor = pOther.m_MetalicnessFactor;
		m_RoughnessFactor   = pOther.m_RoughnessFactor;
		m_BaseColorFactor   = pOther.m_BaseColorFactor;
	
	}

	Material & Material::operator=(Material && pOther)
	{
		if (this != &pOther)
		{
			m_BaseColor        = std::move(pOther.m_BaseColor);
			m_MetalicRoughness = std::move(pOther.m_MetalicRoughness);

			m_Name				= pOther.m_Name;
			m_MetalicnessFactor = pOther.m_MetalicnessFactor;
			m_RoughnessFactor	= pOther.m_RoughnessFactor;
			m_BaseColorFactor	= pOther.m_BaseColorFactor;
		}
		return *this;
	}
	void Material::BindBaseColor(uint32 Slot)
	{
		m_BaseColor.Bind(Slot);
	}
	
	void Material::BindMetalicColor(uint32 Slot)
	{
		m_MetalicRoughness.Bind(Slot);
	}
	
	void Material::UnbindBaseColor()
	{
		m_BaseColor.Unbind();
	}

	void Material::UnbindMetalicColor()
	{
		m_MetalicRoughness.Unbind();
	}

};