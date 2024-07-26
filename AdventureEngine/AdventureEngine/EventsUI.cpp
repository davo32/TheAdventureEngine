#include "EventsUI.h"
#include "Application.h"
#include "MonitorInfo.h"
#include <iostream>


char EventsUI::inputText[128] = "";

void EventsUI::DrawUI()
{
	//Draw UI Here
	HierachyElement();
	EventSheetElement();
}


void EventsUI::TextCentered(std::string text)
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

void EventsUI::HierachyElement()
{
	if (ImGui::BeginChild("Inspector", ImVec2(300, MonitorInfo::GetMode()->height - 70.5f),
		true, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			TextCenteredInMenuBar("Inspector");
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginChild("Components", ImVec2(280, (MonitorInfo::GetMode()->height - 70.5f) - MonitorInfo::GetMode()->height / 2),
			true, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				TextCenteredInMenuBar("Components");
				ImGui::EndMenuBar();
			}

			// Root node with default open state
			if (ImGui::TreeNode("Root"))
			{
				for (size_t i = 0; i < events.size(); ++i)
				{
					Event* E = events[i];

					// Unique identifier for the selectable item
					std::string selectableID = E->GetEventName() + " : " + std::to_string(i); // Unique ID based on event name

					// Check if this event is selected
					bool wasSelected = (ActiveEvent == E);

					// Create the selectable item
					if (ImGui::Selectable(selectableID.c_str(), wasSelected)) {
						// If this item was selected, deselect it
						if (wasSelected) {
							ActiveEvent = nullptr;
							if (ActiveNode)
							{
								// Reset previous node state
								ActiveNode->isActive = false;
								ActiveNode = nullptr;
							}
						}
						else {
							// Deselect the previously selected item
							if (ActiveNode) {
								ActiveNode->isActive = false; // Reset previous node state
							}

							// Set the new active event and node
							ActiveEvent = E;
							ActiveNode = nullptr;

							for (EventNode* N : Nodes)
							{
								if (N->GetEvent() == ActiveEvent)
								{
									ActiveNode = N;
									ActiveNode->isActive = true;
									break;
								}
							}
						}
					}

					// Set the selection state for this item
					eventSelectionState[E] = (ActiveEvent == E);
				}

				ImGui::TreePop(); // Close the tree node
			}

			ImGui::EndChild();
		}
		if (ActiveNode != nullptr || ActiveEvent != nullptr)
		{
			if (ImGui::BeginChild("Node Inspector", ImVec2(280, (MonitorInfo::GetMode()->height - 50.5f) - MonitorInfo::GetMode()->height / 2),
				true, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					TextCenteredInMenuBar("Node");
					ImGui::EndMenuBar();
				}
				
				
				if (ActiveEvent != nullptr)
				{
					std::string eventName = ActiveEvent->GetEventName();
					char temp[256];
					strcpy_s(temp, eventName.c_str());
					ImGui::Text("Name: ");
					ImGui::SameLine();
					if (ImGui::InputText("##", temp, sizeof(temp)))
					{
						ActiveEvent->SetEventName(temp);

						for (EventNode* node : Nodes)
						{
							if (node->GetEvent() != nullptr && ActiveEvent == node->GetEvent())
							{
								node->SetText(temp);
							}
						}
					}
				}
				else if (ActiveNode != nullptr)
				{
					if (ActiveNode->GetEvent() != nullptr)
					{
						std::string eventName = ActiveNode->GetText();
						char temp[256];
						strcpy_s(temp, eventName.c_str());
						ImGui::Text("Name: ");
						ImGui::SameLine();
						if (ImGui::InputText("##", temp, sizeof(temp)))
						{
							ActiveNode->SetText(temp);

							for (Event* event : events)
							{
								if (event != nullptr && ActiveNode->GetEvent() == event)
								{
									event->SetEventName(temp);
								}
							}
						}
					}
				}
				
				ImGui::EndChild();
			}
		}
		ImGui::EndChild();
	}
}

void EventsUI::EventSheetElement()
{
	ImGui::SetWindowPos(ImVec2(350, -MonitorInfo::GetMode()->height + 80));
	if (ImGui::BeginChild
	("EditableEvents", ImVec2(MonitorInfo::GetMode()->width - 400, MonitorInfo::GetMode()->height - 80), true, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.2, 0.0));
			TextCenteredInMenuBar(EventWindowTitle.c_str());
			ImGui::EndMenuBar();
			ImGui::PopStyleColor();
		}

		if (ImGui::BeginChild("Event Window",
			ImVec2(MonitorInfo::GetMode()->width - 414.5, ImGui::GetContentRegionAvail().y), true, ImGuiChildFlags_Border))
		{
			
			if (currWindow == CurrentWindow::GRAPH)
			{
				DrawViewport();
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
	}
}

