#include "ChapterEditor.h"
#include <iostream>
#include "Core/Nodes/CustomNodes/BeginNode.h"
#include "Core/Nodes/CustomNodes/EventNode.h"

char ChapterEditor::inputText[128] = "";

void ChapterEditor::RenderViewport(Chapter* chapter)
{
	ImVec2 ParentSize = ImGui::GetContentRegionAvail();
	ImVec2 ChildSize = ImVec2(ParentSize.x - 610, ParentSize.y - 300);
	ImVec2 ParentPos = ImGui::GetWindowPos();
	ImVec2 ChildPos = ImVec2(ParentPos.x + 310, ParentPos.y + 80);
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ChildPos);
	ImGui::SetCursorPos(ChildPos);
	if (ImGui::BeginChild("##Viewport", ChildSize, ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
	{
		RenderBackground(ChildSize, ChildPos, chapter);

		if (chapter != nullptr)
		{
			chapter->screenPos = ImGui::GetCursorScreenPos(); // Top-left
			chapter->screenSize = ImGui::GetContentRegionAvail(); // Size of the drawing area

			// Handle panning
			if (ImGui::IsMouseHoveringRect(chapter->screenPos,
				ImVec2(chapter->screenPos.x + chapter->screenSize.x,
					chapter->screenPos.y + chapter->screenSize.y)))
			{
				HandlePanning(io, chapter);
				HandleZooming(io, chapter);

				ContextMenuOpen();
				RenderContextMenu(chapter);
			}

			RenderNodes(chapter);
			NodeInteraction(chapter);
			NodeDrag(io.MousePos, chapter);
		}
		ImGui::EndChild();
	}
}

void ChapterEditor::NodeGraphShortcuts(Chapter* chapter)
{
	//Delete Shortcut
	if (ImGui::IsKeyPressed(ImGuiKey_Delete))
	{
		chapter->DeleteActiveNode();
	}

	//Duplicate Node
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_D))
	{
		if (chapter->ActiveNode != nullptr)
		{
			// Ensure that ActiveNode is of type EventNode
			EventNode* eventNode = dynamic_cast<EventNode*>(chapter->ActiveNode);
			if (eventNode != nullptr)
			{
				Event* nodeEvent = eventNode->GetEvent();
				ImVec2 nodePos = chapter->ActiveNode->GetPosition();
				ImVec2 nodeSize = chapter->ActiveNode->GetSize();
				ImVec2 newNodePos = ImVec2(nodePos.x + nodeSize.x + 10, nodePos.y); // Adding an offset to avoid overlap

				// Create a new node and add it to the NodeFamily
				chapter->NodeFamily.emplace_back(new EventNode(newNodePos, nodeSize, nodeEvent));
			}
		}
	}
}

void ChapterEditor::HandlePanning(ImGuiIO& io, Chapter* chapter)
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
	{
		isPanning = true;
		chapter->lastMousePos = io.MousePos;
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
	{
		isPanning = false;
	}

	if (isPanning)
	{
		ImVec2 delta = ImVec2(io.MousePos.x - chapter->lastMousePos.x, io.MousePos.y - chapter->lastMousePos.y);
		chapter->viewportOffset.x += delta.x;
		chapter->viewportOffset.y += delta.y;
		chapter->lastMousePos = io.MousePos;
	}
}

void ChapterEditor::EventRenamer(Chapter* chapter)
{
	if (!chapter->ActiveEvent && !chapter->ActiveNode) return;

	// Get the current event name
	std::string eventName = chapter->ActiveEvent ? chapter->ActiveEvent->GetEventName() : chapter->ActiveNode->GetText();
	char temp[256];
	strcpy_s(temp, sizeof(temp), eventName.c_str());

	ImGui::Text("Name: ");
	ImGui::SameLine();
	if (ImGui::InputText("##Name", temp, sizeof(temp))) {
		// Update the event or node name
		if (chapter->ActiveEvent) {
			chapter->ActiveEvent->SetEventName(temp);
			for (Node* E : chapter->NodeFamily)
			{
				EventNode* EN = dynamic_cast<EventNode*>(E);
				if (chapter->ActiveEvent == EN->GetEvent())
				{
					chapter->ActiveNode = E;
				}
			}
		}
		else if (chapter->ActiveNode) {
			chapter->ActiveNode->SetText(temp);
			for (Event* E : chapter->events)
			{
				EventNode* ActiveEventNode = dynamic_cast<EventNode*>(chapter->ActiveNode);
				if (ActiveEventNode->GetEvent() == E)
				{
					chapter->ActiveEvent = E;
				}
			}
		}
		UpdateNodeNames(temp, chapter);
	}
}

