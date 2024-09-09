#include "pch.h"
#include "Node.h"
#include "imgui.h"
#include <iostream>

int Node::nextID = 0; //start ID Generation From 0

Node::Node(ImVec2 pos, ImVec2 size, std::string name)
	: position(pos), size(size), text(std::move(name)) 
{
	id = nextID++;
}

void Node::ConnectTo(Node* targetNode, int outputIndex, int inputIndex)
{
	// Check if the output pin is already connected
	if (outputPoints[outputIndex].isConnected)
	{
		std::cout << "Output is Connected" << '\n';

		// If it is, break the existing connection
		for (auto& connection : connections)
		{
			if (connection.outputIndex == outputIndex)
			{
				std::cout << "Index is Correct" << '\n';
				RemoveConnection(connection.inputIndex);
				break;
			}
		}
	}

	// Add the connection to the current node's output connections
	Connection newConnection;
	newConnection.targetNodeID = targetNode->id;
	newConnection.targetNode = targetNode;
	newConnection.outputIndex = outputIndex;
	newConnection.inputIndex = inputIndex;
	//newConnection.targetNode->inputPoints[inputIndex].isConnected = true;
	connections.push_back(newConnection);

	targetNode->inputPoints[inputIndex].isConnected = true;
	outputPoints[outputIndex].isConnected = true;

	std::cout << "Connection Made!" << '\n';
}

void Node::RenderConnections(ImDrawList* drawList, float zoomLevel, ImVec2 viewportOffset) const
{
	for (const Connection& conn : connections)
	{
		if (conn.targetNode != nullptr)
		{
			// Get the position of the output and input points
			ImVec2 outputPointPos = GetOutputPoint(conn.outputIndex).position;
			ImVec2 inputPointPos = conn.targetNode->GetInputPoint(conn.inputIndex).position;

			// Define the vector to add
			ImVec2 offset = ImVec2(0.0f, 0.0f);

			// Calculate the result by adding x and y components separately
			ImVec2 OutputResult = ImVec2(
				outputPointPos.x + offset.x, // Sum of x components
				outputPointPos.y + offset.y  // Sum of y components
			);

			// Calculate the result by adding x and y components separately
			ImVec2 InputResult = ImVec2(
				inputPointPos.x + offset.x, // Sum of x components
				inputPointPos.y + offset.y  // Sum of y components
			);

			// Define control points for the Bézier curve
			ImVec2 controlPoint1 = OutputResult; // Example control point, adjust as needed
			ImVec2 controlPoint2 = InputResult; // Example control point, adjust as needed

			// Draw the Bézier curve using AddBezierCubic
			drawList->AddBezierCubic(outputPointPos, controlPoint1, controlPoint2, inputPointPos, ConColor, 2.0f, 4.0f);

		}
		else
		{
			std::cerr << "Target Node is Nullptr" << '\n';
		}
	}
}

void Node::SetDragStartPos(ImVec2 pos)
{
	dragStartPos = pos;
}

ImVec2 Node::GetDragStartPos() const
{
	return dragStartPos;
}

void Node::SetInitialPosition(ImVec2 pos)
{
	initialPosition = pos;
}

ImVec2 Node::GetInitialPosition() const
{
	return initialPosition;
}

bool Node::IsMouseHovering(ImVec2 mousePos, float zoomLevel, ImVec2 viewportOffset) const
{
	// Calculate the node's position and size relative to the viewport and zoom level
	ImVec2 nodeMin = ImVec2((position.x - viewportOffset.x) * zoomLevel, (position.y - viewportOffset.y) * zoomLevel);
	ImVec2 nodeMax = ImVec2(nodeMin.x + size.x * zoomLevel, nodeMin.y + size.y * zoomLevel);

	// Check if the mouse position is within the node's bounding box
	return mousePos.x >= nodeMin.x && mousePos.x <= nodeMax.x &&
		mousePos.y >= nodeMin.y && mousePos.y <= nodeMax.y;
}

void Node::DrawNode(const ImVec2& position, const ImVec2& size, float zoomLevel) {
	ImGui::BeginGroup();
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	const float headerRadius = 6.0f;
	const float headerHeight = 30.0f;
	const float padding = 5.0f;
	float pinOffsetY = 0.0f;

	CreateNodeVisuals(position, size, drawList, headerRadius, headerHeight);
	CreateInputsAndOutputs(position, padding, headerHeight, size, drawList, headerRadius, 2.0f, pinOffsetY);

	ImGui::EndGroup();
}

