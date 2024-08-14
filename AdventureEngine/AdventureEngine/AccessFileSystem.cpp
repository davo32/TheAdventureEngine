#include "AccessFileSystem.h"

std::string AccessFileSystem::OpenFileDialog()
{
    nfdchar_t* outPath = nullptr;
    // Correctly escape backslashes in the file path
    const char* projectFolderPath = "C:\\REPO\\TheAdventureEngine\\AdventureEngine\\AE Projects";

    // Open the file dialog
    nfdresult_t result = NFD_OpenDialog("Nova", projectFolderPath, &outPath);
    if (result == NFD_OKAY) {
        std::string filePath(outPath);
        free(outPath);
        return filePath;
    }
    else if (result == NFD_CANCEL) {
        std::cout << "User pressed cancel." << std::endl;
    }
    else {
        std::cerr << "Error: " << NFD_GetError() << std::endl;
    }

    return "";
}

void AccessFileSystem::OpenExplorer()
{
    FilePath = OpenFileDialog();
    if (!FilePath.empty()) {
        std::cout << "Selected file: " << FilePath << std::endl;
    }
    else {
        std::cout << "No file selected." << std::endl;
    }
}
