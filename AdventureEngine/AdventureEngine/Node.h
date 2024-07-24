#pragma once
#include "imgui.h"
#include <string>

class Node
{
public:
    Node(ImVec2 Pos, ImVec2 _size, std::string Name) 
        : position(Pos), size(_size), text(Name) {}
    ~Node() = default;

    ImVec2 GetPosition() { return position; }
    ImVec2 GetSize() { return size; }
    ImVec2 GetCablePosition() { return cableStartPos; }
    
    // Methods for dragging functionality
    void SetDragStartPos(const ImVec2 pos);
    ImVec2 GetDragStartPos() const;

    void SetInitialPosition(const ImVec2 pos);
    ImVec2 GetInitialPosition() const;

    bool IsMouseHovering(ImVec2 mousePos, float zoomLevel, ImVec2 viewportOffset);

    bool GetIsDragging() { return isDragging; }
    bool GetIsDraggingCable() { return isDraggingCable; }
    
    std::string GetText() { return text; }

    void SetSize(ImVec2 newSize) { size = newSize; }
    void SetText(std::string _text) { text = _text; }
    void SetPosition(ImVec2 newPosition) { position = newPosition; }
    void SetCablePosition(ImVec2 newPosition) { cableStartPos = newPosition; }
    void DrawNode(const ImVec2& position, const ImVec2& size);
    void DrawConnection(ImDrawList* drawList, ImVec2 start, ImVec2 end);
    void SetIsDragging(bool check) { isDragging = check; }
    
    
    ImVec2 minSize = ImVec2(100, 25);
    ImVec2 maxSize = ImVec2(250, 150);
    
private:
    ImVec2 position;
    ImVec2 size;
    std::string text;
    bool isDraggingCable = false;
    ImVec2 cableStartPos; // The starting position of the cable when dragging
    
    bool isDragging = false;

    ImVec2 dragStartPos; // Mouse position when dragging starts
    ImVec2 initialPosition; // Node position when dragging starts

};