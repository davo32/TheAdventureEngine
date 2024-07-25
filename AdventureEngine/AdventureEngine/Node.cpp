#include "Node.h"
#include "imgui.h"

// Draw a rectangle node with text and a draggable cable point
void Node::DrawNode(const ImVec2& drawPosition, const ImVec2& drawSize,const float& zoomLevel)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Constants
    const float headerHeight = 30.0f;
    const float pinRadius = 6.0f;
    const float pinInnerRadius = 4.0f;
    const float padding = 5.0f;
    const float dropdownHeight = 20.0f;
    const float headerRadius = 6.0f;  // Radius for rounded corners

    // Background
    ImVec2 shadowOffset(5, 5);
    ImColor shadowColor(0, 0, 0, 100);

    // Draw shadow
    ImVec2 shadowPos = ImVec2(drawPosition.x + shadowOffset.x, drawPosition.y + shadowOffset.y);
    ImVec2 shadowEnd = ImVec2(drawPosition.x + drawSize.x + shadowOffset.x, drawPosition.y + drawSize.y + shadowOffset.y);
    drawList->AddRectFilled(shadowPos, shadowEnd, shadowColor, headerRadius);

    // Node background
    ImColor nodeColor(60, 60, 60,128);
    drawList->AddRectFilled(drawPosition,
        ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
        nodeColor, headerRadius);

    // Define colors for the gradient
    ImU32 colorTop = ImColor(30, 30, 30); // Top color (black)
    ImU32 colorBottom = ImColor(60, 60, 60); // Bottom color (dark gray)

    // Calculate header position and size
    ImVec2 headerPos(drawPosition.x, drawPosition.y);
    ImVec2 headerSize(drawSize.x, headerHeight);

    // Draw the gradient background
    drawList->AddRectFilledMultiColor(
        headerPos,
        ImVec2(headerPos.x + headerSize.x, headerPos.y + headerSize.y),
        colorTop, colorTop, colorBottom, colorBottom
    );

    // Draw text on header
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
    ImVec2 textPos(
        drawPosition.x + (drawSize.x - textSize.x) * 0.5f,
        drawPosition.y + (headerHeight - textSize.y) * 0.5f
    );
    drawList->AddText(textPos, ImColor(255, 255, 255), text.c_str());


    // Draw node border
    ImColor borderColor = isActive ? ImColor(255, 255, 0) : ImColor(100, 100, 100); // Yellow if active
    drawList->AddRect(drawPosition,
        ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
        borderColor, headerRadius, ImDrawFlags_None, 6.0f);

    // Draw connection points
    ImVec2 inputStartPos(drawPosition.x + padding, drawPosition.y + headerHeight + padding);
    ImVec2 outputStartPos(drawPosition.x + drawSize.x - padding, drawPosition.y + headerHeight + padding);
    float pinOffsetY = 0.0f;


    DrawComponents(position, size, zoomLevel);
    
    // Draw input points
    for (const ImVec2& point : inputPoints) {
        ImVec2 circlePos(inputStartPos.x + point.x, inputStartPos.y + point.y);
        drawList->AddCircleFilled(circlePos, pinRadius, ImColor(255, 255, 255)); // Outer circle
        drawList->AddCircleFilled(circlePos, pinInnerRadius, ImColor(0, 0, 0)); // Inner circle
        pinOffsetY += pinRadius * 2.0f + padding;
    }

    // Draw output points
    for (const ImVec2& point : outputPoints) {
        ImVec2 circlePos(outputStartPos.x + point.x, outputStartPos.y + point.y);
        drawList->AddCircleFilled(circlePos, pinRadius, ImColor(255, 255, 255)); // Outer circle
        drawList->AddCircleFilled(circlePos, pinInnerRadius, ImColor(0, 0, 0)); // Inner circle
        pinOffsetY += pinRadius * 2.0f + padding;
    }

}



void Node::DrawConnection(ImDrawList* drawList, ImVec2 start, ImVec2 end) {
    ImColor connectionColor = ImColor(0, 0, 0); // Connection line color
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


