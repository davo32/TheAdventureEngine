#pragma once
#include "imgui.h"
#include  <string>
#include "Event.h"

class Widget
{
public:
	Widget(Event* newEvent) : StoredEvent(newEvent) { }
	~Widget() { delete StoredEvent; }
	void DrawWidget();
	Event* GetStoredEvent() { return StoredEvent; }
	void SetIsActive(bool check) { isActive = check; }
private:
	Event* StoredEvent;
	bool isActive = false;
};