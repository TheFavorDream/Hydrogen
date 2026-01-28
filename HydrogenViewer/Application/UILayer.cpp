#include "UILayer.h"

void UILayer::Setup()
{
	m_UI = Hydrogen::UICore::Self();
	m_UI->InitUI();


	m_UI->CreateUIWindow("Control", Hydrogen::Vec2(0.0f), Hydrogen::Vec2(0.0f), Hydrogen::UP_RIGHT);
	m_UI->CreateUIWindow("Editor", Hydrogen::Vec2(0.0f), Hydrogen::Vec2(0.0f), Hydrogen::DOWN_LEFT);

	m_ControlWindow		= m_UI->GetWindow("Control");
	m_EditorWindow		= m_UI->GetWindow("Editor");

	m_EditorWindow->CreateComboBox("Options", {"Shaders", "OP2", "OP3"}, Hydrogen::Vec2(10.0f, 20.0f), "Options");

	m_ControlWindow->CreateText("", Hydrogen::Vec2(10.0f, 520.0f), "DeltaMeter");
	m_ControlWindow->CreateText("", Hydrogen::Vec2(10.0f, 540.0f), "FPSMeter");
	m_ControlWindow->CreateButton("Button", Hydrogen::Vec2(50.0f, 20.0f), Hydrogen::Vec2(10.0f, 20.0f));
	m_ControlWindow->CreateInputF("Value:", Hydrogen::Vec2(10.0f, 50.0f), "Value");
	m_ControlWindow->CreateSliderF("Slider", Hydrogen::Vec2(10.0f, 80.0f));
	m_ControlWindow->CreateCheckbox("check", Hydrogen::Vec2(10.0f, 110.0f));
	m_ControlWindow->CreateComboBox("Stuff", { "Op1", "Op2" , "Op3", "Op4"}, Hydrogen::Vec2(10.0f, 140.0f), "Combo");

}

void UILayer::Shutdown()
{
	m_UI->ShutdownUI();
	m_UI = nullptr;
}


void UILayer::Event(float pDeltaTime)
{
   	m_DeltaTimeAvg.AddValue(pDeltaTime);

	if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_UP) == Hydrogen::KEY_DOWN)
	{
		m_EditorHeight += 1.0f;
	}
	if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_DOWN) == Hydrogen::KEY_DOWN)
	{
		m_EditorHeight -= 1.0f;
	}

	m_EditorHeight = Hydrogen::Clamp((m_WindowSize.Y - m_ViewPortSize.Y), m_WindowSize.Y, m_EditorHeight);
}

void UILayer::Update()
{
	//Update the Wedgets size every frame
	m_ViewPortSize = Hydrogen::Window::GetCurrentWindow()->GetViewportSize();
	
	m_WindowSize.X = Hydrogen::Window::GetCurrentWindow()->GetWidth();
	m_WindowSize.Y = Hydrogen::Window::GetCurrentWindow()->GetHeight();

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


	m_ControlWindow->GetElement("DeltaMeter")->GetLabel() = "Average Delta Time:" + std::to_string(m_DeltaTimeAvg.CalcAvg());
	m_ControlWindow->GetElement("FPSMeter")->GetLabel() = "Average FPS:" + std::to_string(int(1000.0f/(m_DeltaTimeAvg.CalcAvg())));
}

void UILayer::Render()
{
	m_UI->Render();
}

