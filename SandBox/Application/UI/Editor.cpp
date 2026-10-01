#include "Editor.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <vulkan/vulkan_core.h>

using namespace Hydrogen;

void Editor::Setup() 
{
    Hydrogen::Renderer::Self().InitUICore();

    SetStyle();
}

void Editor::Shutdown() 
{

}

void Editor::Event(
	Hydrogen::FrameEvent& pEvents
) 
{
	Hydrogen::UI::Core::Self().Event(pEvents);
    
    Editor::s_WindowWidth  = pEvents.WindowSize.X;
    Editor::s_WindowHeight = pEvents.WindowSize.Y;


    if (pEvents.IsViewportResized)
    {
        Editor::s_ViewportRatio = pEvents.ViewportSize;
        
        //Editor::s_UtilityWindowRatio.X = 
        Editor::s_UtilityWindowRatio.Y = 1.0f-(s_ViewportRatio.W /100.0f);
        
        Hydrogen::Log::SetInfo(
            Hydrogen::Log::FmtStr("Viewport Resized: Width:%f, Height:%f", pEvents.ViewportSize.Z,  pEvents.ViewportSize.W)
        );
    }


    if (pEvents.IsWindowResized)
    {
        s_UpdateEditorWindowSize  = true;
        s_UpdateUtilityWindowSize = true;

        Hydrogen::Log::SetInfo(
            Hydrogen::Log::FmtStr("Window Resized: X:%f, Y:%f", pEvents.WindowSize.X,  pEvents.WindowSize.Y)
        );
    }

}

void Editor::Update() 
{

}

void Editor::Render(
    Hydrogen::FrameRenderConfig& pRenderConf
) 
{
    Hydrogen::UI::Core::Self().NewFrame();

    Editor::RenderMainMenuBar();

    Editor::EditorWindow();
    
    Editor::UtilityWindow();


    Hydrogen::UI::Core::Self().Render();
} 

//----------------------Vars------------------

Hydrogen::Ptr<Hydrogen::Scene> Editor::s_SelectedScene = nullptr;
Hydrogen::Ptr<Hydrogen::Node>  Editor::s_SelectedNode  = nullptr;

float           Editor::s_WindowWidth  = 0.0f;
float           Editor::s_WindowHeight = 0.0f;
float           Editor::s_WindowBoarderSize;
float           Editor::s_LeftPanelWidthRatio;
Hydrogen::VecF2 Editor::s_UtilityWindowRatio;
Hydrogen::VecF4 Editor::s_ViewportRatio;

bool Editor::s_UpdateEditorWindowSize;
bool Editor::s_UpdateUtilityWindowSize;


bool  Editor::RenderGrid  = true;
float Editor::FogQuad     = 0.05f;
float Editor::FogLinear   = 0.0005f;
float Editor::FogConstant = 1.0f;




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


    s_ViewportRatio = Hydrogen::Renderer::Self().GetWindow().GetViewportRatio();

    s_LeftPanelWidthRatio  = 1.0f - (s_ViewportRatio.Z/100.0f);

    s_WindowBoarderSize = 20.0f;

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


void Editor::RenderMainMenuBar() noexcept
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {}

            
            if (ImGui::MenuItem("Open"))
            {}


            if (ImGui::MenuItem("Load GLTF"))
            {}

            if (ImGui::MenuItem("Exit"))
                Hydrogen::Core::s_Self->Terminate();
            

            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::EndMenu();
        }

        ImGui::Text("FPS:%f", ImGui::GetIO().Framerate); 
        ImGui::EndMainMenuBar();
    }
}



/*
	Purpose: Call this to set the Scene to be graphed
*/
void Editor::SetCurrentScene(
	Hydrogen::Ptr<Hydrogen::Scene> pScene
) noexcept
{
    s_SelectedScene = pScene;
    s_SelectedNode  = dynamic_cast<Hydrogen::Node*>(pScene);
}



/*
	Purpose: Draw the Editor Window
*/
void Editor::EditorWindow() noexcept
{

    float Width  = s_WindowWidth*(s_LeftPanelWidthRatio);
    float Height = s_WindowHeight-20.0f;

    if (s_UpdateEditorWindowSize)
    {
        ImGui::SetNextWindowSize(ImVec2(
            Width,
            Height
        ),
         ImGuiCond_Always
        );
        s_UpdateEditorWindowSize = false;
    }

    //Calculate Position:
    ImGui::SetNextWindowPos(ImVec2(0.0f, s_WindowBoarderSize), 0, ImVec2(0.0f, 0.0f));
    if (!ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize) || s_SelectedScene == nullptr)
    {
        ImGui::End();
        return;
    }

    
    DrawGraph(
        ImVec2(Width, Height/2.0f)
    );
    
    NodeEditor(
        ImVec2(Width, Height/2.0f)
    );

    ImGui::End();
}

void Editor::UtilityWindow() noexcept
{

    if (s_UpdateUtilityWindowSize)
    {
        ImGui::SetNextWindowSize(
            ImVec2(
                s_WindowWidth-(s_LeftPanelWidthRatio*s_WindowWidth), 
                s_WindowHeight*(s_UtilityWindowRatio.Y)
            )
        );

        //s_UpdateUtilityWindowSize = false;
    }

    ImGui::SetNextWindowPos(ImVec2(s_WindowWidth, s_WindowHeight), 0, ImVec2(1.0f, 1.0f));
    ImGui::Begin("Utility", nullptr, ImGuiWindowFlags_NoCollapse);
    

       // float Values[10] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 2.0f}; 
       // ImGui::PlotLines("Time:", Values, 10);


    if (ImGui::IsWindowFocused())
    {
        s_UtilityWindowRatio.Y = std::clamp(
            (ImGui::GetWindowHeight() / s_WindowHeight),
            1.0f-(s_ViewportRatio.W /100.0f), 1.0f
        );
    }

    ImGui::End();
    

}

