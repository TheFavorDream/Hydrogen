#include "ApplicationLayer.h"
#include <glm/ext/matrix_float4x4.hpp>




void AppLayer::Setup()
{


	m_Scene = Core::CreateScene();

	//Camera Setup
	m_Scene->GetCamera().SetupCamera(45.0f, glm::vec3(0.0f), 2.0f, 0.1f, 1000.0f);
	
	Renderer::Self().SetCamera(&m_Scene->GetCamera());
	
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

	//Pipeline Creation:
	m_Scene->ConfigurePipeline(
		ConfigPipeline(),
		SceneUniformLayout
	);


	Xenon::Model model = Core::Load(
		"/home/Volta/Desktop/Dev/Hydrogen/SandBox/Resources/Models/Pistol/scene.gltf",
		Xenon::LF_BASE_NORMAL_ONLY
	);
	

	m_Scene->LoadScene(
		model[0] //first scene
	);

	//Hydrogen::Quaternion Rot;  Rot.Euler(0.0f, 180.0f, 0.0f);
	//m_Scene->GetTransform().t_Rotate = Rot;
	//m_Scene->GetTransform().t_Scale = Hydrogen::VecF3(0.1f);

}


void AppLayer::Shutdown()
{
	m_Scene.ResetWithoutRefDrop();
}

void AppLayer::Event()
{

	m_Scene->GetCamera().HandleCameraLooking();
	m_Scene->GetCamera().HandleCameraMovement();

}

void AppLayer::Update()
{
	//m_Scene->Render();
}


Hydrogen::GraphicsPipelineConfiguration AppLayer::ConfigPipeline() noexcept
{
	//Shader Compliation:
	Hydrogen::ShaderConfiguration VertexShaderConf;
	VertexShaderConf.Type = HYD_STAGE_VERTEX_SHADER;
 	VertexShaderConf.Path = "/home/Volta/Desktop/Dev/Hydrogen/SandBox/Shaders/Vertex.spv";
	

	Hydrogen::ShaderConfiguration FragmentShaderConf;
	FragmentShaderConf.Type = HYD_STAGE_FRAGMENT_SHADER;
 	FragmentShaderConf.Path = "/home/Volta/Desktop/Dev/Hydrogen/SandBox/Shaders/Fragment.spv";



	Hydrogen::GraphicsPipelineConfiguration PipelineConf;

	PipelineConf.SetSubpass(0);

	PipelineConf.AttachShader(VertexShaderConf);
	PipelineConf.AttachShader(FragmentShaderConf);

	Hydrogen::Vec4<uint32> Viewport = Hydrogen::Vec4<uint32>(0, 0, 1400, 700);

	PipelineConf.SetViewport(Viewport);
	PipelineConf.SetRasterizer(Hydrogen::HYD_POLYGON_MODE_FILL, Hydrogen::HYD_CULL_MODE_BACK, Hydrogen::HYD_FRONT_FACE_COUNTER_CLOCKWISE);
	PipelineConf.SetDepthStencil(true, true,Hydrogen::HYD_COMPARE_OP_GREATER);


	return PipelineConf;
}