void EventsUI::DrawViewport()
{
	ImVec2 canvasPos = ImGui::GetCursorScreenPos();   // Top-left
	ImVec2 canvasSize = ImGui::GetContentRegionAvail(); // Size of the drawing area

	ImGuiIO& io = ImGui::GetIO();

	DrawBackground(canvasSize, canvasPos);
	std::string ZoomLevelText = " x " + std::to_string(zoomLevel);
	ImGui::Text(ZoomLevelText.c_str());

	// Handle panning
	if (ImGui::IsMouseHoveringRect(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y)))
	{
		if (ImGui::IsMouseClicked(2))
		{
			isPanning = true;
			lastMousePos = io.MousePos;
		}
		if (ImGui::IsMouseReleased(2))
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

		// Handle zoom
		float zoomDelta = io.MouseWheel * 0.1f; // Adjust the zoom speed here
		zoomLevel = std::max(0.1f, std::min(2.0f,zoomLevel + zoomDelta)); // Prevent zooming too far out/in
	
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImVec2 mousePos = ImGui::GetMousePos();
			ImVec2 offset(10.0f, 10.0f); // Offset from the mouse position
			contextMenuPos = mousePos; // Store the position for node creation
			ImGui::SetNextWindowPos(ImVec2(mousePos.x + offset.x, mousePos.y + offset.y));
			ImGui::OpenPopup("Context Menu");
		}
		DrawContextMenu();
	}


	// Handle node dragging
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		for (Node* node : Nodes)
		{
			if (node->GetIsDragging())
			{
				// Calculate the mouse position in world space
				ImVec2 mousePosInWorld = ImVec2((io.MousePos.x - viewportOffset.x) / zoomLevel, (io.MousePos.y - viewportOffset.y) / zoomLevel);

				// Calculate the delta movement in world space
				ImVec2 delta = ImVec2(mousePosInWorld.x - node->GetDragStartPos().x, mousePosInWorld.y - node->GetDragStartPos().y);

				// Update the node's position
				ImVec2 newPosition = ImVec2(node->GetPosition().x + delta.x, node->GetPosition().y + delta.y);
				node->SetPosition(newPosition);

				// Update the drag start position for the next frame
				node->SetDragStartPos(mousePosInWorld);

				// Debug output
				std::cout << " New Position: (" << newPosition.x << ", " << newPosition.y << ")" << std::endl;

				break; // Only handle one node per drag operation
			}
		}
	}

	// Update dragging state on mouse release
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		for (Node* node : Nodes) {
			if (node->IsMouseHovering(io.MousePos, zoomLevel, viewportOffset)) {
				if (!node->GetIsDragging()) {
					// If a node is being hovered and is not already being dragged, start dragging
					node->SetIsDragging(true);
					ImVec2 mousePosInWorld = ImVec2((io.MousePos.x - viewportOffset.x) / zoomLevel, (io.MousePos.y - viewportOffset.y) / zoomLevel);
					node->SetDragStartPos(mousePosInWorld);
				}
			}
			else if (node->GetIsDragging())
			{
				// If the node was dragging and is no longer hovered, stop dragging
				node->SetIsDragging(false);
			}
		}
	}

	// Draw nodes after handling interaction and context menu

	if (!Nodes.empty())
	{
		for (Node* node : Nodes) 
		{
			ImVec2 nodePosition = node->GetPosition();
			ImVec2 nodeSize = node->GetSize();

			// Apply zoom and viewport offset
			//ImVec2 scaledPosition = ImVec2(canvasPos.x + (nodePosition.x * zoomLevel) + viewportOffset.x, canvasPos.y + (nodePosition.y * zoomLevel) + viewportOffset.y);
			ImVec2 scaledPosition = ImVec2((nodePosition.x * zoomLevel) + viewportOffset.x, (nodePosition.y * zoomLevel) + viewportOffset.y);
			ImVec2 scaledSize = ImVec2(nodeSize.x * zoomLevel, nodeSize.y * zoomLevel);

			// Draw the node with adjusted position and size
			node->DrawNode(scaledPosition, scaledSize,zoomLevel);
		}

		// Pass canvas position to HandleNodeClicks
		HandleNodeClicks(io.MousePos, canvasPos);
	}
}

