#pragma once

#include "Common.h"
#include "Element.h"

namespace Hydrogen
{


	class Checkbox : public Element
	{
	public:


		HYD int Setup(const std::string& pTitle, VecF2 pPosition)
		{
			m_Label = pTitle;
			m_Position = pPosition;
			return HYD_OK;
		}

		HYD void Render() override
		{
			ImGui::SetCursorPos(ImVec2(m_Position.X, m_Position.Y));
			ImGui::Checkbox(m_Label.c_str(), &m_State);
		}

		HYD inline bool& GetValue() { return m_State; }

	private:
		bool m_State;
	};

};