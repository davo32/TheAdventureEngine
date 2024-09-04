#pragma once
#include "UserInterface.h"
#include "MonitorInfo.h"
#include "ButtonID.h"
#include "AccessFileSystem.h"
#include "Globals.h"

class Application;

class MenuBar : public UserInterface
{
public:
	void FileMenu();
	void ProjectMenu();
	void Shortcuts();
	void DrawUI() override;
private:
	ButtonID buttonID;
	AccessFileSystem fileSystem;
};

