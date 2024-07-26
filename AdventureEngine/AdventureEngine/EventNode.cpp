#include "EventNode.h"


EventNode::EventNode(ImVec2 Position, ImVec2 Size, Event* newEvent)
	: Node(Position, Size, newEvent->GetEventName()), StoredEvent(newEvent)
{
	// Initialize input points close to the left side
	inputPoints.push_back(ImVec2(-20, Size.y / 2));

	// Initialize output points close to the right side
	outputPoints.push_back(ImVec2(0, Size.y / 2));
}

EventNode::~EventNode() 
{ 
	delete StoredEvent;
}

void EventNode::DrawComponents(const ImVec2& position, const ImVec2& size, const float& zoomLevel)
{
	// Example implementation; replace with your actual drawing code
	ImGui::Text("EventNode Components");
}

Event* EventNode::GetEvent() 
{ 
	return StoredEvent;
}
