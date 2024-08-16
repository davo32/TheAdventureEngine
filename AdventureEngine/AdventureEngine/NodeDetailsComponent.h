#pragma once
#include "NodeComponent.h"
#include "Node.h"

class NodeDetailsComponent : public NodeComponent
{
public:
	NodeDetailsComponent(Node* newParent) : NodeComponent(newParent) {}
	void RenderComponent() override;

private:
	//Debug Component UI stuff
	float DefaultComponentY = 70;
	float ClosedComponentY = 10;
	float CurrentComponentY = 70;
	bool IsOpen = true;
	bool isEditable = false;
	bool CanEdit = true;
};

