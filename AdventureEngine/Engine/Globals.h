#pragma once
#include "FileSystem/ProjectBrowser/ProjectBrowser.h"
#include "FileSystem/ProjectBrowser/FileExts/FileExtensions.h"
#include "Loaders/FontLoader/FontLoader.h"

namespace Globals
{
	static ProjectBrowser* projectBrowser = ProjectBrowser::GetInstance();
	static FontLoader* fontLoader = FontLoader::GetInstance();
	static FileExtensions* FileExts = FileExtensions::GetInstance();
}