void Editor::DrawGraph(
    ImVec2 pSize
) noexcept
{
    
    ImGui::BeginChild("Scene Graph", pSize);
    
    DrawTreeNode(
        *s_SelectedScene,
        ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DefaultOpen
    );

    ImGui::EndChild();
} 


/*
	Purpose: Node Editor Window
*/

void Editor::NodeEditor(
    ImVec2 pSize
) noexcept
{

    ImGui::BeginChild("Node Editor", pSize);

    if (!s_SelectedNode)
    {
        ImGui::EndChild();
        return;
    }


    ImGui::Text("Name:%s", s_SelectedNode->GetName().c_str());

    if (ImGui::CollapsingHeader("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
    {
        NodeTransformation(s_SelectedNode->GetTransform());
    }

    if (s_SelectedNode->HasMesh() && ImGui::CollapsingHeader("Mesh"))
    {
        MeshEditor(*s_SelectedNode->GetMesh().GetPtr());
    }

    if (s_SelectedNode->HasLight() && ImGui::CollapsingHeader("Light"))
    {
        //LightEditor(s_SelectedScene->EditLight(s_SelectedNode->GetLight()));
    }

    ImGui::EndChild();
}

void Editor::DrawTreeNode(
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
            
            s_SelectedNode = &pNode;
        }
        
        if (pNode.HasMesh())
            DrawTreeMesh(*pNode.GetMesh().GetPtr(), ImGuiTreeNodeFlags_Bullet);
        if (pNode.HasLight())
            DrawTreeLight(pNode.GetLight(), ImGuiTreeNodeFlags_Bullet);

        for (auto& child : pNode)
            DrawTreeNode(child, pFlags);

        ImGui::TreePop();
    }
}


/*
	Purpose: Renders the Node with a mesh
*/
void Editor::DrawTreeMesh(
	Hydrogen::Mesh& 	pMesh,
	ImGuiTreeNodeFlags  pFlags
) noexcept
{
    if (ImGui::TreeNodeEx(pMesh.GetName().c_str(), pFlags))
    {
        ImGui::TreePop();
    }
}

/*
	Purpose: Renders the Node's Light
*/
void Editor::DrawTreeLight(
	HYD_ID_SPACE 		pLight,
	ImGuiTreeNodeFlags  pFlags
) noexcept
{
    //const Hydrogen::Light& light = s_SelectedScene->AccessLight(pLight); 
    //if (ImGui::TreeNodeEx(light.Name().c_str(), pFlags))
    //{
    //    ImGui::TreePop();
    //}
}


/*
	Purpose: Light Editor 
*/


void Editor::LightEditor(
    Hydrogen::Light& pLight
) noexcept
{
    
    ImGui::ColorPicker3("Light Color", reinterpret_cast<float*>(&pLight.Color()));        
    ImGui::InputFloat3("Position:", reinterpret_cast<float*>(&pLight.Position()));
    
    ImGui::Text("Ligth Properties:");
    
    ImGui::InputFloat3("Ambient:",  reinterpret_cast<float*>(&pLight.Ambient()));
    ImGui::InputFloat3("Diffuse:",  reinterpret_cast<float*>(&pLight.Diffuse()));
    ImGui::InputFloat3("Specular:", reinterpret_cast<float*>(&pLight.Specular()));

} 



/*
	Purpose: Controll for Node Transformation:
*/
void Editor::NodeTransformation(
	Hydrogen::Transformation& pTrans
) noexcept
{
    ImGui::Text("Scale:");
    ImGui::InputFloat3("##Scale",     reinterpret_cast<float*>(&pTrans.t_Scale));


    const  char* RotateMethod[] = {"Quaternion", "Euler"};
    static int32 Selection = 0;

    ImGui::Combo("##RotateMethod", &Selection, RotateMethod, 2);


    ImGui::Text("Rotation:");
    switch (Selection)
    {
    case 0: //Quaternion
        ImGui::InputFloat4("##Rotate",    reinterpret_cast<float*>(&pTrans.t_Rotate));
        break;
    case 1: //Euler
        float EulerRot[3] = {};
        ImGui::InputFloat3("##Rotate", EulerRot);
        pTrans.t_Rotate = Hydrogen::Quaternion().Euler(EulerRot[0], EulerRot[1], EulerRot[2]);
        break;
    }
    
    ImGui::Text("Translation:");
    ImGui::InputFloat3("##Translate", reinterpret_cast<float*>(&pTrans.t_Translate));
}

/*
	Purpose: Node's Mesh Editor
*/
void Editor::MeshEditor(
	Hydrogen::Mesh& pMesh
) noexcept
{
    ImGui::Text("Name:%s", pMesh.GetName().c_str());
    //ImGui::Checkbox("Disable Mesh", pMesh.IsEnable());

}

/*
	Purpose: Node's Camera Editor:
*/
void Editor::CameraEditor(
	Hydrogen::Camera& pCamera
) noexcept
{

}


void Editor::HandleResize() noexcept
{
    ImVec2 CurrentSize     = ImGui::GetWindowSize();
    s_LeftPanelWidthRatio  = (CurrentSize.x / s_WindowWidth);
}




