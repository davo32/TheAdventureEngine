#include "pch.h"
#include "Chapter.h"
#include <iostream>
#include "Core/Nodes/CustomNodes/EventNode.h"


void Chapter::DeleteActiveNode()
{
	if (ActiveNode != nullptr)
	{
		EventNode* ActiveEventNode = dynamic_cast<EventNode*>(ActiveNode);
		// Get the associated event before deleting the node
		Event* associatedEvent = ActiveEventNode->GetEvent();

		if (associatedEvent != nullptr)
		{

			// Find and delete the node
			auto nodeIt = std::find(NodeFamily.begin(), NodeFamily.end(), ActiveNode);
			if (nodeIt != NodeFamily.end())
			{
				// Erase node from vector
				NodeFamily.erase(nodeIt);

				// Delete the node
				delete ActiveNode;
				ActiveNode = nullptr;
			}

			// Find and delete the associated event
			if (associatedEvent != nullptr)
			{
				auto eventIt = std::find(events.begin(), events.end(), associatedEvent);
				if (eventIt != events.end())
				{
					events.erase(eventIt);
					delete associatedEvent;
				}
			}
		}
		else
		{
			std::cerr << "Associated Event is null" << std::endl;
		}

	}
}

void Chapter::ToggleSelection(Event* E, bool wasSelected)
{
	if (wasSelected)
	{
		DeselectCurrent();
	}
	else
	{
		DeselectCurrent();
		SelectEvent(E);
	}
}

void Chapter::DeselectCurrent()
{
	ActiveEvent = nullptr;
	if (ActiveNode) {
		ActiveNode->SetIsActive(false);
		ActiveNode = nullptr;
	}
}

void Chapter::SelectEvent(Event* E)
{
	ActiveEvent = E;
	ActiveNode = nullptr;
	for (Node* N : NodeFamily)
	{
		EventNode* EN = dynamic_cast<EventNode*>(N);
		if (EN->GetEvent() == ActiveEvent)
		{
			ActiveNode = EN;
			ActiveNode->SetIsActive(true);
			break;
		}
	}
}

std::vector<uint8_t> Chapter::serialize() const
{
	std::vector<uint8_t> data;

	// Serialize ChapterName
	size_t nameLength = ChapterName.size();
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&nameLength), reinterpret_cast<const uint8_t*>(&nameLength) + sizeof(size_t));
	data.insert(data.end(), ChapterName.begin(), ChapterName.end());

	// Serialize NodeFamily
	size_t nodeCount = NodeFamily.size();
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&nodeCount), reinterpret_cast<const uint8_t*>(&nodeCount) + sizeof(size_t));
	for (const auto& node : NodeFamily) {
		// Determine the type identifier for this node
		uint8_t nodeType;
		if (dynamic_cast<BeginNode*>(node)) {
			nodeType = 0; // Example type identifier for BeginNode
		}
		else if (dynamic_cast<EventNode*>(node)) {
			nodeType = 1; // Example type identifier for EndNode
		}

		// Serialize the type identifier
		data.insert(data.end(), reinterpret_cast<const uint8_t*>(&nodeType), reinterpret_cast<const uint8_t*>(&nodeType) + sizeof(uint8_t));

		// Serialize the node data
		std::vector<uint8_t> nodeData = node->serialize();
		size_t nodeDataSize = nodeData.size();
		data.insert(data.end(), reinterpret_cast<const uint8_t*>(&nodeDataSize), reinterpret_cast<const uint8_t*>(&nodeDataSize) + sizeof(size_t));
		data.insert(data.end(), nodeData.begin(), nodeData.end());
	}

	return data;
}

size_t Chapter::deserialize(const std::vector<uint8_t>& data, size_t offset)
{
	// Deserialize ChapterName
	size_t nameLength;
	memcpy(&nameLength, &data[offset], sizeof(size_t));
	offset += sizeof(size_t);
	ChapterName.assign(data.begin() + offset, data.begin() + offset + nameLength);
	offset += nameLength;

	// Deserialize NodeFamily
	size_t nodeCount;
	memcpy(&nodeCount, &data[offset], sizeof(size_t));
	offset += sizeof(size_t);
	for (size_t i = 0; i < nodeCount; ++i) {
		// Deserialize the type identifier
		uint8_t nodeType;
		memcpy(&nodeType, &data[offset], sizeof(uint8_t));
		offset += sizeof(uint8_t);

		// Deserialize the node data size
		size_t nodeDataSize;
		memcpy(&nodeDataSize, &data[offset], sizeof(size_t));
		offset += sizeof(size_t);
		std::vector<uint8_t> nodeData(data.begin() + offset, data.begin() + offset + nodeDataSize);
		offset += nodeDataSize;

		// Create a node based on the type identifier
		Node* node = nullptr;
		switch (nodeType)
		{
		case 0:
			node = new BeginNode();
			break;
		case 1:
			node = new EventNode(); // Example of a derived class
			break;
			// Add cases for other derived classes here
		default:
			throw std::runtime_error("Unknown node type during deserialization");
		}

		// Deserialize the node
		node->deserialize(nodeData);
		NodeFamily.push_back(node);
	}

	//map the target nodes here...
	finalizeConnections(NodeFamily);

	return offset;
}

void Chapter::finalizeConnections(std::vector<Node*> nodes)
{
	std::unordered_map<int, Node*> nodeMap;

	// Build a map of node IDs to node pointers
	for (auto& node : nodes)
	{
		nodeMap[node->id] = node;
	}

	for (auto& node : nodes)
	{
		// Update connections with actual node references
		for (auto& conn : node->connections)
		{
			if (conn.targetNodeID != -1)
			{
				conn.targetNode = nodeMap[conn.targetNodeID];
			}
		}
	}
}


