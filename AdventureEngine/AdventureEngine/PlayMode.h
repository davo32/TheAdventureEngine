#pragma once
#include "imgui.h"
#include "Core/Nodes/Node.h"
class PlayMode
{
public:
	void RenderPlayViewport(Node* startNode);
	std::string ExecuteFirstNode();
	void PlayModeLoop();

	void SetInPlayMode(bool check) { inPlayMode = check; }
	bool GetInPlayMode() { return inPlayMode; }

private:
	Node* StartingNode;
	bool inPlayMode = true;
};

