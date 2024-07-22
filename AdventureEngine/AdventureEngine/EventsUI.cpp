#include "EventsUI.h"
#include "Application.h"
#include "MonitorInfo.h"
void EventsUI::Test()
{
	//TESTING
	Event* testEvent = new Event("Unknown Event", "You Test This Program..");
	Event* One = new Event("1", "1");
	Event* Two = new Event("2", "2");
	testEvent->AddBranchingEvents(
		{
			std::make_pair("One", One),
			std::make_pair("Two", Two)
		});
	Widget* newWidget = new Widget(testEvent);
	Buttons.push_back(newWidget);
}

Widget* EventsUI::GetWidgetByRef(Widget* ref)
{
	auto it = std::find(Buttons.begin(), Buttons.end(), ref);
	if (it != Buttons.end())
	{
		return *it;
	}
	return nullptr; // Return nullptr if widget is not found
}

void EventsUI::DrawUI()
{
	//Draw UI Here
	HierachyElement();
}

void EventsUI::TextCentered(std::string text)
{
		// Get the size of the available content region
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();

		// Calculate the size of the text
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

		// Calculate the position to center the text
		ImVec2 textPos;
		textPos.x = (contentRegion.x - textSize.x) * 0.5f;
		textPos.y = (contentRegion.y - textSize.y) * 0.5f;

		// Set the cursor position and draw the text
		ImGui::SetCursorPos(textPos);
		ImGui::Text(text.c_str());
}

void EventsUI::HierachyElement()
{
	if (ImGui::BeginChild("Events", ImVec2(300, MonitorInfo::GetMode()->height - 80), true, ImGuiChildFlags_Border))
	{
		ImGui::PushStyleColor(ImGuiCol(ImGuiCol_ChildBg), ImVec4(0.5, 0.5, 0.5, 1.0));
		if (ImGui::BeginChild("HierachyHeader", ImVec2(300, 25)))
		{
			// Text to be centered
			const char* text = "Events";
			TextCentered(text);
			
			ImGui::EndChild();
		}
		ImGui::PopStyleColor();

		
		/////////////////////////////////////////////////////////////////////
		for (Widget* widget : Buttons)
		{
			widget->DrawWidget();

			// Check if the mouse is hovering over the button
			if (ImGui::IsItemHovered())
			{
				// Check for left mouse button click
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					if (ActiveWidget != nullptr)
					{
						//Remove the old Active Widget
						ActiveWidget->SetIsActive(false);
					}
						//Setup the new Active Widget
						ActiveWidget = widget;
						ActiveWidget->SetIsActive(true);
					
				}

				// Check for right mouse button click
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					// Remove the widget from the Buttons vector
					auto it = std::find(Buttons.begin(), Buttons.end(), widget);
					if (it != Buttons.end())
					{
						Buttons.erase(it); // Erase the widget
						delete widget;
					}
				}
			}
		}
		
		if (ImGui::Button("Create Event", ImVec2(300, 50)))
		{
			Test();
		}
	}
}
