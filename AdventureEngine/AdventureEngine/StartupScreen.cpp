#include "StartupScreen.h"
#include "MonitorInfo.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include <iostream>

#include "JSONHandler.h"

#include "FileExtensions.h"

void StartupScreen::DrawUI()
{
    ImVec2 windowSize(Application::g_WindowWidth - 15, Application::g_WindowHeight - 40);
    ImVec2 WindowPos(15, 30);

    SetupMainWindow(windowSize, WindowPos);

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    if (ImGui::BeginChild("##InvisibleChild", windowSize, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
    {
        DrawBackground(drawList, WindowPos, windowSize);

        ImVec2 startOfArea = ImVec2(((windowSize.x / 3) - 300) + 10, WindowPos.y + 10);
		DrawCentreContent(drawList, WindowPos, startOfArea);

        DrawLeftPanel(drawList, startOfArea, WindowPos, windowSize);
        DrawHeader(drawList, WindowPos, windowSize);

        if (projectBrowser->GetActiveProject() != nullptr)
        {
            DrawRightPanel(drawList, WindowPos, windowSize);
        }

        ImGui::EndChild();
    }
}

void StartupScreen::SetupMainWindow(const ImVec2& windowSize, const ImVec2& WindowPos)
{
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetCursorPos(WindowPos);
    Application::fontLoader.SetFont("NSBold");
}

void StartupScreen::DrawBackground(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize)
{
    drawList->AddRectFilled(WindowPos, windowSize, ImColor(0.1f, 0.1f, 0.1f, 1.0f), 10.0f);
}

void StartupScreen::DrawHeader(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize)
{
    Application::fontLoader.DrawText(drawList, ImVec2(WindowPos.x + 80, WindowPos.y + 10), IM_COL32(255, 255, 255, 255), "Adventure Engine");
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

    ImGui::SetCursorPos(ImVec2(WindowPos.x - 15, WindowPos.y + 30));
    ImGui::PushFont(Application::fontLoader.GetFont("NSReg"));
	

    if (ImGui::Button("Open Project", ImVec2(LeftPanelSize.x - 15, 50)))
    {
        Application::SetTitleText("Adventure Engine - " + projectBrowser->GetActiveProject()->name);
        Application::UICounter = 2;
    }
    if (ImGui::Button("Documentation", ImVec2(LeftPanelSize.x - 15, 50)))
    {
        // Button action goes here
    }

    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
}

void StartupScreen::DrawCreateProjectButton(ImVec2 WindowPos,ImVec2 LeftPanelSize)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f); // Set rounding radius
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.1, 0.1, 0.8f)); // Invisible background
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.4f)); // Background color on hover
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); // Background color when clicked
	ImGui::PushFont(Application::fontLoader.GetFont("NSReg"));

	ImGui::SetCursorPos(ImVec2(WindowPos.x, LeftPanelSize.y - 150));
	if (ImGui::Button("+ Create Project", ImVec2(LeftPanelSize.x - 45, 50)))
	{
		std::string newFileName = "Untitled Project" + FileExtensions::MasterExt;
		std::string newProjectFolder = "Untitled Project";
		projectBrowser->CreateNewProject(newProjectFolder, newFileName);
		projectBrowser->ReloadProjects();
	}

	ImGui::PopStyleColor(3); // Pop the 3 style colors we pushed
	ImGui::PopStyleVar();
	ImGui::PopFont();

}

