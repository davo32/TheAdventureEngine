#include "MenuBar.h"
#include "imgui.h"
#include "Application.h"


void MenuBar::FileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New","CRTL + N",false,true))
		{

		}
		if (ImGui::MenuItem("Open","CTRL + O",false,true))
		{
			Application::SetTitleText("Adventure Engine - No Project");
			Application::UICounter = 1;
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Save","CRTL + S",false,true))
		{
			
		}
		if (ImGui::MenuItem("Save As"))
		{

		}
		ImGui::Separator();
		if (ImGui::MenuItem("Quit","CRTL + Q",false,true))
		{
			Application::SetShutdownState(true);
			
		}
		ImGui::EndMenu();
	}
}

void MenuBar::ProjectMenu()
{
	if (ImGui::BeginMenu("Project"))
	{
		if (ImGui::MenuItem("Export"))
		{

		}
		if (ImGui::MenuItem("Import"))
		{

		}
		ImGui::Separator();
		if (ImGui::MenuItem("Settings"))
		{

		}
		ImGui::EndMenu();
	}
}



void MenuBar::Shortcuts()
{
	// Handle shortcuts
	if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_N))
	{
		// Trigger New Project action
		Application::SetTitleText("Adventure Engine - New Project(Unsaved)");
	}

	if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_O))
	{
		// Trigger Open action
		Application::SetTitleText("Adventure Engine - No Project");
		Application::UICounter = 1;
	}

	if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S))
	{
		// Trigger Save action
	}

	if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_Q))
	{
		// Trigger Quit action
		Application::SetShutdownState(true);
	}
}

void MenuBar::DrawUI()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.2, 0.0));
	Shortcuts();
	if (ImGui::BeginMenuBar())
	{
		FileMenu();
		ProjectMenu();
		ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x - 150,ImGui::GetContentRegionAvail().y));
		std::string temp = std::to_string(ImGui::GetMousePos().x) + " : " + std::to_string(ImGui::GetMousePos().y);
		ImGui::Text(temp.c_str());
		ImGui::EndMenuBar();
	}
	ImGui::PopStyleColor();
}