void Node::DrawComponents()
{
	if (!Components.empty())
	{
		for (NodeComponent* NC : Components)
		{
			if (NC != nullptr)
			{
				NC->RenderComponent();
			}
			else
			{
				std::cerr << "Null NodeComponent found in Components vector!" << std::endl;
			}
		}
	}
}

void Node::StartConnecting(int outputIndex, ImVec2 startPos)
{
	isConnecting = true;
	connectionStartPos = startPos;
	//dragStartPos = startPos;
	dragStartOutputIndex = outputIndex;
}

void Node::UpdateConnection(ImVec2 endPos)
{
	if (isConnecting)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddLine(connectionStartPos, endPos, ImColor(255, 255, 255), 2.0f);
	}
}

void Node::EndConnection(Node* targetNode, int inputIndex)
{
	if (targetNode)
	{
		ConnectTo(targetNode, dragStartOutputIndex, inputIndex);
	}
	isConnecting = false;
}

void Node::CreateInputsAndOutputs(const ImVec2& drawPosition, float padding, float headerHeight, const ImVec2& drawSize, ImDrawList* drawList, float headerRadius, float borderThickness, float& pinOffsetY)
{
	const float circleRadius = 8.0f; // Radius of the circle
	const float blackSectionOffsetY = headerHeight + padding; // Start pin placement below header
	ImGuiIO& io = ImGui::GetIO();

	// Initialize pin offset Y for inputs
	float inputPinOffsetY = drawPosition.y + blackSectionOffsetY;

	for (int i = 0; i < inputPoints.size(); i++)
	{
		ImVec2 center = ImVec2(drawPosition.x + padding + 20.0f, inputPinOffsetY + circleRadius);
		inputPoints[i].position = center;

		bool isHovered = GetHoveredInputPointIndex(io.MousePos) == i;
		// Draw outer circle
		drawList->AddCircle(center, circleRadius, isHovered ? ImColor(255, 0, 0) : ImColor(255, 255, 255, 128), 16, borderThickness);

		if (inputPoints[i].isConnected == false)
		{
			// Draw inner circle
			drawList->AddCircleFilled(center, circleRadius - borderThickness, ImColor(0, 0, 0, 128), 16);
		}
		else
		{
			// Draw inner circle
			drawList->AddCircleFilled(center, circleRadius - borderThickness, ConColor, 16);
		}

		// Draw label
		drawList->AddText(ImVec2(center.x + circleRadius + 5.0f, center.y - 8.0f), ImColor(255, 255, 255), inputPoints[i].label.c_str());

		// Update pin offset for the next input
		inputPinOffsetY += circleRadius * 2.0f + padding;
	}

	// Initialize pin offset Y for outputs
	float outputPinOffsetY = drawPosition.y + blackSectionOffsetY;

	for (int i = 0; i < outputPoints.size(); i++)
	{
		ImVec2 center = ImVec2(drawPosition.x + drawSize.x - padding - 20.0f, outputPinOffsetY + circleRadius);
		outputPoints[i].position = center;

		bool isHovered = GetHoveredOutputPointIndex(io.MousePos) == i;
		// Draw outer circle
		drawList->AddCircle(center, circleRadius, isHovered ? ImColor(255, 0, 0) : ImColor(255, 255, 255, 128), 16, borderThickness);

		if (!outputPoints[i].isConnected)
		{
			// Draw inner circle
			drawList->AddCircleFilled(center, circleRadius - borderThickness, ImColor(0, 0, 0, 128), 16);
		}
		else
		{
			drawList->AddCircleFilled(center, circleRadius - borderThickness, ConColor, 16);
		}

		// Draw label
		drawList->AddText(ImVec2(center.x - circleRadius - 45.0f, center.y - 8.0f), ImColor(255, 255, 255, 255), outputPoints[i].label.c_str());

		// Update pin offset for the next output
		outputPinOffsetY += circleRadius * 2.0f + padding;
	}
}



