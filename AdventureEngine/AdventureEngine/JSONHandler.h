#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

class JSONHandler
{
public:

	static bool saveToFile(const std::string& filePath,const json& jsonObject)
	{
		std::ofstream outFile(filePath);
		if(!outFile.is_open())
		{
			std::cerr << "Error: Could Not Open File For Writing: " << filePath << '\n';
			return false;
		}

		outFile << jsonObject.dump(4); // Pretty print with an indent of 4 Spaces
		outFile.close();
		return true;
	}

	// Function to load a JSON object from a file
	static json loadFromFile(const std::string& filePath) {
		std::ifstream inFile(filePath);
		if (!inFile.is_open()) {
			std::cerr << "Error: Could not open file for reading: " << filePath << std::endl;
			return nullptr;
		}

		json jsonObject;
		inFile >> jsonObject;
		inFile.close();
		return jsonObject;
	}
};