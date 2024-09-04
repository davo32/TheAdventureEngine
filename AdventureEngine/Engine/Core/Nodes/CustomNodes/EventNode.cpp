#include "pch.h"
#include "Core/Nodes/CustomNodes/EventNode.h"
#include "Core/NodeComponents/CustomNodeComponents/NodeDetailsComponent.h"
EventNode::EventNode()
{
	nodeType = NodeType::EventType;



}
EventNode::EventNode(ImVec2 Position, ImVec2 Size, Event* newEvent)
	: Node(Position, Size, newEvent->GetEventName()), StoredEvent(newEvent)
{
	nodeType = NodeType::EventType;

	// Initialize input points close to the left side
	inputPoints.push_back(ImVec2(Position.x + Size.x, Position.y + Size.y / 2));
	
	ImVec2 Combine = ImVec2(Position.x, Position.y);

	// Initialize output points close to the right side
	outputPoints.push_back(ImVec2(Combine));

	//Node Details Component - Default For Re-namable Nodes
	Components.push_back(new NodeDetailsComponent(this));
}

EventNode::~EventNode() 
{ 
}

void EventNode::DrawComponents()
{
	if (GetEvent()->GetEventText().empty() || GetEvent()->GetEventText().empty())
	{
		isEmpty = true;
	}
	else
	{
		isEmpty = false;
	}
}

Event* EventNode::GetEvent() 
{ 
	return StoredEvent;
}

bool EventNode::ExecuteNodeAction()
{
	return false;
}

//Important for saving and Loading
std::vector<uint8_t> EventNode::serialize() const
{
	std::vector<uint8_t> data;

	// Serialize ID
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&id), reinterpret_cast<const uint8_t*>(&id) + sizeof(int));

	// Serialize position
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&position.x), reinterpret_cast<const uint8_t*>(&position.x) + sizeof(float));
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&position.y), reinterpret_cast<const uint8_t*>(&position.y) + sizeof(float));

	// Serialize size
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&size.x), reinterpret_cast<const uint8_t*>(&size.x) + sizeof(float));
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&size.y), reinterpret_cast<const uint8_t*>(&size.y) + sizeof(float));


	// Serialize inputPoints
	size_t inputPointsSize = inputPoints.size();
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&inputPointsSize), reinterpret_cast<const uint8_t*>(&inputPointsSize) + sizeof(size_t));

	for (const auto& point : inputPoints)
	{
		data.insert(data.end(), reinterpret_cast<const uint8_t*>(&point.x), reinterpret_cast<const uint8_t*>(&point.x) + sizeof(float));
		data.insert(data.end(), reinterpret_cast<const uint8_t*>(&point.y), reinterpret_cast<const uint8_t*>(&point.y) + sizeof(float));
	}

	// Serialize outputPoints
	size_t outputPointsSize = outputPoints.size();
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&outputPointsSize), reinterpret_cast<const uint8_t*>(&outputPointsSize) + sizeof(size_t));

	for (const auto& point : outputPoints)
	{
		data.insert(data.end(), reinterpret_cast<const uint8_t*>(&point.x), reinterpret_cast<const uint8_t*>(&point.x) + sizeof(float));
		data.insert(data.end(), reinterpret_cast<const uint8_t*>(&point.y), reinterpret_cast<const uint8_t*>(&point.y) + sizeof(float));
	}

	//// Serialize text
	size_t textSize = text.size();
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&textSize), reinterpret_cast<const uint8_t*>(&textSize) + sizeof(size_t));
	data.insert(data.end(), text.begin(), text.end());

	// Serialize booleans
	data.push_back(static_cast<uint8_t>(LeftisConnected ? 1 : 0));
	data.push_back(static_cast<uint8_t>(RightisConnected ? 1 : 0));

	// Serialize connections
	size_t connectionsSize = connections.size();
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&connectionsSize), reinterpret_cast<const uint8_t*>(&connectionsSize) + sizeof(size_t));

    for (const auto& conn : connections)
    {
        // Serialize targetNode ID
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&conn.targetNodeID), reinterpret_cast<const uint8_t*>(&conn.targetNodeID) + sizeof(int));
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&conn.outputIndex), reinterpret_cast<const uint8_t*>(&conn.outputIndex) + sizeof(int));
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&conn.inputIndex), reinterpret_cast<const uint8_t*>(&conn.inputIndex) + sizeof(int));
    }

    /*size_t ComponentSize = Components.size();
    data.insert(data.end(), reinterpret_cast<const uint8_t*>(&ComponentSize), reinterpret_cast<const uint8_t*>(&ComponentSize) + sizeof(size_t));

    for()*/

	return data;
}

