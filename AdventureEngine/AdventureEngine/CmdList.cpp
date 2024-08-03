#include "CmdList.h"
#include <iostream>

void CmdList::RegisterCommand(std::string name, Command cmd)
{
	commands[name] = cmd;
}

void CmdList::ExecuteCommand(std::string name)
{
    auto it = commands.find(name);
    if (it != commands.end()) {
        it->second(); // Execute the command function
    }
    else {
        std::cout << "Unknown command: " << name << std::endl;
    }
}
