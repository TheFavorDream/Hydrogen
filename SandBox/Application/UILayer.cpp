#include "UILayer.h"
#include <string>

using namespace Hydrogen;

void UILayer::Setup()
{
	m_UI = Hydrogen::UICore::Self();
	m_UI->InitUI();


	m_UI->CreateUIWindow("Control", Hydrogen::VecF2(0.0f), Hydrogen::VecF2(0.0f), Hydrogen::UP_RIGHT);
	m_UI->CreateUIWindow("Editor", Hydrogen::VecF2(0.0f), Hydrogen::VecF2(0.0f), Hydrogen::DOWN_LEFT);

	
	m_ControlWindow		= m_UI->GetWindow("Control");
	m_EditorWindow		= m_UI->GetWindow("Editor");

	m_ControlWindow->CreateComboBox("Models", {"none"}, Hydrogen::VecF2(20.0f, 30.0f), "models");
	m_ControlWindow->CreateComboBox("Mesh", { "none" }, Hydrogen::VecF2(20.0f, 60.0f), "mesh");

	m_ControlWindow->CreateText("FPS:", Hydrogen::VecF2(10, 650.0f), "FPSMeter");
	
	
	m_ControlWindow->CreateInputF("Angle X", VecF2(20.0f, 100.0f), "A_X");
	m_ControlWindow->CreateInputF("Angle Y", VecF2(20.0f, 120.0f), "A_Y");
	m_ControlWindow->CreateInputF("Angle Z", VecF2(20.0f, 140.0f), "A_Z");


	m_EditorHeight = 0.0f;
	UpdateWidgetSizes();


}

void UILayer::Shutdown()
{
	m_UI->ShutdownUI();
	delete m_UI;
	m_UI = nullptr;
}


void UILayer::Event()
{
   	m_DeltaTimeAvg.AddValue(Hydrogen::Core::GetDeltaTime());

	if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_UP) == Hydrogen::KEY_DOWN)
	{
		m_EditorHeight += 2.0f;
	}
	if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_DOWN) == Hydrogen::KEY_DOWN)
	{
		m_EditorHeight -= 2.0f;
	}

	m_EditorHeight = Hydrogen::Clamp((m_WindowSize.Y - m_ViewPortSize.Y), m_WindowSize.Y, m_EditorHeight);
}

void UILayer::Update()
{


	UpdateWidgetSizes();

	m_ControlWindow->GetElement("FPSMeter")->GetLabel() = "Average FPS:" +  std::to_string(m_UI->GetFPS());

	static bool First = true;

	if (First)
	{
	
		Hydrogen::Ptr<Hydrogen::Scene> Current = Hydrogen::Core::GetCurrentScene();
		for (auto& model : Current->GetModels())
		{
			Ref<Model> ref = Current->GetModel(model.first);
			m_ControlWindow->GetCombo("models")->PushItem(ref.GetName());
			m_CurrentModel = model.first;
			for (auto& mesh : ref.GetMeshPool())
			{
				m_CurrentMesh = mesh.first;
				m_ControlWindow->GetCombo("mesh")->PushItem(ref.GetMesh(mesh.first).GetName());
			}
		}
		First = false;
	}


	
	static float PrevAngleX = 0.0f;
	static float PrevAngleY = 0.0f;
	static float PrevAngleZ = 0.0f;


	float AngleX = m_ControlWindow->GetInput("A_X")->GetValue();
	float AngleY = m_ControlWindow->GetInput("A_Y")->GetValue();
	float AngleZ = m_ControlWindow->GetInput("A_Z")->GetValue();



	if ((PrevAngleX != AngleX || PrevAngleY != AngleY || PrevAngleZ != AngleZ) && m_CurrentModel != 0 )
	{
		Quaternion quat(0.0f, 0.0f, 0.0f, 1.0f);
		quat.Euler(AngleX, AngleY, AngleZ);

		Core::GetCurrentScene()->GetModel(m_CurrentModel).GetTransformation().t_Rotate = quat;
		Core::GetCurrentScene()->GetModel(m_CurrentModel).GetTransformation().IsDirty = true;

		PrevAngleX = AngleX;
		PrevAngleY = AngleY;
		PrevAngleZ = AngleZ;

	}

}

void UILayer::UpdateWidgetSizes()
{
	//Update the Wedgets size every frame
	m_ViewPortSize = Hydrogen::Window::GetCurrentWindow()->GetViewportSize();

	m_WindowSize.X = (float)Hydrogen::Window::GetCurrentWindow()->GetWidth();
	m_WindowSize.Y = (float)Hydrogen::Window::GetCurrentWindow()->GetHeight();

	float VPSpaceX = m_WindowSize.X - m_ViewPortSize.X;
	float VPSpaceY = m_WindowSize.Y - m_ViewPortSize.Y;


	//TODO: make a window change call back function and move these to there
	m_ControlWindow->SetSizeMax(m_WindowSize.X, m_WindowSize.Y);
	m_ControlWindow->SetSizeMin(VPSpaceX, m_WindowSize.Y);
	m_EditorWindow->SetSizeMax(m_ViewPortSize.X, m_WindowSize.Y);
	m_EditorWindow->SetSizeMin(m_ViewPortSize.X, VPSpaceY);

	m_ControlWindow->SetWindowSize(m_ControlWidth, m_WindowSize.Y);
	m_ControlWindow->SetWindowPos(m_WindowSize.X, 0.0f);

	m_EditorWindow->SetWindowSize(m_ViewPortSize.X, m_EditorHeight);
	m_EditorWindow->SetWindowPos(0.0f, m_WindowSize.Y);
}


