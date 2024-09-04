#pragma once
#include "Core/Nodes/Node.h"
#include "Core/Events/Event.h"
//#include "NodeType.h"

class EventNode : public Node
{
public:
	EventNode();
	EventNode(ImVec2 Position, ImVec2 Size, Event* newEvent);
	~EventNode();
	void DrawComponents();
	Event* GetEvent();
	
	virtual bool ExecuteNodeAction() override;

	std::vector<uint8_t> serialize() const override;
	void deserialize(const std::vector<uint8_t>& data) override;
	Event* StoredEvent;
	const size_t MAX_ALLOWED_SIZE = 10000; // Adjust this based on your needs
};