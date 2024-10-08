#include "pch.h"
#include "ProjectBrowser.h"
#include "Core/Chapter/Chapter.h"
#include "Globals.h"

// Define the static instance variable
ProjectBrowser* ProjectBrowser::instance = nullptr;

// Static method to get the singleton instance
ProjectBrowser* ProjectBrowser::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new ProjectBrowser("../UserProjects");
    }
    return instance;
}


//
//
//bool ProjectBrowser::insertProjectData( const std::vector<uint8_t>& serializedData)
//{
//	sqlite3* db = database;
//	sqlite3_stmt* stmt;
//	const char* sql = "INSERT INTO UserProjects (Project Binary) VALUES (?)";
//
//	// Prepare the SQL statement
//	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//		std::cerr << "Failed to prepare SQL statement: " << sqlite3_errmsg(db) << std::endl;
//		return false;
//	}
//
//	// Bind the serialized data as a BLOB
//	if (sqlite3_bind_blob(stmt, 1, serializedData.data(), serializedData.size(), SQLITE_STATIC) != SQLITE_OK) {
//		std::cerr << "Failed to bind BLOB: " << sqlite3_errmsg(db) << std::endl;
//		sqlite3_finalize(stmt);
//		return false;
//	}
//
//	// Execute the statement
//	if (sqlite3_step(stmt) != SQLITE_DONE) {
//		std::cerr << "Failed to execute SQL statement: " << sqlite3_errmsg(db) << std::endl;
//		sqlite3_finalize(stmt);
//		return false;
//	}
//
//	// Finalize the statement
//	sqlite3_finalize(stmt);
//	return true;
//}
//
//bool ProjectBrowser::getProjectData(int id, std::vector<uint8_t>& serializedData)
//{
//	sqlite3* db = database;
//	sqlite3_stmt* stmt;
//	const char* sql = "SELECT Project Binary FROM Projects WHERE id = ?";
//
//	// Prepare the SQL statement
//	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//		std::cerr << "Failed to prepare SQL statement: " << sqlite3_errmsg(db) << std::endl;
//		return false;
//	}
//
//	// Bind the ID parameter
//	if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK) {
//		std::cerr << "Failed to bind ID: " << sqlite3_errmsg(db) << std::endl;
//		sqlite3_finalize(stmt);
//		return false;
//	}
//
//	// Execute the statement
//	if (sqlite3_step(stmt) == SQLITE_ROW) {
//		const void* blob = sqlite3_column_blob(stmt, 0);
//		int blobSize = sqlite3_column_bytes(stmt, 0);
//
//		serializedData.assign(static_cast<const uint8_t*>(blob), static_cast<const uint8_t*>(blob) + blobSize);
//	}
//	else {
//		std::cerr << "No data found for the given ID" << std::endl;
//		sqlite3_finalize(stmt);
//		return false;
//	}
//
//	// Finalize the statement
//	sqlite3_finalize(stmt);
//	return true;
//}
//
//std::vector<int> ProjectBrowser::getAllProjectIds()
//{
//	std::vector<int> ids;
//	sqlite3* db = database;
//	sqlite3_stmt* stmt;
//	const char* sql = "SELECT id FROM UserProjects"; // SQL to fetch all project IDs
//
//	// Prepare the SQL statement
//	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//		std::cerr << "Failed to prepare SQL statement: " << sqlite3_errmsg(db) << std::endl;
//		return ids;
//	}
//
//	// Execute the statement and fetch results
//	while (sqlite3_step(stmt) == SQLITE_ROW) {
//		int id = sqlite3_column_int(stmt, 0);
//		ids.push_back(id);
//	}
//
//	// Finalize the statement
//	sqlite3_finalize(stmt);
//	return ids;
//}