void EventNode::deserialize(const std::vector<uint8_t> &data)
{
    size_t offset = 0;

    // Check if data is large enough for ID
    if (offset + sizeof(int) > data.size()) {
        throw std::runtime_error("Data size too small for ID");
    }
    id = *reinterpret_cast<const int*>(&data[offset]);
    offset += sizeof(int);

    // Check if data is large enough for position
    if (offset + 2 * sizeof(float) > data.size()) {
        throw std::runtime_error("Data size too small for position");
    }
    position.x = *reinterpret_cast<const float*>(&data[offset]);
    offset += sizeof(float);
    position.y = *reinterpret_cast<const float*>(&data[offset]);
    offset += sizeof(float);

    // Check if data is large enough for size
    if (offset + 2 * sizeof(float) > data.size()) {
        throw std::runtime_error("Data size too small for size");
    }
    size.x = *reinterpret_cast<const float*>(&data[offset]);
    offset += sizeof(float);
    size.y = *reinterpret_cast<const float*>(&data[offset]);
    offset += sizeof(float);

    // Deserialize inputPoints size
    if (offset + sizeof(size_t) > data.size()) {
        throw std::runtime_error("Data size too small for inputPoints size");
    }
    size_t inputPointsSize;
    std::memcpy(&inputPointsSize, &data[offset], sizeof(size_t));
    offset += sizeof(size_t);

    if (inputPointsSize > MAX_ALLOWED_SIZE) { // Define MAX_ALLOWED_SIZE based on your use case
        throw std::runtime_error("Invalid input points size");
    }
    inputPoints.resize(inputPointsSize);

    // Check if data is large enough for each ImVec2 point
    if (offset + inputPointsSize * 2 * sizeof(float) > data.size()) {
        throw std::runtime_error("Data size too small for input points");
    }

    // Deserialize each ImVec2 point
    for (auto& point : inputPoints)
    {
        point.x = *reinterpret_cast<const float*>(&data[offset]);
        offset += sizeof(float);
        point.y = *reinterpret_cast<const float*>(&data[offset]);
        offset += sizeof(float);
    }

    // Deserialize outputPoints size
    if (offset + sizeof(size_t) > data.size()) {
        throw std::runtime_error("Data size too small for outputPoints size");
    }
    size_t outputPointsSize;
    std::memcpy(&outputPointsSize, &data[offset], sizeof(size_t));
    offset += sizeof(size_t);

    if (outputPointsSize > MAX_ALLOWED_SIZE) { // Define MAX_ALLOWED_SIZE based on your use case
        throw std::runtime_error("Invalid output points size");
    }
    outputPoints.resize(outputPointsSize);

    // Check if data is large enough for each ImVec2 point
    if (offset + outputPointsSize * 2 * sizeof(float) > data.size()) {
        throw std::runtime_error("Data size too small for output points");
    }

    // Deserialize each ImVec2 point
    for (auto& point : outputPoints)
    {
        point.x = *reinterpret_cast<const float*>(&data[offset]);
        offset += sizeof(float);
        point.y = *reinterpret_cast<const float*>(&data[offset]);
        offset += sizeof(float);
    }

    // Define a maximum reasonable text size
    const size_t MAX_TEXT_SIZE = 1024 * 1024; // Example: 1 MB

    // Deserialize text size
    if (offset + sizeof(size_t) > data.size()) {
        throw std::runtime_error("Data size too small for text size");
    }
    size_t textSize;
    std::memcpy(&textSize, &data[offset], sizeof(size_t));
    offset += sizeof(size_t);

    if (textSize > data.size() - offset) {
        throw std::runtime_error("Data size too small for text");
    }
    text.resize(textSize);

    // Deserialize text
    std::memcpy(&text[0], &data[offset], textSize);
    offset += textSize;

    // Deserialize booleans
    if (offset + 2 > data.size())
    {
        throw std::runtime_error("Data size too small for booleans");
    }
    LeftisConnected = data[offset] != 0;
    offset += 1;
    RightisConnected = data[offset] != 0;
    offset += 1;

    // Deserialize connections
    if (offset + sizeof(size_t) > data.size()) {
        throw std::runtime_error("Data size too small for connections size");
    }
    size_t connectionsSize;
    std::memcpy(&connectionsSize, &data[offset], sizeof(size_t));
    offset += sizeof(size_t);

    if (connectionsSize > MAX_ALLOWED_SIZE) { // Define MAX_ALLOWED_SIZE based on your use case
        throw std::runtime_error("Invalid connections size");
    }
    connections.resize(connectionsSize);

    // Check if data is large enough for each connection
    if (offset + connectionsSize * (sizeof(int) * 3) > data.size()) {
        throw std::runtime_error("Data size too small for connections");
    }

    for (auto& conn : connections)
    {
        // Deserialize targetNode ID
        int targetNodeId = *reinterpret_cast<const int*>(&data[offset]);
        offset += sizeof(int);

        // Temporarily store the ID
        conn.targetNodeID = targetNodeId; // Store the ID for later resolution

        conn.outputIndex = *reinterpret_cast<const int*>(&data[offset]);
        offset += sizeof(int);

        conn.inputIndex = *reinterpret_cast<const int*>(&data[offset]);
        offset += sizeof(int);

        conn.targetNode = nullptr;
    }
}
