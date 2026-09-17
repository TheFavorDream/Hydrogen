#pragma once
#include "../../../Hydrogen/Hydrogen.h"
#include <vulkan/vulkan_core.h>



/*
	Purpose: Scene Graph UI
*/
class SceneGraph
{
public:

	 SceneGraph() = delete;
	~SceneGraph() = delete;

	/*
		Purpose: Call this to set the Scene to be graphed
	*/
	static void SetCurrentScene(
		Hydrogen::Ptr<Hydrogen::Scene> pScene
	) noexcept;

	/*
		Purpose: Draw the Graph window
	*/
	static void DrawGraph() noexcept;

	/*
		Purpose: Node Editor Window
	*/
	static void NodeEditor(
		Hydrogen::Node& pNode
	) noexcept;


	/*
		Purpose: Light Editor 
	*/
	static void LightEditor(
		Hydrogen::Light& pLight
	) noexcept; 


private:
	
	/*
		Purpose: Calls itself recursevly to draw each node in the tree
	*/
	static void DrawTreeNode(
		Hydrogen::Node& 	pNode,
		ImGuiTreeNodeFlags  pFlags
	) noexcept;

public:
	static Hydrogen::Ptr<Hydrogen::Scene> s_CurrentScene;
	static Hydrogen::Ptr<Hydrogen::Node>  s_SelectedNode;
	static Hydrogen::Ptr<Hydrogen::Light> s_SelectedLight;


	static 	bool 						  s_IsNodeEditor;
	static 	bool 						  s_IsLightEditor; 		

};


class Editor : public Hydrogen::Layer
{
public:

     Editor() = default;
    ~Editor() = default;


	void Setup()    override;
	void Shutdown() override;

	void Event(
		Hydrogen::FrameEvent& pEvents
	)  override;

	void Update() override;

	void Render(
		Hydrogen::FrameRenderConfig& pRenderConf
	) override; 


	static void GridController()  	   noexcept; 

private:

	void SetStyle() noexcept;

public:
	bool IsGridWindow       = false;
	bool IsSceneGraghWindow = true;


	static bool  RenderGrid;
	static float FogQuad     ;
	static float FogLinear   ;
	static float FogConstant ;

	ImFont* RegularFont = nullptr;

};