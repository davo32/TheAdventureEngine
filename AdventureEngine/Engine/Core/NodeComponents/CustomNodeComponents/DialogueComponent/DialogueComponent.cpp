#include "pch.h"
#include "DialogueComponent.h"

void DialogueComponent::RenderComponent()
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
	if (ImGui::BeginChild("##NodeDialogueComponent", ImVec2(ImGui::GetContentRegionAvail().x, CurrentComponentY), false, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::SetCursorPosX(-1);
			ImGui::Button("?");

			if (ImGui::IsItemHovered())
			{
				ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
				ImGui::BeginTooltip();
				ImGui::Text("[?] : Dialogue Creation Component");
				ImGui::EndTooltip();
			}
			ImGui::SetCursorPosX(10);
			ImVec2 ButtonPos = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
			if (ImGui::InvisibleButton("##Toggle", ButtonPos))
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

			TextCenteredInMenuBar("Node Dialogue");
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 15);
			if (ImGui::Button("..."))
			{
				ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
				ImGui::OpenPopup("ComponentContextPopup");
			}
			if (ImGui::BeginPopup("ComponentContextPopup"))
			{
				if (ImGui::Button("Reset Component"))
				{
					//Handle everything to reset the component to factory settings!!
					//This may entail saving the old name of the node for this component.
					// Then setting the name of the component to the old name.
				}
				ImGui::Separator();
				if (ImGui::Button("Remove Component"))
				{
					//Get Parent Node
					//Loop through components to find this component
					//Remove this component from the vector
					//Component is now "Deleted"
					//THIS SHOULD BE A FUNCTION!!!
				}

				// Add more buttons as needed


			// End the popup
				ImGui::EndPopup();
			}
			ImGui::EndMenuBar();
			ImGui::PopStyleColor();
		}

		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) - 20);
		ImGui::Text("Dialogue");

		EventNode* ENode = reinterpret_cast<EventNode*>(ParentNode);
		char DialogueBuffer[128];
		strncpy_s(DialogueBuffer, ENode->GetEvent()->GetEventText().c_str(), IM_ARRAYSIZE(DialogueBuffer));

		ImGui::SetCursorPosX(30);
		// Input field to edit the name of the input pin
		if (ImGui::InputTextMultiline("##DialogueText", DialogueBuffer, IM_ARRAYSIZE(DialogueBuffer),ImVec2(ImGui::GetContentRegionAvail().x - 30,100)))
		{
			ENode->GetEvent()->SetEventText(DialogueBuffer);
		}

		ImGui::EndChild();
	}
}
