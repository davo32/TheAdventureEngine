#include "OpenGL/gl_core_4_4.h"
#include "LauncherApp.h"
#include <iostream>
#include "ImGuiSetup.h"
#include "MonitorInfo.h"
#include "stb_image.h"



// Global variables to store window size
int Launcher::g_WindowWidth = 800;
int Launcher::g_WindowHeight = 600;

std::string Launcher::AppTitleText = "Adventure Launcher";
bool Launcher::isShuttingdown = false;
GLFWwindow* Launcher::window = nullptr;
int Launcher::UICounter = 1;

bool Launcher::isWindowFocused = true;

ImGuiContext* Launcher::context = nullptr;

// Initialize the static FontLoader instance
ImGuiIO* Launcher::io = nullptr;


bool Launcher::glfwSetup()
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

	glfwWindowHint(GLFW_FLOATING, GL_FALSE);
	glfwWindowHint(GLFW_DECORATED,GL_TRUE);

	window = glfwCreateWindow(
		1000,
		800,
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

	g_WindowWidth = mode->width;
	g_WindowHeight = mode->height;


	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Vsync

	

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cerr << "Failed to load the OpenGL functions\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	//// Load the project icon
	//GLFWimage icons[1];
	//icons[0].pixels = stbi_load("../Resources/Images/file.png", &icons[0].width, &icons[0].height, 0, 4);
	//if (icons[0].pixels)
	//{
	//	glfwSetWindowIcon(window, 1, icons);
	//	stbi_image_free(icons[0].pixels);
	//}

	context = ImGui::CreateContext();
	
	ImGuiIO& io = ImGui::GetIO();
	//Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
	UImanager.StartupByIndex(2);

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");


	Launcher::io = &io;
	Globals::fontLoader->SetIO(io); //Launcher::fontLoader.SetIO(io);

	if (!Globals::fontLoader->LoadFont("../Resources/Fonts/roboto/Roboto-Regular.ttf", 12.0f, "Regular"))
	{
		std::cerr << "Failed to load Regular font\n";
	}

	if (!Globals::fontLoader->LoadFont("../Resources/Fonts/roboto/Roboto-Bold.ttf", 13.0f, "Bold"))
	{
		std::cerr << "Failed to load Bold font\n";
	}

	if (!Globals::fontLoader->LoadFont("../Resources/Fonts/NotoSans/NotoSans_Condensed-SemiBold.ttf", 30.0f, "NSBold"))
	{
		std::cerr << "Failed to load Bold font\n";
	}

	if (!Globals::fontLoader->LoadFont("../Resources/Fonts/NotoSans/NotoSans_Condensed-Regular.ttf", 26.0f, "NSReg"))
	{
		std::cerr << "Failed to load Bold font\n";
	}

	if (!Globals::fontLoader->LoadFont("../Resources/Fonts/NotoSans/NotoSans_Condensed-Regular.ttf", 18.0f, "NSRegSmall"))
	{
		std::cerr << "Failed to load Bold font\n";
	}
	Globals::projectBrowser->LoadDefaultIcon();
	Globals::projectBrowser->LoadProjectsFromDatabase();

	return true;
}

bool Launcher::startup()
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

void Launcher::update()
{
	if (isWindowFocused)
	{
		glfwPollEvents();
	}
}

void Launcher::draw()
{
	if (!glfwGetWindowAttrib(window, GLFW_ICONIFIED))
	{
		int winWidth, winHeight;
		glfwGetWindowSize(window, &winWidth, &winHeight);

		glClearColor(0.08f, 0.08f, 0.08f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Start the ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(1000, 800));
			if(ImGui::Begin("##Blank",nullptr, ImGuiWindowFlags_NoTitleBar
					| ImGuiWindowFlags_NoScrollWithMouse
					| ImGuiWindowFlags_NoScrollbar
					| ImGuiWindowFlags_NoResize
					| ImGuiWindowFlags_NoMove))
			{
				/*Globals::projectBrowser->Render();*/


				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 start(0, 700);
				ImVec2 end(ImGui::GetContentRegionAvail().x, 700);
				drawList->AddLine(start, end, ImColor(255, 255, 255, 110),2.5f);


				ImGui::SetNextWindowPos(ImVec2(0, 700));
				if(ImGui::BeginChild("#BoTbar",ImVec2(ImGui::GetContentRegionAvail().x,200)))
				{
					ImGui::Text("Text");
					ImGui::EndChild();
				}

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



void Launcher::shutdown()
{
	TerminateWindow();
}

void Launcher::TerminateWindow()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}




