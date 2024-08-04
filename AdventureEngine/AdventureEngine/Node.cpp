#include "Node.h"
#include "imgui.h"
#include "Application.h"
#include <iostream>

Node::Node(ImVec2 pos, ImVec2 size, std::string name)
	: position(pos), size(size), text(std::move(name)) {}

void Node::ConnectTo(Node* targetNode, int outputIndex, int inputIndex)
{
	// Ensure the target node and indices are valid
	/*if (!targetNode || outputIndex < 0 || inputIndex < 0) {
		return;
	}*/

	// Add the connection to the current node's output connections
	Connection newConnection;
	newConnection.targetNode = targetNode;
	newConnection.outputIndex = outputIndex;
	newConnection.inputIndex = inputIndex;

	connections.push_back(newConnection);
	std::cout << "Connection Made!" << '\n';
}

void Node::RenderConnections(ImDrawList* drawList, float zoomLevel, ImVec2 viewportOffset) const
{
	for (const Connection& conn : connections) 
	{
		// Get the position of the output and input points
		ImVec2 outputPointPos = GetOutputPoint(conn.outputIndex);
		ImVec2 inputPointPos = conn.targetNode->GetInputPoint(conn.inputIndex);

		//// Convert the points to screen coordinates
		//outputPointPos = ImVec2((outputPointPos.x * zoomLevel) + viewportOffset.x, (outputPointPos.y * zoomLevel) + viewportOffset.y);
		//inputPointPos = ImVec2((inputPointPos.x * zoomLevel) + viewportOffset.x, (inputPointPos.y * zoomLevel) + viewportOffset.y);

		// Draw the connection line
		drawList->AddLine(outputPointPos, inputPointPos, ImColor(255, 255, 255), 2.0f);
	}
}

void Node::SetDragStartPos(ImVec2 pos) 
{
	dragStartPos = pos;
}

ImVec2 Node::GetDragStartPos() const 
{
	return dragStartPos;
}

void Node::SetInitialPosition(ImVec2 pos) 
{
	initialPosition = pos;
}

ImVec2 Node::GetInitialPosition() const 
{
	return initialPosition;
}

bool Node::IsMouseHovering(ImVec2 mousePos, float zoomLevel, ImVec2 viewportOffset) const
{
	// Calculate the node's position and size relative to the viewport and zoom level
	ImVec2 nodeMin = ImVec2((position.x - viewportOffset.x) * zoomLevel, (position.y - viewportOffset.y) * zoomLevel);
	ImVec2 nodeMax = ImVec2(nodeMin.x + size.x * zoomLevel, nodeMin.y + size.y * zoomLevel);

	// Check if the mouse position is within the node's bounding box
	return mousePos.x >= nodeMin.x && mousePos.x <= nodeMax.x &&
		   mousePos.y >= nodeMin.y && mousePos.y <= nodeMax.y;
}

void Node::DrawNode(const ImVec2& position, const ImVec2& size, float zoomLevel) {
	
	ImGui::BeginGroup();
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	const float headerRadius = 6.0f;
	const float headerHeight = 30.0f;
	const float padding = 5.0f;
	float pinOffsetY = 0.0f;
	
	CreateNodeVisuals(position, size, drawList, headerRadius, headerHeight);
	CreateInputsAndOutputs(position, padding, headerHeight, size, drawList, headerRadius, 2.0f, pinOffsetY);
	DrawComponents(position, size, zoomLevel);
	
	ImGui::EndGroup();
}

void Node::StartConnecting(int outputIndex, ImVec2 startPos)
{
	isConnecting = true;
	connectionStartPos = startPos;
	//dragStartPos = startPos;
	dragStartOutputIndex = outputIndex;
}

void Node::UpdateConnection(ImVec2 endPos)
{
	if (isConnecting)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddLine(connectionStartPos, endPos, ImColor(255, 255, 255), 2.0f);
	}
}

void Node::EndConnection(Node* targetNode, int inputIndex)
{
	if (targetNode)
	{
		ConnectTo(targetNode, dragStartOutputIndex, inputIndex);
	}
	isConnecting = false;
}

