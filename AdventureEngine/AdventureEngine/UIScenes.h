#pragma once
#include "EventsUI.h"
#include "MenuBar.h"

class UIScenes
{
public:
	UIScenes() 
	{
		menuBar = new MenuBar();
		eventsUI = new EventsUI();
	}
	MenuBar* menuBar;
	EventsUI* eventsUI;
};

