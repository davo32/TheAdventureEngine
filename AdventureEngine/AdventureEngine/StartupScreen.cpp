#include "StartupScreen.h"
#include "MonitorInfo.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include "Globals.h"
//#include "JSONHandler.h"
#include <iostream>

void StartupScreen::DrawUI()
{
	ImVec2 windowSize(Application::g_WindowWidth - 15, Application::g_WindowHeight - 40);
	ImVec2 WindowPos(15, 30);

	////SetupMainWindow(windowSize, WindowPos);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	//ImGui::SetNextWindowPos(ImVec2(0, 0));
	//ImGui::SetNextWindowSize(ImVec2(Application::g_WindowWidth, Application::g_WindowHeight));

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08, 0.08, 0.08, 1.0f));
	if (ImGui::Begin("##InvisibleChild", nullptr, /*ImGuiWindowFlags_NoBackground |*/ ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.16f, 1.00f));
		if (ImGui::BeginChild("##LeftPanel", ImVec2(200, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::SetCursorPos(ImVec2(20, 20));
			ImGui::PushFont(Globals::fontLoader->GetFont("NSBold"));
			ImGui::Text("Adventure Engine");
			ImGui::PopFont();
			DrawLeftPanelButtons(ImVec2(0, 60), WindowPos, ImVec2(200, ImGui::GetContentRegionAvail().y));
			ImGui::PopStyleColor();
			ImGui::EndChild();
		}

		//DrawBackground(drawList, WindowPos, windowSize);
		ImGui::SetCursorPos(ImVec2(200, 0));
		if (ImGui::BeginChild("##ContentHeader", ImVec2(ImGui::GetContentRegionAvail().x, 80), false, ImGuiWindowFlags_NoTitleBar))
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImGui::SetCursorPos(ImVec2(30, 30));
			/*ImGui::PushFont(Globals::fontLoader->GetFont("NSBold"));
			ImGui::Text("Projects");
			ImGui::PopFont();*/
			ImGui::SetNextItemWidth(200.0f);
			ImGui::PushFont(Globals::fontLoader->GetFont("NSReg"));
			if (ImGui::InputTextWithHint("##CreateProjectText", "Create Project...", newProjectName, IM_ARRAYSIZE(newProjectName), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (newProjectName != "\0")
				{
					std::string newFileName = newProjectName;
					std::string newProjectFolder = newProjectName;
					projectBrowser->CreateNewProject(newProjectFolder, newFileName);
					projectBrowser->ReloadProjects();

					// Clear the input field properly
					memset(newProjectName, 0, IM_ARRAYSIZE(newProjectName));
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("+", ImVec2(30, 30)))
			{
				if (newProjectName != "\0")
				{
					std::string newFileName = newProjectName;
					std::string newProjectFolder = newProjectName;
					projectBrowser->CreateNewProject(newProjectFolder, newFileName);
					projectBrowser->ReloadProjects();

					// Clear the input field properly
					memset(newProjectName, 0, IM_ARRAYSIZE(newProjectName));
				}
			}
			ImGui::PopFont();
			drawList->AddLine(ImVec2(0, 79), ImVec2(ImGui::GetContentRegionAvail().x + 200, 79), ImColor(0.16f, 0.16f, 0.16f, 1.0f), 1.0f);
			ImGui::EndChild();
		}
		ImGui::SetCursorPos(ImVec2(200, 90));
		if (ImGui::BeginChild("##CenterPanel", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_NoTitleBar))
		{
			ImVec2 startOfArea = ImVec2(((windowSize.x / 3) - 300) + 10, WindowPos.y + 10);
			DrawCentreContent(drawList, WindowPos, startOfArea);
			ImGui::EndChild();
		}

		ImGui::End();
	}
}

void StartupScreen::Render()
{
	// Define parameters for the list-style buttons
	const int columns = 1; // Single column for the list layout
	const float buttonHeight = 50.0f; // Height of each button
	const float spacing = 5.0f; // Spacing between buttons

	// Calculate window size and start position
	ImVec2 windowPos = ImGui::GetCursorPos();
	float startX = windowPos.x + 10;
	float startY = windowPos.y;

	// Calculate button width based on window size (or set a fixed width)
	float buttonWidth = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2;

	// Iterate over projects to create buttons
	for (size_t i = 0; i < Globals::projectBrowser->projects.size(); ++i)
	{
		// Calculate button position based on index
		int row = static_cast<int>(i / columns);
		ImVec2 buttonPos = ImVec2(startX, startY + row * (buttonHeight + spacing));

		// Set cursor position for the button
		ImGui::SetCursorPos(buttonPos);

		// Create an invisible button to hold the background and the text separately
		if (ImGui::InvisibleButton("button_background", ImVec2(buttonWidth, buttonHeight)))
		{
			Globals::projectBrowser->ActiveProject = Globals::projectBrowser->projects[i];
			Application::cScreen = CurrentScreen::EDITOR;
			Application::SetTitleText("Adventure Engine - " + Globals::projectBrowser->GetActiveProject()->name);
			Application::UICounter = 2; // Safely change the state after ImGui processing is complete
		}

		// Check if the button is hovered or active
		bool isHovered = ImGui::IsItemHovered();
		bool isActive = (Globals::projectBrowser->ActiveProject != nullptr && strcmp(Globals::projectBrowser->ActiveProject->name.c_str(), Globals::projectBrowser->projects[i]->name.c_str()) == 0);

		// Draw a background for the button with different colors based on the hover state
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();

		// Default background color
		ImU32 bgColor = IM_COL32(0, 0, 0, 0); // Dark color

		// Lighter background color on hover
		if (isHovered)
		{
			bgColor = IM_COL32(50, 50, 50, 110); // Lighter color when hovered
		}

		// Even lighter color if active
		if (isActive)
		{
			bgColor = IM_COL32(80, 80, 80, 110); // Highlight the active project
		}

		// Draw the background rectangle with rounded corners
		drawList->AddRectFilled(min, max, bgColor, 8.0f); // 8.0f for rounded corners

		// Manually set the text's cursor position relative to the button
		ImGui::SetCursorScreenPos(ImVec2(min.x + ImGui::GetStyle().ItemSpacing.x, min.y + (buttonHeight - ImGui::CalcTextSize(Globals::projectBrowser->projects[i]->name.c_str()).y) / 2));

		// Draw the project name inside the button
		ImGui::TextUnformatted(Globals::projectBrowser->projects[i]->name.c_str());
	}
}

void StartupScreen::SetupMainWindow(const ImVec2& windowSize, const ImVec2& WindowPos)
{
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::SetCursorPos(WindowPos);
	Globals::fontLoader->SetFont("NSBold");
}

void StartupScreen::DrawBackground(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize)
{
	drawList->AddRectFilled(WindowPos, windowSize, ImColor(0.1f, 0.1f, 0.1f, 1.0f), 10.0f);
}

void StartupScreen::DrawHeader(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize)
{
	Globals::fontLoader->DrawText(drawList, ImVec2(WindowPos.x + 80, WindowPos.y + 10), IM_COL32(255, 255, 255, 255), "Adventure Engine");
	drawList->AddLine(ImVec2(WindowPos.x, WindowPos.y + 50), ImVec2((windowSize.x / 3) - 300, WindowPos.y + 50), ImColor(0.2f, 0.2f, 0.2f, 0.5f), 0.08f);
}

void StartupScreen::DrawLeftPanel(ImDrawList* drawList, const ImVec2& startOfArea, const ImVec2& WindowPos, const ImVec2& windowSize)
{
	ImVec2 LeftPanelSize((windowSize.x / 3) - 300, windowSize.y);

	drawList->AddRectFilled(WindowPos, ImVec2(LeftPanelSize), ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f);
	DrawLeftPanelButtons(startOfArea, WindowPos, LeftPanelSize);
	DrawCreateProjectButton(WindowPos, LeftPanelSize);
}

void StartupScreen::DrawLeftPanelButtons(const ImVec2& startOfArea, const ImVec2& WindowPos, const ImVec2& LeftPanelSize)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

	ImGui::SetCursorPos(ImVec2(WindowPos.x, WindowPos.y + 60));
	ImGui::PushFont(Globals::fontLoader->GetFont("NSReg"));

	ImGui::SetCursorPosX(WindowPos.x);
	if (ImGui::Button("Documentation", ImVec2(LeftPanelSize.x - 30, 50)))
	{
		// Button action goes here
	}
	ImGui::SetCursorPosX(WindowPos.x);
	if (ImGui::Button("Exit", ImVec2(LeftPanelSize.x - 30, 50)))
	{
		std::exit(0);
	}

	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();
}

