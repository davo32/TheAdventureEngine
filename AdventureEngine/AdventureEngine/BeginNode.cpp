#include "BeginNode.h"
#include "NodeDetailsComponent.h"

BeginNode::BeginNode(ImVec2 _Position, ImVec2 _Size)
	:Node(_Position,_Size,"Begin"), Position(_Position), Size(_Size)
{
	colorTop = ImColor(255, 0, 0, 128);

	// Initialize output points close to the right side
	outputPoints.push_back(ImVec2(Position.x + Size.x / 2,Position.y + Size.y / 2));
	Components.push_back(new NodeDetailsComponent(this));
}
