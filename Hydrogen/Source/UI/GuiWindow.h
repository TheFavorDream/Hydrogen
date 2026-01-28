#pragma once

#include "Common.h"
#include "3rdParty/imgui/imgui.h"
#include "3rdParty/imgui/imgui_impl_opengl3.h"
#include "3rdParty/imgui/imgui_impl_glfw.h"
#include "Math/Vector/VectorDef.h"
#include "Elements/Element.h"
#include "Elements/Button.h"
#include "Elements/Text.h"
#include "Elements/InputF.h"
#include "Elements/SliderF.h"
#include "Elements/CheckBox.h"
#include "Elements/ComboBox.h"
#include <unordered_map>
#include <string>
#include "Log/Log.h"
namespace Hydrogen
{


	enum  RefernceOrigin {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
	class GuiWindow
	{
	public:

		HYD GuiWindow() = default;
		HYD ~GuiWindow();

		HYD int CreateUIWindow(const std::string& pTitle, Vec2 pSize, Vec2 pPosition, RefernceOrigin pOrigin);
		HYD void Render();



		HYD int CreateButton(std::string pLabel, Vec2 pSize, Vec2 pPos, std::string pID="");
		HYD int CreateText(std::string pText, Vec2 pPos, std::string pID);
		HYD int CreateInputF(std::string pLabel, Vec2 pPos, std::string pID="");
		HYD int CreateSliderF(std::string pLabel, Vec2 pPos, float pMin=0.0f, float pMax=1.0f, std::string pID="");
		HYD int CreateCheckbox(std::string pLabel, Vec2 pPos, std::string pID="");
		HYD int CreateComboBox(std::string pLabel, std::vector<std::string> Items, Vec2 pPos, std::string pID);


		HYD Button* GetButton(std::string pID);
		HYD Text* GetText(std::string pID);
		HYD SliderF* GetSliderF(std::string pID);
		HYD InputF* GetInput(std::string pID);
		HYD Combo* GetCombo(std::string pID);


		HYD int CreateElement(Element* pElement, std::string pID = "");
		HYD Element* GetElement(const std::string& pID);


		//Recommended to only use these functions to set your window Size and Pos 
		HYD void SetWindowSize(float pWidth, float pHeight);
		HYD void SetWindowPos(float pX, float pY);

		//sets the Max and Min values for windows:
		HYD void SetSizeMax(float pWidth, float pHeight);
		HYD void SetSizeMin(float pWidth, float pHeight);


		HYD inline Vec2 GetMinSize() { return m_MinSize; }
		HYD inline Vec2 GetMaxSize() { return m_MaxSize; }


		HYD inline std::string& GetTitle() { return m_Title; }
		HYD inline Vec2 GetSize() { return m_Size; }
		HYD inline Vec2 GetPos() { return m_Position; }

	private:

		//Window Properties:
		std::string m_Title;
		Vec2 m_Size;
		Vec2 m_Position;
		Vec2 m_Origin;

		Vec2 m_MaxSize = Vec2(-1.0f);
		Vec2 m_MinSize = Vec2(-1.0f);

		bool m_FirstRender = false; //Used for setting the size of the window but only once

		std::unordered_map<std::string, Element*> m_Elements;
	};

};