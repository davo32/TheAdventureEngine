#ifndef EVENTNODE_H
#define EVENTNODE_H

#include "Node.h"
#include "Event.h"

class EventNode : public Node
{
public:
	EventNode() = default;
	EventNode(ImVec2 Position, ImVec2 Size, Event* newEvent);
	~EventNode();
	void DrawComponents(const ImVec2& position, const ImVec2& size, const float& zoomLevel) override;
	Event* GetEvent();
	
private:
	Event* StoredEvent;
};
#endif // EVENTNODE_H