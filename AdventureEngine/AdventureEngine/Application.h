#pragma once
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "UISetup.h"
#include "FontLoader.h"
#include "Console.h"



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
	static FontLoader& fontLoader; // Static instance of FontLoader
	static Console* console;

private: //GLFW
	static GLFWwindow* window;
	const GLFWvidmode* mode;
	GLFWmonitor* primaryMonitor;
	static void window_focus_callback(GLFWwindow* window, int focused);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	

	static bool isShuttingdown;
	

private: //UI
	MenuBar menuBar;
	UIManager UImanager;
	static bool isWindowFocused;
	static std::string AppTitleText;
	static ImGuiIO* io; // Pointer to ImGuiIO
	static ImGuiContext* context;

	
};

