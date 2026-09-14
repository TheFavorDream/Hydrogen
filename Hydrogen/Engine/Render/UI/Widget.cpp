#include "Widget.h"
#include "../../../3rdParty/imgui/imgui.h"
#include <cstdint>

namespace Hydrogen
{
namespace UI
{


//-------------------------Window----------------------------

    Canvas::Canvas(
        const std::string& pName, 
        VecI2              pSize,
        VecI2              pPosition// = VecI2(INT32_MAX) 
    ) noexcept 
    {
        m_Label    = pName;
        m_Size     = pSize;
        m_Position = pPosition;
    }

    void Canvas::Render() noexcept
    {
        ImGui::SetNextWindowPos(ImVec2(m_Position.X, m_Position.Y));
        ImGui::SetNextWindowSize(ImVec2(m_Size.X, m_Size.Y));

        ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

        ImGui::Begin(m_Label.c_str(), nullptr, WindowFlags);

        for (auto& widget : m_Widgets)
            widget->Render();

        ImGui::End();
    };

    void Canvas::PushWidget(
        Ptr<Widget> pWidget
    ) noexcept
    {
        m_Widgets.push_back(pWidget);
    }

//--------------------------------Button-------------------------------------


    Button::Button(
        const std::string& pLabel,
        VecI2              pPosition,
        VecI2              pSize     //= VecI2(INT32_MAX) //Auto adjust
    ) noexcept
    {
        m_Label    = pLabel;
        m_Position = pPosition;
        m_Size     = pSize;
    }


    /*
        Purpose: Render the button:
    */
    void  Button::Render() noexcept
    {
        ImGui::SetCursorPos(ImVec2(m_Position.X, m_Position.Y));
        m_IsPressed = ImGui::Button(m_Label.c_str());
    }

};
};