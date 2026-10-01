#include "ApplicationLayer.h"
#include <glm/ext/matrix_float4x4.hpp>
#include "UI/Editor.h"



void AppLayer::Setup()
{


	m_Scene = Core::CreateScene();

	//Camera Setup
	m_Scene->GetCamera().SetupCamera(45.0f, glm::vec3(-3.0f, 0.5f, 0.0f), 2.0f, 0.1f, 1000.0f);
	
	
	//Layout Setup:
	Hydrogen::SceneShaderLayout SceneUniformLayout;

	SceneUniformLayout.MVP = 	Hydrogen::ShaderUniformBinding{
		.Binding     = 0,
		.Set         = 0,
		.ShaderStage = Hydrogen::HYD_SHADER_STAGE_VERTEX_BIT,
		.Type    	 = Hydrogen::HYD_DESCRIPTOR_TYPE_UNIFORM_BUFFER 
	};

	 
	SceneUniformLayout.Material.BaseColorBinding 	 	   = 0;
	SceneUniformLayout.Material.NormalMapBinding 	 	   = 1;
	SceneUniformLayout.Material.MettallicRoughnessBinding  = 2;
	SceneUniformLayout.Material.Set     	 			   = 1;
	SceneUniformLayout.Material.ShaderStage 			   = Hydrogen::HYD_SHADER_STAGE_FRAGMENT_BIT;
	SceneUniformLayout.Material.Type		 			   = Hydrogen::HYD_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;



	Xenon::Model model = Core::Load(
		"/home/Volta/Desktop/Dev/Hydrogen/SandBox/Resources/Models/Interior2/scene.gltf",
		Xenon::LF_NO_MATERIAL
	);
	
	std::vector<Hydrogen::ShaderConfiguration> Shaders = {	//Shader Compliation:
		Hydrogen::ShaderConfiguration{
			.Type = HYD_STAGE_VERTEX_SHADER,
 			.Path = "/home/Volta/Desktop/Dev/Hydrogen/SandBox/Shaders/Vertex.spv"
		},

		Hydrogen::ShaderConfiguration{
			.Type = HYD_STAGE_FRAGMENT_SHADER,
 			.Path = "/home/Volta/Desktop/Dev/Hydrogen/SandBox/Shaders/Fragment.spv"
		}
	};

	m_Scene->LoadScene(
		std::move(Shaders),
		model[0] //first scene
	);

	Hydrogen::Quaternion Rot = Rot.Euler(0.0f, 0.0f, 180.0f);
	//m_Scene->GetTransform().t_Rotate = Rot;
	m_Scene->GetTransform().t_Scale = Hydrogen::VecF3(0.5f);



	
	
	Editor::SetCurrentScene(m_Scene.GetPtr());
	m_Grid.GenerateGrid();
}


void AppLayer::Shutdown()
{
	m_Grid.DestroyGrid();
	//m_Scene.ResetWithoutRefDrop();
}

void AppLayer::Event(
	Hydrogen::FrameEvent& pEvents
)
{

	if (!Hydrogen::UI::Core::Self().IsUIEvent())
	{
		m_Scene->GetCamera().HandleCameraLooking(pEvents.PositionOffset);
		m_Scene->GetCamera().HandleCameraMovement(pEvents);
	}
}

void AppLayer::Update()
{
	//Update Grid Values:
	m_Grid.SetFog(
		Editor::FogQuad, Editor::FogLinear, Editor::FogConstant
	);
}


void AppLayer::Render(
	Hydrogen::FrameRenderConfig& pRenderConf
)
{
	/*
	if (Editor::RenderGrid)
	{
		//Render the Grid
		pRenderConf.PushInstruction(
			m_Grid.Render()
		);
	}
	*/
	
	pRenderConf.BindDescriptorSet(m_Scene->PrimitiveListBind());
	pRenderConf.BindDescriptorSet(m_Scene->CameraBind());
	//Render the Scene:
	pRenderConf.PushInstruction(
		m_Scene->Render()
	);
	

} 

Hydrogen::GraphicsPipelineConfiguration AppLayer::ConfigPipeline() noexcept
{




	Hydrogen::GraphicsPipelineConfiguration PipelineConf;

	/*
	PipelineConf.SetSubpass(0);

	PipelineConf.AttachShader(VertexShaderConf);
	PipelineConf.AttachShader(FragmentShaderConf);

	Hydrogen::Vec4<uint32> Viewport = Hydrogen::Vec4<uint32>(0, 0, 1400, 700);

	PipelineConf.SetViewport(Viewport);
	PipelineConf.SetRasterizer(Hydrogen::HYD_POLYGON_MODE_FILL, Hydrogen::HYD_CULL_MODE_BACK, Hydrogen::HYD_FRONT_FACE_COUNTER_CLOCKWISE);
	PipelineConf.SetDepthStencil(true, true,Hydrogen::HYD_COMPARE_OP_GREATER);
	*/

	return PipelineConf;
}

