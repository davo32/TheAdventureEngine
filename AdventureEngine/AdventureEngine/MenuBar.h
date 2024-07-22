#pragma once
#include "UserInterface.h"
class Application;

class MenuBar : public UserInterface
{
public:
	void FileMenu();
	void PlayerMenu();
	void InventoryMenu();
	void EventsMenu();
	void ItemMenu();
	void DrawUI() override;
};

