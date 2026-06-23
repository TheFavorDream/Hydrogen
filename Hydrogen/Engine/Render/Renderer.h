/*

	Main Renderer of Hydrogen.
	Rendering happens via a Rendering Request:

	-----------			------------	   ---------------
	|	Data  |  ---->  | Renderer | ----> | FrameBuffer |
	-----------         ------------       ---------------
*/

#pragma once

#include "Common.h"
#include "Log/Log.h"
#include "ShaderPool.h"
#include "Geometry/Mesh.h"
#include "Core/Core.h"
#include "UI/UICore.h"


namespace Hydrogen
{

	//Vulkan not suported
	enum APIs { API_NONE, API_OPENGL, API_VULKAN };

	class Renderer
	{

	public:

		//Init and shutdown
		HYD static uint32 Init(APIs pRenderingAPI);
		HYD static uint32 Shutdown();

		//Adds Renderable Primitive to the Rendering queue
		HYD static void PushPrimitive(Primitive* pPrimitive);

		//Main Rendering 
		HYD static uint32 Render();


		HYD static inline Instance<Shader>     GetDefaultShader() noexcept {   return s_DefaultShader; };
		HYD static inline Instance<Material>   GetDefaultMaterial() noexcept { return s_DefaultMaterial; }
		HYD static inline APIs GetRenderingAPI() noexcept { return s_RenderingAPI; }

	private:
		HYD static uint32 InitAPI();
	private:

		HYD static APIs                  s_RenderingAPI;

		HYD static	Instance<Shader>	 s_DefaultShader;
		HYD static	Instance<Shader>	 s_StageTwo;
		HYD static	Instance<Shader>	 s_StageThree;

		HYD static Instance<Material>    s_DefaultMaterial;

		HYD static ShaderPool			  s_Shaders;
		
		HYD static std::queue<Ptr<Primitive>> s_PrimitiveQueue;
	};


};