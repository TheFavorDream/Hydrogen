#pragma once
#include "../../../Hydrogen/Hydrogen.h"
#include <vulkan/vulkan_core.h>


class Editor : public Hydrogen::Layer
{
public:

     Editor() = default;
    ~Editor() = default;

	//Layer Methods:

	void Setup()    override;
	void Shutdown() override;

	void Event(
		Hydrogen::FrameEvent& pEvents
	)  override;

	void Update() override;

	void Render(
		Hydrogen::RenderStates& pRenderStates
	) override; 


	//UI Methods:

	/*
		Purpose: Call this to set the Scene to be graphed
	*/
	static void SetCurrentScene(
		Hydrogen::Ptr<Hydrogen::Scene> pScene
	) noexcept;

//---------------------------Main Widgets----------------------------
	
	/*
		Purpose: Draw the Editor Window
	*/
	static void EditorWindow() noexcept;

	/*
		Purpose: Renders the Utility Window
	*/
	static void UtilityWindow()  noexcept;

//--------------------------Controller Widgets-------------------

	static void GridController() noexcept; 

private:

	void SetStyle() noexcept;

	/*
		Purpose: Main Menu Bar Handling
	*/

	static void RenderMainMenuBar() noexcept;

	/*
		Purpose: Draw the Graph window
	*/
	static void DrawGraph(
		ImVec2 pSize
	) noexcept;

	/*
		Purpose: Node Editor Window
	*/
	static void NodeEditor(
		ImVec2 pSize
	) noexcept;


	/*
		Purpose: Calls itself recursevly to draw each node in the tree
	*/
	static void DrawTreeNode(
		Hydrogen::Node& 	pNode,
		ImGuiTreeNodeFlags  pFlags
	) noexcept;

	/*
		Purpose: Renders the Node' mesh
	*/
	static void DrawTreeMesh(
		Hydrogen::Mesh& 	pMesh,
		ImGuiTreeNodeFlags  pFlags
	) noexcept;

	/*
		Purpose: Renders the Node's Light
	*/
	static void DrawTreeLight(
		HYD_ID_SPACE 		pLight,
		ImGuiTreeNodeFlags  pFlags
	) noexcept;


	/*
		Purpose: Controll for Node Transformation:
	*/
	static void NodeTransformation(
		Hydrogen::Transformation& pTrans
	) noexcept;

	/*
		Purpose: Node's Mesh Editor
	*/
	static void MeshEditor(
		Hydrogen::Mesh& pMesh
	) noexcept;

	/*
		Purpose: Node's Camera Editor:
	*/

	static void CameraEditor(
		Hydrogen::Camera& pCamera
	) noexcept;

	/*
		Purpose: Node's Light Editor
	*/
	static void LightEditor(
		Hydrogen::Light& pLight
	) noexcept;



	/*
		Purpose: Handles the Resizing of the windows
	*/
	static void HandleResize() noexcept;


public:

	static Hydrogen::Ptr<Hydrogen::Scene> s_SelectedScene;
	static Hydrogen::Ptr<Hydrogen::Node>  s_SelectedNode;

	static float 		   s_WindowWidth;
	static float 		   s_WindowHeight;
	static float 		   s_WindowBoarderSize;
	static float 		   s_LeftPanelWidthRatio;
	static Hydrogen::VecF2 s_UtilityWindowRatio;
	static Hydrogen::VecF4 s_ViewportRatio;

	static bool s_UpdateEditorWindowSize;
	static bool s_UpdateUtilityWindowSize;

	bool IsGridWindow       = false;

	static bool  RenderGrid;
	static float FogQuad     ;
	static float FogLinear   ;
	static float FogConstant ;

	ImFont* RegularFont = nullptr;

};