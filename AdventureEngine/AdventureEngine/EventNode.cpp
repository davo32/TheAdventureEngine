#include "EventNode.h"


EventNode::EventNode(ImVec2 Position, ImVec2 Size, Event* newEvent,NodeType _nodeType)
	: Node(Position, Size, newEvent->GetEventName()), StoredEvent(newEvent), nodeType(_nodeType)
{

	if (nodeType != NodeType::CHPTRSTART)
	{
		// Initialize input points close to the left side
		inputPoints.push_back(ImVec2(-20, Size.y / 2));
	}
	else
	{
		colorTop = ImColor(255, 0, 0);
		colorBottom = ImColor(60, 60, 60,0);
	}

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
