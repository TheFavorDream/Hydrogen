#include "Widget.h"
#include "../../../3rdParty/imgui/imgui.h"

namespace Hydrogen
{
namespace UI
{


//-------------------------Window----------------------------

    void Canvas::Render() noexcept
    {
        ImGui::Begin(m_Name.c_str());
        ImGui::Text("Hello, This is some Text");        
        ImGui::End();
    };


};
};