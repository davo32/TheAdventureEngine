#include "pch.h"
#include "Core/NodeComponents/CustomNodeComponents/NodeDetailsComponent/NodeDetailsComponent.h"

void NodeDetailsComponent::RenderComponent()
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
	if (ImGui::BeginChild("##NodeDetailsComponent", ImVec2(ImGui::GetContentRegionAvail().x, CurrentComponentY), false, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::SetCursorPosX(-1);
			ImGui::Button("?");

			if (ImGui::IsItemHovered())
			{
				ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
				ImGui::BeginTooltip();
				ImGui::Text("[?] : Allows You To Rename the Node.");
				ImGui::EndTooltip();
			}
			ImGui::SetCursorPosX(10);
			ImVec2 ButtonPos = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
			if (ImGui::InvisibleButton("##Toggle",ButtonPos))
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


		ImGui::Separator();
		ImGui::Text("Inputs:");
		for (size_t i = 0; i < ParentNode->inputPoints.size(); ++i)
		{
			Pin& inputPin = ParentNode->inputPoints[i];

			char inputNameBuffer[128];
			strncpy_s(inputNameBuffer, inputPin.label.c_str(), IM_ARRAYSIZE(inputNameBuffer));

			ImGui::PushID(i);  // Ensure unique ID for each input to avoid ImGui ID conflicts

			// Input field to edit the name of the input pin
			if (ImGui::InputText("##InputPinName", inputNameBuffer, IM_ARRAYSIZE(inputNameBuffer)))
			{
				inputPin.label = inputNameBuffer;  // Update the label when text changes
			}

			ImGui::SameLine();
			if (ImGui::Button("Remove"))
			{
				// Logic to remove the input pin if the remove button is clicked
				ParentNode->inputPoints.erase(ParentNode->inputPoints.begin() + i);
				if (ParentNode->inputPoints.size() == ParentNode->outputPoints.size())
				{
					ParentNode->SetSize(ImVec2(ParentNode->GetSize().x, ParentNode->GetSize().y - 15));  // Resize node
				}
			}

			ImGui::PopID();
		}
		ImGui::Separator();

		ImGui::Text("Outputs:");
		for (size_t i = 0; i < ParentNode->outputPoints.size(); ++i)
		{
			Pin& outputPin = ParentNode->outputPoints[i];

			char outputNameBuffer[128];
			strncpy_s(outputNameBuffer, outputPin.label.c_str(), IM_ARRAYSIZE(outputNameBuffer));

			ImGui::PushID(i + ParentNode->inputPoints.size());  // Ensure unique ID by offsetting from inputs

			// Input field to edit the name of the output pin
			if (ImGui::InputText("##OutputPinName", outputNameBuffer, IM_ARRAYSIZE(outputNameBuffer)))
			{
				outputPin.label = outputNameBuffer;  // Update the label when text changes
			}

			ImGui::SameLine();
			if (ImGui::Button("Remove"))
			{
				// Logic to remove the output pin if the remove button is clicked
				ParentNode->outputPoints.erase(ParentNode->outputPoints.begin() + i);
				if (ParentNode->inputPoints.size() == ParentNode->outputPoints.size())
				{
					ParentNode->SetSize(ImVec2(ParentNode->GetSize().x, ParentNode->GetSize().y - 15));  // Resize node
				}
			}

			ImGui::PopID();
		}
		ImGui::Separator();

		// Optionally, buttons to add more input/output pins
		if (ImGui::Button("Add Input"))
		{
			ImVec2 newPos;
			if (!ParentNode->inputPoints.empty())
			{
				// Get the last input pin's position
				ImVec2 lastPos = ParentNode->inputPoints.back().position;
				newPos = ImVec2(lastPos.x, lastPos.y + 50);  // Offset by 30 units below the last one
			}
			else
			{
				// Default position for the first input
				newPos = ImVec2(20, 100);
			}

			ParentNode->inputPoints.emplace_back(newPos, "Input");
			if (ParentNode->inputPoints.size() != ParentNode->outputPoints.size())
			{
				ParentNode->SetSize(ImVec2(ParentNode->GetSize().x, ParentNode->GetSize().y + 15));  // Resize node
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Output"))
		{
			ImVec2 newPos;
			if (!ParentNode->outputPoints.empty())
			{
				// Get the last output pin's position
				ImVec2 lastPos = ParentNode->outputPoints.back().position;
				newPos = ImVec2(lastPos.x, lastPos.y + 50);  // Offset by 30 units below the last one
			}
			else
			{
				// Default position for the first output
				newPos = ImVec2(ParentNode->GetSize().x - 20, 100);
			}

			ParentNode->outputPoints.emplace_back(newPos, "Output");
			if (ParentNode->inputPoints.size() != ParentNode->outputPoints.size())
			{
				ParentNode->SetSize(ImVec2(ParentNode->GetSize().x, ParentNode->GetSize().y + 15));  // Resize node
			}
		}

		ImGui::EndChild();
	}
}