void Node::CreateNodeVisuals(const ImVec2& drawPosition, const ImVec2& drawSize, ImDrawList* drawList, float headerRadius, float headerHeight)
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

	Globals::fontLoader->SetFont("Bold");
	Globals::fontLoader->DrawText(drawList, textPos, ImColor(255, 255, 255), text);

	if (isEmpty)
	{
		if (sizeAdjusted == false)
		{
			SetSize(ImVec2(size.x, size.y * 1.2f));
			sizeAdjusted = true;
		}

		// Draw the red rectangle at the bottom
		ImVec2 redRectPos(drawPosition.x, drawPosition.y + drawSize.y - 30); // 30 is the height of the red rect
		ImVec2 redRectSize(drawSize.x, 30);
		ImColor redRectColorUpper(0, 0, 0, 0);
		ImColor redRectColorLower(255, 0, 0, 128);
		drawList->AddRectFilledMultiColor(redRectPos, ImVec2(redRectPos.x + redRectSize.x, redRectPos.y + redRectSize.y), redRectColorUpper, redRectColorUpper, redRectColorLower, redRectColorLower);

		// Draw text on the red rectangle
		std::string bottomText = "Empty Event!";
		ImVec2 bottomTextSize = ImGui::CalcTextSize(bottomText.c_str());
		ImVec2 bottomTextPos(
			redRectPos.x + (redRectSize.x - bottomTextSize.x) * 0.5f,
			redRectPos.y + (redRectSize.y - bottomTextSize.y) * 0.5f
		);
		Globals::fontLoader->DrawText(drawList, bottomTextPos, ImColor(255, 255, 255), bottomText);
	}
	else
	{
		if (sizeAdjusted)
		{
			SetSize(ImVec2(size.x, size.y / 1.2));
			sizeAdjusted = false;
		}
	}

	// Draw node border
	ImColor borderColor = isActive ? ImColor(255, 255, 0) : ImColor(100, 100, 100); // Yellow if active
	drawList->AddRect(drawPosition,
		ImVec2(drawPosition.x + drawSize.x, drawPosition.y + drawSize.y),
		borderColor, headerRadius, ImDrawFlags_None, 6.0f);
}

int Node::GetHoveredInputPointIndex(ImVec2 MousePos)
{
	float HoverRadius = 8.0f;
	for (int i = 0; i < inputPoints.size(); ++i)
	{
		if (Distance(MousePos, inputPoints[i].position) < HoverRadius)
		{
			return i;
		}
	}
	return -1;
}

int Node::GetHoveredOutputPointIndex(ImVec2 MousePos)
{
	float HoverRadius = 8.0f;
	for (int i = 0; i < outputPoints.size(); i++)
	{
		if (Distance(MousePos, outputPoints[i].position) < HoverRadius)
		{
			return i;
		}
	}
	return -1;
}

std::vector<uint8_t> Node::serialize() const
{
	std::vector<uint8_t> data;

	// Serialize position
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&position.x), reinterpret_cast<const uint8_t*>(&position.x) + sizeof(float));
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&position.y), reinterpret_cast<const uint8_t*>(&position.y) + sizeof(float));

	// Serialize size
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&size.x), reinterpret_cast<const uint8_t*>(&size.x) + sizeof(float));
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&size.y), reinterpret_cast<const uint8_t*>(&size.y) + sizeof(float));

	// Serialize text
	size_t textSize = text.size();
	data.insert(data.end(), reinterpret_cast<const uint8_t*>(&textSize), reinterpret_cast<const uint8_t*>(&textSize) + sizeof(size_t));
	data.insert(data.end(), text.begin(), text.end());

	return data;
}

void Node::deserialize(const std::vector<uint8_t>& data)
{
	size_t offset = 0;

	// Deserialize position
	memcpy(&position.x, data.data() + offset, sizeof(float));
	offset += sizeof(float);
	memcpy(&position.y, data.data() + offset, sizeof(float));
	offset += sizeof(float);

	// Deserialize size
	memcpy(&size.x, data.data() + offset, sizeof(float));
	offset += sizeof(float);
	memcpy(&size.y, data.data() + offset, sizeof(float));
	offset += sizeof(float);

	// Deserialize text
	size_t textSize;
	memcpy(&textSize, data.data() + offset, sizeof(size_t));
	offset += sizeof(size_t);
	text.assign(reinterpret_cast<const char*>(data.data() + offset), textSize);
	offset += textSize;
}