#pragma once
#include <vector>
#include <string>
#include "Core/Events/Event.h"
#include "Core/Nodes/Node.h"
#include "Core/Nodes/CustomNodes/BeginNode.h"

class Chapter
{
public:
	Chapter() = default;
	Chapter(std::string newName) : ChapterName(newName) 
	{
		ImVec2 NodePos = ImVec2(935, 362);
		NodeFamily.emplace_back(new BeginNode(NodePos, ImVec2(100, 70)));
	}

	void DeleteActiveNode();

	void ToggleSelection(Event* E, bool wasSelected);
	void DeselectCurrent();
	void SelectEvent(Event* E);

	std::string GetChapterName() { return ChapterName; }
	void SetChapterName(std::string newName) { ChapterName = newName; }

	Node* GetActiveNode() { return ActiveNode; }
	void SetActiveNode(Node* newNode) { ActiveNode = newNode; }
	
	Event* GetActiveEvent() { return ActiveEvent; }

	ImVec2 viewportOffset = ImVec2(300, 65);
	ImVec2 lastMousePos = ImVec2(0, 0);
	ImVec2 screenPos;
	ImVec2 screenSize;

	float zoomLevel = 1.0f;


	Node* ActiveNode;
	Event* ActiveEvent;

	std::vector<uint8_t> serialize() const;
	size_t deserialize(const std::vector<uint8_t>& data, size_t offset);
	void finalizeConnections(std::vector<Node*> nodes);
	
	//serializable Data

	std::string ChapterName;
	std::vector<Event*> events;
	std::vector<Node*> NodeFamily;
};

