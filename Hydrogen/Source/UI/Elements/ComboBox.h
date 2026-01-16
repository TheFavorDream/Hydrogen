#pragma once

#include "Common.h"
#include "Element.h"

namespace Hydrogen
{


	class Combo : public Element
	{
	public:


		int Setup(const std::string& pTitle, const std::vector<std::string>& pItems, Vec2 pPosition)
		{
			m_Label = pTitle;
			m_Position = pPosition;
			m_Items = pItems;
			return HYD_OK;
		}

		void Render() override
		{
			ImGui::SetCursorPos(ImVec2(m_Position.X, m_Position.Y));

			if (ImGui::BeginCombo(m_Label.c_str(), m_Items[m_CurrentItem].c_str()))
			{
				for (int i = 0; i < m_Items.size(); ++i)
				{
					int32 IsSelected;
					if (ImGui::Selectable(m_Items[i].c_str(), &IsSelected))
					{
						m_CurrentItem = i;
					}

					if (IsSelected)
						ImGui::SetItemDefaultFocus();

				}

				ImGui::EndCombo();
			}
			

			
		}

		inline int32& GetCurrentItem() { return m_CurrentItem; }
		inline std::vector<std::string>& GetItems() { return m_Items; }

	private:
		int32 m_CurrentItem =0;
		std::vector<std::string> m_Items;
	};

};