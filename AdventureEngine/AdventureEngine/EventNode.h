#pragma once
#include "Node.h"
#include "Event.h"
#include "NodeType.h"

class EventNode : public Node
{
public:
	//EventNode() = default;
	EventNode(ImVec2 Position, ImVec2 Size, Event* newEvent,NodeType _nodeType);
	~EventNode();
	Event* GetEvent();
	void DrawComponents(const ImVec2& position, const ImVec2& size, float zoomLevel) override;
	
	bool GetIsChapterStarter() { return isChapterStarter; }
	void SetIsChapterStarter(bool check) { isChapterStarter = check; }

private:
	Event* StoredEvent;
	bool isChapterStarter = false;
	NodeType nodeType = NodeType::PLOT;
};