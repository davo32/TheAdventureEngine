#include "EventsUI.h"
#include "Application.h"
#include "MonitorInfo.h"
#include <iostream>

//Node Inspector
//If Event:
// - Allow Event to be editable (Input Fields for Name and Event Text)
// Else if Character:
// - Allow Character Name to be editable (InputField)
// - List of character sayings (in a drop down list)
// Else if LootTable:
// - Display editable/add/remove list of Items with their drop chance (also editable)
// Else if Item:
// - Allow Item to be editable (Input Field for Item Name)
// - (Sldier for quantity) - Dropdown for Item Type

char EventsUI::inputText[128] = "";

void EventsUI::DrawUI()
{
	//Draw UI Here
	RenderInspector();
	RenderEventWindow();
}

void EventsUI::text_centered(const std::string& text)
{
	// Get the size of the available content region
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();

	// Calculate the size of the text
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

	// Calculate the position to center the text
	ImVec2 textPos;
	textPos.x = (contentRegion.x - textSize.x) * 0.5f;
	textPos.y = (contentRegion.y - textSize.y) * 0.5f;

	// Set the cursor position and draw the text
	ImGui::SetCursorPos(textPos);
	ImGui::Text(text.c_str());
}

void EventsUI::RenderEventWindow()
{
	ImGui::SetWindowPos(ImVec2(350, -MonitorInfo::GetMode()->height + 80));
	if (ImGui::BeginChild
	("EditableEvents", ImVec2(static_cast<float>(MonitorInfo::GetMode()->width) - 400, static_cast<float>(MonitorInfo::GetMode()->height) - 80.0f), true,
	 ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.2, 0.0));
			TextCenteredInMenuBar(EventWindowTitle);
			ImGui::EndMenuBar();
			ImGui::PopStyleColor();
		}

		if (ImGui::BeginChild("Event Window",
			ImVec2(MonitorInfo::GetMode()->width - 414.5, ImGui::GetContentRegionAvail().y), true, ImGuiChildFlags_Border))
		{
			if (currWindow == CurrentWindow::GRAPH)
			{
				RenderViewport();
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
	}
}

void EventsUI::RenderViewport()
{
	ImVec2 canvasPos = ImGui::GetCursorScreenPos(); // Top-left
	ImVec2 canvasSize = ImGui::GetContentRegionAvail(); // Size of the drawing area

	ImGuiIO& io = ImGui::GetIO();

	DrawBackground(canvasSize, canvasPos);

	std::string ZoomLevelText = " x " + std::to_string(zoomLevel);
	ImGui::Text(ZoomLevelText.c_str());

	// Handle panning
	if (ImGui::IsMouseHoveringRect(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y)))
	{
		ViewportPanning(io);

		// Handle zoom
		float zoomDelta = io.MouseWheel * 0.1f; // Adjust the zoom speed here
		zoomLevel = std::max(0.7f, std::min(2.0f, zoomLevel + zoomDelta)); // Prevent zooming too far out/in

		ContextMenuOpen();
		DrawContextMenu();
	}

	NodeClicked(io);
	HandleNodeDragging(io.MousePos);
	RenderNodes();
}

void EventsUI::ContextMenuOpen()
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 offset(10.0f, 10.0f); // Offset from the mouse position
		contextMenuPos = mousePos; // Store the position for node creation
		ImGui::SetNextWindowPos(ImVec2(mousePos.x + offset.x, mousePos.y + offset.y));
		ImGui::OpenPopup("Context Menu");
	}
}

void EventsUI::ViewportPanning(ImGuiIO& io)
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
	{
		isPanning = true;
		lastMousePos = io.MousePos;
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
	{
		isPanning = false;
	}

	if (isPanning)
	{
		ImVec2 delta = ImVec2(io.MousePos.x - lastMousePos.x, io.MousePos.y - lastMousePos.y);
		viewportOffset.x += delta.x;
		viewportOffset.y += delta.y;
		lastMousePos = io.MousePos;
	}
}