void StartupScreen::DrawCreateProjectButton(ImVec2 WindowPos, ImVec2 LeftPanelSize)
{
	//ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f); // Set rounding radius
	//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.1, 0.1, 0.8f)); // Invisible background
	//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.4f)); // Background color on hover
	//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); // Background color when clicked
	//ImGui::PushFont(Globals::fontLoader->GetFont("NSReg"));

	//ImGui::SetCursorPos(ImVec2(WindowPos.x + 15, LeftPanelSize.y - 150));

	//ImGui::PopStyleColor(3); // Pop the 3 style colors we pushed
	//ImGui::PopStyleVar();
	//ImGui::PopFont();
}

void StartupScreen::DrawCentreContent(ImDrawList* drawList, ImVec2 WindowPos, ImVec2 startOfArea)
{
	Globals::fontLoader->SetFont("NSBold");

	drawList->AddLine(ImVec2(WindowPos.x, WindowPos.y + 50), ImVec2(ImGui::GetContentRegionAvail().x, WindowPos.y + 50), ImColor(0.08f, 0.08f, 0.08f, 1.0f), 0.05f);
	projectBrowser->LoadDefaultIcon();

	//ImGui::SetCursorPos(ImVec2(startOfArea.x + 20, WindowPos.y + 80));
	ImGui::PushFont(Globals::fontLoader->GetFont("NSReg"));
	Render();
	ImGui::PopFont();
}

