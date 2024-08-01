#pragma once
#include "Node.h"

class BeginNode : public Node
{
public:
	BeginNode(ImVec2 _Position, ImVec2 _Size);
	void DrawComponents(const ImVec2& position, const ImVec2& size, float zoomLevel) override;
private:
	ImVec2 Position;
	ImVec2 Size;
};