void EventsUI::NodeClicked(ImGuiIO& io)
{
	// Handle node dragging and selection on mouse down
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		bool nodeClicked = false;

		for (Node* node : Nodes)
		{
			ImVec2 nodePos = node->GetPosition();
			ImVec2 nodeSize = node->GetSize();

			// Apply zoom and viewport offset to the node's position and size
			ImVec2 scaledNodePos = ImVec2((nodePos.x * zoomLevel) + viewportOffset.x, (nodePos.y * zoomLevel) + viewportOffset.y);
			ImVec2 scaledNodeSize = ImVec2(nodeSize.x * zoomLevel, nodeSize.y * zoomLevel);

			// Check if the mouse is over the node
			if (io.MousePos.x >= scaledNodePos.x && io.MousePos.x <= scaledNodePos.x + scaledNodeSize.x &&
				io.MousePos.y >= scaledNodePos.y && io.MousePos.y <= scaledNodePos.y + scaledNodeSize.y)
			{
				if (ActiveNode != nullptr)
				{
					ActiveNode->SetIsActive(false);
				}
				ActiveNode = node;
				node->SetIsActive(true);
				node->SetIsDragging(true);
				ImVec2 mousePosInWorld = ImVec2((io.MousePos.x - viewportOffset.x) / zoomLevel, (io.MousePos.y - viewportOffset.y) / zoomLevel);
				node->SetDragStartPos(mousePosInWorld);
				nodeClicked = true;
				break;
			}
		}

		// Deselect if clicked on empty space within the viewport
		if (!nodeClicked)
		{
			if (ActiveNode != nullptr)
			{
				ActiveNode->SetIsActive(false);
				ActiveNode = nullptr;
			}
		}
	}
}

void EventsUI::RenderNodes()
{
	// Draw nodes after handling interaction and context menu
	if (!Nodes.empty())
	{
		for (Node* node : Nodes)
		{
			ImVec2 nodePosition = node->GetPosition();
			ImVec2 nodeSize = node->GetSize();

			// Apply zoom and viewport offset
			ImVec2 scaledPosition = ImVec2((nodePosition.x * zoomLevel) + viewportOffset.x, (nodePosition.y * zoomLevel) + viewportOffset.y);
			ImVec2 scaledSize = ImVec2(nodeSize.x * zoomLevel, nodeSize.y * zoomLevel);

			// Draw the node with adjusted position and size
			node->DrawNode(scaledPosition, scaledSize, zoomLevel);
		}
	}
}

void EventsUI::DeleteActiveNode()
{
	if (ActiveNode != nullptr)
	{
		EventNode* ActiveEventNode = dynamic_cast<EventNode*>(ActiveNode);
		// Get the associated event before deleting the node
		Event* associatedEvent = ActiveEventNode->GetEvent();

		if (associatedEvent != nullptr)
		{

			// Find and delete the node
			auto nodeIt = std::find(Nodes.begin(), Nodes.end(), ActiveNode);
			if (nodeIt != Nodes.end())
			{
				// Erase node from vector
				Nodes.erase(nodeIt);

				// Delete the node
				delete ActiveNode;
				ActiveNode = nullptr;
			}

			// Find and delete the associated event
			if (associatedEvent != nullptr)
			{
				auto eventIt = std::find(events.begin(), events.end(), associatedEvent);
				if (eventIt != events.end())
				{
					events.erase(eventIt);
					delete associatedEvent;
				}
			}
		}
		else
		{
			std::cerr << "Associated Event is null" << std::endl;
		}

	}
}

void EventsUI::HandleNodeDragging(ImVec2 mousePos)
{
	// Handle node dragging
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		if (ActiveNode && ActiveNode->IsDragging())
		{
			// Calculate the mouse position in world space
			ImVec2 mousePosInWorld = ImVec2((mousePos.x - viewportOffset.x) / zoomLevel, (mousePos.y - viewportOffset.y) / zoomLevel);

			// Calculate the delta movement in world space
			ImVec2 delta = ImVec2(mousePosInWorld.x - ActiveNode->GetDragStartPos().x, mousePosInWorld.y - ActiveNode->GetDragStartPos().y);

			// Update the node's position
			ImVec2 newPosition = ImVec2(ActiveNode->GetPosition().x + delta.x, ActiveNode->GetPosition().y + delta.y);
			ActiveNode->SetPosition(newPosition);

			// Update the drag start position for the next frame
			ActiveNode->SetDragStartPos(mousePosInWorld);

			// Debug output
			std::cout << " New Position: (" << newPosition.x << ", " << newPosition.y << ")" << '\n';
		}
	}

	// Update dragging state on mouse release
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		if (ActiveNode)
		{
			ActiveNode->SetIsDragging(false);
		}
	}
}

void EventsUI::RenderMenuBar(const char* title)
{
	if (ImGui::BeginMenuBar())
	{
		TextCenteredInMenuBar(title);
		ImGui::EndMenuBar();
	}
}

void EventsUI::RenderComponents()
{
	if (!ImGui::BeginChild("Components", ImVec2(280, MonitorInfo::GetMode()->height / 2), true, ImGuiWindowFlags_MenuBar))
		return;

	RenderMenuBar("Components");
	RenderEventsList();

	ImGui::EndChild();
}