void StartupScreen::DrawRightPanel(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize)
{
	ImVec2 RightPanelSize(windowSize.x, windowSize.y);
	ImVec2 RightPanelPos(ImGui::GetContentRegionAvail().x - 300, WindowPos.y);

	drawList->AddRectFilled(RightPanelPos, ImVec2(RightPanelSize), ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f);

	DrawRightPanelHeader(drawList, RightPanelPos);
	DrawRightPanelContent(drawList, RightPanelPos, RightPanelSize);
}

void StartupScreen::DrawRightPanelHeader(ImDrawList* drawList, const ImVec2& RightPanelPos)
{
	Globals::fontLoader->DrawText(drawList, ImVec2(RightPanelPos.x + 80, RightPanelPos.y + 70), IM_COL32(255, 255, 255, 255), "Project Summary");
}

void StartupScreen::DrawRightPanelContent(ImDrawList* drawList, const ImVec2& RightPanelPos, const ImVec2& RightPanelSize)
{
	// Add your logic here to display project summary, chapter count, and other details
	if (projectBrowser->GetActiveProject() != nullptr)
	{
		drawList->AddRectFilled(RightPanelPos, ImVec2(RightPanelSize), ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f);
		{
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Invisible background
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));  // Invisible border

			ImGui::PushFont(Globals::fontLoader->GetFont("NSBold"));

			std::string removedExtensionPath;
			std::string text;

			char temp[256];
			if (projectBrowser->GetActiveProject() != nullptr)
			{
				std::string name = projectBrowser->GetActiveProject()->name;
				strcpy_s(temp, name.c_str());
				float InputSize = 250;
				ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 10, RightPanelPos.y + 5));
				ImGui::SetNextItemWidth(InputSize);
				ImGui::InputText("##Name", temp, IM_ARRAYSIZE(temp));

				// Check if the InputText widget lost focus or Enter key is pressed
				if (!ImGui::IsItemActive() || ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					std::string oldName = projectBrowser->GetActiveProject()->name + Globals::FileExts->MasterExt;
					std::string newName = temp + Globals::FileExts->MasterExt;

					if (oldName != newName) // Ensure the name has actually changed
					{
						// Update the active project's name before saving data
						std::string baseName = projectBrowser->RemoveExtension(projectBrowser->GetActiveProject()->name, Globals::FileExts->MasterExt);
						projectBrowser->GetActiveProject()->name = baseName; //+ FileExtensions::MasterExt;

						projectBrowser->RenameProject(baseName, oldName, newName, temp);

						QueueProjectForSaving(projectBrowser->GetActiveProject());

						// Reload projects to reflect changes
						projectBrowser->ReloadProjects();
					}
				}
			}

			// Handle drawing styles and fonts
			ImGui::PopFont();
			ImGui::PopStyleColor(2); // Restore style colors

			// Draw the line separator
			drawList->AddLine(ImVec2(RightPanelPos.x, RightPanelPos.y + 50), ImVec2(RightPanelSize.x, RightPanelPos.y + 50), ImColor(0.2f, 0.2f, 0.2f, 0.5f), 0.08f);

			// Draw the project summary panel
			ImU32 rectColor = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 0.8f));
			ImVec2 rectPos = ImVec2(RightPanelPos.x + 10, RightPanelPos.y + 60);
			ImVec2 rectSize = ImVec2(280, 220);
			drawList->AddRectFilled(rectPos, ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y), rectColor, 6.0f);

			Globals::fontLoader->SetFont("NSReg");
			Globals::fontLoader->DrawText(drawList, ImVec2(RightPanelPos.x + 80, RightPanelPos.y + 70), IM_COL32(255, 255, 255, 255), "Project Summary");
			Globals::fontLoader->DrawText(drawList, ImVec2(RightPanelPos.x + 80, RightPanelPos.y + 270), IM_COL32(255, 255, 255, 255), text.c_str());

			// Draw chapter count
			if (projectBrowser->GetActiveProject() != nullptr)
			{
				ImVec2 ChpterRectPos = ImVec2(RightPanelPos.x + 10, RightPanelPos.y + 290);
				ImVec2 ChpterRectSize = ImVec2(140, 40);
				drawList->AddRectFilled(ChpterRectPos, ImVec2(ChpterRectPos.x + ChpterRectSize.x, ChpterRectPos.y + ChpterRectSize.y), rectColor, 6.0f);

				// Count chapter files
				int chapterCount = projectBrowser->CountChapterFiles();

				// Convert chapter count to string
				std::string chapterCountStr = std::to_string(chapterCount);

				// Create the full text string
				text = "Chapters: " + chapterCountStr;

				ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 20, RightPanelPos.y + 295));
				ImGui::PushFont(Globals::fontLoader->GetFont("NSReg"));
				ImGui::Text(text.c_str());
				ImGui::PopFont();
			}
			else
			{
				removedExtensionPath = "\0";
				text = "\0";
			}

			// Draw project summary input
			ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 20, RightPanelPos.y + 120));
			if (projectBrowser->GetActiveProject() != nullptr)
			{
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Invisible background
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));  // Invisible border
				ImGui::PushFont(Globals::fontLoader->GetFont("NSRegSmall"));
				ImGui::InputTextMultiline("##Summary", projectBrowser->GetActiveProject()->Summary, IM_ARRAYSIZE(projectBrowser->GetActiveProject()->Summary), ImVec2(250, 120));
				ImGui::PopFont();

				// Check if the item is hovered
				if (ImGui::IsItemHovered())
				{
					ImDrawList* drawList = ImGui::GetWindowDrawList();
					ImVec2 min = ImGui::GetItemRectMin();
					ImVec2 max = ImGui::GetItemRectMax();
					ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 0.6f); // Background color when hovered
					drawList->AddRectFilled(min, max, ImColor(bgColor), 6.0f);
				}

				QueueProjectForSaving(projectBrowser->GetActiveProject());

				ImGui::PopStyleColor(2); // Restore style colors
			}

			// Draw additional elements like buttons at the bottom
			DrawRightPanelButtons(drawList, RightPanelPos, RightPanelSize);
		}
	}
}

