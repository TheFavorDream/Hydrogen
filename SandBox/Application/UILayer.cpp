#include "UILayer.h"

void UILayer::Setup() 
{
    Hydrogen::Renderer::Self().InitUICore();

    m_DebugCanvas.Name() = "Debug Window";

    Hydrogen::UI::Core::Self().PushWidget(&m_DebugCanvas);
}

void UILayer::Shutdown() 
{

}

void UILayer::Event(
	Hydrogen::FrameEvent& pEvents
) 
{

	Hydrogen::UI::Core::Self().Event(pEvents);
}

void UILayer::Update() 
{
    Hydrogen::UI::Core::Self().Render();
}