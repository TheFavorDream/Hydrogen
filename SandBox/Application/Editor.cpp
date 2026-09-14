#include "Editor.h"


float Editor::FogQuad     = 0.05f;
float Editor::FogLinear   = 0.0005f;
float Editor::FogConstant = 1.0f;

void Editor::Setup() 
{
    Hydrogen::Renderer::Self().InitUICore();

}

void Editor::Shutdown() 
{

}

void Editor::Event(
	Hydrogen::FrameEvent& pEvents
) 
{

	Hydrogen::UI::Core::Self().Event(pEvents);
}

void Editor::Update() 
{

}


void Editor::Render(
    Hydrogen::FrameRenderConfig& pRenderConf
) 
{
    Hydrogen::UI::Core::Self().NewFrame();
    
    Editor::GridController();
    
    Hydrogen::UI::Core::Self().Render();
} 


void Editor::GridController() noexcept
{
    ImGui::Begin("Editor");
    
    ImGui::Text("Grid Fog Coefficents:");
    ImGui::NewLine();
    
    ImGui::SliderFloat("Fog Quadratic", &FogQuad, 0.0f, 10.0f);
    ImGui::SliderFloat("Fog Linear", &FogLinear, 0.0f, 10.0f);
    ImGui::SliderFloat("Fog Constant", &FogConstant, 0.0f, 10.0f);
    
    ImGui::End();
} 