//std::vector<Project*> ProjectBrowser::LoadProjects()
//{
//	projects.clear();
//	
//	// Fetch all project IDs
//	std::vector<int> projectIds = getAllProjectIds();
//
//	for (int id : projectIds) {
//		std::vector<uint8_t> serializedData;
//		if (getProjectData(id, serializedData)) {
//			Project* project = deserializeProject(serializedData);
//			if (project) {
//				projects.push_back(project);
//			}
//			else {
//				std::cerr << "Failed to deserialize project with ID " << id << std::endl;
//			}
//		}
//		else {
//			std::cerr << "Failed to retrieve project data for ID " << id << std::endl;
//		}
//	}
//
//	return projects;
//}


//void ProjectBrowser::Render()
//{
//	// Define parameters for the list-style buttons
//	const int columns = 1; // Single column for the list layout
//	const float buttonHeight = 50.0f; // Height of each button
//	const float spacing = 5.0f; // Spacing between buttons
//
//	// Calculate window size and start position
//	ImVec2 windowPos = ImGui::GetCursorPos();
//	float startX = windowPos.x + 10;
//	float startY = windowPos.y;
//
//	// Calculate button width based on window size (or set a fixed width)
//	float buttonWidth = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2;
//
//	// Iterate over projects to create buttons
//	for (size_t i = 0; i < projects.size(); ++i)
//	{
//		// Calculate button position based on index
//		int row = static_cast<int>(i / columns);
//		ImVec2 buttonPos = ImVec2(startX, startY + row * (buttonHeight + spacing));
//
//		// Set cursor position for the button
//		ImGui::SetCursorPos(buttonPos);
//
//		// Create an invisible button to hold the background and the text separately
//		if (ImGui::InvisibleButton("button_background", ImVec2(buttonWidth, buttonHeight)))
//		{
//			if (projectBrowser->GetActiveProject() != nullptr)
//			{
//				Application::cScreen = CurrentScreen::EDITOR;
//				Application::SetTitleText("Adventure Engine - " + projectBrowser->GetActiveProject()->name);
//				Application::UICounter = 2; // Safely change the state after ImGui processing is complete
//			}
//		}
//
//		// Check if the button is hovered or active
//		bool isHovered = ImGui::IsItemHovered();
//		bool isActive = (ActiveProject != nullptr && strcmp(ActiveProject->name.c_str(), projects[i]->name.c_str()) == 0);
//
//		// Draw a background for the button with different colors based on the hover state
//		ImDrawList* drawList = ImGui::GetWindowDrawList();
//		ImVec2 min = ImGui::GetItemRectMin();
//		ImVec2 max = ImGui::GetItemRectMax();
//
//		// Default background color
//		ImU32 bgColor = IM_COL32(0, 0, 0, 0); // Dark color
//
//		// Lighter background color on hover
//		if (isHovered)
//		{
//			bgColor = IM_COL32(50, 50, 50, 110); // Lighter color when hovered
//		}
//
//		// Even lighter color if active
//		if (isActive)
//		{
//			bgColor = IM_COL32(80, 80, 80, 110); // Highlight the active project
//		}
//
//		// Draw the background rectangle with rounded corners
//		drawList->AddRectFilled(min, max, bgColor, 8.0f); // 8.0f for rounded corners
//
//		// Manually set the text's cursor position relative to the button
//		ImGui::SetCursorScreenPos(ImVec2(min.x + ImGui::GetStyle().ItemSpacing.x, min.y + (buttonHeight - ImGui::CalcTextSize(projects[i]->name.c_str()).y) / 2));
//
//		// Draw the project name inside the button
//		ImGui::TextUnformatted(projects[i]->name.c_str());
//	}
//}



//updated for database
void ProjectBrowser::CreateNewProject(const std::string& directoryName, const std::string& fileName)
{

	Project* newProject = new Project();
	newProject->name = fileName;
	strcpy_s(newProject->Summary, "Example Summary");
	newProject->ProjectChapters.push_back(new Chapter("Example Chapter"));

	projects.push_back(newProject);

	InitalSaveToDatabase(newProject);
	
}

