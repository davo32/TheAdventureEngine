#pragma once
#include "imgui.h"
#include <unordered_map>
#include <string>

class FontLoader
{
public:
    // Get the singleton instance
    static FontLoader& GetInstance() {
        static FontLoader instance; // Guaranteed to be destroyed
        return instance; // Instantiated on first use
    }

    // Load a font and store its pointer
    bool LoadFont(const std::string& path, float size, const std::string& name) {
        if (io) {
            ImFont* font = io->Fonts->AddFontFromFileTTF(path.c_str(), size);
            if (font) {
                fonts[name] = font;
                return true;
            }
        }
        return false;
    }

    // Set the current font by name
    void SetFont(const std::string& name) {
        auto it = fonts.find(name);
        if (it != fonts.end()) {
            currentFont = it->second;
        }
        else {
            currentFont = nullptr;
        }
    }

    ImFont* GetFont(const std::string& name)
    {
        auto it = fonts.find(name);
        if (it != fonts.end())
        {
            return it->second;
        }
        else
        {
            return nullptr;
        }
    }

    // Draw text with the current font
    void DrawText(ImDrawList* drawList, const ImVec2& position, const ImColor& color, const std::string& text) {
        if (currentFont) {
            ImGui::PushFont(currentFont);
            drawList->AddText(position, color, text.c_str());
            ImGui::PopFont();
        }
        else {
            drawList->AddText(position, color, text.c_str()); // Use default font if none is set
        }
    }

    void SetIO(ImGuiIO& _io)
    {
        io = &_io;
    }

private:
    FontLoader() : io(nullptr), currentFont(nullptr) {}
    ImGuiIO* io; // Changed to a pointer
    std::unordered_map<std::string, ImFont*> fonts;
    ImFont* currentFont;

    // Prevent copy construction and assignment
    FontLoader(const FontLoader&) = delete;
    FontLoader& operator=(const FontLoader&) = delete;
};