void EventsUI::RenderEventsList()
{
	ImGui::TreePush("##Root");
	for (auto& E : events)
	{
		if (!E) continue;

		std::string eventName = E->GetEventName();
		std::cout << "Event Name: " << eventName << ", Length: " << eventName.length() << std::endl;

		std::string selectableID = "[Event]  " + eventName;

			bool wasSelected = (ActiveEvent == E);

			if (ImGui::Selectable(selectableID.c_str(), wasSelected)) 
			{
				ToggleSelection(E, wasSelected);
			}
	}
	ImGui::TreePop();
}

void EventsUI::ToggleSelection(Event* E, bool wasSelected)
{
	if (wasSelected)
	{
		DeselectCurrent();
	}
	else
	{
		DeselectCurrent();
		SelectEvent(E);
	}
}

void EventsUI::DeselectCurrent()
{
	ActiveEvent = nullptr;
	if (ActiveNode) {
		ActiveNode->SetIsActive(false);
		ActiveNode = nullptr;
	}
}

void EventsUI::SelectEvent(Event* E)
{
	ActiveEvent = E;
	ActiveNode = nullptr;
	for (Node* N : Nodes)
	{
		EventNode* EN = dynamic_cast<EventNode*>(N);
		if (EN->GetEvent() == ActiveEvent)
		{
			ActiveNode = EN;
			ActiveNode->SetIsActive(true);
			break;
		}
	}
}

void EventsUI::RenderNodeInspector()
{
	if (!ActiveNode && !ActiveEvent) return;

	if (!ImGui::BeginChild("Node Inspector", ImVec2(280, (MonitorInfo::GetMode()->height / 2) - 120), true, ImGuiWindowFlags_MenuBar))
		return;

	RenderMenuBar("Node");
	RenderEventRename();
	RenderEventText();

	ImGui::EndChild();
}

void EventsUI::RenderEventRename()
{
	if (!ActiveEvent && !ActiveNode) return;

	// Get the current event name
	std::string eventName = ActiveEvent ? ActiveEvent->GetEventName() : ActiveNode->GetText();
	char temp[256];
	strcpy_s(temp, sizeof(temp), eventName.c_str());

	ImGui::Text("Name: ");
	ImGui::SameLine();
	if (ImGui::InputText("##Name", temp, sizeof(temp))) {
		// Update the event or node name
		if (ActiveEvent) {
			ActiveEvent->SetEventName(temp);
			for (Node* E : Nodes)
			{
				EventNode* EN = dynamic_cast<EventNode*>(E);
				if (ActiveEvent == EN->GetEvent())
				{
					ActiveNode = E;
				}
			}
		}
		else if (ActiveNode) {
			ActiveNode->SetText(temp);
			for (Event* E : events)
			{
				EventNode* ActiveEventNode = dynamic_cast<EventNode*>(ActiveNode);
				if (ActiveEventNode->GetEvent() == E)
				{
					ActiveEvent = E;
				}
			}
		}
		UpdateNodeNames(temp);
	}
}

void EventsUI::UpdateNodeNames(const std::string& newName)
{
	for (Node* node : Nodes)
	{
		EventNode* ActiveEventNode = dynamic_cast<EventNode*>(node);
		if (ActiveEventNode->GetEvent() == ActiveEvent) {
			ActiveEventNode->SetText(newName);
		}
	}
}

void EventsUI::RenderEventText()
{
	EventNode* ActiveEventNode = dynamic_cast<EventNode*>(ActiveNode);
	std::string eventText = ActiveEvent ? ActiveEvent->GetEventText() : ActiveEventNode->GetEvent()->GetEventText();
	char temp0[256];
	strcpy_s(temp0, eventText.c_str());
	ImGui::Text("Event Text");
	if (ImGui::InputTextMultiline("##EventText", temp0, sizeof(temp0), ImVec2(ImGui::GetWindowSize().x - 15, 100))) {
		if (ActiveEvent) {
			ActiveEvent->SetEventText(temp0);
		}
		else {
			for (Event* event : events) 
			{
				EventNode* ENActive = dynamic_cast<EventNode*>(ActiveNode);
				if (event == ENActive->GetEvent()) {
					event->SetEventText(temp0);
				}
			}
		}
	}
}

void EventsUI::RenderInspector()
{
	if (!ImGui::BeginChild("Inspector", ImVec2(300, MonitorInfo::GetMode()->height - 70.5f), true, ImGuiWindowFlags_MenuBar))
		return;

	RenderMenuBar("Inspector");
	RenderComponents();
	RenderNodeInspector();

	ImGui::EndChild();
}

