#include "StartupScreen.h"
#include "MonitorInfo.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include <iostream>

#include "JSONHandler.h"

void StartupScreen::DrawUI()
{
	ImVec2 windowSize(Application::g_WindowWidth - 15, Application::g_WindowHeight - 40);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImVec2 WindowPos(15, 30);
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	
	ImGui::SetCursorPos(WindowPos);
	Application::fontLoader.SetFont("NSBold");
	if (ImGui::BeginChild("##InvisibleChild", windowSize, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
	{

		drawList->AddRectFilled(WindowPos, windowSize, ImColor(0.1f, 0.1f, 0.1f, 1.0f), 10.0f);
		{
			
			ImVec2 startOfArea = ImVec2(((windowSize.x / 3) - 300) + 10, WindowPos.y + 10);

			
			drawList->AddLine(ImVec2(WindowPos.x, WindowPos.y + 50), ImVec2(ImGui::GetContentRegionAvail().x, WindowPos.y + 50), ImColor(0.08f, 0.08f, 0.08f, 1.0f), 0.05f);
			
			projectBrowser->LoadDefaultIcon();

			ImGui::SetCursorPos(ImVec2(startOfArea.x + 20, WindowPos.y + 80));
			ImGui::PushFont(Application::fontLoader.GetFont("NSReg"));
			projectBrowser->Render();
			ImGui::PopFont();

			ImVec2 LeftPanelSize((windowSize.x / 3) - 300, windowSize.y);
			ImVec2 RightPanelSize(windowSize.x, windowSize.y);
			ImVec2 RightPanelPos(ImGui::GetContentRegionAvail().x - 300, WindowPos.y);
			drawList->AddRectFilled(WindowPos, ImVec2(LeftPanelSize), ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f);
			{
				Application::fontLoader.DrawText(drawList, ImVec2(WindowPos.x + 80, WindowPos.y + 10), IM_COL32(255, 255, 255, 255), "Adventure Engine");
				drawList->AddLine(ImVec2(WindowPos.x, WindowPos.y + 50), ImVec2((windowSize.x / 3) - 300, WindowPos.y + 50), ImColor(0.2f, 0.2f, 0.2f, 0.5f), 0.08f);

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f); // Set rounding radius
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Invisible background
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.4f)); // Background color on hover
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Background color when clicked

				ImGui::SetCursorPos(ImVec2(WindowPos.x - 15, WindowPos.y + 30));
				ImGui::PushFont(Application::fontLoader.GetFont("NSReg"));
				// Draw the button
				if (ImGui::Button("Open Project", ImVec2(LeftPanelSize.x - 15, 50)))
				{
					// Button action goes here
					Application::SetTitleText("Adventure Engine - " + projectBrowser->GetActiveProject()->name);
					Application::UICounter = 2;
				}
				if (ImGui::Button("Documentation", ImVec2(LeftPanelSize.x - 15, 50)))
				{
					// Button action goes here
				}

				ImGui::PopStyleColor(3); // Pop the 3 style colors we pushed
				ImGui::PopStyleVar();

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f); // Set rounding radius
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.1, 0.1, 0.8f)); // Invisible background
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.4f)); // Background color on hover
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); // Background color when clicked

				ImGui::SetCursorPos(ImVec2(WindowPos.x, LeftPanelSize.y - 150));
				if (ImGui::Button("+ Create Project", ImVec2(LeftPanelSize.x - 45, 50)))
				{
					std::string newFileName = "Untitled Project" + std::string(".nova");
					projectBrowser->CreateNewFile(newFileName);
					projectBrowser->ReloadProjects();
				}

				ImGui::PopStyleColor(3); // Pop the 3 style colors we pushed
				ImGui::PopStyleVar();
				ImGui::PopFont();
				

			}

			if (projectBrowser->GetActiveProject() != nullptr)
			{
				drawList->AddRectFilled(RightPanelPos, ImVec2(RightPanelSize), ImColor(0.08f, 0.08f, 0.08f, 1.0f), 10.0f);
				{

					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Invisible background
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));  // Invisible border

					ImGui::PushFont(Application::fontLoader.GetFont("NSBold"));

					std::string name = projectBrowser->GetActiveProject()->name;
					char temp[256];
					strcpy_s(temp, name.c_str());

					ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 60, RightPanelPos.y - 25));

					ImGui::InputText("##Name", temp, IM_ARRAYSIZE(temp));

					// Check if the InputText widget lost focus or Enter key is pressed
					if (ImGui::IsItemDeactivatedAfterEdit())
					{
						std::string oldName = projectBrowser->GetActiveProject()->name + ".nova";
						std::string newName = temp + std::string(".nova");

						if (oldName != newName) // Ensure the name has actually changed
						{
							projectBrowser->RenameFileInSameFolder(projectBrowser->GetDirectoryPath(), oldName, newName);
							projectBrowser->ReloadProjects();
						}
					}


					ImGui::PopFont();
					ImGui::PopStyleColor(2); // Restore style colors

					//Application::fontLoader.DrawText(drawList, ImVec2(RightPanelPos.x + 80, RightPanelPos.y + 10), IM_COL32(255, 255, 255, 255), projectBrowser->GetActiveProject()->name);
					drawList->AddLine(ImVec2(RightPanelPos.x, RightPanelPos.y + 50), ImVec2(RightPanelSize.x, RightPanelPos.y + 50), ImColor(0.2f, 0.2f, 0.2f, 0.5f), 0.08f);

					Application::fontLoader.SetFont("NSReg");
					Application::fontLoader.DrawText(drawList, ImVec2(RightPanelPos.x + 80, RightPanelPos.y + 70), IM_COL32(255, 255, 255, 255), "Project Summary");

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
								{"Project Summary",projectBrowser->GetActiveProject()->Summary}
							};

							JSONHandler::saveToFile(projectBrowser->GetActiveProject()->path, Data);
						}

						ImGui::PopStyleColor(2); // Restore style colors
					}

				
					ImGui::SetCursorPos(ImVec2(RightPanelPos.x + 20, RightPanelSize.y - 125));

					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f); // Set rounding radius
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.0, 0.0, 0.5f)); // Invisible background
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0, 0, 0.3f)); // Background color on hover
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0, 0, 0.3f)); // Background color when clicked
					ImGui::PushFont(Application::fontLoader.GetFont("NSReg"));
					if (ImGui::Button("Delete Project",ImVec2(250,50)))
					{

					}
					ImGui::PopFont();
					ImGui::PopStyleColor(3); // Pop the 3 style colors we pushed
					ImGui::PopStyleVar();
					
				
				}
			}
			
		}
		//ImGui::SameLine();

		ImGui::EndChild();
	}
	//if (ImGui::BeginChild("Startup Screen", windowSize, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
	//{
	//    //    // Define sizes for the sections
	//    //    float mainSectionWidth = 310.0f;   // Width for Main Section
	//    //    float hotfixesWidth = 400.0f;      // Increased width for Hotfixes
	//    //    float documentationWidth = windowSize.x - mainSectionWidth - hotfixesWidth; // Remaining width for Documentation

	//    //    // Main Section on the left
	//    //    ImGui::BeginChild("Main Section", ImVec2(mainSectionWidth, 0), true);
	//    //    {
	//    //        int width, height, channels;
	//    //        if (Logo.load("../Resources/Images/Logo.png"))
	//    //        {
	//    //            Logo.bind(0);
	//    //        }
	//    //        else
	//    //        {
	//    //            std::cout << "Error: Loading Texture..." << std::endl;
	//    //        }

	//    //        // Get the texture ID and size
	//    //        ImTextureID textureID = (ImTextureID)Logo.getHandle();
	//    //        ImVec2 imageSize(Logo.getWidth(), Logo.getHeight());

	//    //        // Main section content
	//    //        ImGui::Spacing();

	//    //        // Center the image horizontally
	//    //        ImVec2 availableSpace = ImGui::GetContentRegionAvail();
	//    //        ImVec2 imagePos = ImVec2((availableSpace.x - imageSize.x) * 0.5f, 0.0f);  // Center horizontally, keep top
	//    //        ImGui::SetCursorPos(imagePos);
	//    //        if (textureID != nullptr)
	//    //        {
	//    //            ImGui::Image(textureID, imageSize);
	//    //        }
	//    //        else
	//    //        {
	//    //            std::cout << "Error: Loading TextureID..." << std::endl;
	//    //        }

	//    //        ImGui::Spacing();
	//    //        ImGui::Separator();
	//    //        ImGui::Spacing();

	//    //        // Center and place the buttons and text
	//    //        float buttonWidth = 200.0f;
	//    //        float buttonHeight = 40.0f;
	//    //        float textWidth = ImGui::CalcTextSize("Version 1.0.0").x;

	//    //        ImVec2 buttonSize(buttonWidth, buttonHeight);

	//    //        auto centerButton = [&](const char* label) {
	//    //            float buttonPosX = (availableSpace.x - buttonWidth) * 0.5f;
	//    //            ImGui::SetCursorPosX(buttonPosX);
	//    //            bool clicked = ImGui::Button(label, buttonSize);
	//    //            ImGui::Spacing();
	//    //            return clicked;
	//    //            };

	//    //        if (centerButton("Create New Project"))
	//    //        {
	//    //            Application::SetTitleText("Adventure Engine - New Project(Un-Saved)");
	//    //            Application::UICounter = 2;
	//    //        }

	//    //        if (centerButton("Open Existing Project")) {
	//    //            // Handle "Open Existing Project" click
	//    //        }

	//    //        if (centerButton("Exit"))
	//    //        {
	//    //            Application::SetShutdownState(true);
	//    //        }

	//    //        ImGui::Spacing();
	//    //        ImVec2 textPos((availableSpace.x - textWidth) * 0.5f, ImGui::GetCursorPosY());
	//    //        ImGui::SetCursorPos(textPos);
	//    //        ImGui::Text("Version 1.0.0");
	//    //    }
	//    //    ImGui::EndChild();

	//    //    // Documentation in the center
	//    //    ImGui::SameLine();
	//    //    ImGui::BeginChild("Documentation", ImVec2(documentationWidth, 0), true,ImGuiWindowFlags_MenuBar);
	//    //    {
	//    //        if (ImGui::BeginMenuBar())
	//    //        {
	//    //            TextCenteredInMenuBar("Documentation");
	//    //            ImGui::EndMenuBar();
	//    //        }
	//    //        if (ImGui::BeginTabBar("DocumentationTabs"))
	//    //        {
	//    //            if (ImGui::BeginTabItem("Overview"))
	//    //            {
	//    //                OverviewTab();
	//    //                ImGui::EndTabItem();
	//    //            }
	//    //            if (ImGui::BeginTabItem("Getting Started"))
	//    //            {
	//    //                GettingStartedTab();
	//    //                ImGui::EndTabItem();
	//    //            }
	//    //            if (ImGui::BeginTabItem("Advanced Topics"))
	//    //            {
	//    //                ImGui::Text("Advanced topics content: Explore more in-depth features and configurations of the application. This section covers advanced usage and tips.");
	//    //                ImGui::EndTabItem();
	//    //            }
	//    //            ImGui::EndTabBar();
	//    //        }
	//    //        ImGui::EndChild();
	//    //    }
	//    //

	//    //    // Hotfixes on the right
	//    //    ImGui::SameLine();
	//    //    ImGui::BeginChild("Hotfixes", ImVec2(hotfixesWidth - 35, 0), true, ImGuiWindowFlags_MenuBar);
	//    //    {
	//    //        if (ImGui::BeginMenuBar())
	//    //        {
	//    //            TextCenteredInMenuBar("Recent Hotfixes");
	//    //            ImGui::EndMenuBar();
	//    //        }

	//    //        ImGui::Text("+ [2024-07-25]");
	//    //        ImGui::Text("  -  Fixed UI layout issues in the startup screen.");
	//    //        ImGui::Text("  - Improved alignment of buttons and text.");
	//    //        ImGui::Text("  - Adjusted image size for better fit.");
	//    //        ImGui::Spacing();

	//    //        ImGui::Text("+ [2024-07-15] ");
	//    //        ImGui::Text("  - Updated texture handling.");
	//    //        ImGui::Text("  - Added support for different image formats.");
	//    //        ImGui::Text("  - Enhanced performance of texture loading.");
	//    //        ImGui::Spacing();

	//    //        ImGui::Text("+ [2024-07-10]");
	//    //        ImGui::Text("  - Resolved bugs in the hotfix display.");
	//    //        ImGui::Text("  - Fixed display issues with recent updates.");
	//    //        ImGui::Text("  - Improved error handling and logging.");
	//    //        ImGui::Spacing();
	//    //        ImGui::EndChild();
	//    //    }
	//    //

	//    //    ImGui::EndChild();
	//    //}
	//}
}

