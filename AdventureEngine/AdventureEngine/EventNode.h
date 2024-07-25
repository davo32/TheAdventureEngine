#include "Node.h"
#include "Event.h"

class EventNode : public Node
{
public:
	EventNode(ImVec2 Position,ImVec2 Size,std::string name,Event* newEvent)
		: Node(Position,Size,name) , StoredEvent(newEvent) {}
	~EventNode() { delete StoredEvent; }
	void DrawComponents(const ImVec2& position, const ImVec2& size, const float& zoomLevel) override;
private:
	Event* StoredEvent;
};