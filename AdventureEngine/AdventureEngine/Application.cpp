#include "gl_core_4_4.h"
#include "Application.h"
#include <iostream>
#include "ImGuiSetup.h"
#include "MonitorInfo.h"
#include "stb_image.h"



// Global variables to store window size
int Application::g_WindowWidth = 800;
int Application::g_WindowHeight = 600;

std::string Application::AppTitleText = "Adventure Engine - No Project";
bool Application::isShuttingdown = false;
GLFWwindow* Application::window = nullptr;
int Application::UICounter = 1;

bool Application::isWindowFocused = true;

ImGuiContext* Application::context = nullptr;

// Initialize the static FontLoader instance
ImGuiIO* Application::io = nullptr;
FontLoader& Application::fontLoader = FontLoader::GetInstance();
Console* Application::console = new Console();

// Callback to handle framebuffer size changes
void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Adjust the viewport when the framebuffer size changes
	glViewport(0, 0, width, height);
}

void Application::window_focus_callback(GLFWwindow* window, int focused)
{
	isWindowFocused = focused;
}
bool Application::glfwSetup()
{
	if (!glfwInit())
	{
		return false;
	}

	//glfwWindowHint(GLFW_DECORATED, NULL); // Remove the border and titlebar..
	glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	MonitorInfo::Init();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);


	window = glfwCreateWindow
	(MonitorInfo::GetMode()->width,
		MonitorInfo::GetMode()->height,
		"Adventure Engine",
		MonitorInfo::GetPrimaryMonitor(),
		nullptr); //AppTitleText.c_str(), nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Vsync
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetWindowFocusCallback(window, window_focus_callback);


	g_WindowWidth = MonitorInfo::GetMode()->width;
	g_WindowHeight = MonitorInfo::GetMode()->height;

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


	context = ImGui::CreateContext();
	UImanager.StartupByIndex(2);
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
	if (isWindowFocused)
	{
		glfwPollEvents();
	}
}

void Application::draw()
{
	if (!glfwGetWindowAttrib(window, GLFW_ICONIFIED))
	{
		int winWidth, winHeight;
		glfwGetWindowSize(window, &winWidth, &winHeight);

		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Start the ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			ImVec2 windowSize(Application::g_WindowWidth, Application::g_WindowHeight);
			ImVec2 windowPos((winWidth - windowSize.x) / 2, (winHeight - windowSize.y) / 2);

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
				if (ImGui::IsKeyPressed(ImGuiKey_Tab) && UICounter == 2)
				{
					console->ShowConsole = !console->ShowConsole;
					std::cout << "Console: " << console->ShowConsole << '\n';
				}
				
				ImGui::End();
			}

			// Always render the console on top if it is shown
			if (console->ShowConsole)
			{
				ImGui::SetNextWindowSize(ImVec2(MonitorInfo::GetMode()->width, 500));
				ImGui::SetNextWindowPos(ImVec2(0, 0));
				ImGui::Begin("Console Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
				console->RenderConsole();
				ImGui::End();
			}

			ImGui::Render();

			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			///glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
			

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




