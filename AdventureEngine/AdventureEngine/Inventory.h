#pragma once
#include "Owner.h"
#include <vector>
#include "Item.h"

class Inventory
{
public:
	Inventory(Owner setOwner) : owner(setOwner) { }
	void SetOwnership(Owner newOwner) { owner = newOwner; }
	Item* GetInventoryItem(int index) { return InventoryItems[index]; }
private:
	Owner owner;
	std::vector<Item*> InventoryItems;
};

