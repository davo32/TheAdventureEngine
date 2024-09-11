#pragma once
#include "Core/NodeComponents/NodeComponent.h"
#include "Core/Nodes/Node.h"

class DialogueComponent : public NodeComponent
{
public:

	DialogueComponent(Node* newParent)
		: NodeComponent(newParent)
	{
		ComponentName = "Dialogue Component";
	}

	void RenderComponent() override;

private:
	// In your class definition
	char DialogueBuffer[128] = { 0 };

	//Debug Component UI stuff
	float DefaultComponentY = 160;
	float ClosedComponentY = 10;
	float CurrentComponentY = 160;
	bool IsOpen = true;
};

