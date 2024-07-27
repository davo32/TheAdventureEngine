#include "Node.h"
#include "imgui.h"
#include "Application.h"

// Draw a rectangle node with text and a draggable cable point
void Node::DrawNode(const ImVec2& drawPosition, const ImVec2& drawSize,const float& zoomLevel)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Constants
    const float headerRadius = 6.0f;  // Radius for rounded corners
    const float headerHeight = 30.0f;
    const float pinSize = 10.0f;
    const float borderThickness = 2.0f;
    const float padding = 5.0f;
    float pinOffsetY = 0.0f;
    bool dragging = false; // Flag to track dragging state

   CreateNodeVisuals(drawPosition, drawSize, drawList, headerRadius, headerHeight);
   CreateInputsAndOutputs(drawPosition, padding, headerHeight, drawSize, drawList, headerRadius, borderThickness, pinOffsetY);
   DrawComponents(position, size, zoomLevel);
}

void Node::CreateInputsAndOutputs(const ImVec2& drawPosition, const float padding, const float headerHeight, const ImVec2& drawSize, ImDrawList* drawList, const float headerRadius, const float borderThickness, float& pinOffsetY)
{
    // Calculate input and output starting positions
    ImVec2 inputStartPos = ImVec2(drawPosition.x + padding, drawPosition.y + headerHeight);
    ImVec2 outputStartPos = ImVec2(drawPosition.x + drawSize.x - padding, drawPosition.y + headerHeight);

    // Constants for rectangle size
    const float rectWidth = 15.0f;
    const float rectHeight = 20.0f;
    const float rectOffsetY = headerHeight / 2; // Adjust this value as needed

    // Draw input rectangles
    for (const ImVec2& point : inputPoints) {
        ImVec2 center((drawPosition.x + padding - rectWidth / 2) + 20.0f, (drawPosition.y + rectOffsetY) + 35.0f);

        // Calculate rectangle corners
        ImVec2 rectMin(center.x - rectWidth / 2, center.y - rectHeight / 2);
        ImVec2 rectMax(center.x + rectWidth / 2, center.y + rectHeight / 2);

        // Draw outer rectangle as an outlined rectangle
        drawList->AddRect(rectMin, rectMax, ImColor(255, 255, 255), headerRadius);

        // Draw inner rectangle with border
        ImVec2 innerRectMin(rectMin.x + borderThickness, rectMin.y + borderThickness);
        ImVec2 innerRectMax(rectMax.x - borderThickness, rectMax.y - borderThickness);

        drawList->AddRectFilled(innerRectMin, innerRectMax, ImColor(0, 0, 0, 128), headerRadius);

        pinOffsetY += rectHeight + padding;

        drawList->AddText(ImVec2(center.x + rectWidth, center.y - 8.0), ImColor(255, 255, 255, 255), "Input");
    }

    // Draw output rectangles
    for (const ImVec2& point : outputPoints) {
        ImVec2 center((drawPosition.x + drawSize.x - padding + rectWidth / 2) - 20.0f, (drawPosition.y + rectOffsetY) + 35.0f);

        // Calculate rectangle corners
        ImVec2 rectMin(center.x - rectWidth / 2, center.y - rectHeight / 2);
        ImVec2 rectMax(center.x + rectWidth / 2, center.y + rectHeight / 2);

        // Draw outer rectangle as an outlined rectangle
        drawList->AddRect(rectMin, rectMax, ImColor(255, 255, 255), headerRadius);

        // Draw inner rectangle with border
        ImVec2 innerRectMin(rectMin.x + borderThickness, rectMin.y + borderThickness);
        ImVec2 innerRectMax(rectMax.x - borderThickness, rectMax.y - borderThickness);

        drawList->AddRectFilled(innerRectMin, innerRectMax, ImColor(0, 0, 0, 128), headerRadius);

        pinOffsetY += rectHeight + padding;

        drawList->AddText(ImVec2(center.x - rectWidth - 32.0f, center.y - 8.0), ImColor(255, 255, 255, 255), "Output");
    }
}

void Node::CreateNodeVisuals(const ImVec2& drawPosition, const ImVec2& drawSize, ImDrawList* drawList, const float headerRadius, const float headerHeight)
{
    // Background
    ImVec2 shadowOffset(5, 5);
    ImColor shadowColor(0, 0, 0, 100);

    // Draw shadow
    ImVec2 shadowPos = ImVec2(drawPosition.x + shadowOffset.x, drawPosition.y + shadowOffset.y);
    ImVec2 shadowEnd = ImVec2(drawPosition.x + drawSize.x + shadowOffset.x, drawPosition.y + drawSize.y + shadowOffset.y);
    drawList->AddRectFilled(shadowPos, shadowEnd, shadowColor, headerRadius);

    // Node background
    ImColor nodeColor(0, 0, 0, 128);
    drawList->AddRectFilled(drawPosition,
        ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
        nodeColor, headerRadius);


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
    Application::fontLoader.SetFont("Bold");
    Application::fontLoader.DrawText(drawList, textPos, ImColor(255, 255, 255), text.c_str());
    //drawList->AddText(textPos, ImColor(255, 255, 255), text.c_str());


    // Draw node border
    ImColor borderColor = isActive ? ImColor(255, 255, 0) : ImColor(100, 100, 100); // Yellow if active
    drawList->AddRect(drawPosition,
        ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
        borderColor, headerRadius, ImDrawFlags_None, 6.0f);
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


