/*

	Materials consist of textures and metalicness information, etc required to render
*/

#pragma once

#include "Common.h"
#include "Texture/Texture.h"

namespace Hydrogen
{
	
	class Material
	{
	public:

		HYD Material() = default;
		HYD ~Material();
		 
		HYD Material(const Material& pOther) = delete;
		HYD Material(Material&& pOther);

		HYD Material& operator=(const Material& pOther) = delete;
		HYD Material& operator=(Material&& pOther);

		HYD void BindBaseColor(uint32 Slot=0);
		HYD void BindMetalicColor(uint32 Slot = 1);
		
		HYD void UnbindBaseColor();
		HYD void UnbindMetalicColor();


	private:

		std::string m_Name;

		float m_MetalicnessFactor = 1.0f;
		float m_RoughnessFactor   = 1.0f;
		Vec4 m_BaseColorFactor = Vec4(1.0f);
		Texture m_BaseColor;
		Texture m_MetalicRoughness;
	
		friend class Model;
		friend class GLTFLoader;
	};

};