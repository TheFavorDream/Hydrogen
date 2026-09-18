#include "Editor.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <ctime>
#include <stack>
#include <vulkan/vulkan_core.h>


bool  Editor::RenderGrid  = true;
float Editor::FogQuad     = 0.05f;
float Editor::FogLinear   = 0.0005f;
float Editor::FogConstant = 1.0f;





void Editor::Setup() 
{
    Hydrogen::Renderer::Self().InitUICore();

    SetStyle();

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
            
            if (ImGui::MenuItem("New"))
            {

            }

            
            if (ImGui::MenuItem("Open"))
            {

            }

            
            if (ImGui::MenuItem("Load GLTF"))
            {

            }

            if (ImGui::MenuItem("Exit"))
            {
                Hydrogen::Core::s_Self->Terminate();
            }

            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Grid Editor"))
            {
                IsGridWindow = !IsGridWindow;
            }
            
            if (ImGui::MenuItem("Resources"))
            {

            }

            if (ImGui::MenuItem("Renderer"))
            {

            }



            ImGui::EndMenu();
        }

        ImGui::Text("FPS:%f", ImGui::GetIO().Framerate); 
        ImGui::EndMainMenuBar();
    }
    


    if (IsGridWindow)
        Editor::GridController();
    if (IsSceneGraghWindow)
        SceneGraph::DrawGraph();
    if (IsTimeLineWindow)
        TimeLine::TimeLineWindow();

    Hydrogen::UI::Core::Self().Render();
} 

void Editor::SetStyle() noexcept
{


    Hydrogen::Buffer FontData = Hydrogen::FileSys::ReadFile("/home/Volta/Desktop/Dev/Hydrogen/SandBox/Resources/Fonts/Medium.ttf");
    ImGuiIO& IO = ImGui::GetIO();

    IO.Fonts->Clear();
    IO.Fonts->AddFontFromMemoryTTF(
     FontData.GetPtr(),
     FontData.Length(),
     15.0f
    );
    //IO.Fonts->AddFontFromFileTTF("/home/Volta/Desktop/Dev/Hydrogen/SandBox/Resources/Fonts/Bold.otf",20.0f);
    //IO.Fonts->AddFontFromFileTTF("/home/Volta/Desktop/Dev/Hydrogen/SandBox/Resources/Fonts/Medium.otf",14.0f);

    FontData.Reset();
    IO.Fonts->Build();


    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
    /*
    style.Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Separator]             = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
    */
    //style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
    //style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
}


void Editor::GridController() noexcept
{
    ImGui::SetNextWindowSize(ImVec2(300.0f, 200.0f));
    ImGui::Begin("Grid", nullptr, ImGuiWindowFlags_NoResize);
    

    ImGui::Checkbox("Render", &Editor::RenderGrid);

    ImGui::Text("Grid Fog Coefficents:");
    ImGui::SliderFloat("Fog Quadratic", &FogQuad, 0.0f, 10.0f);
    ImGui::SliderFloat("Fog Linear", &FogLinear, 0.0f, 10.0f);
    ImGui::SliderFloat("Fog Constant", &FogConstant, 0.0f, 10.0f);
    
    ImGui::End();
} 




//---------------------------------------------Scene Graph--------------------------------

Hydrogen::Ptr<Hydrogen::Mesh>  SceneGraph::s_SelectedMesh  = nullptr;
Hydrogen::Ptr<Hydrogen::Node>  SceneGraph::s_SelectedNode  = nullptr;
Hydrogen::Ptr<Hydrogen::Light> SceneGraph::s_SelectedLight = nullptr;
Hydrogen::Ptr<Hydrogen::Scene> SceneGraph::s_CurrentScene  = nullptr;

bool                           SceneGraph::s_IsNodeEditor  = false;
bool                           SceneGraph::s_IsLightEditor = false;
bool                           SceneGraph::s_IsMeshEditor  = false;


/*
	Purpose: Call this to set the Scene to be graphed
*/
void SceneGraph::SetCurrentScene(
	Hydrogen::Ptr<Hydrogen::Scene> pScene
) noexcept
{
    s_CurrentScene = pScene;
}

void SceneGraph::DrawGraph() noexcept
{
    static bool FirstTime = true;

    float BeginX = 0.0f;
    float BeginY = 20.0f;

    VkViewport viewport = Hydrogen::Renderer::Self().GetWindow().GetViewportSize();

    float WindowHeight = (float)Hydrogen::Renderer::Self().GetWindow().GetWindowHeight(); 

    float Width  = (float)Hydrogen::Renderer::Self().GetWindow().GetWindowWidth() - viewport.width;
    float Height = viewport.height*(80.0f/100.0f);

    if (FirstTime)
    {
        Hydrogen::Log::SetInfo(
            Hydrogen::Log::FmtStr("Width: %f, Height: %f", Width, Height)
        );

        ImGui::SetNextWindowSize(
            ImVec2(Width, Height)
        );

        s_SelectedNode = s_CurrentScene;
        s_IsNodeEditor = true;
        FirstTime      = false;
    }

    //Calculate Position:

    ImGui::SetNextWindowPos(ImVec2(BeginX, BeginY));
    if (!ImGui::Begin("Scene Graph", nullptr, ImGuiWindowFlags_NoCollapse) || s_CurrentScene == nullptr)
    {
        ImGui::End();
        return;
    }


    DrawTreeNode(*s_CurrentScene, ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DefaultOpen);
    ImGui::End();


    if (s_IsNodeEditor)
        SceneGraph::NodeEditor(
        *s_SelectedNode,
           ImVec2(0.0f, Height+20.0f),
          ImVec2(Width, WindowHeight-Height+20.0f)
        );


    if (s_IsMeshEditor)
        SceneGraph::MeshEditor(
            *s_SelectedMesh,
            ImVec2(0.0f, Height+20.0f),
            ImVec2(Width, WindowHeight-Height+20.0f)
        );

    if (s_IsLightEditor)
        SceneGraph::LightEditor(*s_SelectedLight);

} 


