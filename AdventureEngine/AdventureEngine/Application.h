#pragma once
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "UISetup.h"
#include "Console.h"
#include "Globals.h"

#include "EventsUI.h"

enum CurrentScreen
{
	STARTUP,
	EDITOR
};

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
	
	//static vars
	static int g_WindowWidth;
	static int g_WindowHeight;
	static void SetShutdownState(bool check) { isShuttingdown = check; }
	static std::string GetTitleText() { return AppTitleText; }
	static void SetTitleText(std::string text) { AppTitleText = text; glfwSetWindowTitle(window, AppTitleText.c_str()); }
	static int UICounter;
	static CurrentScreen cScreen;

private: //GLFW
	static GLFWwindow* window;
	const GLFWvidmode* mode;
	GLFWmonitor* primaryMonitor;
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	static bool isShuttingdown;
	void CreateMainDockSpace(ImVec2 size);
	void CreateStyleLayout();
	void SetupDockingLayout();
	void SetImGuiUnityStyle();

private: //UI
	MenuBar menuBar;
	UIManager UImanager;
	static bool isWindowFocused;
	static std::string AppTitleText;
	static ImGuiIO* io; // Pointer to ImGuiIO
	static ImGuiContext* context;
	EventsUI* eventUI = new EventsUI();


	//static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	
};

