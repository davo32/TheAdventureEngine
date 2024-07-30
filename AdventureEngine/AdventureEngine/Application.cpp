#include "gl_core_4_4.h"
#include "Application.h"
#include <iostream>
#include "ImGuiSetup.h"
#include "MonitorInfo.h"
#include "stb_image.h"

std::string Application::AppTitleText = "Adventure Engine - No Project";
bool Application::isShuttingdown = false;
GLFWwindow* Application::window = nullptr;
int Application::UICounter = 1;

// Initialize the static FontLoader instance
ImGuiIO* Application::io = nullptr;
FontLoader& Application::fontLoader = FontLoader::GetInstance();

bool Application::glfwSetup()
{
	if (!glfwInit())
	{
		return false;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	MonitorInfo::Init();

	window = glfwCreateWindow(MonitorInfo::GetMode()->width, MonitorInfo::GetMode()->height, AppTitleText.c_str(), nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cout << "Failed to load the openGL functions";
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	//Generates the Project Icon
	GLFWimage icons[1];
	icons[0].pixels = stbi_load("../Resources/Images/file.png", &icons[0].width, &icons[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, icons);
	stbi_image_free(icons[0].pixels);


	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Get the ImGuiIO object and pass it to FontLoader
	ImGuiIO& io = ImGui::GetIO();
	Application::io = &io;  // Ensure io is correctly assigned
	Application::fontLoader.SetIO(io);

	// Load fonts
	if (!Application::fontLoader.LoadFont("../Resources/Fonts/roboto/Roboto-Regular.ttf", 12.0f, "Regular"))
	{
		std::cerr << "Failed to load Regular font\n";
	}

	if (!Application::fontLoader.LoadFont("../Resources/Fonts/roboto/Roboto-Bold.ttf", 13.0f, "Bold"))
	{
		std::cerr << "Failed to load Bold font\n";
	}

	/*if (!Application::fontLoader.LoadFont("../Resources/Fonts/roboto/Roboto-Regular.ttf", 16.0f, "Regular2"))
	{
		std::cerr << "Failed to load Regular font\n";
	}*/

	return true;
}

bool Application::startup()
{
	if (!glfwSetup())
	{
		std::cout << "Failed to run GLFW setup";
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}


	return true;
}

void Application::update()
{
		glfwPollEvents();
}

void Application::draw()
{
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (!glfwGetWindowAttrib(window, GLFW_ICONIFIED))
	{
		int winWidth, winHeight;
		glfwGetWindowSize(window, &winWidth, &winHeight);
		ImVec2 windowSize(MonitorInfo::GetMode()->width, MonitorInfo::GetMode()->height - 24);
		ImVec2 windowPos((winWidth - windowSize.x) / 2, (winHeight - windowSize.y) / 2 );

	
		//Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(windowSize);
		ImGui::SetNextWindowPos(windowPos);
		if (ImGui::Begin("Background Window",
			(bool*)1,
			ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_MenuBar
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove))
		{
			//MenuBar
			UImanager.DrawUIByIndex(0);
			UImanager.DrawUIByIndex(UICounter);
			ImGui::End();
		}


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
}



void Application::shutdown()
{
	TerminateWindow();
}

void Application::TerminateWindow()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}