void EventsUI::HandleNodeClicks(ImVec2 mousePos,ImVec2 canvasPos)
{
	// Get the current position and size of the viewport
	ImVec2 viewportMin = ImGui::GetCursorScreenPos();
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	ImVec2 viewportMax = ImVec2(viewportMin.x + viewportSize.x, viewportMin.y + viewportSize.y);

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		bool nodeClicked = false;

		for (EventNode* node : Nodes) {
			ImVec2 nodePos = node->GetPosition();
			ImVec2 nodeSize = node->GetSize();

			// Apply zoom and viewport offset to the node's position and size
			ImVec2 scaledNodePos = ImVec2((nodePos.x * zoomLevel) + viewportOffset.x, (nodePos.y * zoomLevel) + viewportOffset.y);
			ImVec2 scaledNodeSize = ImVec2(nodeSize.x * zoomLevel, nodeSize.y * zoomLevel);

			// Check if the mouse is over the node
			if (mousePos.x >= scaledNodePos.x && mousePos.x <= scaledNodePos.x + scaledNodeSize.x &&
				mousePos.y >= scaledNodePos.y && mousePos.y <= scaledNodePos.y + scaledNodeSize.y) {
				ActiveNode = node;
				node->isActive = true;
				nodeClicked = true;
			}
		}

		// Deselect only if clicked within the viewport
		if (!nodeClicked && mousePos.x >= viewportMin.x && mousePos.x <= viewportMax.x &&
			mousePos.y >= viewportMin.y && mousePos.y <= viewportMax.y)
		{
			if (ActiveNode != nullptr)
			{
				ActiveNode->isActive = false;
				ActiveNode = nullptr;

				if (ActiveEvent != nullptr)
				{
					ActiveEvent = nullptr;
				}
			}
		}
	}
}

void EventsUI::DrawBackground(ImVec2 canvasSize,ImVec2 canvasPos)
{
	if (canvasSize.x < 50.0f) canvasSize.x = 50.0f;
	if (canvasSize.y < 50.0f) canvasSize.y = 50.0f;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Draw background
	drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(50, 50, 50, 255));

	// Draw a grid
	float gridSize = 32.0f * zoomLevel;
	for (float x = canvasPos.x; x < canvasPos.x + canvasSize.x; x += gridSize)
		drawList->AddLine(ImVec2(x, canvasPos.y), ImVec2(x, canvasPos.y + canvasSize.y), IM_COL32(200, 200, 200, 40));
	for (float y = canvasPos.y; y < canvasPos.y + canvasSize.y; y += gridSize)
		drawList->AddLine(ImVec2(canvasPos.x, y), ImVec2(canvasPos.x + canvasSize.x, y), IM_COL32(200, 200, 200, 40));

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
			if (ImGui::MenuItem("Create Event"))
			{
				//Create Event
				ImVec2 nodePosition = ImVec2(contextMenuPos.x, contextMenuPos.y + 50); // Position the node below the context menu
				Event* newEvent = new Event("Un-Named Event", "Test");
				EventNode* CreatedNode = new EventNode(nodePosition, ImVec2(200, 150), newEvent);
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
				Node* NodeToDelete;
				for (Node* node : Nodes)
				{
					if (node == ActiveNode)
					{
						NodeToDelete = node;
					}
				}

				auto it = std::remove(Nodes.begin(), Nodes.end(), NodeToDelete);
				if (it != Nodes.end())
				{
					// Optionally delete the node if you own it
					delete* it;

					// Remove the node from the vector
					Nodes.erase(it, Nodes.end());
					ActiveNode = nullptr;
				}
			}

			ImGui::EndPopup();
		}
	}
	
}

void EventsUI::DrawEventDetails(Event* event)
{
	if (event == nullptr)
	{
		return;
	}

	ImGui::Text("Event Details");

	ImGui::Text("Event Name: %s", event->GetEventName().c_str());
	ImGui::Text("Event Text: %s", event->GetEventText().c_str());

	// Display
	ImGui::Text("Branches: %d", event->GetBranchCount());

	// Display
	ImGui::Text("Reward Items: %d", event->GetRewardItemCount());
}