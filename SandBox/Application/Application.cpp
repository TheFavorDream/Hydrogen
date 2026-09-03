#include "Application.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>



Application::Application()
	: Core(WindowInfo(1400, 700, "Hydrogen"))
{



	m_AppLayer = new AppLayer();
	PushLayer(m_AppLayer);
}

Application::~Application()								
{
	
}

void Application::Run()
{
	Loop();
}

	

