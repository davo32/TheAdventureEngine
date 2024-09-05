#pragma once
#include "Core/NodeComponents/NodeComponent.h"
#include "Core/Nodes/Node.h"

class PlayerChoiceComponent : public NodeComponent
{
public:
	PlayerChoiceComponent(Node* newParent)
		: NodeComponent(newParent) 
	{ 
		ComponentName = "Choice Component";
	}

	void RenderComponent() override;

private:
	float DefaultComponentY = 70;
	float ClosedComponentY = 10;
	float CurrentComponentY = 70;
	bool IsOpen = true;
};

