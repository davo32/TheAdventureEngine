#pragma once
#include "nfd.h"
#include <string.h>
#include <iostream>



class AccessFileSystem
{
public:
	AccessFileSystem() = default;
	~AccessFileSystem() = default;
	std::string OpenFileDialog();
	void OpenExplorer();
private:
	std::string FilePath;
};

