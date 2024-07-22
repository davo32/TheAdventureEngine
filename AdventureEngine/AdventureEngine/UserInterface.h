#pragma once
#include "glfw/glfw3.h"
#include "imgui.h"

class UserInterface
{
public:
	virtual void DrawUI() = 0;
	virtual ~UserInterface() = default;
};
