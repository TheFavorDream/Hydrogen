#pragma once

#include "Common.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "VecMath/Vector/Vectors.h"
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

		HYD int CreateUIWindow(const std::string& pTitle, VecF2 pSize, VecF2 pPosition, RefernceOrigin pOrigin);
		HYD void Render();



		HYD int CreateButton(  const std::string& pLabel, VecF2 pSize, VecF2 pPos, std::string pID="");
		HYD int CreateText(    const std::string& pText,  VecF2 pPos, std::string pID);
		HYD int CreateInputF(  const std::string& pLabel, VecF2 pPos, std::string pID="");
		HYD int CreateSliderF( const std::string& pLabel, VecF2 pPos, float pMin=0.0f, float pMax=1.0f, std::string pID="");
		HYD int CreateCheckbox(const std::string& pLabel, VecF2 pPos, std::string pID="");
		HYD int CreateComboBox(const std::string& pLabel, std::vector<std::string> Items, VecF2 pPos, std::string pID);


		HYD Button*  GetButton (const std::string& pID);
		HYD Text*    GetText   (const std::string& pID);
		HYD SliderF* GetSliderF(const std::string& pID);
		HYD InputF*  GetInput  (const std::string& pID);
		HYD Combo*   GetCombo  (const std::string& pID);


		HYD int CreateElement(Element* pElement, std::string pID = "");
		HYD Element* GetElement(const std::string& pID);


		//Recommended to only use these functions to set your window Size and Pos 
		HYD void SetWindowSize(float pWidth, float pHeight);
		HYD void SetWindowPos(float pX, float pY);

		//sets the Max and Min values for windows:
		HYD void SetSizeMax(float pWidth, float pHeight);
		HYD void SetSizeMin(float pWidth, float pHeight);


		HYD inline VecF2 GetMinSize() { return m_MinSize; }
		HYD inline VecF2 GetMaxSize() { return m_MaxSize; }


		HYD inline std::string& GetTitle() { return m_Title; }
		HYD inline VecF2 GetSize() { return m_Size; }
		HYD inline VecF2 GetPos() { return m_Position; }

	private:

		//Window Properties:
		std::string m_Title;
		VecF2 m_Size;
		VecF2 m_Position;
		VecF2 m_Origin;

		VecF2 m_MaxSize = VecF2(-1.0f);
		VecF2 m_MinSize = VecF2(-1.0f);

		bool m_FirstRender = false; //Used for setting the size of the window but only once

		std::unordered_map<std::string, Element*> m_Elements;
	};

};