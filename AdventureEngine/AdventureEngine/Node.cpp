#include "Node.h"
#include "imgui.h"
#include "Application.h"

Node::Node(ImVec2 pos, ImVec2 size, std::string name)
	: position(pos), size(size), text(std::move(name)) {}

void Node::SetDragStartPos(ImVec2 pos) {
	dragStartPos = pos;
}

ImVec2 Node::GetDragStartPos() const {
	return dragStartPos;
}

void Node::SetInitialPosition(ImVec2 pos) {
	initialPosition = pos;
}

ImVec2 Node::GetInitialPosition() const {
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
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	const float headerRadius = 6.0f;
	const float headerHeight = 30.0f;
	const float padding = 5.0f;
	float pinOffsetY = 0.0f;

	CreateNodeVisuals(position, size, drawList, headerRadius, headerHeight);
	CreateInputsAndOutputs(position, padding, headerHeight, size, drawList, headerRadius, 2.0f, pinOffsetY);
	DrawComponents(position, size, zoomLevel);
}

void Node::CreateInputsAndOutputs(const ImVec2& drawPosition, float padding, float headerHeight, const ImVec2& drawSize, ImDrawList* drawList, float headerRadius, float borderThickness, float& pinOffsetY) {
	const float rectWidth = 15.0f;
	const float rectHeight = 20.0f;
	const float rectOffsetY = headerHeight / 2;

	for (const ImVec2& point : inputPoints) {
		ImVec2 center(drawPosition.x + padding + 20.0f, drawPosition.y + rectOffsetY + 35.0f);
		ImVec2 rectMin(center.x - rectWidth / 2, center.y - rectHeight / 2);
		ImVec2 rectMax(center.x + rectWidth / 2, center.y + rectHeight / 2);

		drawList->AddRect(rectMin, rectMax, ImColor(255, 255, 255), headerRadius);
		ImVec2 innerRectMin(rectMin.x + borderThickness, rectMin.y + borderThickness);
		ImVec2 innerRectMax(rectMax.x - borderThickness, rectMax.y - borderThickness);
		drawList->AddRectFilled(innerRectMin, innerRectMax, ImColor(0, 0, 0, 128), headerRadius);

		pinOffsetY += rectHeight + padding;
		drawList->AddText(ImVec2(center.x + rectWidth, center.y - 8.0), ImColor(255, 255, 255, 255), "Input");
	}

	for (const ImVec2& point : outputPoints) {
		ImVec2 center(drawPosition.x + drawSize.x - padding - 20.0f, drawPosition.y + rectOffsetY + 35.0f);
		ImVec2 rectMin(center.x - rectWidth / 2, center.y - rectHeight / 2);
		ImVec2 rectMax(center.x + rectWidth / 2, center.y + rectHeight / 2);

		drawList->AddRect(rectMin, rectMax, ImColor(255, 255, 255), headerRadius);
		ImVec2 innerRectMin(rectMin.x + borderThickness, rectMin.y + borderThickness);
		ImVec2 innerRectMax(rectMax.x - borderThickness, rectMax.y - borderThickness);
		drawList->AddRectFilled(innerRectMin, innerRectMax, ImColor(0, 0, 0, 128), headerRadius);

		pinOffsetY += rectHeight + padding;
		drawList->AddText(ImVec2(center.x - rectWidth - 32.0f, center.y - 8.0), ImColor(255, 255, 255, 255), "Output");
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

	// Draw node border
	ImColor borderColor = isActive ? ImColor(255, 255, 0) : ImColor(100, 100, 100); // Yellow if active
	drawList->AddRect(drawPosition,
		ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
		borderColor, headerRadius, ImDrawFlags_None, 6.0f);
}

void Node::DrawConnection(ImDrawList* drawList, ImVec2 start, ImVec2 end,ImColor color) 
{
	drawList->AddLine(start, end, color);
}