void ChapterEditor::UpdateNodeNames(std::string newName, Chapter* chapter)
{
	for (Node* node : chapter->NodeFamily)
	{
		EventNode* ActiveEventNode = dynamic_cast<EventNode*>(node);
		if (ActiveEventNode->GetEvent() == chapter->ActiveEvent) {
			ActiveEventNode->SetText(newName);
		}
	}
}

void ChapterEditor::HandleZooming(const ImGuiIO& io, Chapter* chapter)
{
	// Handle zoom
	float zoomDelta = io.MouseWheel * 0.1f; // Adjust the zoom speed here
	chapter->zoomLevel = std::max(0.7f, std::min(2.0f, chapter->zoomLevel + zoomDelta)); // Prevent zooming too far out/in

	std::string ZoomLevelText = " x " + std::to_string(chapter->zoomLevel);
	ImGui::Text(ZoomLevelText.c_str());
}

void ChapterEditor::RenderBackground(const ImVec2& childSize, const ImVec2& childPos, Chapter* chapter)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Draw background
	drawList->AddRectFilled(childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), IM_COL32(40, 40, 40, 100), 10.0f);

	if (chapter != nullptr)
	{
		float GRID_STEP = 16.0f * chapter->zoomLevel;
		float MAJOR_GRID_STEP = GRID_STEP * 8.0f; // Major grid lines every 5 minor steps

		// Adjust thickness dynamically based on zoom level
		float majorLineThickness = 2.0f; // Adjust as needed
		float minorLineThickness = 1.0f; // Minor lines should remain the same

		// Draw major grid lines (black)
		for (float x = fmodf(chapter->viewportOffset.x, MAJOR_GRID_STEP); x < childSize.x; x += MAJOR_GRID_STEP)
		{
			drawList->AddLine(ImVec2(childPos.x + x, childPos.y), ImVec2(childPos.x + x, childPos.y + childSize.y), IM_COL32(200, 200, 200, 80), majorLineThickness);
		}
		for (float y = fmodf(chapter->viewportOffset.y, MAJOR_GRID_STEP); y < childSize.y; y += MAJOR_GRID_STEP)
		{
			drawList->AddLine(ImVec2(childPos.x, childPos.y + y), ImVec2(childPos.x + childSize.x, childPos.y + y), IM_COL32(200, 200, 200, 80), majorLineThickness);
		}

		// Draw minor grid lines (lighter)
		for (float x = fmodf(chapter->viewportOffset.x, GRID_STEP); x < childSize.x; x += GRID_STEP)
		{
			if (fmodf(x, MAJOR_GRID_STEP) != 0.0f) // Skip major grid lines
			{
				drawList->AddLine(ImVec2(childPos.x + x, childPos.y), ImVec2(childPos.x + x, childPos.y + childSize.y), IM_COL32(200, 200, 200, 50), minorLineThickness);
			}
		}
		for (float y = fmodf(chapter->viewportOffset.y, GRID_STEP); y < childSize.y; y += GRID_STEP)
		{
			if (fmodf(y, MAJOR_GRID_STEP) != 0.0f) // Skip major grid lines
			{
				drawList->AddLine(ImVec2(childPos.x, childPos.y + y), ImVec2(childPos.x + childSize.x, childPos.y + y), IM_COL32(200, 200, 200, 50), minorLineThickness);
			}
		}
	}
	drawList->AddRect(childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), IM_COL32(0, 0, 0, 200), 10.0f, 0, 8.0f);
}

