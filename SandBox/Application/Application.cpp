#include "Application.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>



Application::Application()
	: Core(WindowInfo(1400, 700, "Hydrogen"))
{

	float ViewportX = 80.0f;
	float ViewportY = 80.0f;

	Hydrogen::Renderer::Self().GetWindow().SetViewportRatio(ViewportX, ViewportY);

	Hydrogen::Renderer::Self().GetWindow().SetViewportPositionWithRatio(100.0f-ViewportX, 0.0f);

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

	

