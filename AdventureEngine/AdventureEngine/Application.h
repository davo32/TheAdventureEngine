#pragma once
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "UISetup.h"
#include "FontLoader.h"

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
	static std::string GetTitleText() { return AppTitleText; }
	static void SetTitleText(std::string text) { AppTitleText = text; glfwSetWindowTitle(window, AppTitleText.c_str()); }
	static int UICounter;
	static FontLoader& fontLoader; // Static instance of FontLoader

private: //GLFW
	static GLFWwindow* window;
	const GLFWvidmode* mode;
	GLFWmonitor* primaryMonitor;
	


	static bool isShuttingdown;
	

private: //UI
	MenuBar menuBar;
	UIManager UImanager;
	static std::string AppTitleText;
	static ImGuiIO* io; // Pointer to ImGuiIO
	static ImGuiContext* context;
	
};

