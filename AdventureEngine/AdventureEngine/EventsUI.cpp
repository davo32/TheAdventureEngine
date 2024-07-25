#include "EventsUI.h"
#include "Application.h"
#include "MonitorInfo.h"
#include <iostream>

char EventsUI::inputText[128] = "";

void EventsUI::Test()
{
	testcounter = Buttons.size() + 1;
	//TESTING
	Event* testEvent = new Event("Unknown Event ("+ std::to_string(testcounter) + ")", "You Test This Program..");
	Event* One = new Event("1", "1");
	Event* Two = new Event("2", "2");
	testEvent->AddBranchingEvents(
		{
			std::make_pair("One", One),
			std::make_pair("Two", Two)
		});
	Widget* newWidget = new Widget(testEvent);
	Buttons.push_back(newWidget);
}

Widget* EventsUI::GetWidgetByRef(Widget* ref)
{
	auto it = std::find(Buttons.begin(), Buttons.end(), ref);
	if (it != Buttons.end())
	{
		return *it;
	}
	return nullptr; // Return nullptr if widget is not found
}

void EventsUI::DrawUI()
{
	//Draw UI Here
	HierachyElement();
	EventSheetElement();
}

void EventsUI::TextCenteredInMenuBar(std::string text)
{
	// Calculate the size of the text
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

	// Get the window width
	float windowWidth = ImGui::GetWindowWidth();

	// Calculate the position to center the text
	float textPosX = (windowWidth - textSize.x) * 0.5f;

	// Align text to center
	ImGui::SetCursorPosX(textPosX);
	ImGui::Text(text.c_str());
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
	if (ImGui::BeginChild("Events", ImVec2(300, MonitorInfo::GetMode()->height - 70.5f),
		true, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			TextCenteredInMenuBar("Events");
			ImGui::EndMenuBar();
		}
		if (ImGui::BeginChild("Events", ImVec2(285, ImGui::GetContentRegionAvail().y), true, ImGuiChildFlags_Border))
		{
			/////////////////////////////////////////////////////////////////////
			for (Widget* widget : Buttons)
			{
				widget->DrawWidget();

				// Check if the mouse is hovering over the button
				if (ImGui::IsItemHovered())
				{
					// Check for left mouse button click
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						if (ActiveWidget != widget)
						{
							if (ActiveWidget != nullptr)
							{
								//Remove the old Active Widget
								ActiveWidget->SetIsActive(false);
								ActiveWidget->isClicked = false;
							}

							//Setup the new Active Widget
							ActiveWidget = widget;
							ActiveWidget->SetIsActive(true);
							ActiveWidget->isClicked = true;
						}
						else
						{
							// Toggle off if clicking the active widget again
							ActiveWidget->isClicked = !ActiveWidget->isClicked;
							if (!ActiveWidget->isClicked)
							{
								ActiveWidget->SetIsActive(false);
								ActiveWidget = nullptr;
							}
						}
					}

					// Check for right mouse button click
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
					{
						// Remove the widget from the Buttons vector
						auto it = std::find(Buttons.begin(), Buttons.end(), widget);
						if (it != Buttons.end())
						{
							if (ActiveWidget == widget)
							{
								ActiveWidget = nullptr;
							}
							Buttons.erase(it); // Erase the widget
							delete widget;
						}
					}
				}
			}

			if (ImGui::Button("Create Event", ImVec2(270, 50)))
			{
				Test();
			}
			ImGui::EndChild();
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
			if (ImGui::Button("Event Graph"))
			{
				EventWindowTitle = "Event Graph";
				currWindow = CurrentWindow::GRAPH;
			}
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
	std::string ZoomLevelText = "x" + std::to_string(zoomLevel);
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
			else if (node->GetIsDragging()) {
				// If the node was dragging and is no longer hovered, stop dragging
				node->SetIsDragging(false);
			}
		}
	}

	// Draw nodes after handling interaction and context menu
	if (!Nodes.empty())
	{
		for (Node* node : Nodes) {
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
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		bool nodeClicked = false;
		for (Node* node : Nodes) {
			ImVec2 nodePos = node->GetPosition();
			ImVec2 nodeSize = node->GetSize();

			// Apply zoom and viewport offset to the node's position and size
			ImVec2 scaledNodePos = ImVec2((nodePos.x * zoomLevel) + viewportOffset.x, (nodePos.y * zoomLevel) + viewportOffset.y);
			ImVec2 scaledNodeSize = ImVec2(nodeSize.x * zoomLevel, nodeSize.y * zoomLevel);

			// Debug output
			std::cout << "Node Pos: (" << scaledNodePos.x << ", " << scaledNodePos.y << ") Size: (" << scaledNodeSize.x << ", " << scaledNodeSize.y << ")" << std::endl;
			std::cout << "Mouse Pos: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;

			// Check if the mouse is over the node
			if (mousePos.x >= scaledNodePos.x && mousePos.x <= scaledNodePos.x + scaledNodeSize.x &&
				mousePos.y >= scaledNodePos.y && mousePos.y <= scaledNodePos.y + scaledNodeSize.y) {
				ActiveNode = node;
				node->isActive = true;
				nodeClicked = true;
			}
			else {
				node->isActive = false;
			}
		}

		if (!nodeClicked) {
			ActiveNode = nullptr;
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
				Test();
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
			for (Widget* W : Buttons)
			{
				std::string eventName = W->GetStoredEvent()->GetEventName();

				if (!searchQuery.empty())
				{
					if (eventName.find(searchQuery) != std::string::npos)
					{
						if (ImGui::MenuItem(eventName.c_str()))
						{
							// Action for Option
							ImVec2 nodePosition = ImVec2(contextMenuPos.x, contextMenuPos.y + 50); // Position the node below the context menu
							Node* CreatedNode = new Node(nodePosition, ImVec2(200, 150), eventName);
							Nodes.push_back(CreatedNode);
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