#include "EventsUI.h"
#include "Application.h"
#include "MonitorInfo.h"
#include <iostream>

using namespace aie;

InspectorType EventsUI::Itype = InspectorType::CHAPTER;

void EventsUI::StartupUI()
{
	if (!PlayTexture.load("../Resources/Images/PlayIcon.png"))
	{
		//Not Loaded...
	}

	
}

void EventsUI::SetupInitialChapter()
{
	if (Globals::projectBrowser->GetActiveProject() != nullptr)
	{
		if (ActiveChapter == nullptr)
		{
			ImGuiIO& io = ImGui::GetIO();
			//Create an example Chapter and set it as active
			Chapters = Globals::projectBrowser->GetActiveProject()->ProjectChapters;
			OpenChapters.emplace_back(Chapters[0]);
			ActiveChapter = Chapters[0];
		}
		else
		{
			//std::cout << "Active Chapter is Set" << '\n';
		}
	}
	else
	{
		std::cout << "ERROR: Active Project IS Nullptr" << "\n";
	}
}

void EventsUI::DrawUI()
{
	//Draw UI Here
	RenderEventWindow();

	SetupInitialChapter();


	if (ActiveChapter)
	{
		chapterEditor->NodeGraphShortcuts(ActiveChapter);
	}
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

		chapterEditor->RenderViewport(ActiveChapter);
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

	RenderPlayModeToolBar();
	RenderGraphTabBar();
	RenderConsoleLog();
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
	ImVec2 tabBarPos = ImVec2(310, 62); // Set this to the top-left corner of your rectangle
	ImGui::SetCursorPos(tabBarPos); // Relative to the window's position

	// Define the size of the child window to control the width of the tab bar
	float tabBarWidth = 1295.0f; // Set the desired width for the tab bar
	if(ImGui::BeginChild("TabBarContainer", ImVec2(tabBarWidth, 0), false,
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
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
	}
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
	if (ImGui::BeginChild("##Data", ImVec2((rectSize.x + WindowPos.x) - 25, (rectSize.y + WindowPos.y) - 50),
		false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
	{
		// Draw the rectangle at the calculated position with rounded corners
		drawList->AddRectFilled(WindowPos, ImVec2(WindowPos.x + rectSize.x, WindowPos.y + rectSize.y),
			ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f, ImDrawFlags_RoundCornersTopLeft);
		{
			ImGui::SetCursorPosX(WindowPos.x + 100);
			ImGui::PushFont(Globals::fontLoader->GetFont("NSBold"));
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
					ImGui::PushFont(Globals::fontLoader->GetFont("NSRegSmall"));
					// Set the rounding radius for the button corners
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Adjust the value for more or less rounding
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
					ImGui::PopStyleVar();
				}
				ImGui::EndChild();
			}
			ImGui::SetCursorPosX(WindowPos.x);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.8f, 0.2f));

			// Set the rounding radius for the button corners
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Adjust the value for more or less rounding

			ImGui::PushFont(Globals::fontLoader->GetFont("NSRegSmall"));
			if (ImGui::Button("+ Create New Chapter", ImVec2(270, 40.0f)))
			{
				ImGuiIO& io = ImGui::GetIO();
				Chapters.push_back(new Chapter("Untitled Chapter"/*,io.MousePos*/));
				Globals::projectBrowser->GetActiveProject()->ProjectChapters = Chapters;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
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
				ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f, ImDrawFlags_RoundCornersBottomLeft);
			{
				ImGui::SetCursorPos(ImVec2(WindowPos.x + 50, ImGui::GetCursorPosY() - 5));
				ImGui::PushFont(Globals::fontLoader->GetFont("NSBold"));
				ImGui::Text("Chapter Properties");
				ImGui::PopFont();

				//Buffer Var for Input Text
				char temp[256];
				//Copy Chapter Name string into Buffer Var
				strncpy_s(temp, ActiveChapter->GetChapterName().c_str(), sizeof(temp));
				ImGui::PushFont(Globals::fontLoader->GetFont("NSRegSmall"));
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
				ImGui::Text("Name ");
				ImGui::SameLine();
				// Set the rounding radius for the button corners
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f); // Adjust the value for more or less rounding
				ImGui::InputTextWithHint("##Empty", temp, temp, IM_ARRAYSIZE(temp));
				ImGui::PopStyleVar();
				ImGui::PopFont();

				if (ImGui::IsItemFocused())
				{
					if (ImGui::IsKeyPressed(ImGuiKey_Enter))
					{
						ActiveChapter->SetChapterName(temp);
						Globals::projectBrowser->GetActiveProject()->ProjectChapters = Chapters;
					}
				}

				if (Chapters.size() > 1)
				{
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
					ImGui::PushFont(Globals::fontLoader->GetFont("NSRegSmall"));

					// Set the rounding radius for the button corners
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f); // Adjust the value for more or less rounding
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
								OpenChapters.push_back(Chapters[0]);
								ActiveChapter = OpenChapters[0]; // No chapters left, reset the active chapter
							}

							Globals::projectBrowser->GetActiveProject()->ProjectChapters = Chapters;

						}
					}
					ImGui::PopFont();
					ImGui::PopStyleVar();
				}
			}
			ImGui::EndChild();
		}
	}
}

