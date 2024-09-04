#pragma once
#include "imgui.h"
#include  <string>
#include "Events/Event.h"

class Widget
{
public:
	Widget(Event* newEvent) : StoredEvent(newEvent) { }
	~Widget() { delete StoredEvent; }
	void DrawWidget();
	Event* GetStoredEvent() { return StoredEvent; }
	void SetIsActive(bool check) { isActive = check; }
	bool isClicked = false;
private:
	Event* StoredEvent;
	bool isActive = false;
	
};