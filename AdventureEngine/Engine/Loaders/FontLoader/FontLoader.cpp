#include "pch.h"
#include "FontLoader.h"

// Define the static member
FontLoader* FontLoader::instance = nullptr;

// Load a font and store its pointer
bool FontLoader::LoadFont(const std::string& path, float size, const std::string& name) 
{
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
void FontLoader::SetFont(const std::string& name)
{
    auto it = fonts.find(name);
    if (it != fonts.end()) {
        currentFont = it->second;
    }
    else {
        currentFont = nullptr;
    }
}

ImFont* FontLoader::GetFont(const std::string& name)
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
void FontLoader::DrawText(ImDrawList* drawList, const ImVec2& position, const ImColor& color, const std::string& text) {
    if (currentFont) {
        ImGui::PushFont(currentFont);
        drawList->AddText(position, color, text.c_str());
        ImGui::PopFont();
    }
    else {
        drawList->AddText(position, color, text.c_str()); // Use default font if none is set
    }
}