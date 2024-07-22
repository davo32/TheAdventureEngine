#pragma once
#include "UserInterface.h"
#include <string>
#include <GLFW/glfw3.h>
#include "Widget.h"
#include <vector>

class Application;
class EventsUI : public UserInterface
{
public:
	EventsUI() : ActiveWidget(nullptr) {}
	void DrawUI() override;

	void TextCentered(std::string text);
	void HierachyElement();
	void Test();
	Widget* GetWidgetByRef(Widget* ref);
private:
	std::vector<Widget*> Buttons;
	Widget* ActiveWidget;
};

