#pragma once

#include "Common.h"
#include "Element.h"

namespace Hydrogen
{

	class Text : public Element
	{
	public:


		HYD int  Setup(const std::string& pText, Vec2 pPosition)
		{
			m_Label = pText;
			m_Position = pPosition;
			return HYD_OK;
		}

		HYD void Render() override
		{
			ImGui::SetCursorPos(ImVec2(m_Position.X, m_Position.Y));
			ImGui::Text(m_Label.c_str());
		}

		
	
	};

};