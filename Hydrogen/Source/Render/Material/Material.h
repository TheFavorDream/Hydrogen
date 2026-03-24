#pragma once

#include "Common.h"
#include "Render/RenderPool/TexturePool.h"
#include "Render/Platform/OpenGL/GLShader.h"

namespace Hydrogen
{

	class Material
	{
	public:

		HYD  Material();
		HYD ~Material();

		HYD Material(const Material& pOther) = delete;
		HYD Material(Material&& pOther);

		HYD Material& operator=(const Material& pOther) = delete;
		HYD Material& operator=(Material&& pOther);


		HYD uint32 SetMetallicnessFactor(float pMetalicnessFactor);
		HYD uint32 SetRoughnessFactor(float   pRoughnessFactor);

		HYD uint32 SetBaseColorTexture(Texture** pBaseColor);
		HYD uint32 SetNormalMapTexture(Texture** pNormalMap);
		HYD uint32 SetMetallicTexture(Texture**   pMetallic);


		//Bind Material:
		HYD uint32 Bind(const Shader& pShader);
		HYD uint32 Unbind();


	private:

		float m_MatallicnessFactor = 0.0f;
		float m_RoughnessFactor   = 1.0f;

		Id          m_BaseColor  = 0;
		Id          m_NormalMap  = 0;
		Id          m_MetallicMap = 0;
		TexturePool m_TexturePool;
	};

};