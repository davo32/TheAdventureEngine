#pragma once
#include "UserInterface.h"
#include "MonitorInfo.h"
#include "ButtonID.h"

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
};

