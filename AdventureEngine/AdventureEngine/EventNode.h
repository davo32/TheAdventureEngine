#pragma once
#include "Node.h"
#include "Event.h"
#include "NodeType.h"

class EventNode : public Node
{
public:
	//EventNode() = default;
	EventNode(ImVec2 Position, ImVec2 Size, Event* newEvent);
	~EventNode();
	Event* GetEvent();
	

private:
	Event* StoredEvent;
};