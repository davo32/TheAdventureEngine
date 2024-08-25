#include "imgui.h"
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Texture.h"
#include "JSONHandler.h"
#include "FileExtensions.h"


struct Project
{
	std::string name;
	std::string path;
	char Summary[256];
	std::string LastEdited;
};

// Namespace for file system operations
namespace fs = std::filesystem;

class ProjectBrowser
{
public:
	

	ProjectBrowser(std::string directoryPath) : directoryPath(std::move(directoryPath))
	{
		LoadProjects();
	}

	~ProjectBrowser()
	{
		// Clean up dynamically allocated memory
		for (Project* project : projects)
		{
			delete project;
		}
	}

	void Render() 
	{
		// Define grid parameters
		const int columns = 11; // Number of columns in the grid
		const float buttonSize = 100.0f; // Size of each button
		const float spacing = 30.0f; // Spacing between buttons

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

	bool DeleteProject()
	{
		std::string TruePath = RemoveExtension(ActiveProject->path, "\\" + ActiveProject->name + FileExtensions::MasterExt);

		// Remove_all deletes the directory and all its contents
		std::uintmax_t numDeleted = std::filesystem::remove_all(TruePath);
		if (numDeleted > 0)
		{
			ActiveProject = nullptr;
			ReloadProjects();
			return true;
		}
		return false;
	}

	void CreateNewProject(const std::string& directoryName, const std::string& fileName)
	{
		std::string FolderPath = directoryPath + "/" + directoryName;

		// Create the directory with the specified name
		if (std::filesystem::create_directory(FolderPath))
		{
			std::cout << "Project folder created: " << directoryName << std::endl;
			std::string ChapterFolderPath = FolderPath + "/" + "Chapters";
			std::filesystem::create_directory(ChapterFolderPath);

			std::string newFileName = "New Chapter" + FileExtensions::ChapterExt;
			std::string ChapterPath = ChapterFolderPath + "/" + newFileName;
			std::ofstream outFile(ChapterPath);
			if (outFile.is_open())
			{
				outFile << "";  // Create an empty file
				outFile.close();
			}
		}
		else
		{
			std::cerr << "Failed to create project folder or it already exists: " << directoryName << std::endl;
		}

		// Construct the full file path within the newly created directory
		std::string filePath = FolderPath + "/" + fileName;

		std::ofstream outFile(filePath);
		if (outFile.is_open())
		{
			outFile << "";  // Create an empty file
			outFile.close();

			// JSON DATA HANDLING
			{
				json Data =
				{
					{"Prelim Data",
					{
						{"Project Name", "Untitled Project"},
						{"Project Summary", "This is an Example Summary..."}/*,
						{"Chapter Count", "1" }*/
					}}
				};

				JSONHandler::saveToFile(filePath, Data);
			}

			std::cout << "File created: " << filePath << std::endl;
		}
		else
		{
			std::cerr << "Failed to create file: " << filePath << std::endl;
		}
	}

	void RenameProject(const std::string& oldProjectFolderName, const std::string& oldFileName, const std::string& newFileName, const std::string& newProjectFolderName = "")
	{
		try
		{
			// Construct the full paths for the file
			fs::path oldFolderPath = fs::path(directoryPath) / oldProjectFolderName;
			fs::path oldFilePath = oldFolderPath / oldFileName;
			fs::path newFilePath = oldFolderPath / newFileName;
			

			if (!std::filesystem::exists(oldFolderPath))
			{
				std::cerr << "Directory does not exist: " << oldFolderPath << std::endl;
				return;
			}

			// Rename the file
			fs::rename(oldFilePath, newFilePath);
			
			std::string JSONFolderPath = directoryPath;
			std::string JSONPath = JSONFolderPath + "\\" 
				+ RemoveExtension(newFileName, FileExtensions::MasterExt)
				+ "\\" + newFileName;

			GetActiveProject()->path = JSONPath;
			GetActiveProject()->name = RemoveExtension(newFileName, FileExtensions::MasterExt);

			std::cout << "File renamed successfully from " << oldFilePath << " to " << newFilePath << std::endl;

			// Rename the folder if a new folder name is provided
			if (!newProjectFolderName.empty() && oldProjectFolderName != newProjectFolderName)
			{
				fs::path newFolderPath = fs::path(directoryPath) / newProjectFolderName;

				fs::rename(oldFolderPath, newFolderPath);
				std::cout << "Folder renamed successfully from " << oldFolderPath << " to " << newFolderPath << std::endl;
			}
		}
		catch (const fs::filesystem_error& e)
		{
			std::cerr << "Error renaming file or folder: " << e.what() << std::endl;
		}
	}

	int CountChapterFiles(const std::string& directoryPath)
	{
		int count = 0;

		try
		{
			// Iterate through the directory
			for (const auto& entry : fs::directory_iterator(directoryPath))
			{
				// Check if the entry is a regular file and has a .Chapter extension
				if (entry.is_regular_file() && entry.path().extension() == ".Chapter")
				{
					++count;
				}
			}
		}
		catch (const fs::filesystem_error& e)
		{
			std::cerr << "Error accessing directory: " << e.what() << std::endl;
		}

		return count;
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

	std::string RemoveExtension(const std::string& path, const std::string& ext)
	{
		// Check if the path ends with the extension
		if (path.size() >= ext.size() && path.compare(path.size() - ext.size(), ext.size(), ext) == 0)
		{
			return path.substr(0, path.size() - ext.size());
		}
		return path;
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

	void DeSelectProject()
	{
		ActiveProject = nullptr;
	}
	
	std::vector<Project*> projectsToSave;

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
			if (entry.is_directory())
			{
				std::string folderName = entry.path().filename().string();
				std::string projectFilePath;

				// Iterate through files inside the directory
				for (const auto& fileEntry : fs::directory_iterator(entry.path()))
				{
					if (fileEntry.is_regular_file())
					{
						// Get the full path of the file inside the folder
						projectFilePath = fileEntry.path().string();
						break;  // Assuming you only need the first file found
					}
				}

				if (!projectFilePath.empty())
				{
					// Create a new Project object and add it to the vector
					Project* project = new Project{ folderName, projectFilePath, "This is an Example Summary..." };
					projects.push_back(project);
				}
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
	bool ImageButtonWithText(ImTextureID texture, const char* label, const ImVec2& imageSize, const ImVec2& buttonSize) 
	{
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

		// Draw the rounded rectangle background behind the image
		ImVec2 min = cursorPos;
		ImVec2 max = ImVec2(cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y);
		float rounding = 8.0f; // Rounding radius for the rectangle

		ImColor mainColor = IM_COL32(20, 20, 20, 110);
		ImColor offColor = IM_COL32(80, 80, 80, 110);

		drawList->AddRectFilledMultiColor(min, max, mainColor, offColor, mainColor, mainColor); // Gray background

		ImColor DefhighColor = IM_COL32(20, 20, 20, 250);
		ImColor NewhighColor = IM_COL32(120, 0, 210, 250);
		ImColor highColor = DefhighColor;

		if (ActiveProject != nullptr)
		{
			if (ActiveProject->name == label)
			{
				highColor = NewhighColor;
			}
			else
			{
				highColor = DefhighColor;
			}
		}

		drawList->AddRect(min, max, highColor, rounding, 0, 8.0f);
		// Draw the image
		ImGui::SetCursorScreenPos(imagePos);
		ImGui::Image(texture, imageSize);

		// Optional: Draw a border around the button when hovered
		if (isHovered)
		{	
			drawList->AddRect(min, max, IM_COL32(255, 255, 255, 255),8.0f); // White border
		}

		// Draw the text
		ImGui::SetCursorScreenPos(textPos);
		ImGui::Text("%s", label);

		ImGui::EndGroup(); // End the button group

		return isClicked;
	}

	
};