void EventsUI::RenderNodeInspector()
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 ListPos = ImVec2(ImGui::GetWindowWidth() - 280, 10); // Set this to the current cursor position, which should be below the tab bar
	ImGui::SetCursorPos(ListPos); // Relative to the window's position
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
	if (ImGui::BeginChild("##NodePropInspector", ImVec2(290, ImGui::GetWindowHeight() - 10),
		false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
	{
		// Calculate the size of the rectangle
		ImVec2 rectSize = ImVec2(285, Application::g_WindowHeight - 10);

		// Ensure the rectangle stays within the window's bounds
		rectSize.y = std::max(10.0f, rectSize.y);  // Ensure the height isn't negative or too small

		// Draw the rectangle at the calculated position with rounded corners
		drawList->AddRectFilled(ListPos, ImVec2(ListPos.x + rectSize.x, (ListPos.y + rectSize.y) - 40),
			ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f, ImDrawFlags_RoundCornersBottomRight | ImDrawFlags_RoundCornersTopRight);

		if (ActiveChapter != nullptr)
		{
			if (ActiveChapter->GetActiveNode() != nullptr)
			{
				//Get half the Rect Size then use a mod float to fine tune X Axis placement of Text
				// Y Axis Seems alright where it is!!
				ImGui::SetCursorPosX(rectSize.x - (rectSize.x / 2) - 100);
				ImGui::PushFont(Globals::fontLoader->GetFont("NSBold"));
				ImGui::Text("Node Components");
				ImGui::PopFont();

				ImGui::SetCursorPosX(ImGui::GetCursorPosX());
				if (ImGui::BeginChild("##Components",
					ImVec2(ImGui::GetContentRegionAvail().x - 25, ImGui::GetContentRegionAvail().y - 80)
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
					ImGui::Separator();
					// Set the rounding radius for the button corners
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f); // Adjust the value for more or less rounding
					if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 25)))
					{
						openComponentsList = !openComponentsList;
					}
					ImGui::PopStyleVar();
					if (openComponentsList)
					{
						ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.04, 0.04, 0.04, 1.0f));
						if (ImGui::BeginChild("##ComponentsList",
							ImVec2(ImGui::GetContentRegionAvail().x - 5, 300)
							,ImGuiChildFlags_Border, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar))
						{

							char temp[256] = "";

							if (ImGui::BeginMenuBar())
							{
								ImGui::SetCursorPosX(50);
								ImGui::InputTextWithHint("##Temp", "Search Components...", temp, sizeof(temp));
								ImGui::EndMenuBar();
							}
							ImGui::PopStyleColor();
							ImGui::EndChild();
						}
					}

					ImGui::EndChild();
				}
			}

			/*ImGui::EndChild();
			ImGui::PopStyleColor();*/
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
}

void EventsUI::RenderPlayModeToolBar() 
{

	ImGui::SetCursorPos(ImVec2(0, 0));
	ImDrawList* drawList = ImGui::GetWindowDrawList();  // Use the existing drawList pointer
	ImVec2 TBarSize(1294.0, 45);
	ImVec2 TBarPos(326, 20.0f);
	ImGui::SetNextWindowPos(TBarPos);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
	if (ImGui::BeginChild("##TopBar", TBarSize, false, ImGuiWindowFlags_NoTitleBar))
	{
	//	// Draw the button with rounded corners
	//	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		// Save the current color settings
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Set the button color to green
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); // Set the hovered color to light green
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Set the active color to dark green

		PlayTexture.bind(0);
		ImTextureID texture = reinterpret_cast<ImTextureID>(PlayTexture.getHandle());
		/*ImGui::SetCursorPos(ImVec2(10.0f,11.5f));*/
		if (ImGui::ImageButton("##Play", texture, ImVec2(38, 38)))
		{
		}
		//ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleColor();
		ImGui::EndChild();
	}
}

void EventsUI::RenderConsoleLog()
{
	ImGui::SetCursorPos(ImVec2(0,0));
	ImDrawList* drawList = ImGui::GetWindowDrawList();  // Use the existing drawList pointer
	ImVec2 TBarSize(1295.0, 190);
	ImVec2 TBarPos(325, 850.0f);
	ImGui::SetNextWindowPos(TBarPos);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
	if (ImGui::BeginChild("##Toolbar", TBarSize, false,ImGuiWindowFlags_NoTitleBar))
	{

		ImGui::PopStyleColor();
		ImVec2 ChildSize(TBarSize.x, 20);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.03f, 0.03f, 0.03f, 1.0f));
		if (ImGui::BeginChild("##Header", ChildSize, false, ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::PushFont(Globals::fontLoader->GetFont("NSRegSmall"));
			ImGui::SetCursorPosX(10.0f);
			ImGui::Text("Console");
			ImGui::SameLine();

			ImGui::PopFont();
			ImGui::PopStyleColor();
			ImGui::EndChild();
		}
		ImVec2 BodySize(TBarSize.x, 170);
		ImGui::SetNextWindowPos(ImVec2(TBarPos.x, TBarPos.y + 20));
		if (ImGui::BeginChild("##Body", BodySize, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::SetCursorPos(ImVec2(20, 5));
			ImGui::PushFont(Globals::fontLoader->GetFont("NSRegSmall"));
			ImGui::Text("[Sys] : Welcome To The Adventure Engine v 0.1");
			ImGui::PopFont();



			ImGui::EndChild();
		}

		ImGui::EndChild();

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