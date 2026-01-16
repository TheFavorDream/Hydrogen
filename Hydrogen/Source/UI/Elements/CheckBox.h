#pragma once

#include "Common.h"
#include "Element.h"

namespace Hydrogen
{


	class Checkbox : public Element
	{
	public:


		int Setup(const std::string& pTitle, Vec2 pPosition)
		{
			m_Label = pTitle;
			m_Position = pPosition;
			return HYD_OK;
		}

		void Render() override
		{
			ImGui::SetCursorPos(ImVec2(m_Position.X, m_Position.Y));
			ImGui::Checkbox(m_Label.c_str(), &m_State);
		}

		inline bool& GetValue() { return m_State; }

	private:
		bool m_State;
	};

};