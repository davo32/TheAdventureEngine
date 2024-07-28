#pragma once
#include "EventNode.h"

class Connection
{
public:
	Connection(EventNode* _startNode, EventNode* _endNode)
	: startNode(_startNode), endNode(_endNode) {}

	EventNode* GetStartNode() const { return startNode; }
	EventNode* GetEndNode() const { return endNode; }

	void DrawConnection(ImDrawList* drawList);
	
private:
	EventNode* startNode = nullptr;
	EventNode* endNode = nullptr;
};

