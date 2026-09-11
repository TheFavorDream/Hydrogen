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

void UILayer::Event() 
{

}

void UILayer::Update() 
{
    Hydrogen::UI::Core::Self().Render();
}