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
#include "RenderPool/ShaderPool.h"
#include "RenderPool/BufferPool.h"
#include "RenderPool/MaterialPool.h"
#include "Model/ModelPool.h"
#include "Model/Geometry/Mesh.h"
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


		HYD static void SetDefaultMaterial(Id pMaterialID) noexcept;

		HYD static inline Id   GetDefaultShader() noexcept { return s_DefaultShader; };
		HYD static inline Id   GetDefaultMaterial() noexcept { return s_DefaultMaterial; }
		HYD static inline APIs GetRenderingAPI() noexcept { return s_RenderingAPI; }

	private:
		HYD static uint32 InitAPI();
	private:

		HYD static APIs                  s_RenderingAPI;

		HYD static Id					  s_DefaultShader;
		HYD static Id					  s_StageTwo;
		HYD static Id					  s_StageThree;

		HYD static Id s_DefaultMaterial;

		HYD static ShaderPool			  s_Shaders;
		
		HYD static std::queue<Primitive*> s_PrimitiveQueue;
	};


};