void ProjectBrowser::InitalSaveToDatabase(Project* newProject)
{

	sqlite3* db = database;
	sqlite3_stmt* stmt;

	int rc = sqlite3_open("UserProjects.db", &db);
	if (rc)
	{
		std::cerr << "Cannot open Database: " << sqlite3_errmsg(db) << '\n';
		return;
	}

	// Prepare the SQL statement for inserting the project
	const char* sqlInsert = "INSERT INTO UserProjects (name, summary, chapters) VALUES (?, ?, ?);";
	rc = sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr);
	if (rc != SQLITE_OK) 
	{
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}

	// Bind the project name, summary to the statement
	sqlite3_bind_text(stmt, 1, newProject->name.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, newProject->Summary, -1, SQLITE_TRANSIENT);

	// Serialize the vector of Chapter* into binary data
	std::vector<uint8_t> serializedData;
	for (Chapter* chapter : newProject->ProjectChapters) 
	{
		// Serialize each Chapter object (this requires that Chapter has a proper serialization method)
		std::vector<uint8_t> serializedChapter = chapter->serialize(); // Assuming Serialize returns a std::string with binary data
		serializedData.insert(serializedData.end(), serializedChapter.begin(), serializedChapter.end());
	}

	// Bind the serialized data as a BLOB
	sqlite3_bind_blob(stmt, 3, serializedData.data(), static_cast<int>(serializedData.size()), SQLITE_TRANSIENT);

	// Execute the statement
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) 
	{
		std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
	}
	else
	{
		std::cout << "Project inserted successfully!" << std::endl;
	}

	// Finalize the statement and close the connection
	sqlite3_finalize(stmt);
	sqlite3_close(db);

}

void ProjectBrowser::SaveUpdatesToDatabase(Project* newProject)
{
	for (int i = 0; i < projects.size();i++)
	{
		if (newProject == projects[i])
		{
			newProject->id = i + 1;
			break;
		}
	}
	// Get the database connection
	sqlite3* db = database;
	sqlite3_stmt* stmt;

	int rc1 = sqlite3_open("UserProjects.db", &db);
	if (rc1) {
		std::cerr << "Cannot open Database: " << sqlite3_errmsg(db) << '\n';
		return;
	}

	// Serialize the vector of Chapter* into binary data
	std::vector<uint8_t> serializedData;
	for (Chapter* chapter : newProject->ProjectChapters) {
		std::vector<uint8_t> serializedChapter = chapter->serialize();
		serializedData.insert(serializedData.end(), serializedChapter.begin(), serializedChapter.end());
	}

	// Prepare the SQL statement for replacing the project
	const char* sqlReplace = "INSERT OR REPLACE INTO UserProjects (id, name, summary, chapters) VALUES (?, ?, ?, ?);";
	int rc = sqlite3_prepare_v2(db, sqlReplace, -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}

	// Bind the ID, name, summary, and chapters to the statement
	sqlite3_bind_int(stmt, 1, newProject->id); // Assuming newProject->id is already set
	sqlite3_bind_text(stmt, 2, newProject->name.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, newProject->Summary, -1, SQLITE_TRANSIENT);
	sqlite3_bind_blob(stmt, 4, serializedData.data(), static_cast<int>(serializedData.size()), SQLITE_TRANSIENT);

	// Execute the statement
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) 
	{
		std::cerr << "Failed to replace data: " << sqlite3_errmsg(db) << std::endl;
	}
	else 
	{
		std::cout << "Project replaced successfully!" << std::endl;
	}

	// Finalize the statement and close the connection
	sqlite3_finalize(stmt);
	sqlite3_close(db);

}

