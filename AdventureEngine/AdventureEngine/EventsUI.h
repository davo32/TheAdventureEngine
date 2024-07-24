#pragma once
#include "UserInterface.h"
#include <string>
#include <GLFW/glfw3.h>
#include "Widget.h"
#include <vector>
#include "CurrentWindow.h"
#include "Node.h"

class Application;
class EventsUI : public UserInterface
{
public:
	EventsUI() : ActiveWidget(nullptr) {}
	void DrawUI() override;

	void TextCentered(std::string text);
	void TextCenteredInMenuBar(std::string text);
	void HierachyElement();
	void EventSheetElement();
	void Test();
	Widget* GetWidgetByRef(Widget* ref);
	void DrawEventDetails(Event* event);
	void DrawViewport();
	void DrawContextMenu();
	void DrawBackground(ImVec2 canvasSize, ImVec2 canvasPos);
	std::string EventWindowTitle = "Event Creation";
private:
	//Events
	std::vector<Widget*> Buttons;
	Widget* ActiveWidget;
	CurrentWindow currWindow = CurrentWindow::CREATE;
	int testcounter = 0;

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

