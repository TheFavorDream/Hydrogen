#include "Application.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>



Application::Application()
	: Core(WindowInfo(1400, 700, "Hydrogen"))
{

	float ViewportX = 80.0f;
	float ViewportY = 85.0f;

	Hydrogen::Renderer::Self().GetWindow().SetViewportRatio(ViewportX, ViewportY, 20.0f, 0.0f);


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

	

