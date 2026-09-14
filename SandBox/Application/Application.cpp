#include "Application.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>



Application::Application()
	: Core(WindowInfo(1400, 700, "Hydrogen"))
{

	Hydrogen::Renderer::Self().GetWindow().SetViewportRatio(100.0f, 100.0f);

	m_AppLayer = new AppLayer();
	m_Editor   = new Editor();

	PushLayer(m_AppLayer);
	PushLayer(m_Editor);
}

Application::~Application()								
{

}

void Application::Run()
{
	Loop();
}

	

