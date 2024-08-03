#pragma once
#include "imgui.h"
#include <vector>
#include <string>
#include "CmdList.h"

class Console
{
public:
	Console()
	{
		RegisterCommands();
	};
	void RenderConsole();
	void ProcessCommand(const char* command);
	void ClearLog();
	void RegisterCommands();
	bool ShowConsole = false;

private:
	std::vector<std::string> logs;
	CmdList cmdList;
};

