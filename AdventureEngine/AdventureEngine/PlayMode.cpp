#include "PlayMode.h"
#include "Core/Nodes/CustomNodes/EventNode.h"
#include "Globals.h"

void PlayMode::RenderPlayViewport(Node* startNode)
{
	StartingNode = startNode;
	ImVec2 newViewportSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y /*- 20*/));
	if (ImGui::BeginChild("##Viewport", newViewportSize, ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 childPos = ImGui::GetWindowPos();
		ImVec2 childSize = newViewportSize;
		// Draw background
		drawList->AddRectFilled(childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), IM_COL32(0, 0, 0, 255), 0.0f);
		if (StartingNode != nullptr)
		{
			PlayModeLoop();
		}

		ImGui::EndChild();
	}
}

std::string PlayMode::ExecuteFirstNode()
{
	// Check if StartingNode and its connections are valid
	if (StartingNode == nullptr || StartingNode->connections.empty())
	{
		return "";
	}

	// Access the first connection
	EventNode* TargetNode = dynamic_cast<EventNode*>(StartingNode->connections[0].targetNode);
	if (TargetNode != nullptr && TargetNode->GetEvent() != nullptr)
	{
		std::cout << "Set Properly" << '\n';
		return TargetNode->GetEvent()->GetEventText();
	}
	return "";
}

void PlayMode::PlayModeLoop()
{
	/*while (inPlayMode)
	{*/	ImGui::PushFont(Globals::fontLoader->GetFont("NSReg"));
		ImVec2 size(ImGui::CalcTextSize(ExecuteFirstNode().c_str()));
		ImGui::SetCursorPos(ImVec2((ImGui::GetContentRegionAvail().x / 2) - (size.x / 2),80.0f));
		ImGui::Text(ExecuteFirstNode().c_str());
		ImGui::PopFont();
	/*}*/
}