void ChapterEditor::RenderNodes(Chapter* chapter)
{
	// Draw nodes after handling interaction and context menu
	if (!chapter->NodeFamily.empty())
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		for (Node* node : chapter->NodeFamily)
		{
			ImVec2 nodePosition = node->GetPosition();
			ImVec2 nodeSize = node->GetSize();

			// Apply zoom and viewport offset
			ImVec2 scaledPosition = ImVec2((nodePosition.x * chapter->zoomLevel) + chapter->viewportOffset.x, (nodePosition.y * chapter->zoomLevel) + chapter->viewportOffset.y);
			ImVec2 scaledSize = ImVec2(nodeSize.x * chapter->zoomLevel, nodeSize.y * chapter->zoomLevel);

			// Draw the node with adjusted position and size
			node->RenderConnections(drawList, chapter->zoomLevel, chapter->viewportOffset);
			node->DrawNode(scaledPosition, scaledSize, chapter->zoomLevel);
		}
	}
}

void ChapterEditor::NodeInteraction(Chapter* chapter)
{
	ImGuiIO& io = ImGui::GetIO();

	// Handle node dragging and selection on mouse down
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		nodeClicked = false;

		ImVec2 mousePos = ImVec2((io.MousePos.x - chapter->viewportOffset.x) / chapter->zoomLevel, (io.MousePos.y - chapter->viewportOffset.y) / chapter->zoomLevel);
		for (Node* node : chapter->NodeFamily)
		{
			ImVec2 nodeSizeDivided = ImVec2(node->GetSize().x, (node->GetSize().y /*/ 6*/) + 5.0f);
			ImVec2 nodePos = node->GetPosition();
			ImVec2 nodeSize = nodeSizeDivided;//node->GetSize();

			// Apply zoom and viewport offset to the node's position and size
			ImVec2 scaledNodePos = ImVec2((nodePos.x * chapter->zoomLevel) + chapter->viewportOffset.x, (nodePos.y * chapter->zoomLevel) + chapter->viewportOffset.y);
			ImVec2 scaledNodeSize = ImVec2(nodeSize.x * chapter->zoomLevel, nodeSize.y * chapter->zoomLevel);

			//std::cout << "Node Pos X: " << scaledNodePos.x << " Node Pos Y: " << scaledNodePos.y << '\n';

			if (!dragStartNode && dragStartNode == nullptr)
			{
				int outputIndex = node->GetHoveredOutputPointIndex(io.MousePos);
				if (outputIndex != -1)
				{
					dragStartNode = node;
					dragStartOutputIndex = outputIndex;
					dragStartPos = node->GetOutputPoint(outputIndex).position;
					node->StartConnecting(outputIndex, dragStartPos);
					return;
				}
			}

			// Check if the mouse is over the node
			if (io.MousePos.x >= scaledNodePos.x && io.MousePos.x <= scaledNodePos.x + scaledNodeSize.x &&
				io.MousePos.y >= scaledNodePos.y && io.MousePos.y <= scaledNodePos.y + scaledNodeSize.y)
			{
				if (chapter->ActiveNode != nullptr)
				{
					chapter->ActiveNode->SetIsActive(false);
				}
				chapter->ActiveNode = node;
				node->SetIsActive(true);
				node->SetIsDragging(true);
				ImVec2 mousePosInWorld = ImVec2((io.MousePos.x - chapter->viewportOffset.x) / chapter->zoomLevel, (io.MousePos.y - chapter->viewportOffset.y) / chapter->zoomLevel);
				node->SetDragStartPos(mousePosInWorld);
				nodeClicked = true;
				//EventsUI::Itype = InspectorType::NODE;
				break;
			}
		}

		// Deselect if clicked on empty space within the viewport
		if (!nodeClicked)
		{
			// Get mouse position
			ImVec2 mousePos = io.MousePos;

			// Check if mouse cursor is within viewport bounds
			if (mousePos.x >= chapter->screenPos.x && mousePos.x <= (chapter->screenPos.x + chapter->screenSize.x) &&
				mousePos.y >= chapter->screenPos.y && mousePos.y <= (chapter->screenPos.y + chapter->screenSize.y))
			{
				if (chapter->ActiveNode != nullptr)
				{
					chapter->ActiveNode->SetIsActive(false);
					chapter->ActiveNode = nullptr;
				}
			}
		}
	}
}

