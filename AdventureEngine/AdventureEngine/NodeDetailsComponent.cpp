#include "NodeDetailsComponent.h"

void NodeDetailsComponent::RenderComponent()
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
	if (ImGui::BeginChild("##NodeDetailsComponent", ImVec2(ImGui::GetContentRegionAvail().x, CurrentComponentY), ImGuiChildFlags_Border, ImGuiWindowFlags_MenuBar))
	{

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::InvisibleButton("##Toggle", ImGui::GetContentRegionAvail()))
			{
				IsOpen = !IsOpen;

				if (IsOpen)
				{
					CurrentComponentY = ClosedComponentY;
				}
				else
				{
					CurrentComponentY = DefaultComponentY;
				}
			}

			TextCenteredInMenuBar("Node Details");
			ImGui::EndMenuBar();
			ImGui::PopStyleColor();
		}

		char temp[256] = "";
		// Get the text from the active node
		std::string hint = ParentNode->GetText();

		// Display the input text field with hint
		if (isEditable)
		{
			// Fill the temp buffer with the hint text when transitioning to editable
			strncpy_s(temp, hint.c_str(), IM_ARRAYSIZE(temp));
			temp[IM_ARRAYSIZE(temp) - 1] = '\0'; // Ensure null termination
			if (ImGui::InputTextWithHint("##NodeName", hint.c_str(), temp, IM_ARRAYSIZE(temp)))
			{
				ParentNode->SetText(temp);
			}
		}
		else
		{
			ImGui::InputTextWithHint("##NodeName", hint.c_str(), temp, IM_ARRAYSIZE(temp), ImGuiInputTextFlags_ReadOnly);
		}

		// Add a button to toggle the editable state
		ImGui::SameLine();

		if (CanEdit)
		{
			if (ImGui::Button("Edit", ImVec2(40, 20)))
			{
				isEditable = !isEditable;
			}
		}

		ImGui::EndChild();
	}
}
