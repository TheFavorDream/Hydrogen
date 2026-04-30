#pragma once

#include "Common.h"
#include "Element.h"

namespace Hydrogen
{


	class SliderF : public Element
	{
	public:


		HYD int Setup(const std::string& pTitle, VecF2 pPosition, float pMin=0.0f, float pMax=1.0f)
		{
			m_Label = pTitle;
			m_Position = pPosition;
			m_Max = pMax;
			m_Min = pMin;
			return HYD_OK;
		}

		HYD void Render() override
		{
			ImGui::SetCursorPos(ImVec2(m_Position.X, m_Position.Y));
			ImGui::SliderFloat(m_Label.c_str(), &m_Value, m_Min, m_Max);
		}

		HYD inline float& GetValue() { return m_Value; }
		HYD inline float& GetMin() { return m_Min; }
		HYD inline float& GetMax() { return m_Max; }


	private:
		float m_Value;
		float m_Min;
		float m_Max;
	};

};