void StartupScreen::DrawCentreContent(ImDrawList* drawList,ImVec2 WindowPos,ImVec2 startOfArea)
{
	Application::fontLoader.SetFont("NSBold");

	drawList->AddLine(ImVec2(WindowPos.x, WindowPos.y + 50), ImVec2(ImGui::GetContentRegionAvail().x, WindowPos.y + 50), ImColor(0.08f, 0.08f, 0.08f, 1.0f), 0.05f);

	projectBrowser->LoadDefaultIcon();

	ImGui::SetCursorPos(ImVec2(startOfArea.x + 20, WindowPos.y + 80));
	ImGui::PushFont(Application::fontLoader.GetFont("NSReg"));
	projectBrowser->Render();
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
    Application::fontLoader.DrawText(drawList, ImVec2(RightPanelPos.x + 80, RightPanelPos.y + 70), IM_COL32(255, 255, 255, 255), "Project Summary");
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

			ImGui::PushFont(Application::fontLoader.GetFont("NSBold"));


			char temp[256];
			if (projectBrowser->GetActiveProject() != nullptr)
			{
				std::string name = projectBrowser->GetActiveProject()->name;
				strcpy_s(temp, name.c_str());

				ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 60, RightPanelPos.y - 25));

				ImGui::InputText("##Name", temp, IM_ARRAYSIZE(temp));
			}


			// Check if the InputText widget lost focus or Enter key is pressed
			if (ImGui::IsItemDeactivatedAfterEdit())
			{
				std::string oldName = projectBrowser->GetActiveProject()->name + FileExtensions::MasterExt;
				std::string newName = temp + FileExtensions::MasterExt;

				if (oldName != newName) // Ensure the name has actually changed
				{
					projectBrowser->RenameProject(projectBrowser->GetActiveProject()->name, oldName, newName, temp);
					//projectBrowser->DeSelectProject();
					projectBrowser->ReloadProjects();
				}
			}


			ImGui::PopFont();
			ImGui::PopStyleColor(2); // Restore style colors

			//Application::fontLoader.DrawText(drawList, ImVec2(RightPanelPos.x + 80, RightPanelPos.y + 10), IM_COL32(255, 255, 255, 255), projectBrowser->GetActiveProject()->name);
			drawList->AddLine(ImVec2(RightPanelPos.x, RightPanelPos.y + 50), ImVec2(RightPanelSize.x, RightPanelPos.y + 50), ImColor(0.2f, 0.2f, 0.2f, 0.5f), 0.08f);

			std::string removedExtensionPath;
			std::string text;
			if (projectBrowser->GetActiveProject() != nullptr)
			{
				//Grabs the File + Extension then removes the File + extension from Path
					//To access Chapter Count.
				std::string file = projectBrowser->GetActiveProject()->name + FileExtensions::MasterExt;

				// Get the path with .Master removed
				removedExtensionPath = projectBrowser->RemoveExtension(projectBrowser->GetActiveProject()->path, file);

				// Count chapter files
				int chapterCount = projectBrowser->CountChapterFiles(removedExtensionPath + "\\" + FileExtensions::ChapterFolder);

				// Convert chapter count to string (simplified)
				std::string chapterCountStr = std::to_string(chapterCount);

				// Create the full text string
				text = "Chapters: " + chapterCountStr;
			}
			else
			{
				removedExtensionPath = "\0";
				text = "\0";
			}

			Application::fontLoader.SetFont("NSReg");
			Application::fontLoader.DrawText(drawList, ImVec2(RightPanelPos.x + 80, RightPanelPos.y + 70), IM_COL32(255, 255, 255, 255), "Project Summary");
			Application::fontLoader.DrawText(drawList, ImVec2(RightPanelPos.x + 80, RightPanelPos.y + 270), IM_COL32(255, 255, 255, 255), text.c_str());

			ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 10, RightPanelPos.y + 90));


			if (projectBrowser->GetActiveProject() != nullptr)
			{
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Invisible background
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));  // Invisible border
				ImGui::PushFont(Application::fontLoader.GetFont("NSRegSmall"));
				ImGui::InputTextMultiline("##Summary", projectBrowser->GetActiveProject()->Summary, IM_ARRAYSIZE(projectBrowser->GetActiveProject()->Summary), ImVec2(250, 120));
				ImGui::PopFont();
				// Check if the item is hovered
				if (ImGui::IsItemHovered())
				{
					ImDrawList* drawList = ImGui::GetWindowDrawList();
					ImVec2 min = ImGui::GetItemRectMin();
					ImVec2 max = ImGui::GetItemRectMax();
					ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Background color when hovered
					drawList->AddRectFilled(min, max, ImColor(bgColor));
				}

				//JSON DATA HANDLING
				{
					json Data =
					{
						{"Project Name",projectBrowser->GetActiveProject()->name},
						{"Project Summary",projectBrowser->GetActiveProject()->Summary},
						{"Chapter Count",projectBrowser->CountChapterFiles
						(removedExtensionPath + "/" + FileExtensions::ChapterFolder)}
					};

					JSONHandler::saveToFile(projectBrowser->GetActiveProject()->path, Data);
				}

				ImGui::PopStyleColor(2); // Restore style colors
			}

			// Draw additional elements like buttons at the bottom
			DrawRightPanelButtons(RightPanelPos, RightPanelSize);
		}
	}
}

void StartupScreen::DrawRightPanelButtons(const ImVec2& RightPanelPos, const ImVec2& RightPanelSize)
{
    ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 20, RightPanelSize.y - 125));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.0, 0.0, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0, 0, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0, 0, 0.3f));
    ImGui::PushFont(Application::fontLoader.GetFont("NSReg"));

    if (ImGui::Button("Delete Project", ImVec2(250, 50)))
    {
        // Delete project logic
    }

    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
}
