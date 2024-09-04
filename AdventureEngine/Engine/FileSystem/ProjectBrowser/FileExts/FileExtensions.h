#pragma once
#include <string>

class FileExtensions
{
	public:
		FileExtensions() = default;
		static FileExtensions* instance;

		static FileExtensions* GetInstance()
		{
			if (instance == nullptr)
			{
				instance = new FileExtensions();
			}
			return instance;
		}

		const std::string MasterExt = ".Master";
		const std::string ChapterExt = ".Chapter";
		const std::string ChapterFolder = "Chapters";
};