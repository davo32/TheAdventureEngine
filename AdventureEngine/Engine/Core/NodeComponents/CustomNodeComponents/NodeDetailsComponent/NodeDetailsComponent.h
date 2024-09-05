#pragma once
#include "Core/NodeComponents/NodeComponent.h"
#include "Core/Nodes/Node.h"

class NodeDetailsComponent : public NodeComponent
{
public:
	NodeDetailsComponent(Node* newParent) : NodeComponent(newParent) 
	{
		ComponentName = "General Component";
	}

	void RenderComponent() override;

private:
	//Debug Component UI stuff
	float DefaultComponentY = 200;
	float ClosedComponentY = 10;
	float CurrentComponentY = 200;
	bool IsOpen = true;
	bool isEditable = false;
	bool CanEdit = true;
};

