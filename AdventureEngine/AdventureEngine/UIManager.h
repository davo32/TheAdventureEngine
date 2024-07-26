#pragma once
#include "UserInterface.h"
#include "EventsUI.h"
#include <vector>
#include "UIScenes.h"


class UIManager
{
public:
	UIManager() 
	{
		UI.push_back(UIscenes.menuBar);
		UI.push_back(UIscenes.startScreenUI);
		UI.push_back(UIscenes.eventsUI);
	}
	void DrawUIByIndex(int index) { UI[index]->DrawUI(); }
	EventsUI* GetEventUI() { return UIscenes.eventsUI; }

	
private:
	std::vector<UserInterface*> UI;
	UIScenes UIscenes;
	
};

