#include "Editor.h"


float Editor::FogQuad     = 0.05f;
float Editor::FogLinear   = 0.0005f;
float Editor::FogConstant = 1.0f;

Hydrogen::Ptr<Hydrogen::LightCollection> Editor::Lights; 


void Editor::Setup() 
{
    Hydrogen::Renderer::Self().InitUICore();



    
  //  RegularFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(
  //      "/home/Volta/Desktop/Dev/Hydrogen/SandBox/Resources/Fonts/Reg.ttf",14.0f
  //  );
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

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {

            if (ImGui::MenuItem("Grid Editor"))
            {
                IsGridWindow = !IsGridWindow;
            }

            if (ImGui::MenuItem("Light Editor"))
            {
                IsLightsWindow = !IsLightsWindow;
            }
            
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    


    if (IsGridWindow)
        Editor::GridController();
    if (IsLightsWindow)
        Editor::LightController();

    Hydrogen::UI::Core::Self().Render();
} 


void Editor::GridController() noexcept
{
    ImGui::SetNextWindowSize(ImVec2(300.0f, 200.0f));
    ImGui::Begin("Grid", nullptr, ImGuiWindowFlags_NoResize);
    
    ImGui::Text("Grid Fog Coefficents:");
    ImGui::NewLine();
    
    ImGui::SliderFloat("Fog Quadratic", &FogQuad, 0.0f, 10.0f);
    ImGui::SliderFloat("Fog Linear", &FogLinear, 0.0f, 10.0f);
    ImGui::SliderFloat("Fog Constant", &FogConstant, 0.0f, 10.0f);
    
    ImGui::End();
} 


void Editor::LightController() noexcept
{
    ImGui::SetNextWindowSize(ImVec2(300.0f, 450.0f));
    ImGui::Begin("Light Editor", nullptr, ImGuiWindowFlags_NoResize);

    static int32       SelectedLightIndex = (Lights->GetLightCount())? 0 : -1;
    static const char* SelectedLightName  = "";
    if (SelectedLightIndex >= 0)
        SelectedLightName =  Lights->AccessLight(SelectedLightIndex).Name().c_str();



    if(ImGui::BeginCombo("Lights", SelectedLightName))
    {
        uint32 Counter = 0 ;
        for (auto& light : (*Lights))
        {
            if (ImGui::Selectable(light.Name().c_str()))
            {
                Hydrogen::Log::SetInfo(
                    Hydrogen::Log::FmtStr("Light: %s Selected In Light Editor", light.Name().c_str())
                );
                SelectedLightName  = light.Name().c_str();
                SelectedLightIndex = Counter; 
            }
            Counter ++;
        }
        ImGui::EndCombo();
    }

    
    if (SelectedLightIndex >= 0)
    {
        Hydrogen::Light& CurrentLight = Lights->EditLight(SelectedLightIndex);
        
        ImGui::ColorPicker3("Light Color", reinterpret_cast<float*>(&CurrentLight.Color()));        
        ImGui::InputFloat3("Position:", reinterpret_cast<float*>(&CurrentLight.Position()));


        ImGui::Text("Ligth Properties:");
        ImGui::InputFloat3("Ambient:",  reinterpret_cast<float*>(&CurrentLight.Ambient()));
        ImGui::InputFloat3("Diffuse:",  reinterpret_cast<float*>(&CurrentLight.Diffuse()));
        ImGui::InputFloat3("Specular:", reinterpret_cast<float*>(&CurrentLight.Specular()));
    }    


    ImGui::End();
} 