void ProjectBrowser::LoadProjectsFromDatabase()
{
	sqlite3* db = database;
	sqlite3_stmt* stmt;

	int rc = sqlite3_open("UserProjects.db", &db);
	if (rc) {
		std::cerr << "Cannot open Database: " << sqlite3_errmsg(db) << '\n';
		return;
	}

	// Prepare the SQL statement to select all rows
	const char* sqlSelect = "SELECT name, summary, chapters FROM UserProjects;";
	rc = sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}

	// Execute the query and iterate through the results
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		const char* summary = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		const void* blobData = sqlite3_column_blob(stmt, 2);
		int blobSize = sqlite3_column_bytes(stmt, 2);

		// Create a new Project object
		Project* project = new Project();
		project->name = std::string(name);
		strcpy_s(project->Summary, summary);

		// Deserialize the chapters from the BLOB data
		std::vector<uint8_t> serializedData((uint8_t*)blobData, (uint8_t*)blobData + blobSize);

		// Assume you have a method to deserialize a Chapter from a byte vector
		size_t offset = 0;
		while (offset < serializedData.size()) {
			Chapter* chapter = new Chapter();
			offset = chapter->deserialize(serializedData, offset); // Deserialize chapter and update offset
			project->ProjectChapters.push_back(chapter);
		}

		// Add the project to the list of loaded projects
		projects.push_back(project);
	}

	if (rc != SQLITE_DONE) {
		std::cerr << "Failed to read data: " << sqlite3_errmsg(db) << std::endl;
	}

	// Finalize the statement and close the connection
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}


bool ProjectBrowser::DeleteProject(std::string name)
{
	sqlite3* db = database;
	const char* sqlDelete = "DELETE FROM UserProjects WHERE name = ?;";
	sqlite3_stmt* stmt;

	int rc1 = sqlite3_open("UserProjects.db", &db);
	if (rc1) {
		std::cerr << "Cannot open Database: " << sqlite3_errmsg(db) << '\n';
		return false;
	}

	// Prepare the SQL statement
	if (sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
		return false;
	}

	// Bind the name value (which is a string) to the SQL statement
	if (sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
		std::cerr << "Failed to bind name: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return false;
	}

	// Execute the DELETE statement
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::cerr << "Failed to delete row: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return false;
	}

	// Finalize the statement
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	for (int i = 0; i < projects.size(); i++)
	{
		if (projects[i]->name == name)
		{
			projects.erase(projects.begin() + i);
			break;
		}
	}

	ActiveProject = nullptr;
	return true;
}

void ProjectBrowser::RenameProject(const std::string& oldProjectFolderName, const std::string& oldFileName, const std::string& newFileName, const std::string& newProjectFolderName)
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
			+ RemoveExtension(newFileName, Globals::FileExts->MasterExt)
			+ "\\" + newFileName;

		GetActiveProject()->path = JSONPath;
		GetActiveProject()->name = RemoveExtension(newFileName, Globals::FileExts->MasterExt);

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

int ProjectBrowser::CountChapterFiles()
{
	return ActiveProject->ProjectChapters.size();
}

void ProjectBrowser::LoadDefaultIcon()
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

std::string ProjectBrowser::RemoveExtension(const std::string& path, const std::string& ext)
{
	// Check if the path ends with the extension
	if (path.size() >= ext.size() && path.compare(path.size() - ext.size(), ext.size(), ext) == 0)
	{
		return path.substr(0, path.size() - ext.size());
	}
	return path;
}


