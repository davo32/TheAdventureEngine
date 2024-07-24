#include "Widget.h"

void Widget::DrawWidget()
{
	if (StoredEvent != nullptr)
	{
		if (isActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0,0.5,0,1));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.8f, 0, 1));
		}

		std::string buttonText = StoredEvent->GetEventName()  
			+ '\n'
			+ "Branching Events: " 
			+ std::to_string(StoredEvent->GetBranchCount())
			+ "   "
			+ "Rewards: " + std::to_string(StoredEvent->GetRewardItemCount());
		ImGui::Button(buttonText.c_str(), ImVec2(270, 50));

		if (isActive)
		{
			ImGui::PopStyleColor(2);
		}
	}
}


