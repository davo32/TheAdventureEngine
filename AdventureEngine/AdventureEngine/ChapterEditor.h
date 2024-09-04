#pragma once
#include "imgui.h"
#include "Core/Chapter/Chapter.h"
#include <memory>
#include <unordered_map>

class ChapterEditor
{
public:
    void RenderViewport(Chapter* chapter);

    //Utilities
    void NodeGraphShortcuts(Chapter* chapter);

private:
    void RenderBackground(const ImVec2& childSize, const ImVec2& childPos,Chapter* chapter = nullptr);
    void RenderNodes(Chapter* chapter);
    void RenderContextMenu(Chapter* chapter);

    void ContextMenuOpen();
    
    void NodeInteraction(Chapter* chapter);
    void NodeDrag(const ImVec2& mousePos, Chapter* chapter);
    
    void HandleZooming(const ImGuiIO& io,Chapter* chapter);
    void HandlePanning(ImGuiIO &io, Chapter* chapter);

    void EventRenamer(Chapter* chapter);
    void UpdateNodeNames(std::string newName, Chapter* chapter);
    
    //- Chapter Vars
    std::string ChapterName;

    // - Panning Vars
    bool isPanning = false;

    //- Context Menu Vars
    ImVec2 contextMenuPos;

    //- Drag Node Vars
    Node* dragStartNode = nullptr;
    int dragStartOutputIndex = -1;
    ImVec2 dragStartPos;

    //- Event Vars
    std::unordered_map<Event*, bool> eventSelectionState;
    
   

    //- Search Vars
    std::string searchQuery;
    static char inputText[128];
    bool isContextualMenuOpen = false;

    //Connection Vars
    Node* sourceNode = nullptr;
    ImVec2 connectionEndPos;
    bool isConnecting = false;

    bool hasRun = false;
    bool nodeClicked = false;
   
};
