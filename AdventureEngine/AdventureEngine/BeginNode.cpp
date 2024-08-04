#include "BeginNode.h"

BeginNode::BeginNode(ImVec2 _Position, ImVec2 _Size)
	:Node(_Position,_Size,"Begin"), Position(_Position), Size(_Size)
{
	colorTop = ImColor(255, 0, 0, 128);

	// Initialize output points close to the right side
	outputPoints.push_back(ImVec2(Position.x + Size.x / 2,Position.y + Size.y / 2));
}

void BeginNode::DrawComponents(const ImVec2& position, const ImVec2& size, float zoomLevel)
{
	// Example implementation; replace with your actual drawing code
	ImGui::Text("EventNode Components");
}
