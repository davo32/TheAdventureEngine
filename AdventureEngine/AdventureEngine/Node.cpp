#include "Node.h"
#include "imgui.h"
#include "Application.h"
#include <iostream>

Node::Node(ImVec2 pos, ImVec2 size, std::string name)
	: position(pos), size(size), text(std::move(name)) {}

void Node::ConnectTo(Node* targetNode, int outputIndex, int inputIndex)
{
	// Ensure the target node and indices are valid
	if (!targetNode || outputIndex < 0 || inputIndex < 0) {
		return;
	}

	// Add the connection to the current node's output connections
	Connection newConnection;
	newConnection.targetNode = targetNode;
	newConnection.outputIndex = outputIndex;
	newConnection.inputIndex = inputIndex;

	connections.push_back(newConnection);
}

void Node::RenderConnections(ImDrawList* drawList, float zoomLevel, ImVec2 viewportOffset) const
{
	for (const Connection& conn : connections) 
	{
		// Get the position of the output and input points
		ImVec2 outputPointPos = GetOutputPoint(conn.outputIndex);
		ImVec2 inputPointPos = conn.targetNode->GetInputPoint(conn.inputIndex);

		// Convert the points to screen coordinates
		outputPointPos = ImVec2((outputPointPos.x * zoomLevel) + viewportOffset.x, (outputPointPos.y * zoomLevel) + viewportOffset.y);
		inputPointPos = ImVec2((inputPointPos.x * zoomLevel) + viewportOffset.x, (inputPointPos.y * zoomLevel) + viewportOffset.y);

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
	const float rectWidth = 15.0f;
	const float rectHeight = 20.0f;
	const float rectOffsetY = headerHeight / 2;

	for (const ImVec2& point : inputPoints) 
	{
		ImVec2 center(drawPosition.x + padding + 20.0f, drawPosition.y + rectOffsetY + 35.0f);
		ImVec2 rectMin(center.x - rectWidth / 2, center.y - rectHeight / 2);
		ImVec2 rectMax(center.x + rectWidth / 2, center.y + rectHeight / 2);

		drawList->AddRect(rectMin, rectMax, ImColor(255, 255, 255), headerRadius);
		ImVec2 innerRectMin(rectMin.x + borderThickness, rectMin.y + borderThickness);
		ImVec2 innerRectMax(rectMax.x - borderThickness, rectMax.y - borderThickness);
		drawList->AddRectFilled(innerRectMin, innerRectMax, ImColor(0, 0, 0, 128), headerRadius);

		pinOffsetY += rectHeight + padding;
		drawList->AddText(ImVec2(center.x + rectWidth, center.y - 8.0f), ImColor(255, 255, 255, 255), "Input");
	}

	for (const ImVec2& point : outputPoints) 
	{
		ImVec2 center(drawPosition.x + drawSize.x - padding - 20.0f, drawPosition.y + rectOffsetY + 35.0f);
		ImVec2 rectMin(center.x - rectWidth / 2, center.y - rectHeight / 2);
		ImVec2 rectMax(center.x + rectWidth / 2, center.y + rectHeight / 2);

		drawList->AddRect(rectMin, rectMax, ImColor(255, 255, 255), headerRadius);
		ImVec2 innerRectMin(rectMin.x + borderThickness, rectMin.y + borderThickness);
		ImVec2 innerRectMax(rectMax.x - borderThickness, rectMax.y - borderThickness);
		drawList->AddRectFilled(innerRectMin, innerRectMax, ImColor(0, 0, 0, 128), headerRadius);

		pinOffsetY += rectHeight + padding;
		drawList->AddText(ImVec2(center.x - rectWidth - 32.0f, center.y - 8.0f), ImColor(255, 255, 255, 255), "Output");
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

	/*if (ImGui::InvisibleButton("##Collapse", ImVec2(20, headerSize.y)))
	{

	}*/

	// Draw text on header
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
	ImVec2 textPos(
		drawPosition.x + (drawSize.x - textSize.x) * 0.5f,
		drawPosition.y + (headerHeight - textSize.y) * 0.5f
	);
	Application::fontLoader.SetFont("Bold");
	Application::fontLoader.DrawText(drawList, textPos, ImColor(255, 255, 255), text);

	// Draw node border
	ImColor borderColor = isActive ? ImColor(255, 255, 0) : ImColor(100, 100, 100); // Yellow if active
	drawList->AddRect(drawPosition,
		ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
		borderColor, headerRadius, ImDrawFlags_None, 6.0f);
}

int Node::GetHoveredInputPointIndex(ImVec2 MousePos)
{
	for (int i = 0; i < inputPoints.size(); ++i) 
	{
		if (Distance(MousePos, inputPoints[i]) < 10.0f) 
		{ 
			return i;
		}
	}
	return -1;
}

int Node::GetHoveredOutputPointIndex(ImVec2 MousePos)
{
	for (int i = 0; i < outputPoints.size(); i++)
	{
		std::cout << "Mouse: " << MousePos.x << " : " << MousePos.y << '\n';
		std::cout << " Point: " << outputPoints[i].x << " : " << outputPoints[i].y << '\n';
		ImVec2 OutputPos = ImVec2(outputPoints[0].x /*+ position.x*/, outputPoints[i].y /*+ position.y*/);
		if (Distance(MousePos, OutputPos) < 100.0f)
		{
			return i;
		}
	}
	return -1;
}





