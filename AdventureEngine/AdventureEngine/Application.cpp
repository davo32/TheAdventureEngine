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


void Application::window_focus_callback(GLFWwindow* window, int focused)
{
	isWindowFocused = focused;
}
bool Application::glfwSetup()
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW\n";
		return false;
	}

	MonitorInfo::Init();  // Initialize MonitorInfo before using it

	const GLFWvidmode* mode = MonitorInfo::GetMode();
	if (!mode)
	{
		std::cerr << "Failed to get video mode\n";
		glfwTerminate();
		return false;
	}

	window = glfwCreateWindow(
		mode->width,
		mode->height,
		AppTitleText.c_str(),
		nullptr, // Remove the monitor parameter to not use fullscreen mode
		nullptr
	);

	if (!window)
	{
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Vsync
	/*glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);*/
	glfwMaximizeWindow(window);

	g_WindowWidth = mode->width;
	g_WindowHeight = mode->height;

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cerr << "Failed to load the OpenGL functions\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	// Load the project icon
	GLFWimage icons[1];
	icons[0].pixels = stbi_load("../Resources/Images/file.png", &icons[0].width, &icons[0].height, 0, 4);
	if (icons[0].pixels)
	{
		glfwSetWindowIcon(window, 1, icons);
		stbi_image_free(icons[0].pixels);
	}

	context = ImGui::CreateContext();
	UImanager.StartupByIndex(2);

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGuiIO& io = ImGui::GetIO();
	Application::io = &io;
	Application::fontLoader.SetIO(io);

	if (!Application::fontLoader.LoadFont("../Resources/Fonts/roboto/Roboto-Regular.ttf", 12.0f, "Regular"))
	{
		std::cerr << "Failed to load Regular font\n";
	}

	if (!Application::fontLoader.LoadFont("../Resources/Fonts/roboto/Roboto-Bold.ttf", 13.0f, "Bold"))
	{
		std::cerr << "Failed to load Bold font\n";
	}

	if (!Application::fontLoader.LoadFont("../Resources/Fonts/NotoSans/NotoSans_Condensed-SemiBold.ttf", 30.0f, "NSBold"))
	{
		std::cerr << "Failed to load Bold font\n";
	}

	if (!Application::fontLoader.LoadFont("../Resources/Fonts/NotoSans/NotoSans_Condensed-Regular.ttf", 26.0f, "NSReg"))
	{
		std::cerr << "Failed to load Bold font\n";
	}

	if (!Application::fontLoader.LoadFont("../Resources/Fonts/NotoSans/NotoSans_Condensed-Regular.ttf", 18.0f, "NSRegSmall"))
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
			ImVec2 windowPos(0, 0);//((winWidth - windowSize.x) / 2, (winHeight - windowSize.y) / 2);

			ImGui::SetNextWindowSize(windowSize);
			ImGui::SetNextWindowPos(windowPos);

			// Push a new background color
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 1.0f));
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
				ImGui::PopStyleColor();
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




