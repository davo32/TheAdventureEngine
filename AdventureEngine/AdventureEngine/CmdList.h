#pragma once
#include <functional>
#include <string>
#include <unordered_map>

using Command = std::function<void()>;

class CmdList
{
public:
	void RegisterCommand(std::string name, Command cmd);
	void ExecuteCommand(std::string name);

private:
	std::unordered_map<std::string, Command> commands;
};

