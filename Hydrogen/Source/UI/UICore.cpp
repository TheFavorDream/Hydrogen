#include "UICore.h"

namespace Hydrogen
{

	UICore* UICore::s_Self = nullptr;

	UICore* UICore::Self()
	{
		if (s_Self == nullptr)
			s_Self = new UICore();

		return s_Self;
	}

	int UICore::InitUI()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.IniFilename = nullptr;
		
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(Window::GetCurrentWindow()->GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
		
		

		//Config The Style:

		ImGui::GetStyle().WindowRounding = 2.0f;
		ImGui::GetStyle().FrameRounding = 6.0f;

		Vec4 HoveredColor = Average(m_BackgroundColor, m_ForeGroundColor);
		Vec4 FrameBGColor = Average(m_BackgroundColor, HoveredColor);
		Vec4 ActiveColor = Average(HoveredColor, FrameBGColor);


		ImGui::GetStyle().Colors[ImGuiCol_WindowBg]				 = m_BackgroundColor;
		ImGui::GetStyle().Colors[ImGuiCol_TitleBg]				 = m_BackgroundColor;
		ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive]		 = m_BackgroundColor;

		ImGui::GetStyle().Colors[ImGuiCol_Text]					 = m_ForeGroundColor;
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive]		 = m_ForeGroundColor;
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrab]			 = m_ForeGroundColor;
		ImGui::GetStyle().Colors[ImGuiCol_CheckMark]			 = m_ForeGroundColor;
		ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive]	 = m_ForeGroundColor;
		ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab]		 = m_ForeGroundColor;
		ImGui::GetStyle().Colors[ImGuiCol_Border]				 = m_ForeGroundColor;

		ImGui::GetStyle().Colors[ImGuiCol_FrameBg]				 = FrameBGColor;
		ImGui::GetStyle().Colors[ImGuiCol_Button]				 = FrameBGColor;

		ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]		 = HoveredColor;
		ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered]		 = HoveredColor;

		ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]			 = ActiveColor;
		ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive]		 = ActiveColor;

		ImGui::GetStyle().Colors[ImGuiCol_Header]				 = FrameBGColor;
		ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]			 = ActiveColor;
		ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]		 = HoveredColor;

		return HYD_OK;
	}

	int UICore::ShutdownUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		return HYD_OK;
	}

	int UICore::CreateUIWindow(std::string pTitle, Vec2 pSize, Vec2 pPos, RefernceOrigin pOrigin, std::string pID)
	{
		if (m_GUIs.find(pID) != m_GUIs.end())
		{
			Log::SetError(std::string("ID:" + pID + " Already exists!").c_str(), HYD_UI_INVALID_WINDOW, __FILE__, __LINE__);
			return HYD_UI_INVALID_WINDOW;
		}

		if (pID.size() == 0)
			pID = pTitle;

		int Err = m_GUIs[pID].CreateUIWindow(pTitle, pSize, pPos, pOrigin);
		return Err;
	}

	void UICore::Render()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (auto& i : m_GUIs)
		{
			i.second.Render();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	GuiWindow * UICore::GetWindow(std::string pID)
	{
		return &m_GUIs[pID];
	}

};