void EventsUI::DrawBackground(ImVec2 canvasSize, ImVec2 canvasPos)
{
	float GRID_STEP = 16.0f * zoomLevel;
	float MAJOR_GRID_STEP = GRID_STEP * 5.0f; // Major grid lines every 5 minor steps
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Draw background
	drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(40, 40, 40, 100));
	
	// Adjust thickness dynamically based on zoom level
	float majorLineThickness = 2.0f; // Adjust as needed
	float minorLineThickness = 1.0f; // Minor lines should remain the same

	// Draw major grid lines (black)
	for (float x = fmodf(viewportOffset.x, MAJOR_GRID_STEP); x < canvasSize.x; x += MAJOR_GRID_STEP)
	{
		drawList->AddLine(ImVec2(canvasPos.x + x, canvasPos.y), ImVec2(canvasPos.x + x, canvasPos.y + canvasSize.y), IM_COL32(200, 200, 200, 80), majorLineThickness);
	}
	for (float y = fmodf(viewportOffset.y, MAJOR_GRID_STEP); y < canvasSize.y; y += MAJOR_GRID_STEP)
	{
		drawList->AddLine(ImVec2(canvasPos.x, canvasPos.y + y), ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + y), IM_COL32(200, 200, 200, 80), majorLineThickness);
	}

	// Draw minor grid lines (lighter)
	for (float x = fmodf(viewportOffset.x, GRID_STEP); x < canvasSize.x; x += GRID_STEP)
	{
		if (fmodf(x, MAJOR_GRID_STEP) != 0.0f) // Skip major grid lines
		{
			drawList->AddLine(ImVec2(canvasPos.x + x, canvasPos.y), ImVec2(canvasPos.x + x, canvasPos.y + canvasSize.y), IM_COL32(200, 200, 200, 50), minorLineThickness);
		}
	}
	for (float y = fmodf(viewportOffset.y, GRID_STEP); y < canvasSize.y; y += GRID_STEP)
	{
		if (fmodf(y, MAJOR_GRID_STEP) != 0.0f) // Skip major grid lines
		{
			drawList->AddLine(ImVec2(canvasPos.x, canvasPos.y + y), ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + y), IM_COL32(200, 200, 200, 50), minorLineThickness);
		}
	}
}

void EventsUI::DrawContextMenu()
{
	ImGuiIO& io = ImGui::GetIO();

	if (ActiveNode == nullptr)
	{
		isContextualMenuOpen = ImGui::IsPopupOpen("Context Menu");

		if (ImGui::BeginPopup("Context Menu"))
		{
			ImGui::SetKeyboardFocusHere(); // Focus the next item in the tab order (the input field here)

			// Buffer for input text
			if (ImGui::InputTextWithHint("##input", "Search Events...", inputText, IM_ARRAYSIZE(inputText)))
			{
				searchQuery = inputText;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Create Chapter Start"))
			{
				//Create Event
				ImVec2 nodePosition = ImGui::GetMousePos();//ImVec2(contextMenuPos.x, contextMenuPos.y + 50); // Position the node below the context menu
				Event* newEvent = new Event("Chapter Start", "Test");
				EventNode* CreatedNode = new EventNode(nodePosition, ImVec2(100, 150), newEvent, NodeType::CHPTRSTART);
				//CreatedNode->SetIsChapterStarter(true);
				Nodes.push_back(CreatedNode);
				events.push_back(newEvent);
			}
			if (ImGui::MenuItem("Create Plot Point"))
			{
				//Create Event
				ImVec2 nodePosition = ImVec2(contextMenuPos.x, contextMenuPos.y + 50); // Position the node below the context menu
				Event* newEvent = new Event("Un-Named Plot Point", "Test");
				EventNode* CreatedNode = new EventNode(nodePosition, ImVec2(200, 150), newEvent, NodeType::PLOT);
				Nodes.push_back(CreatedNode);
				events.push_back(newEvent);
			}
			if (ImGui::MenuItem("Create Character"))
			{
			}
			if (ImGui::MenuItem("Create Loot Table"))
			{
			}
			if (ImGui::MenuItem("Create Item"))
			{
			}
			ImGui::Separator();
			for (Event* E : events)
			{
				if (!events.empty())
				{
					std::string eventName = E->GetEventName();

					if (!searchQuery.empty())
					{
						if (eventName.find(searchQuery) != std::string::npos)
						{
							if (ImGui::MenuItem(eventName.c_str()))
							{
								// Action for Option
							}
						}
					}
				}
			}
			ImGui::EndPopup();
		}

		if (!isContextualMenuOpen && ImGui::IsPopupOpen("Context Menu") == false)
		{
			//Clear the Search
			inputText[0] = '\0';
			searchQuery.clear();
		}
	}
	else if (ActiveNode != nullptr && ActiveNode->IsMouseHovering(io.MousePos, zoomLevel, viewportOffset))
	{
		if (ImGui::BeginPopup("Context Menu"))
		{
			if (ImGui::MenuItem("Delete Node"))
			{
				DeleteActiveNode();
			}

			ImGui::EndPopup();
		}
	}
}