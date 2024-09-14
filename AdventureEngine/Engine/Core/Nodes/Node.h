#pragma once
#include "imgui.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Core/NodeComponents/NodeComponent.h"
#include "Globals.h"
enum NodeType
{
    BeginType = 0,
    EventType
};

struct Pin
{
    ImVec2 position;
    std::string label;
    bool isConnected;

    Pin() : position(0,0),label("") 
    {
        isConnected = false;
    }

    Pin(const ImVec2& pos,const std::string& text)
        : position(pos),label(text) 
    {
        isConnected = false;
    }
};


class Node 
{
public:
    Node() = default;
    Node(ImVec2 pos, ImVec2 size, std::string name);
    virtual ~Node() = default;

    struct Connection
    {
        int targetNodeID;
        Node* targetNode;
        int outputIndex; // Which ouput point is connecting
        int inputIndex; // Which input point is receiving the connection

    };

    virtual bool ExecuteNodeAction() = 0;

    void RenderConnections(ImDrawList* drawList, float zoomLevel, ImVec2 viewportOffset) const;

    ImVec2 GetPosition() const { return position; }
    ImVec2 GetSize() const { return size; }
    //ImVec2 GetCablePosition() const { return cableStartPos; }

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
    void DrawComponents();

    void AddComponent(NodeComponent* newComponent) { Components.push_back(newComponent); }
    void RemoveComponent(NodeComponent* component) 
    { 
        for (int i = 0; i < Components.size(); i++)
        {
            Components.erase(Components.begin() + i);
            break;
        }
    }

    Pin GetOutputPoint(int index) const { return outputPoints[index]; }
    Pin GetInputPoint(int index) const { return inputPoints[index]; }

    void RemoveConnection(int index) 
    {
        if (index >= 0 && index < connections.size())
        {
            if (connections[index].targetNode != nullptr)
            {
                connections[index].targetNode->inputPoints[connections[index].inputIndex].isConnected = false;
                std::cout << "Connection Broken!!" << '\n';
            }
            connections.erase(connections.begin() + index);
            std::cout << "Connection Removed!!" << '\n';
        }
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

    virtual std::vector<uint8_t> serialize() const;
    virtual void deserialize(const std::vector<uint8_t>& data);

    int GetID() const { return id; }
    int id; // Unique ID for each Node
    
    std::vector<Connection> connections; //Store connections for the node
    std::vector<Pin> inputPoints;
    std::vector<Pin> outputPoints;

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


    std::vector<NodeComponent*> Components;


    ImColor colorTop = ImColor(128, 0, 128);
    ImColor colorBottom = ImColor(128, 0, 0, 128);
    ImColor ConColor = ImColor(255, 255, 255, 255);
    
   // bool LeftisConnected = false;
   // bool RightisConnected = false;

    bool isActive = false;

    bool isEmpty = false;
    bool sizeAdjusted = false;
    std::string text;

    NodeType nodeType;

    ImVec2 position;
    ImVec2 size;
    

private:
    bool isDrawingConnection = false;
    ImVec2 cableStartPos;
    bool isDragging = false;
    ImVec2 dragStartPos;
    ImVec2 initialPosition;
    static int nextID; // static counter for generating unique IDs
};