#include "pch.h"
#include "PlayerChoiceComponent.h"

void PlayerChoiceComponent::RenderComponent()
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
	if (ImGui::BeginChild("##NodeDetailsComponent", ImVec2(ImGui::GetContentRegionAvail().x, CurrentComponentY), false, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::SetCursorPosX(-1);
			ImGui::Button("?");

			if (ImGui::IsItemHovered())
			{
				ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
				ImGui::BeginTooltip();
				ImGui::Text("[?] : Player Choice Component.");
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

			TextCenteredInMenuBar("Node Details");
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

		if (ImGui::BeginChild("##Body", ImVec2(ImGui::GetContentRegionAvail().x, CurrentComponentY - 30)
			,ImGuiChildFlags_Border, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
		{

			ImGui::EndChild();
		}

		ImGui::EndChild();
	}
}