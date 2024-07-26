#pragma once
#include "EventsUI.h"
#include "StartupScreen.h"
#include "MenuBar.h"

class UIScenes
{
public:
	UIScenes() 
	{
		menuBar = new MenuBar();
		eventsUI = new EventsUI();
		startScreenUI = new StartupScreen();
	}
	MenuBar* menuBar;
	EventsUI* eventsUI;
	StartupScreen* startScreenUI;
};

