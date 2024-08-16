#include "EventsUI.h"
#include "Application.h"
#include "MonitorInfo.h"
#include <iostream>

InspectorType EventsUI::Itype = InspectorType::CHAPTER;

void EventsUI::StartupUI()
{
	if (Chapters.empty())
	{
		ImGuiIO& io = ImGui::GetIO();
		//Create an example Chapter and set it as active
		Chapters.emplace_back(new Chapter("Example Chapter", io.MousePos));
		Chapters.emplace_back(new Chapter("Example Chapter2", io.MousePos));
		OpenChapters.emplace_back(Chapters[0]);
		ActiveChapter = Chapters[0];
	}
}

void EventsUI::DrawUI()
{
	//Draw UI Here
	RenderEventWindow();
	ActiveChapter->NodeGraphShortcuts();
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
	ImVec2 windowSize(Application::g_WindowWidth, Application::g_WindowHeight);
	ImVec2 ParentSize = ImGui::GetContentRegionAvail();

	if (ImGui::BeginChild("EditableEvents", ParentSize, ImGuiChildFlags_None,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
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

	drawList->AddRectFilled(ImVec2(-5, 0), ImVec2(ImGui::GetWindowWidth() + 5, ImGui::GetWindowHeight() + 5), IM_COL32(40, 40, 40, 128));
	RenderNodeInspector();
	RenderChapterList();
	RenderChapterInspector();
	
	
	RenderGraphTabBar();
	
}

void EventsUI::RenderGraphTabBar()
{
	ImGui::BeginGroup();
	// Push style colors and other style settings for the tab bar
	ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TabUnfocused, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 5.0f);

	// Set cursor position to the desired location for the TabBar
	ImVec2 tabBarPos = ImVec2(300, 7); // Set this to the top-left corner of your rectangle
	ImGui::SetCursorPos(tabBarPos); // Relative to the window's position

	// Define the size of the child window to control the width of the tab bar
	float tabBarWidth = 1305.0f; // Set the desired width for the tab bar
	ImGui::BeginChild("TabBarContainer", ImVec2(tabBarWidth, 0), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);


	// Render the TabBar
	if (ImGui::BeginTabBar("ChapterBar"))
	{
		for (Chapter* C : OpenChapters)
		{
			if (ImGui::BeginTabItem(C->GetChapterName().c_str()))
			{
				if (ActiveChapter != C)
				{
					//Make ActiveNode inactive and deselect it before changing to next Chapter!!!
					if (ActiveChapter != nullptr && ActiveChapter->GetActiveNode() != nullptr)
					{
						ActiveChapter->GetActiveNode()->SetIsActive(false);
						ActiveChapter->SetActiveNode(nullptr);
					}
					ActiveChapter = C;
				}
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}

	ImGui::EndChild(); // End the child window

	// Pop style colors and style variables to revert to the previous style
	ImGui::PopStyleColor(5); // Popping the 5 pushed style colors
	ImGui::PopStyleVar();    // Popping the pushed style variable
	ImGui::EndGroup();
}


void EventsUI::RenderChapterList()
{
	ImVec2 ListPos = ImVec2(7, 10); // Set this to the current cursor position, which should be below the tab bar
	ImGui::SetCursorPos(ListPos); // Relative to the window's position
	ImGui::PushStyleColor(ImGuiCol_ChildBg,ImVec4(0.1f,0.1f,0.1f,1.0f));
	if (ImGui::BeginChild("##Data", ImVec2(280, ImGui::GetWindowHeight() - 650),ImGuiChildFlags_Border,ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			TextCenteredInMenuBar("Chapters");
			ImGui::EndMenuBar();
		}


		for (int i = 0; i < Chapters.size(); i++)
		{
			ImVec4 buttonColor;
			if (i % 2 == 0)
			{
				// Define the color slightly lighter than black
				buttonColor = ImVec4(0.2f, 0.2f, 0.2f, 0.4f); // Light grey
			}
			else
			{
				buttonColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Light grey
			}

			// Push custom style color for buttons
			ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Slightly lighter when hovered
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); // Even lighter when active
			if (ImGui::Button(Chapters[i]->GetChapterName().c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 20.0f)))
			{
				if (contains(OpenChapters, Chapters[i]))
				{
					SetActiveChapter(Chapters[i]);
				}
				else
				{
					OpenChapters.emplace_back(Chapters[i]);
					SetActiveChapter(Chapters[i]);
				}
			}
			ImGui::PopStyleColor(3); // Restore original colors

		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
}

void EventsUI::RenderChapterInspector()
{
	ImVec2 ListPos = ImVec2(7, (ImGui::GetWindowHeight() - 650) + 20); // Set this to the current cursor position, which should be below the tab bar
	ImGui::SetCursorPos(ListPos); // Relative to the window's position

	if (ActiveChapter != nullptr)
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		if (ImGui::BeginChild("##ChapterInspector", ImVec2(280, ImGui::GetWindowHeight() - 650), ImGuiChildFlags_Border, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				TextCenteredInMenuBar("Chapter Properties");
				ImGui::EndMenuBar();
			}

			//ImGui::InputTextWithHint("", ActiveChapter->GetChapterName().c_str(), );

			ImGui::EndChild();
			ImGui::PopStyleColor();
		}
	}
}

void EventsUI::RenderNodeInspector()
{
	if (ActiveChapter->GetActiveNode() != nullptr)
	{
		ImVec2 ListPos = ImVec2(ImGui::GetWindowWidth() - 290, 10); // Set this to the current cursor position, which should be below the tab bar
		ImGui::SetCursorPos(ListPos); // Relative to the window's position
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		if (ImGui::BeginChild("##NodePropInspector", ImVec2(270, ImGui::GetWindowHeight() - 350), ImGuiChildFlags_Border, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				TextCenteredInMenuBar("Node Properties");
				ImGui::EndMenuBar();
			}

			Node* activeNode = ActiveChapter->GetActiveNode();
			if (activeNode != nullptr)
			{
				activeNode->DrawComponents();
			}
			else
			{
				std::cerr << "ActiveNode is null!" << std::endl;
			}

			ImGui::Button("Add Component",ImVec2(ImGui::GetContentRegionAvail().x,25));

			ImGui::EndChild();
			ImGui::PopStyleColor();
		}
	}
}

bool EventsUI::contains(std::vector<Chapter*> C, Chapter* value)
{
	return std::find(C.begin(), C.end(), value) != C.end();
}

void EventsUI::SetActiveChapter(Chapter* chapter)
{
	ActiveChapter = chapter;
}