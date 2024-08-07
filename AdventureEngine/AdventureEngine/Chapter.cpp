#include "Chapter.h"
#include <iostream>
#include "EventsUI.h"

char Chapter::inputText[128] = "";

//////CONSTRUCTOR/////////////
Chapter::Chapter(std::string newName,ImVec2 mousePos) : ChapterName(newName)
{
	
}
/////END CONSTRUCTOR//////////

/////////////RENDERING/////////////////////
void Chapter::RenderViewport()
{
	ImVec2 ParentSize = ImGui::GetContentRegionAvail();
	ImVec2 ChildSize = ImVec2(ParentSize.x,ParentSize.y - 40);
	ImVec2 ParentPos = ImGui::GetWindowPos();
	ImVec2 ChildPos = ImVec2(ParentPos.x + 300, ParentPos.y + 40);

	ImGui::SetNextWindowPos(ChildPos);
	ImGui::SetCursorPos(ChildPos);
	if (ImGui::BeginChild("##Viewport", ChildSize, ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
	{
		screenPos = ImGui::GetCursorScreenPos(); // Top-left
		screenSize = ImGui::GetContentRegionAvail(); // Size of the drawing area

		ImGuiIO& io = ImGui::GetIO();

		RenderBackground(screenSize,ChildPos);

		std::string ZoomLevelText = " x " + std::to_string(zoomLevel);
		ImGui::Text(ZoomLevelText.c_str());

		// Handle panning
		if (ImGui::IsMouseHoveringRect(screenPos, ImVec2(screenPos.x + screenSize.x, screenPos.y + screenSize.y)))
		{
			ViewportPanning(io);

			// Handle zoom
			float zoomDelta = io.MouseWheel * 0.1f; // Adjust the zoom speed here
			zoomLevel = std::max(0.7f, std::min(2.0f, zoomLevel + zoomDelta)); // Prevent zooming too far out/in

			ContextMenuOpen();
			RenderContextMenu();
		}

		if (!hasRun) 
		{
			ImVec2 NodePos = ImVec2(ChildPos.x * 2, ChildPos.y * 2);
			NodeFamily.emplace_back(new BeginNode(NodePos, ImVec2(100, 70)));
			std::cout << "This will run only once." << std::endl;
			hasRun = true;
		}
		


		RenderNodes();
		NodeInteraction();
		NodeDrag(io.MousePos);
		ImGui::EndChild();
	}
}



std::string Chapter::GetChapterName()
{
	return ChapterName;
}

void Chapter::RenderBackground(ImVec2 canvasSize, ImVec2 canvasPos)
{
	float GRID_STEP = 16.0f * zoomLevel;
	float MAJOR_GRID_STEP = GRID_STEP * 8.0f; // Major grid lines every 5 minor steps
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

void Chapter::RenderContextMenu()
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
			if (ImGui::MenuItem("Create Plot Point"))
			{
				//Create Event
				ImVec2 nodePosition = ImVec2(contextMenuPos.x, contextMenuPos.y + 50); // Position the node below the context menu
				Event* newEvent = new Event("Un-Named Plot Point", "Test");
				EventNode* CreatedNode = new EventNode(nodePosition, ImVec2(200, 70), newEvent);
				NodeFamily.push_back(CreatedNode);
				events.push_back(newEvent);
			}
			if (ImGui::MenuItem("Create Empty Plot Point"))
			{
				//Create Event
				ImVec2 nodePosition = ImVec2(contextMenuPos.x, contextMenuPos.y + 50); // Position the node below the context menu
				Event* newEvent = new Event("Un-Named Plot Point", "");
				EventNode* CreatedNode = new EventNode(nodePosition, ImVec2(200, 70), newEvent);
				NodeFamily.push_back(CreatedNode);
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

void Chapter::RenderNodes()
{
	// Draw nodes after handling interaction and context menu
	if (!NodeFamily.empty())
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		for (Node* node : NodeFamily)
		{
			ImVec2 nodePosition = node->GetPosition();
			ImVec2 nodeSize = node->GetSize();

			// Apply zoom and viewport offset
			ImVec2 scaledPosition = ImVec2((nodePosition.x * zoomLevel) + viewportOffset.x, (nodePosition.y * zoomLevel) + viewportOffset.y);
			ImVec2 scaledSize = ImVec2(nodeSize.x * zoomLevel, nodeSize.y * zoomLevel);

			// Draw the node with adjusted position and size
			node->RenderConnections(drawList, zoomLevel, viewportOffset);
			node->DrawNode(scaledPosition, scaledSize, zoomLevel);
		}
	}
}

void Chapter::RenderNodeList()
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

void Chapter::RenderEventText()
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

/////////RENDERING END//////////////////////

////////UTILITIES///////////////////////////////////
void Chapter::ViewportPanning(ImGuiIO& io)
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

void Chapter::ContextMenuOpen()
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

void Chapter::NodeGraphShortcuts()
{
	//Delete Shortcut
	if (ImGui::IsKeyPressed(ImGuiKey_Delete))
	{
		DeleteActiveNode();
	}

	//Duplicate Node
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_D))
	{
		if (ActiveNode != nullptr)
		{
			// Ensure that ActiveNode is of type EventNode
			EventNode* eventNode = dynamic_cast<EventNode*>(ActiveNode);
			if (eventNode != nullptr)
			{
				Event* nodeEvent = eventNode->GetEvent();
				ImVec2 nodePos = ActiveNode->GetPosition();
				ImVec2 nodeSize = ActiveNode->GetSize();
				ImVec2 newNodePos = ImVec2(nodePos.x + nodeSize.x + 10, nodePos.y); // Adding an offset to avoid overlap

				// Create a new node and add it to the NodeFamily
				NodeFamily.emplace_back(new EventNode(newNodePos, nodeSize, nodeEvent));
			}
		}
	}
}

ImVec2 Chapter::ScreenToViewport()
{
	// Normalize screen position
	ImVec2 normalizedScreenPos = ImVec2(screenPos.x / screenSize.x, screenPos.y / screenSize.y);

	// Convert to viewport coordinates
	return ImVec2(normalizedScreenPos.x * screenSize.x, normalizedScreenPos.y * screenSize.y);

}

ImVec2 Chapter::ViewportToScreen(ImVec2 viewportPos)
{
	// Normalize viewport position
	ImVec2 normalizedViewportPos = ImVec2(viewportPos.x / screenSize.x, viewportPos.y / screenSize.y);

	// Convert to screen coordinates
	return ImVec2(normalizedViewportPos.x * screenSize.x, normalizedViewportPos.y * screenSize.y);

}

void Chapter::DeleteActiveNode()
{
	if (ActiveNode != nullptr)
	{
		EventNode* ActiveEventNode = dynamic_cast<EventNode*>(ActiveNode);
		// Get the associated event before deleting the node
		Event* associatedEvent = ActiveEventNode->GetEvent();

		if (associatedEvent != nullptr)
		{

			// Find and delete the node
			auto nodeIt = std::find(NodeFamily.begin(), NodeFamily.end(), ActiveNode);
			if (nodeIt != NodeFamily.end())
			{
				// Erase node from vector
				NodeFamily.erase(nodeIt);

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

void Chapter::NodeInteraction()
{
	ImGuiIO& io = ImGui::GetIO();

	// Handle node dragging and selection on mouse down
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		bool nodeClicked = false;
		ImVec2 mousePos = ImVec2((io.MousePos.x - viewportOffset.x) / zoomLevel, (io.MousePos.y - viewportOffset.y) / zoomLevel);
		for (Node* node : NodeFamily)
		{
			ImVec2 nodeSizeDivided = ImVec2(node->GetSize().x, (node->GetSize().y /*/ 6*/) + 5.0f);
			ImVec2 nodePos = node->GetPosition();
			ImVec2 nodeSize = nodeSizeDivided;//node->GetSize();

			// Apply zoom and viewport offset to the node's position and size
			ImVec2 scaledNodePos = ImVec2((nodePos.x * zoomLevel) + viewportOffset.x, (nodePos.y * zoomLevel) + viewportOffset.y);
			ImVec2 scaledNodeSize = ImVec2(nodeSize.x * zoomLevel, nodeSize.y * zoomLevel);

			//std::cout << "Node Pos X: " << scaledNodePos.x << " Node Pos Y: " << scaledNodePos.y << '\n';


			if (!dragStartNode && dragStartNode == nullptr)
			{
				int outputIndex = node->GetHoveredOutputPointIndex(io.MousePos);
				if (outputIndex != -1)
				{
					dragStartNode = node;
					dragStartOutputIndex = outputIndex;
					dragStartPos = node->GetOutputPoint(outputIndex);
					node->StartConnecting(outputIndex, dragStartPos);
					return;
				}
			}

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
				EventsUI::Itype = InspectorType::NODE;
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

void Chapter::NodeDrag(ImVec2 mousePos)
{
	ImGuiIO& io = ImGui::GetIO();

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

		if (dragStartNode)
		{
			ImVec2 mousePosInWorld = ImVec2((mousePos.x - viewportOffset.x) / zoomLevel, (mousePos.y - viewportOffset.y) / zoomLevel);
			
			dragStartNode->UpdateConnection(io.MousePos);

			for (Node* node : NodeFamily) 
			{
				if (node != dragStartNode)
				{
					int inputIndex = node->GetHoveredInputPointIndex(io.MousePos);
					if (inputIndex != -1) 
					{
						node->UpdateConnection(io.MousePos);
						return;
					}
				}
			}
		}
	}

	// Update dragging state on mouse release
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		if (ActiveNode)
		{
			ActiveNode->SetIsDragging(false);
		}

		if (dragStartNode) 
		{
			ImVec2 mousePosInWorld = ImVec2((mousePos.x - viewportOffset.x) / zoomLevel, (mousePos.y - viewportOffset.y) / zoomLevel);

			for (Node* node : NodeFamily) 
			{
				if (node != dragStartNode)
				{
					int inputIndex = node->GetHoveredInputPointIndex(io.MousePos);
					if (inputIndex != -1)
					{
						dragStartNode->ConnectTo(node, dragStartOutputIndex, inputIndex);

						for (Node* Dragnode : NodeFamily)
						{
							if (Dragnode == dragStartNode && Dragnode->GetConnectionCount() > 1)
							{
								Dragnode->RemoveConnection(0);
								break;
							}
						}
						
					}
				}
			}

			dragStartNode = nullptr;
			dragStartOutputIndex = -1;
		}
	}
}

void Chapter::ToggleSelection(Event* E, bool wasSelected)
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

void Chapter::DeselectCurrent()
{
	ActiveEvent = nullptr;
	if (ActiveNode) {
		ActiveNode->SetIsActive(false);
		ActiveNode = nullptr;
	}
}

void Chapter::SelectEvent(Event* E)
{
	ActiveEvent = E;
	ActiveNode = nullptr;
	for (Node* N : NodeFamily)
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

void Chapter::EventRenamer()
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
			for (Node* E : NodeFamily)
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

void Chapter::UpdateNodeNames(std::string newName)
{
	for (Node* node : NodeFamily)
	{
		EventNode* ActiveEventNode = dynamic_cast<EventNode*>(node);
		if (ActiveEventNode->GetEvent() == ActiveEvent) {
			ActiveEventNode->SetText(newName);
		}
	}
}
/////////END UTILITIES//////////////////////////////