#include "StartupScreen.h"
#include "MonitorInfo.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include <iostream>

void StartupScreen::DrawUI()
{
    ImVec2 windowSize(MonitorInfo::GetMode()->width - 30, MonitorInfo::GetMode()->height - 80);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    if (ImGui::BeginChild("Startup Screen", windowSize, true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        // Define sizes for the sections
        float mainSectionWidth = 310.0f;   // Width for Main Section
        float hotfixesWidth = 400.0f;      // Increased width for Hotfixes
        float documentationWidth = windowSize.x - mainSectionWidth - hotfixesWidth; // Remaining width for Documentation

        // Main Section on the left
        ImGui::BeginChild("Main Section", ImVec2(mainSectionWidth, 0), true);
        {
            int width, height, channels;
            if (Logo.load("../Resources/Images/Logo.png"))
            {
                Logo.bind(0);
            }
            else
            {
                std::cout << "Error: Loading Texture..." << std::endl;
            }

            // Get the texture ID and size
            ImTextureID textureID = (ImTextureID)Logo.getHandle();
            ImVec2 imageSize(Logo.getWidth(), Logo.getHeight());

            // Main section content
            ImGui::Spacing();

            // Center the image horizontally
            ImVec2 availableSpace = ImGui::GetContentRegionAvail();
            ImVec2 imagePos = ImVec2((availableSpace.x - imageSize.x) * 0.5f, 0.0f);  // Center horizontally, keep top
            ImGui::SetCursorPos(imagePos);
            if (textureID != nullptr)
            {
                ImGui::Image(textureID, imageSize);
            }
            else
            {
                std::cout << "Error: Loading TextureID..." << std::endl;
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Center and place the buttons and text
            float buttonWidth = 200.0f;
            float buttonHeight = 40.0f;
            float textWidth = ImGui::CalcTextSize("Version 1.0.0").x;

            ImVec2 buttonSize(buttonWidth, buttonHeight);

            auto centerButton = [&](const char* label) {
                float buttonPosX = (availableSpace.x - buttonWidth) * 0.5f;
                ImGui::SetCursorPosX(buttonPosX);
                bool clicked = ImGui::Button(label, buttonSize);
                ImGui::Spacing();
                return clicked;
                };

            if (centerButton("Create New Project"))
            {
                Application::SetTitleText("Adventure Engine - New Project(Un-Saved)");
                Application::UICounter = 2;
            }

            if (centerButton("Open Existing Project")) {
                // Handle "Open Existing Project" click
            }

            if (centerButton("Exit"))
            {
                Application::SetShutdownState(true);
            }

            ImGui::Spacing();
            ImVec2 textPos((availableSpace.x - textWidth) * 0.5f, ImGui::GetCursorPosY());
            ImGui::SetCursorPos(textPos);
            ImGui::Text("Version 1.0.0");
        }
        ImGui::EndChild();

        // Documentation in the center
        ImGui::SameLine();
        ImGui::BeginChild("Documentation", ImVec2(documentationWidth, 0), true,ImGuiWindowFlags_MenuBar);
        {
            if (ImGui::BeginMenuBar())
            {
                TextCenteredInMenuBar("Documentation");
                ImGui::EndMenuBar();
            }
            if (ImGui::BeginTabBar("DocumentationTabs")) 
            {
                if (ImGui::BeginTabItem("Overview"))
                {
                    OverviewTab();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Getting Started"))
                {
                    GettingStartedTab();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Advanced Topics")) 
                {
                    ImGui::Text("Advanced topics content: Explore more in-depth features and configurations of the application. This section covers advanced usage and tips.");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::EndChild();

        // Hotfixes on the right
        ImGui::SameLine();
        ImGui::BeginChild("Hotfixes", ImVec2(hotfixesWidth - 35, 0), true, ImGuiWindowFlags_MenuBar);
        {
            if (ImGui::BeginMenuBar())
            {
                TextCenteredInMenuBar("Recent Hotfixes");
                ImGui::EndMenuBar();
            }

            ImGui::Text("+ [2024-07-25]");
            ImGui::Text("  -  Fixed UI layout issues in the startup screen.");
            ImGui::Text("  - Improved alignment of buttons and text.");
            ImGui::Text("  - Adjusted image size for better fit.");
            ImGui::Spacing();

            ImGui::Text("+ [2024-07-15] ");
            ImGui::Text("  - Updated texture handling.");
            ImGui::Text("  - Added support for different image formats.");
            ImGui::Text("  - Enhanced performance of texture loading.");
            ImGui::Spacing();

            ImGui::Text("+ [2024-07-10]");
            ImGui::Text("  - Resolved bugs in the hotfix display.");
            ImGui::Text("  - Fixed display issues with recent updates.");
            ImGui::Text("  - Improved error handling and logging.");
            ImGui::Spacing();
        }
        ImGui::EndChild();

        ImGui::EndChild();
    }
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
