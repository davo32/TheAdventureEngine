#include "Connection.h"
void Connection::DrawConnection(ImDrawList* drawList)
{
	if (startNode && endNode)
	{
		ImVec2 startPos = startNode->GetCablePosition();
		ImVec2 endPos = endNode->GetCablePosition();

		// Control points for the curve
		ImVec2 control1 = ImVec2(startPos.x + 50, startPos.y);
		ImVec2 control2 = ImVec2(endPos.x - 50, endPos.y);

		// Draw the cubic bezier curve
		drawList->AddBezierCubic(startPos, control1, control2, endPos, ImColor(200, 200, 200), 2.0f);

	}
}


