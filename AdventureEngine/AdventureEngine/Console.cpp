#include "Console.h"
#include <string>
#include <iostream>
#include "Application.h"

void Console::RenderConsole()
{
    static char inputBuffer[128];
    static bool scrollToBottom = false;

    ImVec2 parentSize = ImGui::GetContentRegionAvail();
    ImVec2 childSize = ImVec2(parentSize.x, parentSize.y);

    if (ImGui::BeginChild("##Console", childSize, ImGuiChildFlags_None,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            ImGui::Text("Console");

            // Clear button
            if (ImGui::Button("Clear"))
            {
                ClearLog();
            }
            parentSize = ImGui::GetContentRegionAvail();
            ImGui::Dummy(ImVec2(50, parentSize.y));
            // Set the width of the next item
            ImGui::SetNextItemWidth(150.0f);
            // Input field
            if (ImGui::InputTextWithHint("##Input","Command Input...", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                // Process the input command here
                logs.push_back(inputBuffer);
                ProcessCommand(inputBuffer);
                inputBuffer[0] = '\0';  // Clear input buffer
                scrollToBottom = true;
            }

            ImGui::EndMenuBar();
        }

        // Console log display area
        ImGui::BeginGroup();
        for (const auto& log : logs)
        {
            std::string prefix = "[CMD] : " + log;
            ImGui::Text(prefix.c_str());
        }
        if (scrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;
        ImGui::EndGroup();

       

        ImGui::EndChild();
    }
}

void Console::ClearLog()
{
    logs.clear();
}

void Console::RegisterCommands()
{
    cmdList.RegisterCommand("/test", [this]() {
        std::cout << "This is a test" << '\n';
        });
    cmdList.RegisterCommand("/clear", [this]() {
        ClearLog();
        });
    cmdList.RegisterCommand("/quit", [this]() {
        Application::SetShutdownState(true);
        });
}



void Console::ProcessCommand(const char* command)
{
    cmdList.ExecuteCommand(command);
}
