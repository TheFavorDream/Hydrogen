#include "Application.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>



Application::Application()
	: Core(WindowInfo(1400, 700, "Hydrogen"))
{



	m_AppLayer = new AppLayer();
	m_UILayer  = new UILayer();

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

	

