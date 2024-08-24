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
	ImVec2 ParentSize = ImGui::GetContentRegionAvail();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImVec2 windowSize(Application::g_WindowWidth - 15, Application::g_WindowHeight - 40);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImVec2 WindowPos(15, 20);

	ImGui::SetCursorPos(WindowPos);
	if (ImGui::BeginChild("EditableEvents", ParentSize, ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		drawList->AddRectFilled(WindowPos, windowSize, ImColor(0.1f, 0.1f, 0.1f, 1.0f), 10.0f);

		if (ActiveChapter)
		{
			ActiveChapter->RenderViewport();
		}
		RenderOverlayUI();


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
	RenderNodeInspector();
	RenderChapterList();
	RenderChapterInspector();

	RenderGraphTabBar();
	RenderPlayModeToolBar();
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
ImVec2 tabBarPos = ImVec2(300, 32); // Set this to the top-left corner of your rectangle
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
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Set the size and position for the window and the rectangle inside it
	ImVec2 windowSize(Application::g_WindowWidth - 15, Application::g_WindowHeight - 40);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImVec2 WindowPos(7, 15);  // Positive Y value for positioning inside the window

	// Set the position for the rectangle relative to the window
	ImGui::SetCursorPos(WindowPos);

	// Calculate the size of the rectangle
	ImVec2 rectSize = ImVec2(315, Application::g_WindowHeight - 450);

	// Ensure the rectangle stays within the window's bounds
	rectSize.y = std::max(10.0f, rectSize.y);  // Ensure the height isn't negative or too small
	if (ImGui::BeginChild("##Data", ImVec2((rectSize.x + WindowPos.x) - 25,(rectSize.y + WindowPos.y) - 50),
		false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
	{
		// Draw the rectangle at the calculated position with rounded corners
		drawList->AddRectFilled(WindowPos, ImVec2(WindowPos.x + rectSize.x, WindowPos.y + rectSize.y), 
			ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f, ImDrawFlags_RoundCornersAll);
		{
			ImGui::SetCursorPosX(WindowPos.x + 100);
			ImGui::PushFont(Application::fontLoader.GetFont("NSBold"));
			ImGui::Text("Chapters");
			ImGui::PopFont();
			if (ImGui::BeginChild("##Data", ImVec2((rectSize.x + WindowPos.x) - 25, (rectSize.y + WindowPos.y) - 130), 
				false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
			{
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
					ImGui::SetCursorPosX(WindowPos.x);
					ImGui::PushFont(Application::fontLoader.GetFont("NSRegSmall"));
					if (ImGui::Button(Chapters[i]->GetChapterName().c_str(), ImVec2(270, 30.0f)))
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
					ImGui::PopFont();
					ImGui::PopStyleColor(3); // Restore original colors
				}
				ImGui::EndChild();
			}
			ImGui::SetCursorPosX(WindowPos.x);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.8f, 0.2f));
			ImGui::PushFont(Application::fontLoader.GetFont("NSRegSmall"));
			if(ImGui::Button("+ Create New Chapter", ImVec2(270, 40.0f)))
			{
				//Dummy text...
				ImGuiIO& io = ImGui::GetIO();
				Chapters.push_back(new Chapter("Untitled Chapter",io.MousePos));
			}
			ImGui::PopFont();
			ImGui::PopStyleColor();
			
		}

		ImGui::EndChild();
	}
}

void EventsUI::RenderChapterInspector()
{
	if (ActiveChapter != nullptr)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Set the size and position for the window and the rectangle inside it
		ImVec2 windowSize(Application::g_WindowWidth - 15, Application::g_WindowHeight - 40);
		ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
		ImVec2 WindowPos(7, (Application::g_WindowHeight - 450) + 20);  // Positive Y value for positioning inside the window

		// Set the position for the rectangle relative to the window
		ImGui::SetCursorPos(WindowPos);

		// Calculate the size of the rectangle
		ImVec2 rectSize = ImVec2(315, Application::g_WindowHeight - 650);

		// Ensure the rectangle stays within the window's bounds
		rectSize.y = std::max(10.0f, rectSize.y);  // Ensure the height isn't negative or too small
		if (ImGui::BeginChild("##Data2", ImVec2((rectSize.x + WindowPos.x) - 25, (rectSize.y + WindowPos.y)),
			false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
		{
			// Draw the rectangle at the calculated position with rounded corners
			drawList->AddRectFilled(WindowPos, ImVec2(WindowPos.x + rectSize.x, (WindowPos.y + rectSize.y) - 40),
				ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f, ImDrawFlags_RoundCornersAll);
			{
				ImGui::SetCursorPos(ImVec2(WindowPos.x + 50, ImGui::GetCursorPosY() - 5));
				ImGui::PushFont(Application::fontLoader.GetFont("NSBold"));
				ImGui::Text("Chapter Properties");
				ImGui::PopFont();

				//Buffer Var for Input Text
				char temp[256];
				//Copy Chapter Name string into Buffer Var
				strncpy_s(temp, ActiveChapter->GetChapterName().c_str(), sizeof(temp));
				ImGui::PushFont(Application::fontLoader.GetFont("NSRegSmall"));
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
				ImGui::Text("Name ");
				ImGui::SameLine();
				ImGui::InputTextWithHint("##Empty", temp, temp, IM_ARRAYSIZE(temp));
				ImGui::PopFont();

				if (ImGui::IsItemFocused())
				{
					if (ImGui::IsKeyPressed(ImGuiKey_Enter))
					{
						ActiveChapter->SetChapterName(temp);
					}
				}

				if (Chapters.size() > 1)
				{
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
					ImGui::PushFont(Application::fontLoader.GetFont("NSRegSmall"));
					if (ImGui::Button("Delete Chapter", ImVec2(200, 25)))
					{
						// Find the active chapter in the list of chapters
						auto it = std::find(Chapters.begin(), Chapters.end(), ActiveChapter);
						if (it != Chapters.end() && *it == ActiveChapter)
						{
							// Remove the chapter from the Chapters list
							Chapters.erase(it);

							// Find the active chapter in the OpenChapters list
							auto it2 = std::find(OpenChapters.begin(), OpenChapters.end(), ActiveChapter);

							// Remove the chapter from the OpenChapters list
							if (it2 != OpenChapters.end() && *it2 == ActiveChapter)
							{
								OpenChapters.erase(it2);
							}

							// Optionally, reset or select another active chapter
							if (!OpenChapters.empty())
							{
								SetActiveChapter(OpenChapters.front()); // Set to the first open chapter or any other logic
							}
							else
							{
								ActiveChapter = nullptr; // No chapters left, reset the active chapter
							}
						}
					}
					ImGui::PopFont();
				}

			}

			ImGui::EndChild();
		}
	}
}

void EventsUI::RenderNodeInspector()
{
	if (ActiveChapter != nullptr)
	{
		if (ActiveChapter->GetActiveNode() != nullptr)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 ListPos = ImVec2(ImGui::GetWindowWidth() - 290, 10); // Set this to the current cursor position, which should be below the tab bar
			ImGui::SetCursorPos(ListPos); // Relative to the window's position
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
			if (ImGui::BeginChild("##NodePropInspector", ImVec2(290, ImGui::GetWindowHeight() - 10),
				false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
			{

				// Calculate the size of the rectangle
				ImVec2 rectSize = ImVec2(290, Application::g_WindowHeight - 10);

				// Ensure the rectangle stays within the window's bounds
				rectSize.y = std::max(10.0f, rectSize.y);  // Ensure the height isn't negative or too small

				// Draw the rectangle at the calculated position with rounded corners
				drawList->AddRectFilled(ListPos, ImVec2(ListPos.x + rectSize.x, (ListPos.y + rectSize.y) - 40),
					ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f, ImDrawFlags_RoundCornersAll);
				{
					//Get half the Rect Size then use a mod float to fine tune X Axis placement of Text
					// Y Axis Seems alright where it is!!
					ImGui::SetCursorPosX(rectSize.x - (rectSize.x / 2) - 100);
					ImGui::PushFont(Application::fontLoader.GetFont("NSBold"));
					ImGui::Text("Node Components");
					ImGui::PopFont();

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
					if (ImGui::BeginChild("##Components",
						ImVec2(ImGui::GetContentRegionAvail().x - 50, ImGui::GetContentRegionAvail().y - 80)
						, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
					{
						Node* activeNode = ActiveChapter->GetActiveNode();
						if (activeNode != nullptr)
						{
							activeNode->DrawComponents();
						}
						else
						{
							std::cerr << "ActiveNode is null!" << std::endl;
						}

						ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 25));
						ImGui::EndChild();
					}

				}

				/*if (ImGui::BeginMenuBar())
				{
					ImGui::SetCursorPosX(-1);
					ImGui::Button("?");

					if (ImGui::IsItemHovered())
					{
						ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
						ImGui::BeginTooltip();
						ImGui::Text("[?] : Contains The Active Node Components.");
						ImGui::EndTooltip();
					}

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

				ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 25));*/

				ImGui::EndChild();
				ImGui::PopStyleColor();
			}
		}
	}
}

void EventsUI::RenderPlayModeToolBar() // NEEDS WORK!!!! NOT DONE YET!!!
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();  // Use the existing drawList pointer
	ImVec2 TBarSize(400, 25);
	ImVec2 TBarPos(ImGui::GetContentRegionAvail().x / 2, ImGui::GetWindowPos().y);
	ImGui::SetCursorPos(TBarPos);

	// Position the rectangle for the toolbar
	ImVec2 RectPos(TBarPos.x - 130, TBarPos.y);

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.05f, 0.05f, 0.05f, 1.0f));
	if (ImGui::BeginChild("##Toolbar", RectPos, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
	{
		// Draw the toolbar background
		drawList->AddRectFilled(RectPos, ImVec2((RectPos.x + TBarSize.x) - 90, (RectPos.y + TBarSize.y)),
			ImColor(0.05f, 0.05f, 0.05f, 1.0f));

		// Define the size and position of the button
		ImVec2 buttonPos = ImVec2(RectPos.x + 5, RectPos.y);
		ImVec2 buttonSize = ImVec2(35, 20); // Size derived from (40 - 5) and 20
		ImColor buttonColor = ImColor(0.3f, 0.3f, 0.3f, 1.0f);


		// Custom drawing code

		// Draw the filled rectangle (button background)
		drawList->AddRectFilled(buttonPos, ImVec2(buttonPos.x + buttonSize.x, buttonPos.y + buttonSize.y), buttonColor);

		// Draw the text on top of the rectangle
		Application::fontLoader.DrawText(drawList, ImVec2(buttonPos.x + 5, buttonPos.y + 2), ImVec4(1, 1, 1, 1.0f), "Play");

		// Create an invisible button over the same area
		if (ImGui::InvisibleButton("PlayButton", buttonSize))
		{
			// Button clicked, perform the button action here
			// For example, start playing something
			buttonColor = ImColor(0.3f, 0.0f, 0.3f, 1.0f);
		}


		ImGui::EndChild();
		ImGui::PopStyleColor();
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