#include "pch.h"
#include "Core/Nodes/CustomNodes/BeginNode.h"
#include "Core/NodeComponents/CustomNodeComponents/NodeDetailsComponent/NodeDetailsComponent.h"

BeginNode::BeginNode() 
    :Node(position,size, "Begin")
{
    colorTop = ImColor(255, 0, 0, 128);

    nodeType = NodeType::BeginType;

    position = ImVec2(935, 362);
    size = ImVec2(100, 70);

    // Initialize output points close to the right side
    //outputPoints.push_back(ImVec2(Position.x + Size.x / 2, Position.y + Size.y / 2));
}


BeginNode::BeginNode(ImVec2 _Position, ImVec2 _Size)
	:Node(position,size,"Begin")/*, Position(_Position), Size(_Size)*/
{
    nodeType = NodeType::BeginType;

	colorTop = ImColor(255, 0, 0, 128);

    position = ImVec2(935, 362);
    size = ImVec2(100, 70);

	// Initialize output points close to the right side
    ImVec2 newOutputPos(ImVec2(position.x + size.x / 2, position.y + size.y / 2));
    Pin newOutput = Pin(newOutputPos, "Start");
	outputPoints.push_back(newOutput);
	
}

bool BeginNode::ExecuteNodeAction()
{
    return false;
}

//Important for saving and Loading
std::vector<uint8_t> BeginNode::serialize() const
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
        // Serialize the ImVec2 position (x and y)
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&point.position.x), reinterpret_cast<const uint8_t*>(&point.position.x) + sizeof(float));
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&point.position.y), reinterpret_cast<const uint8_t*>(&point.position.y) + sizeof(float));

        // Serialize the length of the label string (uint32_t for consistency)
        uint32_t labelLength = static_cast<uint32_t>(point.label.size());
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&labelLength), reinterpret_cast<const uint8_t*>(&labelLength) + sizeof(uint32_t));

        // Serialize the actual label string
        data.insert(data.end(), point.label.begin(), point.label.end());
    }

    // Serialize outputPoints
    size_t outputPointsSize = outputPoints.size();
    data.insert(data.end(), reinterpret_cast<const uint8_t*>(&outputPointsSize), reinterpret_cast<const uint8_t*>(&outputPointsSize) + sizeof(size_t));
    for (const auto& point : outputPoints)
    {
        // Serialize the ImVec2 position (x and y)
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&point.position.x), reinterpret_cast<const uint8_t*>(&point.position.x) + sizeof(float));
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&point.position.y), reinterpret_cast<const uint8_t*>(&point.position.y) + sizeof(float));

        // Serialize the length of the label string (uint32_t for consistency)
        uint32_t labelLength = static_cast<uint32_t>(point.label.size());
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&labelLength), reinterpret_cast<const uint8_t*>(&labelLength) + sizeof(uint32_t));

        // Serialize the actual label string
        data.insert(data.end(), point.label.begin(), point.label.end());
    }

    // Serialize text
    size_t textSize = text.size();
    data.insert(data.end(), reinterpret_cast<const uint8_t*>(&textSize), reinterpret_cast<const uint8_t*>(&textSize) + sizeof(size_t));
    if (textSize > 0)
    {
        data.insert(data.end(), text.begin(), text.end());
    }

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

    return data;
}

