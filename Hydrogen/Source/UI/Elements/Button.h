#pragma once

#include "Common.h"
#include "Element.h"

namespace Hydrogen
{

	class Button : public Element
	{
	public:

		HYD int Setup(const std::string& pLabel, Vec2 pSize, Vec2 pPosition)
		{
			
			m_Label = pLabel;
			m_Size = pSize;
			m_Position = pPosition;
			return HYD_OK;
			
		}

		HYD void Render() override
		{
			ImGui::SetCursorPos(ImVec2(m_Position.X, m_Position.Y));
			m_State = ImGui::Button(m_Label.c_str(), ImVec2(m_Size.X, m_Size.Y));
		}


		HYD inline bool GetState() { return m_State; }
	private:
		bool m_State;
	};

};
