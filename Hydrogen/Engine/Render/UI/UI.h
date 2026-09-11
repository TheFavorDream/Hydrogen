/*
    Hydrogen Uses Imgui as its UI lib. 
*/


#pragma once

#include "../../Common.h"
#include "../../../3rdParty/imgui/imgui.h"
#include "../../../3rdParty/imgui/imgui_impl_glfw.h"
#include "../../../3rdParty/imgui/imgui_impl_vulkan.h"
#include "../Window/Window.h"
#include "Widget.h"

namespace Hydrogen
{
namespace UI
{
    typedef ImGui_ImplVulkan_InitInfo ImGuiVulkanInfo;


    //Stores the UI Global Settings, such as Styling, Event Handling etc
    struct CoreConfiguration
    {

    };

    //Main UI Class
    class Core final
    {
    public: 
        static Core& Self() noexcept;
    private:

        static Ptr<Core> Create() noexcept;

        //Must be Initlized by the Renderer
        HYD uint32 Init(
            ImGuiVulkanInfo pInitInfo,
            Window&         pWindow
        ) noexcept;
    
        HYD void   Shutdown() noexcept;

    public:

        HYD  Core() = default;
        HYD ~Core() = default;

        HYD Core(Core&& )      = delete;
        HYD Core(const Core& ) = delete;

        HYD Core& operator=(Core&& )      = delete;
        HYD Core& operator=(const Core& ) = delete;



        HYD void   Render()   noexcept; //Renders the GUI
        
        //Public Functions:
        HYD void PushWidget(
            Ptr<Widget> pNewWidget
        ) noexcept;

    private: // internal Vars

        Ptr<ImGuiIO>         m_ImGuiIO = nullptr;
        HYD_ID_SPACE         m_DescriptorPool;
        std::vector<Widget*> m_Widgets;
        Ptr<ImDrawData>      m_FrameDrawData = nullptr;

    private: //static Vars
        static Ptr<Core> s_Self;

    private: //friend decl
        friend class Hydrogen::Renderer;
    };

};
};