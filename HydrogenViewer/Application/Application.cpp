#include "Application.h"



Application::Application()
	: Core(1200, 600, "Hydrogen")
{
	m_AppLayer = new AppLayer();
	m_UILayer = new UILayer();
	m_Window.SetViewportRatio(80.0f, 75.0f);

	PushLayer(m_AppLayer);
	PushLayer(m_UILayer);
}

Application::~Application()								
{
	
}

void Application::Run()
{
	Loop();
}


	