void Node::CreateInputsAndOutputs(const ImVec2& drawPosition, float padding, float headerHeight, const ImVec2& drawSize, ImDrawList* drawList, float headerRadius, float borderThickness, float& pinOffsetY)
{
	const float circleRadius = 8.0f; // Radius of the circle
	const float rectOffsetY = headerHeight / 2;
	ImGuiIO& io = ImGui::GetIO();

	for (int i = 0; i < inputPoints.size(); i++)
	{
		ImVec2 center(drawPosition.x + padding + 20.0f, drawPosition.y + rectOffsetY + 35.0f);
		inputPoints[i] = center;

		bool isHovered = GetHoveredInputPointIndex(io.MousePos) == i;
		// Draw outer circle
		drawList->AddCircle(center, circleRadius, isHovered ? ImColor(255, 0, 0) : ImColor(255, 255, 255, 128), 16, borderThickness);

		// Draw inner circle
		drawList->AddCircleFilled(center, circleRadius - borderThickness, ImColor(0, 0, 0, 128), 16);

		pinOffsetY += circleRadius * 2.0f + padding;
		drawList->AddText(ImVec2(center.x + circleRadius + 5.0f, center.y - 8.0f), ImColor(255, 255, 255), "Input");
	}
	
	for (int i = 0; i < outputPoints.size(); i++)
	{
		ImVec2 center(drawPosition.x + drawSize.x - padding - 20.0f, drawPosition.y + rectOffsetY + 35.0f);
		outputPoints[i] = center;
		// Check if the mouse is hovering over this output point
		bool isHovered = GetHoveredOutputPointIndex(io.MousePos) == i;
		
		// Draw outer circle
		drawList->AddCircle(outputPoints[i], circleRadius, isHovered ? ImColor(255, 0, 0) : ImColor(255, 255, 255, 128), 16, borderThickness);
		
		// Draw inner circle
		drawList->AddCircleFilled(outputPoints[i], circleRadius - borderThickness, ImColor(0, 0, 0, 128), 16);

		pinOffsetY += circleRadius * 2.0f + padding;
		drawList->AddText(ImVec2(center.x - circleRadius - 45.0f, center.y - 8.0f), ImColor(255, 255, 255, 255), "Output");
	}
}

void Node::CreateNodeVisuals(const ImVec2& drawPosition, const ImVec2& drawSize, ImDrawList* drawList, float headerRadius, float headerHeight)
{
	// Background
	ImVec2 shadowOffset(5, 5);
	ImColor shadowColor(0, 0, 0, 100);

	// Draw shadow
	ImVec2 shadowPos = ImVec2(drawPosition.x + shadowOffset.x, drawPosition.y + shadowOffset.y);
	ImVec2 shadowEnd = ImVec2(drawPosition.x + drawSize.x + shadowOffset.x, drawPosition.y + drawSize.y + shadowOffset.y);
	drawList->AddRectFilled(shadowPos, shadowEnd, shadowColor, headerRadius);

	// Node background
	ImColor nodeColor(0, 0, 0, 128);
	drawList->AddRectFilled(drawPosition,
		ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
		nodeColor, headerRadius);

	// Calculate header position and size
	ImVec2 headerPos(drawPosition.x, drawPosition.y);
	ImVec2 headerSize(drawSize.x, headerHeight);

	// Draw the gradient background
	drawList->AddRectFilledMultiColor(
		headerPos,
		ImVec2(headerPos.x + headerSize.x, headerPos.y + headerSize.y),
		colorTop, colorTop, colorBottom, colorBottom
	);


	// Draw text on header
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
	ImVec2 textPos(
		drawPosition.x + (drawSize.x - textSize.x) * 0.5f,
		drawPosition.y + (headerHeight - textSize.y) * 0.5f
	);
	Application::fontLoader.SetFont("Bold");
	Application::fontLoader.DrawText(drawList, textPos, ImColor(255, 255, 255), text);

	if (isEmpty)
	{
		if (sizeAdjusted == false)
		{
			SetSize(ImVec2(size.x, size.y * 1.2f));
			sizeAdjusted = true;
		}

		// Draw the red rectangle at the bottom
		ImVec2 redRectPos(drawPosition.x, drawPosition.y + drawSize.y - 30); // 30 is the height of the red rect
		ImVec2 redRectSize(drawSize.x, 30);
		ImColor redRectColorUpper(0, 0, 0, 0);
		ImColor redRectColorLower(255, 0, 0, 128);
		drawList->AddRectFilledMultiColor(redRectPos, ImVec2(redRectPos.x + redRectSize.x, redRectPos.y + redRectSize.y), redRectColorUpper, redRectColorUpper, redRectColorLower, redRectColorLower);

		// Draw text on the red rectangle
		std::string bottomText = "Empty Event!";
		ImVec2 bottomTextSize = ImGui::CalcTextSize(bottomText.c_str());
		ImVec2 bottomTextPos(
			redRectPos.x + (redRectSize.x - bottomTextSize.x) * 0.5f,
			redRectPos.y + (redRectSize.y - bottomTextSize.y) * 0.5f
		);
		Application::fontLoader.DrawText(drawList, bottomTextPos, ImColor(255, 255, 255), bottomText);
	}
		

	// Draw node border
	ImColor borderColor = isActive ? ImColor(255, 255, 0) : ImColor(100, 100, 100); // Yellow if active
	drawList->AddRect(drawPosition,
		ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
		borderColor, headerRadius, ImDrawFlags_None, 6.0f);
}

int Node::GetHoveredInputPointIndex(ImVec2 MousePos)
{
	float HoverRadius = 8.0f;
	for (int i = 0; i < inputPoints.size(); ++i) 
	{
		if (Distance(MousePos, inputPoints[i]) < HoverRadius) 
		{ 
			return i;
		}
	}
	return -1;
}

int Node::GetHoveredOutputPointIndex(ImVec2 MousePos)
{
	float HoverRadius = 8.0f;
	for (int i = 0; i < outputPoints.size(); i++)
	{
		if (Distance(MousePos, outputPoints[i]) < HoverRadius)
		{
			return i;
		}
	}
	return -1;
}





