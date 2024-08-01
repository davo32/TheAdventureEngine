#pragma once
#include "IncludeAllNodes.h"
#include <vector>
class Chapter
{
public:
	Chapter(std::string newName, ImVec2 mousePos);
	//Rendering
	void RenderViewport();
private:
	//Rendering
	void RenderBackground(ImVec2 canvasSize, ImVec2 canvasPos);
	void RenderContextMenu();
	void RenderNodes();
	void RenderNodeList();
	void RenderEventText();
	//Utilities
	void ViewportPanning(ImGuiIO& io);
	void ContextMenuOpen();

	//Node Functions
	void DeleteActiveNode();
	void NodeInteraction();
	void NodeDrag(ImVec2 mousePos);
	void ToggleSelection(Event* E, bool wasSelected);
	void DeselectCurrent();
	void SelectEvent(Event* E);

	void EventRenamer();
	void UpdateNodeNames(std::string newName);

	//- Chapter Vars
	
	std::string ChapterName;

	//- Viewport Vars
	ImVec2 viewportOffset = ImVec2(0, 0);
	ImVec2 lastMousePos = ImVec2(0, 0);

	// - Zoom Vars
	float zoomLevel = 1.0f;

	// - Panning Vars
	bool isPanning = false;

	//- Node Vars
	std::vector<Node*> NodeFamily;
	Node* ActiveNode;

	//- Context Menu Vars
	ImVec2 contextMenuPos;

	//- Event Vars
	std::unordered_map<Event*, bool> eventSelectionState;
	std::vector<Event*> events;
	Event* ActiveEvent;

	//- Search Vars
	std::string searchQuery;
	static char inputText[128];
	bool isContextualMenuOpen = false;
	
};

//Chapter Contains:
//Viewport (NodeGraph)
//Nodes
//Chapter Name
//