void ProjectBrowser::LoadChapters()
{
	// Iterate over each project in the projects list
	for (Project* P : projects)
	{
		// Clear existing chapters for the project
		P->ProjectChapters.clear();

		// Convert project name and chapter folder to std::filesystem::path
		std::filesystem::path projectPath(P->name);
		std::filesystem::path chapterFolder("Chapters");

		// Construct the path to the chapters folder
		std::filesystem::path chaptersPath = directoryPath / projectPath / chapterFolder;


		// Check if the directory exists
		if (std::filesystem::exists(chaptersPath) && std::filesystem::is_directory(chaptersPath))
		{
			// Iterate over each file in the chapters directory
			for (const auto& entry : std::filesystem::directory_iterator(chaptersPath))
			{
				if (entry.is_regular_file())
				{
					// Get the file path
					std::filesystem::path filePath = entry.path();

					// Check if the file has a .Chapter extension
					if (filePath.extension() == ".Chapter")
					{
						// Get the file name without extension
						std::string fileNameWithoutExtension = filePath.stem().string();

						// Open the chapter file
						std::ifstream file(filePath);
						if (file.is_open())
						{
							// Read the file content
							std::stringstream buffer;
							buffer << file.rdbuf();
							std::string fileContent = buffer.str();

							// Create a new Chapter object
							Chapter* newChapter = new Chapter(fileNameWithoutExtension);
							//newChapter.LoadFromContent(fileContent);  // Assuming Chapter class has a method to load from content

							// Add the chapter to the project
							P->ProjectChapters.push_back(newChapter);
						}
						else
						{
							// Handle file open error
							std::cerr << "Failed to open chapter file: " << filePath << std::endl;
						}
					}
				}
			}
		}
		else
		{
			// Handle the case where the chapters directory doesn't exist
			std::cerr << "Chapters directory does not exist for project: " << P->name << std::endl;
		}
	}
}

// Function to render a button with an image and text
//bool ProjectBrowser::ImageButtonWithText(ImTextureID texture, const char* label, const ImVec2& imageSize, const ImVec2& buttonSize)
//{
//	ImGui::BeginGroup(); // Start a new group for the button
//	ImDrawList* drawList = ImGui::GetWindowDrawList();
//
//	// Calculate positions
//	ImVec2 cursorPos = ImGui::GetCursorPos();  // Current cursor position in the window
//
//	// Define positions for the image and text
//	ImVec2 imagePos = ImVec2(cursorPos.x + ImGui::GetStyle().ItemSpacing.x, cursorPos.y + (buttonSize.y - imageSize.y) / 2);  // Center the image vertically
//	ImVec2 textPos = ImVec2(cursorPos.x + imageSize.x + ImGui::GetStyle().ItemSpacing.x * 2, cursorPos.y + (buttonSize.y - ImGui::CalcTextSize(label).y) / 2); // Center the text vertically, next to the image
//
//	// Create the invisible button
//	ImGui::InvisibleButton("list_button", buttonSize);
//	bool isHovered = ImGui::IsItemHovered();
//	bool isClicked = ImGui::IsItemClicked();
//
//	// Define the button's min and max coordinates
//	ImVec2 min = cursorPos;
//	ImVec2 max = ImVec2(cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y);
//	float rounding = 8.0f; // Rounding for the corners of the button
//
//	// Colors for the button background
//	ImColor bgColor = IM_COL32(20, 20, 20, 110);  // Default background color
//	ImColor hoveredColor = IM_COL32(50, 50, 50, 180); // Background color when hovered
//	ImColor activeColor = IM_COL32(5, 5, 5, 110); // Highlight color for the active project
//
//	// Determine background color based on hover state
//	if (isHovered)
//	{
//		bgColor = hoveredColor;
//	}
//
//	// Highlight the active project
//	if (ActiveProject != nullptr && strcmp(ActiveProject->name.c_str(), label) == 0)
//	{
//		bgColor = activeColor; // Set to highlight color if this is the active project
//	}
//
//	// Draw the rounded rectangle background
//	drawList->AddRectFilled(min, max, bgColor, rounding);
//
//	// Optional: Draw a border around the button if hovered
//	if (isHovered)
//	{
//		drawList->AddRect(min, max, IM_COL32(255, 255, 255, 255), rounding); // White border when hovered
//	}
//
//	// Draw the image on the left side
//	ImGui::SetCursorScreenPos(imagePos);
//	ImGui::Image(texture, imageSize);
//
//	// Draw the project name text on the right side of the image
//	ImGui::SetCursorScreenPos(textPos);
//	ImGui::TextUnformatted(label);  // Draw the label without formatting
//
//	ImGui::EndGroup(); // End the button group
//
//	return isClicked; // Return true if the button was clicked
//}