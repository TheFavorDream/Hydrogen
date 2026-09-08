#pragma once

#include "../../Common.h"
#include "../../Core/ResourcePool.h"
#include "Texture.h"


//#include "Image.h"

namespace Hydrogen
{


	struct MaterialBinding
	{
	public:
		uint32 		 BaseColorBinding 		   = 0;
		uint32 		 NormalMapBinding 		   = 1; 
		uint32 		 MettallicRoughnessBinding = 2; 
		uint32 		 EmissiveBinding  		   = 3;
		uint32 		 OcolusionBinding 		   = 4;
		int32 	 	 Set 		  			   = -1;
		ShaderStages ShaderStage  			   = HYD_SHADER_STAGE_FRAGMENT_BIT;
		UniformType  Type 	      			   = HYD_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	
	private:
		HYD_ID_SPACE DescriptorSetLayoutID;
	private:
		friend class Scene;
	};

	class Material
	{
	public:
		
		static Material CreateMaterialGLTF(
			const Xenon::Material& 							 pMaterial,
			std::unordered_map<uint64, Instance<Texture2D>>& pTextureTable,
			HYD_ID_SPACE 		     						 pDescSetLayout,
			MaterialBinding	 								 pBindings
		) noexcept;

	public:

		HYD  Material() = default;
		HYD ~Material() = default;

		HYD Material(const Material& pOther) = delete;
		HYD Material(Material&& pOther);

		HYD Material& operator=(const Material& pOther) = delete;
		HYD Material& operator=(Material&& pOther);


		//Bind Material:
		HYD void Bind(
			const Internal::Vulkan::PipelineLayout& pLayout
		)   noexcept;


	private:

		float m_MatallicnessFactor = 0.0f;
		float m_RoughnessFactor    = 1.0f;
		VecF4 m_BaseColorFactor    = VecF4(1.0f);

		Instance<Texture2D>  m_BaseColor;
		Instance<Texture2D>  m_NormalMap;
		Instance<Texture2D>  m_MetallicMap;
		Instance<Texture2D>	 m_EmissiveMap;
		Instance<Texture2D>	 m_OcclusionMap;
		HYD_ID_SPACE 		 m_SetID;
		MaterialBinding      m_Binding;

		friend class Core;
	};

};