void BeginNode::deserialize(const std::vector<uint8_t>& data)
{
    size_t offset = 0;
    std::cout << "Start data size: " << data.size() - offset << std::endl;

    // Deserialize ID
    if (offset + sizeof(int) > data.size()) {
        throw std::runtime_error("Data size too small for ID");
    }
    id = *reinterpret_cast<const int*>(&data[offset]);
    offset += sizeof(int);

    std::cout << "After ID data size: " << data.size() - offset << std::endl;

    // Deserialize position
    if (offset + 2 * sizeof(float) > data.size()) {
        throw std::runtime_error("Data size too small for position");
    }
    position.x = *reinterpret_cast<const float*>(&data[offset]);
    offset += sizeof(float);
    position.y = *reinterpret_cast<const float*>(&data[offset]);
    offset += sizeof(float);

    std::cout << "AfterPos data size: " << data.size() - offset << std::endl;

    // Deserialize size
    if (offset + 2 * sizeof(float) > data.size()) {
        throw std::runtime_error("Data size too small for size");
    }
    size.x = *reinterpret_cast<const float*>(&data[offset]);
    offset += sizeof(float);
    size.y = *reinterpret_cast<const float*>(&data[offset]);
    offset += sizeof(float);

    std::cout << "After Size data size: " << data.size() - offset << std::endl;

    // Deserialize inputPoints size
    if (offset + sizeof(size_t) > data.size()) {
        throw std::runtime_error("Data size too small for inputPoints size");
    }
    size_t inputPointsSize;
    std::memcpy(&inputPointsSize, &data[offset], sizeof(size_t));
    offset += sizeof(size_t);

    std::cout << "After IP Size data size: " << data.size() - offset << std::endl;

    if (inputPointsSize > 10000) { // Consider defining MAX_ALLOWED_SIZE
        throw std::runtime_error("Invalid input points size");
    }
    inputPoints.resize(inputPointsSize);

    // Deserialize inputPoints
    if (offset + inputPointsSize * 2 * sizeof(float) > data.size()) {
        throw std::runtime_error("Data size too small for input points");
    }
    for (auto& point : inputPoints)
    {
        point.position.x = *reinterpret_cast<const float*>(&data[offset]);
        offset += sizeof(float);
        point.position.y = *reinterpret_cast<const float*>(&data[offset]);
        offset += sizeof(float);

        // Deserialize the length of the label string
        uint32_t labelLength = *reinterpret_cast<const uint32_t*>(&data[offset]);
        offset += sizeof(uint32_t);

        // Deserialize the actual label string
        point.label.assign(reinterpret_cast<const char*>(&data[offset]), labelLength);
        offset += labelLength;
    }

    std::cout << "After IP data size: " << data.size() - offset << std::endl;

    // Deserialize outputPoints size
    if (offset + sizeof(size_t) > data.size()) {
        throw std::runtime_error("Data size too small for outputPoints size");
    }
    size_t outputPointsSize;
    std::memcpy(&outputPointsSize, &data[offset], sizeof(size_t));
    offset += sizeof(size_t);

    std::cout << "After OP Size data size: " << data.size() - offset << std::endl;

    if (outputPointsSize > 10000) { // Consider defining MAX_ALLOWED_SIZE
        throw std::runtime_error("Invalid output points size");
    }
    outputPoints.resize(outputPointsSize);

    // Deserialize outputPoints
    if (offset + outputPointsSize * 2 * sizeof(float) > data.size()) {
        throw std::runtime_error("Data size too small for output points");
    }
    for (auto& point : outputPoints)
    {
        point.position.x = *reinterpret_cast<const float*>(&data[offset]);
        offset += sizeof(float);
        point.position.y = *reinterpret_cast<const float*>(&data[offset]);
        offset += sizeof(float);

        // Deserialize the length of the label string
        uint32_t labelLength = *reinterpret_cast<const uint32_t*>(&data[offset]);
        offset += sizeof(uint32_t);

        // Deserialize the actual label string
        point.label.assign(reinterpret_cast<const char*>(&data[offset]), labelLength);
        offset += labelLength;
    }

    std::cout << "After OP data size: " << data.size() - offset << std::endl;

    // Define a maximum reasonable text size
    const size_t MAX_TEXT_SIZE = 1024 * 1024; // Example: 1 MB

    // Deserialize text size
    if (offset + sizeof(size_t) > data.size())
    {
        throw std::runtime_error("Data size too small for text size");
    }
    size_t textSize;
    std::memcpy(&textSize, &data[offset], sizeof(size_t));
    offset += sizeof(size_t);

    std::cout << "After Text Size data size: " << data.size() - offset << std::endl;

    // Log text size and remaining data size
    std::cout << "Text size: " << textSize << std::endl;
    std::cout << "Remaining data size: " << data.size() - offset << std::endl;

    // Check if textSize is reasonable
    if (textSize > data.size() - offset || textSize > MAX_TEXT_SIZE) {
        throw std::runtime_error("Invalid text size");
    }
    text.resize(textSize);

    // Deserialize text
    std::memcpy(&text[0], &data[offset], textSize);
    offset += textSize;

    // Deserialize booleans
    if (offset + 2 > data.size()) {
        throw std::runtime_error("Data size too small for booleans");
    }
    LeftisConnected = data[offset] != 0;
    offset += 1;
    RightisConnected = data[offset] != 0;
    offset += 1;

    // Deserialize connections size
    if (offset + sizeof(size_t) > data.size()) {
        throw std::runtime_error("Data size too small for connections size");
    }
    size_t connectionsSize;
    std::memcpy(&connectionsSize, &data[offset], sizeof(size_t));
    offset += sizeof(size_t);

    if (connectionsSize > 10000) { // Consider defining MAX_ALLOWED_SIZE
        throw std::runtime_error("Invalid connections size");
    }
    connections.resize(connectionsSize);

    // Deserialize connections
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
