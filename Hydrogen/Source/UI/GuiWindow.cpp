#include "GuiWindow.h"
#include "HydPch.h"


namespace Hydrogen
{
	GuiWindow::~GuiWindow()
	{
		for (auto&i : m_Elements)
		{
			delete i.second;
		}
	}
	int GuiWindow::CreateUIWindow(const std::string& pTitle, VecF2 pSize, VecF2 pPosition, RefernceOrigin pOrigin)
	{
		m_Title = pTitle;
		m_Size = pSize;
		m_Position = pPosition;

		switch (pOrigin)
		{
		case UP_LEFT:    m_Origin = Vec2(0.0f, 0.0f); break;
		case DOWN_LEFT:  m_Origin = Vec2(0.0f, 1.0f); break;
		case UP_RIGHT:   m_Origin = Vec2(1.0f, 0.0f); break;
		case DOWN_RIGHT: m_Origin = Vec2(1.0f, 1.0f); break;
		}

		return HYD_OK;
	}

	void GuiWindow::Render()
	{


		//ImGui::SetNextWindowSizeConstraints(m_MinSize, m_MaxSize);
		ImGui::SetNextWindowSize(m_Size);
		ImGui::SetNextWindowPos(m_Position, 0, m_Origin);
		ImGui::Begin(m_Title.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		
		for (auto& i : m_Elements)
		{
			i.second->Render();
		}

		ImGui::End();

	}

	int GuiWindow::CreateButton(const std::string& pLabel, VecF2 pSize, VecF2 pPos, std::string pID)
	{

		if (pID.size() == 0)
		{
			pID = pLabel;
		}

		Button* button = new Button();
		button->Setup(pLabel, pSize, pPos);
		return CreateElement(button, pID);
	}

	int GuiWindow::CreateText(const std::string& pText, VecF2 pPos, std::string pID)
	{

		Text* text = new Text();
		text->Setup(pText, pPos);
		return CreateElement(text, pID);
	}

	int GuiWindow::CreateInputF(const std::string& pLabel, VecF2 pPos, std::string pID)
	{
		if (pID.size() == 0)
		{
			pID = pLabel;
		}
		InputF* input = new InputF();
		input->Setup(pLabel, pPos);
		return CreateElement(input, pID);
	}

	int GuiWindow::CreateSliderF(const std::string& pLabel, VecF2 pPos, float pMin, float pMax, std::string pID)
	{
		if (pID.size() == 0)
		{
			pID = pLabel;
		}
		SliderF* input = new SliderF();
		input->Setup(pLabel, pPos);
		return CreateElement(input, pID);
	}

	int GuiWindow::CreateCheckbox(const std::string& pLabel, VecF2 pPos, std::string pID)
	{
		if (pID.size() == 0)
		{
			pID = pLabel;
		}
		Checkbox* input = new Checkbox();
		input->Setup(pLabel, pPos);
		return CreateElement(input, pID);
	}

	int GuiWindow::CreateComboBox(const std::string& pLabel, std::vector<std::string> Items, VecF2 pPos, std::string pID)
	{
		Combo* combo = new Combo();
		combo->Setup(pLabel, Items, pPos);
		return CreateElement(combo, pID);
	}

	Button * GuiWindow::GetButton(const std::string& pID)
	{
		return dynamic_cast<Button*>(GetElement(pID));
	}

	Text * GuiWindow::GetText(const std::string& pID)
	{
		return dynamic_cast<Text*>(GetElement(pID));
	}

	SliderF * GuiWindow::GetSliderF(const std::string& pID)
	{
		return dynamic_cast<SliderF*>(GetElement(pID));
	}

	InputF * GuiWindow::GetInput(const std::string& pID)
	{
		return dynamic_cast<InputF*>(GetElement(pID));
	}

	Combo * GuiWindow::GetCombo(const std::string& pID)
	{
		return dynamic_cast<Combo*>(GetElement(pID));
	}

	int GuiWindow::CreateElement(Element* pElement, std::string pID)
	{
		if (m_Elements.find(pID) != m_Elements.end())
		{
			Log::SetError("Element Already Exists", HYD_UI_INVALID_ELEMENT, __FILE__, __LINE__);
			return HYD_UI_INVALID_ELEMENT;
		}

		m_Elements[pID] = pElement;
		return HYD_OK;
	}

	Element * GuiWindow::GetElement(const std::string& pID)
	{
		return m_Elements[pID];
	}


	void GuiWindow::SetWindowSize(float pWidth, float pHeight)
	{

		if (pWidth < 0 || pHeight < 0)
			return;

		Vec2 DesiredSize = Vec2(pWidth, pHeight);
		m_Size = Clamp(m_MinSize, m_MaxSize, DesiredSize);
	}

	void GuiWindow::SetWindowPos(float pX, float pY)
	{
		m_Position.X = (pX >= 0) ? pX : m_Position.X; //If value was negative skip it
		m_Position.Y = (pY >= 0) ? pY : m_Position.Y;
	}

	void GuiWindow::SetSizeMax(float pWidth, float pHeight)
	{
		m_MaxSize.X = (pWidth >= 0) ? pWidth : m_MaxSize.X; //If value was negative skip it
		m_MaxSize.Y = (pHeight >= 0) ? pHeight : m_MaxSize.Y;
	}

	void GuiWindow::SetSizeMin(float pWidth, float pHeight)
	{
		m_MinSize.X = (pWidth >= 0) ? pWidth : m_MinSize.X; //If value was negative skip it
		m_MinSize.Y = (pHeight >= 0) ? pHeight : m_MinSize.Y;
	}

};