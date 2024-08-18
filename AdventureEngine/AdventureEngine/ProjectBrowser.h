#include "imgui.h"
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Texture.h"

// Namespace for file system operations
namespace fs = std::filesystem;

class ProjectBrowser
{
public:
	struct Project 
	{
		std::string name;
		std::string path;
		char Summary[256];
		std::string LastEdited;
	};

	ProjectBrowser(const std::string& directoryPath) : directoryPath(directoryPath) 
	{
		LoadProjects();
	}

	~ProjectBrowser()
	{
		// Clean up dynamically allocated memory
		for (Project* project : projects) {
			delete project;
		}
	}

	void Render() 
	{
		// Define grid parameters
		const int columns = 4; // Number of columns in the grid
		const float buttonSize = 100.0f; // Size of each button
		const float spacing = 10.0f; // Spacing between buttons

		// Calculate window size and start position
		ImVec2 windowPos = ImGui::GetCursorPos();
		float startX = windowPos.x;
		float startY = windowPos.y;

		// Iterate over projects to create buttons
		for (size_t i = 0; i < projects.size(); ++i)
		{
			// Calculate button position based on index
			int row = static_cast<int>(i / columns);
			int column = static_cast<int>(i % columns);

			ImVec2 buttonPos = ImVec2(
				startX + column * (buttonSize + spacing),
				startY + row * (buttonSize + spacing)
			);

			// Set cursor position for the button
			ImGui::SetCursorPos(buttonPos);

			// Create button with image and text
			if (ImageButtonWithText(ProjIcon, projects[i]->name.c_str(), ImVec2(buttonSize, buttonSize), ImVec2(buttonSize, buttonSize)))
			{
				OnProjectSelected(projects[i]);
			}
		}

	}

	void CreateNewFile(const std::string& fileName) 
	{
		std::string filePath = directoryPath + "/" + fileName;
		std::ofstream outFile(filePath);
		if (outFile.is_open())
		{
			outFile << "";  // Create an empty file
			outFile.close();
			std::cout << "File created: " << filePath << std::endl;
		}
		else
		{
			std::cerr << "Failed to create file: " << filePath << std::endl;
		}
	}

	void RenameFileInSameFolder(const std::string& folderPath, const std::string& oldFileName, const std::string& newFileName)
	{
		try
		{
			// Construct the full paths
			fs::path oldPath = fs::path(folderPath) / oldFileName;
			fs::path newPath = fs::path(folderPath) / newFileName;

			// Rename the file
			fs::rename(oldPath, newPath);
			std::cout << "File renamed successfully from " << oldPath << " to " << newPath << std::endl;
		}
		catch (const fs::filesystem_error& e)
		{
			std::cerr << "Error renaming file: " << e.what() << std::endl;
		}
	}

	void LoadDefaultIcon()
	{
		if (texture.load("../Resources/Images/DFICON.png"))
		{
			texture.bind(0);
		}
		else
		{
			std::cout << "Error: Loading Texture..." << std::endl;
		}

		// Get the texture ID and size
		ProjIcon = (ImTextureID)texture.getHandle();
	}

	void ReloadProjects()
	{
		LoadProjects();

		ActiveProject = nullptr;
	}

	Project* GetActiveProject()
	{
		return ActiveProject;
	}

	std::string GetDirectoryPath()
	{
		return directoryPath;
	}

private:
	std::string directoryPath;
	std::vector<Project*> projects;
	Project* ActiveProject;
	aie::Texture texture;

	ImTextureID ProjIcon;

	void LoadProjects()
	{
		projects.clear();
		for (const auto& entry : fs::directory_iterator(directoryPath))
		{
			if (entry.is_regular_file())
			{
				// Get the filename without extension
				std::string fileNameWithoutExtension = entry.path().stem().string();
				// Get the full file path
				std::string fullPath = entry.path().string();

				// Create a new Project object and add it to the vector
				Project* project = new Project{ fileNameWithoutExtension, fullPath ,"This is an Example Summary..."};
				projects.push_back(project);
			}
		}
	}

	void OnProjectSelected(Project* project) 
	{
		std::cout << "Selected project: " << project->name << std::endl;
		// Add your project opening logic here
		ActiveProject = project;
	}

	

	// Function to render a button with an image and text
	bool ImageButtonWithText(ImTextureID texture, const char* label, const ImVec2& imageSize, const ImVec2& buttonSize) {
		ImGui::BeginGroup(); // Start a new group for the button
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		// Calculate positions
		ImVec2 cursorPos = ImGui::GetCursorPos();
		ImVec2 imagePos = ImVec2(cursorPos.x + (buttonSize.x - imageSize.x) / 2, cursorPos.y);
		ImVec2 textPos = ImVec2(cursorPos.x + (buttonSize.x - ImGui::CalcTextSize(label).x) / 2, cursorPos.y + imageSize.y + ImGui::GetStyle().ItemSpacing.y);

		// Create the invisible button
		ImGui::InvisibleButton("image_button", buttonSize);
		bool isHovered = ImGui::IsItemHovered();
		bool isClicked = ImGui::IsItemClicked();

		// Draw the image
		ImGui::SetCursorScreenPos(imagePos);
		ImGui::Image(texture, imageSize);

		

		// Optional: Draw a border around the button when hovered
		if (isHovered) {
			
			ImVec2 min = ImGui::GetItemRectMin();
			ImVec2 max = ImGui::GetItemRectMax();
			drawList->AddRect(min, max, IM_COL32(255, 255, 255, 255),8.0f); // White border
		}

		// Draw the text
		ImGui::SetCursorScreenPos(textPos);
		ImGui::Text("%s", label);

		ImGui::EndGroup(); // End the button group

		return isClicked;
	}
};