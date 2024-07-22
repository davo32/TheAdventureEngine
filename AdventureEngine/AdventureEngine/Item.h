#pragma once
#include <string>

class Item
{
public:
	Item(std::string Name) : ItemName(Name)
	{

	}
	std::string GetName() { return ItemName; }
private:
	std::string ItemName;
	//item stats , dynamic
};

