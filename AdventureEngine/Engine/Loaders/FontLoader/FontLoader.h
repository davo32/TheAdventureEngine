#pragma once
#include "imgui.h"
#include <unordered_map>
#include <string>

class FontLoader
{
public:
    // Get the singleton instance
    static FontLoader* GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new FontLoader();
        }
        return instance; // Instantiated on first use
    }

    // Load a font and store its pointer
    bool LoadFont(const std::string& path, float size, const std::string& name);

    // Set the current font by name
    void SetFont(const std::string& name);

    ImFont* GetFont(const std::string& name);

    // Draw text with the current font
    void DrawText(ImDrawList* drawList, const ImVec2& position, const ImColor& color, const std::string& text);

    void SetIO(ImGuiIO& _io)
    {
        io = &_io;
    }

    static FontLoader* instance;

private:
    FontLoader() : io(nullptr), currentFont(nullptr) {}
    ImGuiIO* io; // Changed to a pointer
    std::unordered_map<std::string, ImFont*> fonts;
    ImFont* currentFont;

    // Prevent copy construction and assignment
    FontLoader(const FontLoader&) = delete;
    FontLoader& operator=(const FontLoader&) = delete;
};