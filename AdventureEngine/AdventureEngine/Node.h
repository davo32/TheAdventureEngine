#pragma once

#include "imgui.h"
#include <string>
#include <vector>

class Node {
public:
    Node() = default;
    Node(ImVec2 pos, ImVec2 size, std::string name);
    virtual ~Node() = default;

    struct Connection
    {
        Node* targetNode;
        int outputIndex; // Which ouput point is connecting
        int inputIndex; // Which input point is receiving the connection
    };

    void RenderConnections(ImDrawList* drawList, float zoomLevel, ImVec2 viewportOffset) const;

    ImVec2 GetPosition() const { return position; }
    ImVec2 GetSize() const { return size; }
    ImVec2 GetCablePosition() const { return cableStartPos; }

    void SetDragStartPos(ImVec2 pos);
    ImVec2 GetDragStartPos() const;

    void SetInitialPosition(ImVec2 pos);
    ImVec2 GetInitialPosition() const;

    bool IsMouseHovering(ImVec2 mousePos, float zoomLevel, ImVec2 viewportOffset) const;

    void SetSize(ImVec2 newSize) { size = newSize; }
   
    void SetText(std::string text) { this->text = text; }
    std::string GetText() { return text; }
   
    void SetPosition(ImVec2 newPosition) { position = newPosition; }
    void SetCablePosition(ImVec2 newPosition) { cableStartPos = newPosition; }

    bool IsActive() const { return isActive; }
    void SetIsActive(bool newActive) { isActive = newActive; }

    bool IsDragging() const { return isDragging; }
    void SetIsDragging(bool dragging) { isDragging = dragging; }

    void DrawNode(const ImVec2& position, const ImVec2& size, float zoomLevel);
    virtual void DrawComponents(const ImVec2& position, const ImVec2& size, float zoomLevel) {}

    ImVec2 GetOutputPoint(int index) const { return outputPoints[index]; }
    ImVec2 GetInputPoint(int index) const { return inputPoints[index]; }

    void RemoveConnection(int index) 
    {
       connections[0].targetNode->LeftisConnected = false;
       connections.erase(connections.begin() + index);
    }

    int GetConnectionCount() { return connections.size(); }
    int GetOutputCount() { return outputPoints.size(); }
    int GetInputCount() { return inputPoints.size(); }

    void StartConnecting(int outputIndex, ImVec2 startPos);
    void UpdateConnection(ImVec2 endPos);
    void EndConnection(Node* targetNode, int inputIndex);
    void ConnectTo(Node* targetNode, int outputIndex, int inputIndex);
    int GetHoveredInputPointIndex(ImVec2 mousePos);
    int GetHoveredOutputPointIndex(ImVec2 mousePos);

protected:
    void CreateInputsAndOutputs(const ImVec2& drawPosition, float padding, float headerHeight, const ImVec2& drawSize, ImDrawList* drawList, float headerRadius, float borderThickness, float& pinOffsetY);
    void CreateNodeVisuals(const ImVec2& drawPosition, const ImVec2& drawSize, ImDrawList* drawList, float headerRadius, float headerHeight);


    float Distance(ImVec2 a, ImVec2 b)
    {
        return sqrt((a.x - b.x) * (a.y - b.y));
    }

    int dragStartOutputIndex = -1;
    bool isConnecting = false;
    ImVec2 connectionStartPos;

    ImVec2 minSize = ImVec2(100, 25);
    ImVec2 maxSize = ImVec2(250, 150);

    std::vector<ImVec2> inputPoints;
    std::vector<ImVec2> outputPoints;


    ImColor colorTop = ImColor(128, 0, 128);
    ImColor colorBottom = ImColor(128, 0, 0, 128);
    ImColor ConColor = ImColor(255, 255, 255, 255);
    bool LeftisConnected = false;
    bool RightisConnected = false;

    bool isActive = false;

    bool isEmpty = false;
    bool sizeAdjusted = false;

private:
    ImVec2 position;
    ImVec2 size;
    std::string text;
    bool isDrawingConnection = false;
    ImVec2 cableStartPos;
    bool isDragging = false;
    ImVec2 dragStartPos;
    ImVec2 initialPosition;
    std::vector<Connection> connections; //Store connections for the node
};