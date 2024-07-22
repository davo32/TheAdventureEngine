#pragma once
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "UISetup.h"

class Application
{
public:
	bool glfwSetup();
	bool startup();
	void update();
	void draw();
	void shutdown();
	void TerminateWindow();
	bool shutdownState() { return isShuttingdown; }
	static void SetShutdownState(bool check) { isShuttingdown = check; }
	
	static int UICounter;

private: //GLFW
	GLFWwindow* window;
	const GLFWvidmode* mode;
	GLFWmonitor* primaryMonitor;

	static bool isShuttingdown;

private: //UI
	MenuBar menuBar;
	UIManager UImanager;
};