void StartupScreen::DrawRightPanelButtons(ImDrawList* drawList, const ImVec2& RightPanelPos, const ImVec2& RightPanelSize)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
	ImGui::PushFont(Globals::fontLoader->GetFont("NSReg"));

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.5, 0, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.8, 0, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.4, 0, 0.3f));
	ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 20, RightPanelSize.y - 245));
	if (ImGui::Button("Open Project", ImVec2(250, 50)))
	{
		if (projectBrowser->GetActiveProject() != nullptr)
		{
			Application::cScreen = CurrentScreen::EDITOR;
			Application::SetTitleText("Adventure Engine - " + projectBrowser->GetActiveProject()->name);
			openProjectFlag = true; // Set the flag instead of changing UICounter here
		}
	}
	ImGui::PopStyleColor(3);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.2, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3, 0.3, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1, 0.1, 0.6f));
	ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 20, RightPanelSize.y - 185));
	if (ImGui::Button("Close Settings", ImVec2(250, 50)))
	{
		projectBrowser->DeSelectProject();
	}
	ImGui::PopStyleColor(3);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.0, 0.0, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0, 0, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0, 0, 0.3f));

	ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 20, RightPanelSize.y - 125));
	if (ImGui::Button("Delete Project", ImVec2(250, 50)))
	{
		//May want to add a pop up confirming the action
		//since this is a un-reversable action.
		DeletionConfirmation = true;
	}

	if (DeletionConfirmation)
	{
		ImVec2 buttonPos = ImGui::GetCursorScreenPos();
		ImVec2 buttonSize = ImGui::GetItemRectSize();
		DrawDeletionConfirmation(drawList, ImVec2(RightPanelPos.x - buttonSize.x - 20, RightPanelSize.y - 210), ImVec2(buttonSize.x, 200));
	}

	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	if (openProjectFlag)
	{
		Application::UICounter = 2; // Safely change the state after ImGui processing is complete
	}
}

