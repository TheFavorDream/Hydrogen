#include "Application.h"



Application::Application()
	: Core(1400, 700, "Hydrogen")
{
	m_AppLayer = new AppLayer();
	m_Window.SetViewportRatio(80.0f, 70.0f);

	PushLayer(m_AppLayer);
}

Application::~Application()								
{
	
}

void Application::Run()
{
	Loop();
}


	

