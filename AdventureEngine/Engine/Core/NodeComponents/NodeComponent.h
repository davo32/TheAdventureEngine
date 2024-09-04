#pragma once
#include "imgui.h"
#include <string>

class Node;

//Base Class for all Node Components!!
class NodeComponent
{
public:
	NodeComponent(Node* newParent) : ParentNode(newParent) {}
	virtual void RenderComponent() = 0;
	void TextCenteredInMenuBar(std::string text)
	{
		// Calculate the size of the text
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

		// Get the window width
		float windowWidth = ImGui::GetWindowWidth();

		// Calculate the position to center the text
		float textPosX = (windowWidth - textSize.x) * 0.5f;

		// Align text to center
		ImGui::SetCursorPosX(textPosX);
		ImGui::Text(text.c_str());
	}

protected:
	Node* ParentNode;
};