void ChapterEditor::NodeDrag(const ImVec2& mousePos, Chapter* chapter)
{
	ImGuiIO& io = ImGui::GetIO();

	// Handle node dragging
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		if (chapter->ActiveNode && chapter->ActiveNode->IsDragging())
		{
			// Calculate the mouse position in world space
			ImVec2 mousePosInWorld = ImVec2((mousePos.x - chapter->viewportOffset.x) / chapter->zoomLevel, (mousePos.y - chapter->viewportOffset.y) / chapter->zoomLevel);

			// Calculate the delta movement in world space
			ImVec2 delta = ImVec2(mousePosInWorld.x - chapter->ActiveNode->GetDragStartPos().x, mousePosInWorld.y - chapter->ActiveNode->GetDragStartPos().y);

			// Update the node's position
			ImVec2 newPosition = ImVec2(chapter->ActiveNode->GetPosition().x + delta.x, chapter->ActiveNode->GetPosition().y + delta.y);
			chapter->ActiveNode->SetPosition(newPosition);

			// Update the drag start position for the next frame
			chapter->ActiveNode->SetDragStartPos(mousePosInWorld);

			// Debug output
			std::cout << " New Position: (" << newPosition.x << ", " << newPosition.y << ")" << '\n';
		}

		if (dragStartNode)
		{
			ImVec2 mousePosInWorld = ImVec2((mousePos.x - chapter->viewportOffset.x) / chapter->zoomLevel, (mousePos.y - chapter->viewportOffset.y) / chapter->zoomLevel);

			dragStartNode->UpdateConnection(io.MousePos);

			for (Node* node : chapter->NodeFamily)
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
		if (chapter->ActiveNode)
		{
			chapter->ActiveNode->SetIsDragging(false);
		}

		if (dragStartNode)
		{
			ImVec2 mousePosInWorld = ImVec2((mousePos.x - chapter->viewportOffset.x) / chapter->zoomLevel, (mousePos.y - chapter->viewportOffset.y) / chapter->zoomLevel);

			for (Node* node : chapter->NodeFamily)
			{
				if (node != dragStartNode)
				{
					int inputIndex = node->GetHoveredInputPointIndex(io.MousePos);
					if (inputIndex != -1)
					{
						dragStartNode->ConnectTo(node, dragStartOutputIndex, inputIndex);

						for (Node* Dragnode : chapter->NodeFamily)
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

void ChapterEditor::ContextMenuOpen()
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

void ChapterEditor::RenderContextMenu(Chapter* chapter)
{
	ImGuiIO& io = ImGui::GetIO();

	if (chapter->ActiveNode == nullptr)
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
				chapter->NodeFamily.push_back(CreatedNode);
				chapter->events.push_back(newEvent);
			}
			if (ImGui::MenuItem("Create Empty Plot Point"))
			{
				//Create Event
				ImVec2 nodePosition = ImVec2(contextMenuPos.x, contextMenuPos.y + 50); // Position the node below the context menu
				Event* newEvent = new Event("Un-Named Plot Point", "");
				EventNode* CreatedNode = new EventNode(nodePosition, ImVec2(200, 70), newEvent);
				chapter->NodeFamily.push_back(CreatedNode);
				chapter->events.push_back(newEvent);
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
			for (Event* E : chapter->events)
			{
				if (!chapter->events.empty())
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
	else if (chapter->ActiveNode != nullptr && chapter->ActiveNode->IsMouseHovering(io.MousePos, chapter->zoomLevel, chapter->viewportOffset))
	{
		if (ImGui::BeginPopup("Context Menu"))
		{
			if (ImGui::MenuItem("Delete Node"))
			{
				chapter->DeleteActiveNode();
			}

			ImGui::EndPopup();
		}
	}
}