void StartupScreen::OverviewTab()
{
	ImGui::Text("Welcome to the Adventure Engine!");
	ImGui::Text("The Adventure Engine is a versatile tool designed for creating and managing Text Based Adventure Games.");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Key Features:");
	ImGui::Text("Story Creation: The Node Graph allows for easy story development.");
	ImGui::Text("Play Testing: Play Mode allows for iterative testing of your project.");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("System Requirements:");
	ImGui::BulletText("Operating System: Windows 10 or later, macOS 10.14 or later, or Linux.");
	ImGui::BulletText("Processor: Intel Core i5 or equivalent.");
	ImGui::BulletText("Memory: 1 GB RAM.");
	ImGui::BulletText("Graphics: NVIDIA GTX 960 or equivalent.");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Special Thanks To:");
	ImGui::BulletText("Cliff Sharif - Programming Teacher (Year 1&2)");
	ImGui::BulletText("Shane Chapman - Programming Teacher (Year 2)");
	ImGui::BulletText("Justin Cragg - Programming Teacher (Year 2)");
}

void StartupScreen::GettingStartedTab()
{
	ImGui::Text("Getting Started:");
	ImGui::Text("1. Download and install the Adventure Engine from the official website.");
	ImGui::Text("2. Launch the application and create a new project.");
	ImGui::Text("3. Explore the various tools and features available to start building your game.");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Support and Resources:");
	ImGui::BulletText("For help and support, visit our official support page at [support.example.com](https://support.example.com).");
	ImGui::BulletText("Check out our online tutorials and guides at [docs.example.com](https://docs.example.com) to learn more about using the Adventure Engine.");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
}