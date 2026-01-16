#pragma once

#include "Common.h"
#include "Element.h"

namespace Hydrogen
{


	class InputF : public Element
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
			ImGui::InputFloat(m_Label.c_str(), &m_Value);
		}

		inline float& GetValue() { return m_Value; }

	private:
		float m_Value;
	};

};