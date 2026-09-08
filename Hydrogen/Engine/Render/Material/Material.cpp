#include "Xenon/include/Xenon.h"
#include "Material.h"
#include "HydPch.h"
#include "../Renderer.h"
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{

	
	Material Material::CreateMaterialGLTF(
		const Xenon::Material& 							 pMaterial,
		std::unordered_map<uint64, Instance<Texture2D>>& pTextureTable,
		HYD_ID_SPACE 		     						 pDescSetLayout,
		MaterialBinding	 								 pBindings
	) noexcept
	{
		Material NewMaterial;

		NewMaterial.m_BaseColorFactor    = pMaterial.GetBaseColorFactor();
		NewMaterial.m_RoughnessFactor    = pMaterial.GetRoughnessFactor();
		NewMaterial.m_MatallicnessFactor = pMaterial.GetMetallicFactor();

		NewMaterial.m_SetID = Renderer::Self().AllocateDescriptorSet(pDescSetLayout);
		auto& Set 	 	    = Renderer::Self().m_DescriptorSets.at(NewMaterial.m_SetID);
		
		if (pMaterial.HasBaseColor())
		{
			NewMaterial.m_BaseColor = pTextureTable[pMaterial.GetBaseColor().value()];

			for (auto& set : Set)
			{
				set.AttachTextureSampler(
				 	pBindings.BaseColorBinding,
					NewMaterial.m_BaseColor->m_Sampler,
				   	NewMaterial.m_BaseColor->m_View,
					HYD_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				);
			}
		}

		if (pMaterial.HasNormalMap())
		{
			NewMaterial.m_NormalMap = pTextureTable[pMaterial.GetNormalMap().value()];
			for (auto& set : Set)
			{
				set.AttachTextureSampler(
				 pBindings.NormalMapBinding,
				NewMaterial.m_NormalMap->m_Sampler,
				   NewMaterial.m_NormalMap->m_View,
				  HYD_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				);
			}
		}
		

		if (pMaterial.HasMetallicMap())
		{
			NewMaterial.m_MetallicMap = pTextureTable[pMaterial.GetMetallicMap().value()];
			
			for (auto& set : Set)
			{
				set.AttachTextureSampler(
				 	pBindings.MettallicRoughnessBinding,
					NewMaterial.m_MetallicMap->m_Sampler,
				    NewMaterial.m_MetallicMap->m_View,
				    HYD_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				);
			}
		}

		if (pMaterial.HasEmissiveMap())
		{
			NewMaterial.m_EmissiveMap = pTextureTable[pMaterial.GetEmissiveMap().value()];

			for (auto& set : Set)
			{
				set.AttachTextureSampler(
				 	pBindings.EmissiveBinding,
					NewMaterial.m_EmissiveMap->m_Sampler,
					NewMaterial.m_EmissiveMap->m_View,
				  	HYD_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				);
			}
		}

		if (pMaterial.HasOclusionMap())
		{
			NewMaterial.m_OcclusionMap = pTextureTable[pMaterial.GetOclusionMap().value()];

			for (auto& set : Set)
			{
				set.AttachTextureSampler(
				 	pBindings.OcolusionBinding,
					NewMaterial.m_OcclusionMap->m_Sampler,
				    NewMaterial.m_OcclusionMap->m_View,
				  	HYD_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				);
			}
		}

		for (auto& set : Set)
			set.UpdateDescriptorSet();

		NewMaterial.m_Binding = pBindings;

		return std::move(NewMaterial);
	}





	Material::Material(Material&& pOther)
	{
		m_BaseColor    	     = std::move(pOther.m_BaseColor);
		m_MetallicMap  	     = std::move(pOther.m_MetallicMap);
		m_NormalMap    	     = std::move(pOther.m_NormalMap);
		m_MatallicnessFactor = pOther.m_MatallicnessFactor;
    	m_RoughnessFactor    = pOther.m_RoughnessFactor;
    	m_BaseColorFactor    = pOther.m_BaseColorFactor;

		m_SetID 			 = pOther.m_SetID;
		m_Binding  			 = pOther.m_Binding;
	}

	Material& Material::operator=(Material&& pOther)
	{
		if (this == &pOther)
			return *this;

		m_BaseColor    	     = std::move(pOther.m_BaseColor);
		m_MetallicMap  	     = std::move(pOther.m_MetallicMap);
		m_NormalMap    	     = std::move(pOther.m_NormalMap);
		m_MatallicnessFactor = pOther.m_MatallicnessFactor;
    	m_RoughnessFactor    = pOther.m_RoughnessFactor;
    	m_BaseColorFactor    = pOther.m_BaseColorFactor;
		
		m_SetID 	= pOther.m_SetID;
		m_Binding  	= pOther.m_Binding;

		return *this;
	}

	
	void Material::Bind(
		const Internal::Vulkan::PipelineLayout& pLayout
	) noexcept
	{
		if (!m_SetID)
			return;
		Renderer::Self().AccessDescriptorSet(m_SetID).Bind(pLayout, m_Binding.Set);
	}

};