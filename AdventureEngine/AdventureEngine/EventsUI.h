#pragma once
#include "UserInterface.h"
#include <string>
#include <GLFW/glfw3.h>
#include "Widget.h"
#include <vector>
#include "CurrentWindow.h"
#include "Node.h"
#include "EventNode.h"

class Application;
class EventsUI : public UserInterface
{
public:
	EventsUI()
	{
	}
	void DrawUI() override;

	void text_centered(const std::string& text);
	
	std::string EventWindowTitle = "Event Graph";
	
	//Rendering
	void RenderEventWindow();
	void RenderViewport();
	void ContextMenuOpen();
	void ViewportPanning(ImGuiIO& io);
	void NodeClicked(ImGuiIO& io);
	void RenderNodes();
	void DrawBackground(ImVec2 canvasSize, ImVec2 canvasPos);
	void DrawContextMenu();
	void RenderMenuBar(const char* title);
	void RenderComponents();
	void RenderEventsList();
	void RenderNodeInspector();
	void RenderEventRename();
	void RenderEventText();
	void RenderInspector();

	void DeleteActiveNode();
	void HandleNodeDragging(ImVec2 mousePos);
	void ToggleSelection(Event* E, bool wasSelected);
	void DeselectCurrent();
	void SelectEvent(Event* E);
	void UpdateNodeNames(const std::string& newName);
private:
	//Events
	std::vector<Event*> events;
	Event* ActiveEvent;
	CurrentWindow currWindow = CurrentWindow::GRAPH;
	int testcounter = 0;

	std::unordered_map<Event*, bool> eventSelectionState;

	//Viewport
	ImVec2 viewportOffset = ImVec2(0, 0);
	ImVec2 lastMousePos = ImVec2(0, 0);
	bool isPanning = false;
	float zoomLevel = 1.0f;

	//Nodes
	std::vector<Node*> Nodes;
	Node* ActiveNode;
	ImVec2 contextMenuPos;

	//Search
	std::string searchQuery;
	static char inputText[128];
	bool isContextualMenuOpen = false;
};

