#pragma once
#include "Core/Nodes/Node.h"
#include "Core/Nodes/CustomNodes/EventNode.h"

class BeginNode : public Node
{
public:
	BeginNode();
	BeginNode(ImVec2 _Position, ImVec2 _Size);

	virtual bool ExecuteNodeAction() override;

	std::vector<uint8_t> serialize() const override;
	void deserialize(const std::vector<uint8_t>& data) override;
};

