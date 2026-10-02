#include "ApplicationLayer.h"
#include <glm/ext/matrix_float4x4.hpp>
#include "UI/Editor.h"



void AppLayer::Setup()
{


	m_Scene = Core::CreateScene();

	//Camera Setup
	m_Scene->GetCamera().SetupCamera(45.0f, glm::vec3(0.0f, 0.5f, 0.0f), 2.0f, 0.1f, 1000.0f);
	
	
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
	m_Scene->GetTransform().t_Scale = Hydrogen::VecF3(1.0f);



	
	
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
	Hydrogen::RenderStates& pRenderStates
)
{
	
	if (Editor::RenderGrid)
	{
		//Render the Grid
		pRenderStates.PushRenderState(
			m_Grid.Render()
		);
	}
	

	//Render the Scene:
	pRenderStates.PushRenderState(
		m_Scene->Render()
	);
	

} 


