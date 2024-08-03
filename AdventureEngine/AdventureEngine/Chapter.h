#pragma once
#include "IncludeAllNodes.h"
#include <vector>
class Chapter
{
public:
	Chapter(std::string newName, ImVec2 mousePos);
	//Rendering
	void RenderViewport();
	std::string GetChapterName();
	//Utilities
	void NodeGraphShortcuts();
	// Coordinate transformation functions
	ImVec2 ScreenToViewport();
	ImVec2 ViewportToScreen(ImVec2 viewportPos);
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
	ImVec2 viewportOffset = ImVec2(300, 65);
	ImVec2 lastMousePos = ImVec2(0, 0);
	ImVec2 screenPos;
	ImVec2 screenSize;


	Node* dragStartNode = nullptr;
	int dragStartOutputIndex = -1;
	ImVec2 dragStartPos;

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

	//Connection Vars
	Node* sourceNode = nullptr;
	ImVec2 connectionEndPos;
	bool isConnecting = false;
	
};

//Chapter Contains:
//Viewport (NodeGraph)
//Nodes
//Chapter Name
//

