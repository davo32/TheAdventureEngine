#pragma once
#include <fstream>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

class JSONHandler
{
public:

    // Save a JSON object to a file
    static bool saveToFile(const std::string& filePath, const json& jsonObject)
    {
        try
        {
            std::ofstream outFile(filePath);
            if (!outFile.is_open())
            {
                std::cerr << "Error: Could Not Open File For Writing: " << filePath << '\n';
                return false;
            }

            outFile << jsonObject.dump(4); // Pretty print with an indent of 4 spaces
            outFile.close();
            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception occurred while saving to file: " << e.what() << '\n';
            return false;
        }
    }

    // Load a JSON object from a file
    static json loadFromFile(const std::string& filePath)
    {
        try
        {
            std::ifstream inFile(filePath);
            if (!inFile.is_open())
            {
                std::cerr << "Error: Could Not Open File For Reading: " << filePath << '\n';
                return json(); // Return an empty JSON object
            }

            json jsonObject;
            inFile >> jsonObject;
            inFile.close();
            return jsonObject;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception occurred while loading from file: " << e.what() << '\n';
            return json(); // Return an empty JSON object in case of error
        }
    }
};
