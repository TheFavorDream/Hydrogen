#include "UI.h"
#include "../../Log/Log.h"
#include "../Renderer.h"
#include <GLFW/glfw3.h>
#include <vector>

namespace Hydrogen
{

    Ptr<UI::Core> UI::Core::s_Self = nullptr;

    UI::Core& UI::Core::Self() noexcept {
        return *s_Self;
    }

    Ptr<UI::Core> UI::Core::Create() noexcept
    {
        if (s_Self)
            return s_Self;
        return new UI::Core();
    }



    uint32 UI::Core::Init(
        ImGuiVulkanInfo pInitInfo,
        Window&         pWindow
    ) noexcept
    {


        //Init ImGui Context:

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        m_ImGuiIO               = &ImGui::GetIO();
        m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

        ImGui::StyleColorsDark();

        if (!ImGui_ImplGlfw_InitForVulkan(pWindow.GetHandle(), true))
        {
            Log::SetError("Failed init GLFW Imgui");
            return HYD_FAILED;
        }

        //Allocate Descriptor Pool for imGui:

        std::vector<DescriptorPoolSize> PoolSizes = 
        {
            {HYD_DESCRIPTOR_TYPE_SAMPLED_IMAGE, IMGUI_IMPL_VULKAN_MINIMUM_SAMPLED_IMAGE_POOL_SIZE},
            {HYD_DESCRIPTOR_TYPE_SAMPLER,  IMGUI_IMPL_VULKAN_MINIMUM_SAMPLER_POOL_SIZE},
        };

        m_DescriptorPool         = Renderer::Self().CreateDescriptorPool(PoolSizes, UINT32_MAX, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
        pInitInfo.DescriptorPool = Renderer::Self().AccessDescriptorPool(m_DescriptorPool).GetHandle();

        //Vulkan Init
        if (!ImGui_ImplVulkan_Init( &pInitInfo))
        {
            Log::SetError("Failed init Vulkan Imgui");
            return HYD_FAILED;
        }



        return HYD_OK;
    }

    void UI::Core::Shutdown() noexcept
    {

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext();
    }



    void UI::Core::Render() noexcept
    {
        m_ImGuiIO->AddMousePosEvent(Mouse::GetCursorX(), Mouse::GetCursorY());
        m_ImGuiIO->AddMouseButtonEvent(GLFW_MOUSE_BUTTON_LEFT, Mouse::IsLeftKeyPressed());
        m_ImGuiIO->AddMouseButtonEvent(GLFW_MOUSE_BUTTON_RIGHT, Mouse::IsRightKeyPressed());
        m_ImGuiIO->AddMouseWheelEvent(0.0f,Mouse::GetScrollVerticalOffset());

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        for (auto& widget : m_Widgets)
            widget->Render();
        


        ImGui::Render();
        m_FrameDrawData = ImGui::GetDrawData();
    }

    void UI::Core::PushWidget(
        Ptr<Widget> pNewWidget
    ) noexcept
    {
        if (pNewWidget == nullptr)
            return;

        m_Widgets.push_back(pNewWidget);
    }
};


