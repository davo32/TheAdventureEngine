#include "EventNode.h"


EventNode::EventNode(ImVec2 Position, ImVec2 Size, Event* newEvent)
	: Node(Position, Size, newEvent->GetEventName()), StoredEvent(newEvent)
{

	// Initialize input points close to the left side
	inputPoints.push_back(ImVec2(Position.x + Size.x, Position.y + Size.y / 2));
	
	ImVec2 Combine = ImVec2(Position.x, Position.y);
	// Initialize output points close to the right side
	outputPoints.push_back(ImVec2(Combine));//0, Position + Size.y / 2));
}

EventNode::~EventNode() 
{ 
}

//void EventNode::DrawComponents()
//{
//	// Example implementation; replace with your actual drawing code
//	ImGui::Text("EventNode Components");
//
//	if (GetEvent()->GetEventText().empty() || GetEvent()->GetEventText().empty())
//	{
//		isEmpty = true;
//	}
//	else
//	{
//		isEmpty = false;
//	}
//}

Event* EventNode::GetEvent() 
{ 
	return StoredEvent;
}
