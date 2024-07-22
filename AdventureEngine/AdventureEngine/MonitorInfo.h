#pragma once
#include "GLFW/glfw3.h"

class MonitorInfo
{
public:
	static void Init()
	{
		primaryMonitor = glfwGetPrimaryMonitor();
		if (primaryMonitor) 
		{
			mode = glfwGetVideoMode(primaryMonitor);
		}
	}
	static const GLFWvidmode* GetMode() { return mode; }
	static GLFWmonitor* GetPrimaryMonitor() { return primaryMonitor; }
private:
	static const GLFWvidmode* mode;
	static GLFWmonitor* primaryMonitor;
};
