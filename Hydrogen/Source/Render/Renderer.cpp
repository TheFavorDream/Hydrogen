#include "Renderer.h"

namespace Hydrogen
{


	APIs				   Renderer::s_RenderingAPI  = API_NONE;
	Id					   Renderer::s_DefaultShader = 0;
	Id					   Renderer::s_StageTwo      = 0;
	Id					   Renderer::s_StageThree    = 0;

	ShaderPool			   Renderer::s_Shaders;
	std::queue<Primitive*> Renderer::s_PrimitiveQueue;



	uint32 Renderer::Init(APIs pRenderingAPI)
	{

		Log::SetInfo("Renderer:Init");

		s_RenderingAPI = pRenderingAPI;
		InitAPI();

		//Compiling shaders:
		std::string VS({ 
#include "Shaders/Default_VS.glsl" 
			});

		std::string FS({
#include "Shaders/Default_FS.glsl" 
			});

		//Log::DebugPrint(FS);
		s_DefaultShader = s_Shaders.CreateShader(VS, FS);

		return HYD_OK;
	}

	uint32 Renderer::Shutdown()
	{
		
		return HYD_OK;
	}


	uint32 Renderer::InitAPI()
	{

		switch (s_RenderingAPI)
		{
		case API_OPENGL:

			if (glewInit() != 0)
				return HYD_GLEW_FAILED;

			glEnable(GL_DEPTH_TEST);
			//glCullFace(GL_CCW);
			//glEnable(GL_CULL_FACE);

			break;
		}

		return HYD_OK;
	}


	void Renderer::PushPrimitive(Primitive* pPrimitive)
	{
		if (pPrimitive != nullptr)
			s_PrimitiveQueue.push(pPrimitive);
	}

	uint32 Renderer::Render()
	{

		Scene* CurrentScene = Core::GetCurrentScene();
		
		//Construct Primitive Queue
		CurrentScene->Render();

		//Update View and Projection matrix:
		// TODO: Optimize this shit

		s_Shaders.BroadCastUniformMat4("View",       CurrentScene->m_Camera.GetView());
		s_Shaders.BroadCastUniformMat4("Projection", CurrentScene->m_Camera.GetProjection());


		//Render Queue
		GL_CALL(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		while (s_PrimitiveQueue.empty() != true)
		{

			Primitive* Current = s_PrimitiveQueue.front();
			s_PrimitiveQueue.pop();

			s_Shaders.BindShader(Current->m_Shader);
			s_Shaders.SetUniformMat4(Current->m_Shader, "Model", Current->m_ModelMatrix.GetPtr());

			
			CurrentScene->m_Materials.BindMaterial(Current->m_Material, s_Shaders.GetShader(Current->m_Shader));

			CurrentScene->m_Buffers.BindArray(Current->m_VertexArrays);
			CurrentScene->m_Buffers.BindVertexBuffer(Current->m_VertexBuffer);
			CurrentScene->m_Buffers.BindElementBuffer(Current->m_ElementBuffer);

			GL_CALL(glDrawElements(Current->m_RenderingMode, Current->Count, Current->Type, 0));

			CurrentScene->m_Buffers.UnbindArray(Current->m_VertexArrays);
			CurrentScene->m_Buffers.UnbindVertexBuffer(Current->m_VertexBuffer);
			CurrentScene->m_Buffers.UnbindElementBuffer(Current->m_ElementBuffer);

			s_Shaders.UnbindCurrentShader();
		
		
		}

		return HYD_OK;
	}

};