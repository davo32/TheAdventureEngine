#pragma once
#include "Node.h"

class BeginNode : public Node
{
public:
	BeginNode(ImVec2 _Position, ImVec2 _Size);
private:
	ImVec2 Position;
	ImVec2 Size;
};

