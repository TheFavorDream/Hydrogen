/*

	Materials consist of textures and metalicness information, etc required to render
*/

#pragma once

#include "Common.h"
#include "ResourceHandler/ResourceHandler.h"
#include "ResourceHandler/MaterialHandler.h"

namespace Hydrogen
{
	
	class Material : public Resource
	{
	public:

		HYD Material() = default;
		HYD ~Material();
		 
		HYD Material(const Material& pOther) = delete;
		HYD Material(Material&& pOther);

		HYD Material& operator=(const Material& pOther) = delete;
		HYD Material& operator=(Material&& pOther);

		HYD void BindBaseColor(uint32 Slot=0)      const;
		HYD void BindMetalicColor(uint32 Slot = 1) const;
		
		HYD void UnbindBaseColor()    const;
		HYD void UnbindMetalicColor() const;


	private:

		std::string m_Name;

		float m_MetalicnessFactor = 1.0f;
		float m_RoughnessFactor   = 1.0f;
		Vec4 m_BaseColorFactor = Vec4(1.0f);

		Id m_BaseColorTexture;
		Id m_MetalicRoughnessTexture;
	
		friend class Model;
		friend class GLTFLoader;
	};

};