#pragma once
#include "imgui.h"
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "FileSystem/ProjectBrowser/JSON/JSONHandler.h"
#include "OpenGL/Texture.h"

#include "Database/sqlite3.h"

class Chapter;

struct Project
{
	int id;
	std::string name;
	std::string path;
	char Summary[256];

	std::vector<Chapter*> ProjectChapters;
};

// Namespace for file system operations
namespace fs = std::filesystem;

class ProjectBrowser
{
public:

	ProjectBrowser(std::string directoryPath) : directoryPath(std::move(directoryPath))
	{
		//Load Projects from the Database
		LoadProjectsFromDatabase();
	}

	~ProjectBrowser()
	{
		// Clean up dynamically allocated memory
		for (Project* project : projects)
		{
			delete project;
		}
	}

	void Render();
	bool DeleteProject();
	void CreateNewProject(const std::string& directoryName, const std::string& fileName);
	void RenameProject(const std::string& oldProjectFolderName, const std::string& oldFileName, const std::string& newFileName, const std::string& newProjectFolderName = "");
	int CountChapterFiles();

	void LoadDefaultIcon();

	std::string RemoveExtension(const std::string& path, const std::string& ext);

	void ReloadProjects()
	{
		//LoadProjects();
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

	void SaveUpdatesToDatabase(Project* newProject);
	std::vector<Project*> projectsToSave;
	static ProjectBrowser* GetInstance();
	bool ActiveProjectChanged = false;
	bool ProjectSelected = false;


private:
	sqlite3* database;
	void LoadProjectsFromDatabase();
	void InitalSaveToDatabase(Project* newProject);

	std::string directoryPath;
	std::vector<Project*> projects;
	Project* ActiveProject;
	Chapter* ActiveChapter;
	aie::Texture texture;

	ImTextureID ProjIcon;

	static ProjectBrowser* instance;

	void LoadChapters();

	void OnProjectSelected(Project* project)
	{
		std::cout << "Selected project: " << project->name << std::endl;
		// Add your project opening logic here
		ActiveProject = project;
	}

	// Function to render a button with an image and text
	bool ImageButtonWithText(ImTextureID texture, const char* label, const ImVec2& imageSize, const ImVec2& buttonSize);

	std::vector<unsigned char> serializeProject(const Project& project) 
	{
		std::vector<unsigned char> buffer(sizeof(Project));
		std::memcpy(buffer.data(), &project, sizeof(Project));
		return buffer;
	}

	std::vector<unsigned char> serializeProjectVector(const std::vector<Project*>& vec) 
	{
		std::vector<unsigned char> buffer;
		for (const Project* project : vec) {
			std::vector<unsigned char> serializedProject = serializeProject(*project);
			buffer.insert(buffer.end(), serializedProject.begin(), serializedProject.end());
		}
		return buffer;
	}
};