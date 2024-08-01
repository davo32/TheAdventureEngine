#include "BeginNode.h"

BeginNode::BeginNode(ImVec2 _Position, ImVec2 _Size)
	: Position(_Position), Size(_Size)
{
	colorTop = ImColor(255, 0, 0, 128);

	// Initialize output points close to the right side
	outputPoints.push_back(ImVec2(0, Size.y / 2));
}

void BeginNode::DrawComponents(const ImVec2& position, const ImVec2& size, float zoomLevel)
{
	// Example implementation; replace with your actual drawing code
	ImGui::Text("EventNode Components");
}
