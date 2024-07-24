#include "Node.h"
#include "imgui.h"

// Draw a rectangle node with text and a draggable cable point
void Node::DrawNode(const ImVec2& drawPosition, const ImVec2& drawSize)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Background
    ImVec2 shadowOffset = ImVec2(5, 5);
    ImColor shadowColor = ImColor(0, 0, 0, 100); // Shadow color

    //Offset
    ImVec2 drawOffset = ImVec2(drawPosition.x + shadowOffset.x, drawPosition.y + shadowOffset.y);

    // Draw shadow
    drawList->AddRectFilled(drawOffset,
        ImVec2(drawPosition.x + drawSize.x + shadowOffset.x,
            drawPosition.y + drawSize.y + shadowOffset.y),
        shadowColor, 10.0f); // Rounded corners

    // Draw node background
    ImColor nodeColor = ImColor(60, 60, 60); // Node background color
    drawList->AddRectFilled(drawPosition,
        ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
        nodeColor, 10.0f); // Rounded corners

    // Draw node border
    drawList->AddRect(drawPosition,
        ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
        ImColor(100, 100, 100), 10.0f); // Rounded corners

    // Draw text
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
    ImVec2 textPos = ImVec2(drawPosition.x + (drawSize.x - textSize.x) * 0.5f,
        drawPosition.y + (drawSize.y - textSize.y) * 0.5f);
    drawList->AddText(textPos, ImColor(255, 255, 255), text.c_str());
}

void Node::DrawConnection(ImDrawList* drawList, ImVec2 start, ImVec2 end) {
    ImColor connectionColor = ImColor(255, 0, 0); // Connection line color
    drawList->AddLine(start, end, connectionColor, 2.0f);
}

bool Node::IsMouseHovering(ImVec2 mousePos, float zoomLevel,  ImVec2 viewportOffset) {
    // Convert node position and size to screen space considering the viewport offset and zoom level
    ImVec2 nodeMin = ImVec2(position.x - viewportOffset.x, position.y - viewportOffset.y);
    ImVec2 nodeMax = ImVec2(nodeMin.x + size.x * zoomLevel, nodeMin.y + size.y * zoomLevel);

    // Check if the mouse position is within the node's screen space bounds
    return mousePos.x >= nodeMin.x && mousePos.x <= nodeMax.x &&
        mousePos.y >= nodeMin.y && mousePos.y <= nodeMax.y;
}

void Node::SetDragStartPos(const ImVec2 pos) {
    dragStartPos = pos;
}

ImVec2 Node::GetDragStartPos() const {
    return dragStartPos;
}

void Node::SetInitialPosition(const ImVec2 pos) {
    initialPosition = pos;
}

ImVec2 Node::GetInitialPosition() const {
    return initialPosition;
}


