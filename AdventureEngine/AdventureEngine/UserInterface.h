#pragma once
#include "glfw/glfw3.h"
#include "imgui.h"
#include <string>

class UserInterface
{
public:
	virtual void StartupUI() {}
	virtual void DrawUI() = 0;
	virtual ~UserInterface() = default;
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
};
