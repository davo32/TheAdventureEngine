#include "MenuBar.h"
#include "imgui.h"
#include "Application.h"


void MenuBar::FileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Project"))
		{

		}
		if (ImGui::MenuItem("Open Project"))
		{

		}
		if (ImGui::MenuItem("Save"))
		{

		}
		if (ImGui::MenuItem("Save As"))
		{

		}
		if (ImGui::MenuItem("Quit"))
		{
			ImGui::OpenPopup("Exit", ImGuiPopupFlags_None);
			
		}

		if (ImGui::BeginPopup("Exit"))
		{
			ImGui::Text("Do You Want To Quit?");
			if (ImGui::Button("No", ImVec2(40, 20)))
			{
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Yes", ImVec2(40, 20)))
			{
				Application::SetShutdownState(true);
			}
			ImGui::EndPopup();
		}

		ImGui::EndMenu();
	}
}

void MenuBar::PlayerMenu()
{
	if (ImGui::Button("Player"))
	{

	}
	
}

void MenuBar::InventoryMenu()
{
	if (ImGui::Button("Inventory"))
	{

	}
}

void MenuBar::EventsMenu()
{
	if (ImGui::Button("Events"))
	{

	}
}

void MenuBar::ItemMenu()
{
	if (ImGui::Button("Items"))
	{

	}
}

void MenuBar::DrawUI()
{
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.2, 0.0));
	if (ImGui::BeginMenuBar())
	{
		FileMenu();
		PlayerMenu();
		InventoryMenu();
		EventsMenu();
		ItemMenu();
		ImGui::EndMenuBar();
	}
	ImGui::PopStyleColor();
}
