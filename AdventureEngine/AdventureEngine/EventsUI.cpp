#include "EventsUI.h"
#include "Application.h"
#include "MonitorInfo.h"
#include <iostream>

//Node Inspector
//If Event:
// - Allow Event to be editable (Input Fields for Name and Event Text)
// Else if Character:
// - Allow Character Name to be editable (InputField)
// - List of character sayings (in a drop down list)
// Else if LootTable:
// - Display editable/add/remove list of Items with their drop chance (also editable)
// Else if Item:
// - Allow Item to be editable (Input Field for Item Name)
// - (Sldier for quantity) - Dropdown for Item Type

void EventsUI::StartupUI()
{
	if (Chapters.empty())
	{
		ImGuiIO& io = ImGui::GetIO();
		//Create an example Chapter and set it as active
		Chapters.emplace_back(new Chapter("Example Chapter", io.MousePos));
		ActiveChapter = Chapters[0];
	}
}

void EventsUI::DrawUI()
{
	//Draw UI Here
	RenderEventWindow();
}

void EventsUI::text_centered(const std::string& text)
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

void EventsUI::RenderEventWindow()
{
	ImVec2 windowSize(MonitorInfo::GetMode()->width - 20, MonitorInfo::GetMode()->height - 70);

	if (ImGui::BeginChild("EditableEvents", windowSize, true/*, ImGuiWindowFlags_MenuBar*/))
	{
		if (ActiveChapter)
		{
			ActiveChapter->RenderViewport();
			RenderOverlayUI();
		}

		ImGui::EndChild();
	}
}

void EventsUI::RenderMenuBar(const char* title)
{
	if (ImGui::BeginMenuBar())
	{
		TextCenteredInMenuBar(title);
		ImGui::EndMenuBar();
	}
}

void EventsUI::RenderOverlayUI()
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	drawList->AddRectFilled(ImVec2(-5, 0), ImVec2(300, ImGui::GetWindowHeight() + 30), IM_COL32(20, 20, 20, 255));
	drawList->AddRectFilled(ImVec2(300, 0), ImVec2(ImGui::GetWindowWidth(), 85), IM_COL32(10, 10, 10, 190));
}

//void EventsUI::RenderComponents()
//{
//	if (!ImGui::BeginChild("Chapters", ImVec2(280, MonitorInfo::GetMode()->height / 2), true, ImGuiWindowFlags_MenuBar))
//		return;
//
//	RenderMenuBar("Chapters");
//	//RenderEventsList(); // - RenderChapterList instead
//
//	ImGui::EndChild();
//}

//void EventsUI::RenderNodeInspector()
//{
//	if (!ActiveNode && !ActiveEvent) return;
//
//	if (!ImGui::BeginChild("Node Inspector", ImVec2(280, (MonitorInfo::GetMode()->height / 2) - 120), true, ImGuiWindowFlags_MenuBar))
//		return;
//
//	RenderMenuBar("Node");
//	RenderEventRename();
//	RenderEventText();
//
//	ImGui::EndChild();
//}

//void EventsUI::RenderInspector()
//{
//	if (!ImGui::BeginChild("Inspector", ImVec2(300, MonitorInfo::GetMode()->height - 70.5f), true, ImGuiWindowFlags_MenuBar))
//		return;
//
//	RenderMenuBar("Inspector");
//	//RenderComponents();
//	//RenderNodeInspector();
//
//	ImGui::EndChild();
//}