void StartupScreen::DrawDeletionConfirmation(ImDrawList* drawList, const ImVec2& Pos, const ImVec2& size)
{
	// Draw the background rectangle
	ImU32 rectColor = ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.05f, 0.8f));
	drawList->AddRectFilled(Pos, ImVec2(Pos.x + size.x, Pos.y + size.y), rectColor, 6.0f);

	// Set the cursor position for the dialog content
	ImGui::SetCursorScreenPos(ImVec2(Pos.x + 60, Pos.y + 20));

	// Add the confirmation text
	ImGui::Text("Delete this Project?");

	// Space between text and buttons
	ImGui::SetCursorScreenPos(ImVec2(Pos.x + 20, Pos.y + size.y - 70));

	// Create the Delete button with the same color as the Delete Project button
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.0f, 0.0f, 0.3f));
	if (ImGui::Button("Delete", ImVec2(100, 40)))
	{
		// Logic for deleting the project
		projectBrowser->DeleteProject(projectBrowser->GetActiveProject()->name);
		DeletionConfirmation = false;
	}
	ImGui::PopStyleColor(3);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 0.3f));
	// Create the Cancel button
	if (ImGui::Button("Cancel", ImVec2(100, 40)))
	{
		// Logic to close the confirmation dialog
		DeletionConfirmation = false;
	}
	ImGui::PopStyleColor(3);
}

bool StartupScreen::SavePrelimDataInBulk()
{
	if (projectBrowser->projectsToSave.empty())
	{
		std::cout << "No Files To save..." << '\n';
	}

	for (Project* P : projectBrowser->projectsToSave)
	{
		// Create JSON data
		json Data = {
			{"Prelim Data",
				{
					{"Project Name", P->name},
					{"Project Summary", P->Summary}
				}}
		};

		// Debug output
		std::cout << "Saving JSON Data: " << Data.dump(4) << std::endl;
		std::cout << "Saving to file: " << P->path << std::endl;

		// Save the updated JSON data to file using the new path
		if (!JSONHandler::saveToFile(P->path, Data))
		{
			std::cout << "Failed to save file: " << P->path << std::endl;
			return false;
		}
		return true;
	}
	return false;
}

void StartupScreen::QueueProjectForSaving(Project* project)
{
	projectBrowser->projectsToSave.push_back(project);
}