void SceneGraph::DrawTreeNode(
    Hydrogen::Node&    pNode,
    ImGuiTreeNodeFlags pFlags
) noexcept
{
    if (ImGui::TreeNodeEx(pNode.GetName().c_str(), pNode.HasChild()? pFlags : pFlags | ImGuiTreeNodeFlags_Leaf))
    {

        if (ImGui::IsItemClicked())
        {
            Hydrogen::Log::SetInfo(
                Hydrogen::Log::FmtStr("Name:%s Selected", pNode.GetName().c_str())
            );
            
            s_IsMeshEditor  = false;
            s_SelectedMesh  = nullptr;
            
            s_SelectedNode = &pNode;
            s_IsNodeEditor = true;
        }
        
        if (pNode.HasMesh())
            DrawTreeMesh(*pNode.GetMesh().GetPtr(), ImGuiTreeNodeFlags_Bullet);
        

        for (auto& child : pNode)
            DrawTreeNode(child, pFlags);

        ImGui::TreePop();
    }
}


/*
	Purpose: Renders the Node with a mesh
*/
void SceneGraph::DrawTreeMesh(
	Hydrogen::Mesh& 	pMesh,
	ImGuiTreeNodeFlags  pFlags
) noexcept
{
    if (ImGui::TreeNodeEx(pMesh.GetName().c_str(), pFlags))
    {

        if (ImGui::IsItemClicked())
        {
            s_SelectedNode  = nullptr;
            s_IsNodeEditor  = false;

            s_IsMeshEditor  = true;
            s_SelectedMesh  = &pMesh;
            
        }

        ImGui::TreePop();
    }
}

/*
	Purpose: Node Editor Window
*/

void SceneGraph::NodeEditor(
	Hydrogen::Node& pNode,
	ImVec2 			pPos,
    ImVec2          pSize
) noexcept
{

    static bool FirstTime = true;
    if (FirstTime)
    {
        ImGui::SetNextWindowSize(pSize);
        FirstTime = false;
    }

    ImGui::SetNextWindowPos(pPos);
    ImGui::Begin("Node Editor", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Name:%s", pNode.GetName().c_str());

    //Node Transformation:
    Hydrogen::Transformation& trans = pNode.GetTransform();

    ImGui::Text("Scale:");
    ImGui::InputFloat3("##Scale",     reinterpret_cast<float*>(&trans.t_Scale));


    const  char* RotateMethod[] = {"Quaternion", "Euler"};
    static int32 Selection = 0;

    ImGui::Combo("##RotateMethod", &Selection, RotateMethod, 2);

    static float Rotation[4] = {trans.t_Rotate.X, trans.t_Rotate.Y, trans.t_Rotate.Z, trans.t_Rotate.W};
    
    ImGui::Text("Rotation:");
    switch (Selection)
    {
    case 0: //Quaternion
        ImGui::InputFloat4("##Rotate",    Rotation);
        trans.t_Rotate = Hydrogen::Quaternion(Hydrogen::VecF3(Rotation[0], Rotation[1], Rotation[2]), Rotation[3]);
        break;
    case 1: //Euler
        ImGui::InputFloat3("##Rotate", Rotation);
        trans.t_Rotate = Hydrogen::Quaternion().Euler(Rotation[0], Rotation[1], Rotation[2]);
        break;
    }

    ImGui::Text("Translation:");
    ImGui::InputFloat3("##Translate", reinterpret_cast<float*>(&trans.t_Translate));

    ImGui::End();
}


/*
	Purpose: Node Editor Window
*/
void SceneGraph::MeshEditor(
	Hydrogen::Mesh& pMesh,
	ImVec2 			pPos,
    ImVec2          pSize
) noexcept
{
    static bool FirstTime = true;
    if (FirstTime)
    {
        ImGui::SetNextWindowSize(pSize);
        FirstTime = false;
    }

    ImGui::SetNextWindowPos(pPos);
    ImGui::Begin("Mesh Editor", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Name:%s", pMesh.GetName().c_str());
    
    ImGui::Checkbox("Disable Mesh", &pMesh.MeshEnable());


    ImGui::End();
}

/*
	Purpose: Light Editor 
*/

void SceneGraph::LightEditor(
    Hydrogen::Light& pLight
) noexcept
{
    ImGui::SetNextWindowSize(ImVec2(300.0f, 450.0f));
    ImGui::Begin("Light Editor", nullptr, ImGuiWindowFlags_NoResize);
    /*
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
    */

    ImGui::End();
} 



//-------------------------------------------Time Line----------------------------------------

void TimeLine::TimeLineWindow() noexcept
{

    VkViewport viewport = Hydrogen::Renderer::Self().GetWindow().GetViewportSize();
    
    
    
    float WindowWidth  = Hydrogen::Renderer::Self().GetWindow().GetWindowWidth();
    float WindowHeight = Hydrogen::Renderer::Self().GetWindow().GetWindowHeight();

    float Height        = WindowHeight - viewport.height;
    float Width         = viewport.width;

    static bool FirstTime = true;
    if (FirstTime)
    {
        ImGui::SetNextWindowSize(ImVec2(Width, Height));
        FirstTime = false;
    }

    ImGui::SetNextWindowPos(ImVec2( WindowWidth - viewport.width,viewport.height));
    if (ImGui::Begin("Time Line", nullptr, ImGuiWindowFlags_NoCollapse))
    {

       // float Values[10] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 2.0f}; 
       // ImGui::PlotLines("Time:", Values, 10);

        ImGui::End();
    }

}

