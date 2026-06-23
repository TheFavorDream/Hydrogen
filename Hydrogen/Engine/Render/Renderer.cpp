#include "Renderer.h"
#include "HydPch.h"


namespace Hydrogen
{


	APIs						Renderer::s_RenderingAPI    = API_NONE;
	Instance<Shader>			Renderer::s_DefaultShader  ;
	Instance<Shader>			Renderer::s_StageTwo       ;
	Instance<Shader>			Renderer::s_StageThree     ;
	Instance<Material>			Renderer::s_DefaultMaterial;
	ShaderPool					Renderer::s_Shaders;
	std::queue<Ptr<Primitive>>  Renderer::s_PrimitiveQueue;



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
			glEnable(GL_CULL_FACE);

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

		


		//Render Queue
		GL_CALL(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));



		Ptr<Scene> CurrentScene = Core::GetCurrentWorld()->GetCurrentScene();
		if (!CurrentScene)
			return HYD_OK;

		//Update View and Projection matrix:
		// TODO: Optimize this shit

		s_Shaders.BroadCastUniformMat4("View",       CurrentScene->m_Camera.GetView());
		s_Shaders.BroadCastUniformMat4("Projection", CurrentScene->m_Camera.GetProjection());

		while (s_PrimitiveQueue.empty() != true)
		{

			Ptr<Primitive> Current = s_PrimitiveQueue.front();
			s_PrimitiveQueue.pop();

			Current->m_Shader->Bind();


			s_Shaders.SetUniformMat4(Current->m_Shader, "Model", Current->m_Transform.GetPointer() , true);

			 
			
			s_Shaders.UnbindCurrentShader();
		
		
		}


		//Render UI as needed:

		UICore::Self()->Render();

		return HYD_OK;
	}


};