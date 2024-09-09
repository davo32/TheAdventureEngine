#include "OpenGL/gl_core_4_4.h"
#include "Application.h"
#include <iostream>
#include "ImGuiSetup.h"
#include "MonitorInfo.h"
#include "stb_image.h"



// Global variables to store window size
int Application::g_WindowWidth = 800;
int Application::g_WindowHeight = 600;

CurrentScreen Application::cScreen = CurrentScreen::STARTUP;

std::string Application::AppTitleText = "Adventure Engine - No Project";
bool Application::isShuttingdown = false;
GLFWwindow* Application::window = nullptr;
int Application::UICounter = 1;

bool Application::isWindowFocused = true;

ImGuiContext* Application::context = nullptr;

// Initialize the static FontLoader instance
ImGuiIO* Application::io = nullptr;


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

	glfwWindowHint(GLFW_FLOATING, GL_FALSE);
	glfwWindowHint(GLFW_DECORATED,GL_TRUE);

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

	g_WindowWidth = mode->width;
	g_WindowHeight = mode->height;


	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Vsync
	glfwMaximizeWindow(window);

	

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
	
	
	//UImanager.StartupByIndex(2);

	SetImGuiUnityStyle();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");


	Application::io = &io;
	Globals::fontLoader->SetIO(io); //Application::fontLoader.SetIO(io);

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


		io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			//Start the ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			// Create the DockSpace
			//ImGui::DockSpaceOverViewport();
			
			CreateMainDockSpace();
			

			// Create some dockable windows
			/*ImGui::Begin("Window 1");
			ImGui::Text("This is window 1");
			ImGui::End();

			ImGui::Begin("Window 2");
			ImGui::Text("This is window 2");
			ImGui::End();*/
			//UImanager.DrawUIByIndex(1);
			//if (cScreen == CurrentScreen::STARTUP)
			//{
			//	UImanager.DrawUIByIndex(UICounter);
			//}/*
			/*else
			{
				io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
				CreateMainDockSpace();
			}*/

			//ImVec2 windowSize(Application::g_WindowWidth, Application::g_WindowHeight);
			//ImVec2 windowPos(0, 0);//((winWidth - windowSize.x) / 2, (winHeight - windowSize.y) / 2);

			//ImGui::SetNextWindowSize(windowSize);
			//ImGui::SetNextWindowPos(windowPos);

			//ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent menu bar background
			//// Push a new background color
			//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 1.0f));
			//if (ImGui::Begin("Background Window",
			//	(bool*)1,
			//	ImGuiWindowFlags_NoTitleBar
			//	| ImGuiWindowFlags_MenuBar
			//	| ImGuiWindowFlags_NoScrollWithMouse
			//	| ImGuiWindowFlags_NoScrollbar
			//	| ImGuiWindowFlags_NoResize
			//	| ImGuiWindowFlags_NoMove))
			//{
			//	//MenuBar
			//	UImanager.DrawUIByIndex(0);
			//	ImGui::PopStyleColor();
			//	
			//	UImanager.DrawUIByIndex(UICounter);



			//	ImGui::End();
			//	ImGui::PopStyleColor();
			//}

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

void Application::CreateMainDockSpace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	/*ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);*/

	//std::cout << "viewport Size: " << viewport->Size.x << " : " << viewport->Size.y << '\n';

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.10, 0.10, 0.10, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10, 0.10, 0.10, 1.0f));
	ImGui::Begin("MainDockSpace", nullptr, window_flags);
	ImGui::PopStyleColor(2);

	UImanager.DrawUIByIndex(0);

	if (cScreen == CurrentScreen::STARTUP)
	{
		//startup screen!
		UImanager.DrawUIByIndex(1);
	}

	if (cScreen == CurrentScreen::EDITOR)
	{
		//Dockign for the editor screen
		ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspace_id);

		if (ImGui::Begin("Chapter Heirachy", nullptr, ImGuiWindowFlags_NoCollapse))
		{
			eventUI->RenderChapterList();
		}
			ImGui::End();

		if (ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse))
		{
			ImGui::Text("Test Window");
		}
			ImGui::End();

		if (ImGui::Begin("Inspector",nullptr, ImGuiWindowFlags_NoCollapse))
		{
			ImGui::Text("Test Window");
		}
			ImGui::End();

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10, 0.10, 0.10, 1.0f));
		if (ImGui::Begin("Toolbar",nullptr/*, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar *//*| ImGuiWindowFlags_NoDocking*/))
		{
			ImGui::PopStyleColor();

			//ImGuiDockNode* node = ImGui::GetWindowDockNode();

			/*if (node != nullptr)
			{
				node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
				node->LocalFlags |= ImGuiDockNodeFlags_NoResize;
			}*/
		}
			ImGui::End();

		if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			eventUI->RenderEventWindow();
		}
			ImGui::End();

	}

	ImGui::End();
}

void Application::CreateStyleLayout()
{
	
}

void Application::SetupDockingLayout()
{
	// Ensure that docking is enabled
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Get the main docking space ID
	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");

	// Clear any existing layout
	ImGui::DockBuilderRemoveNode(dockspace_id); // Clear previous layout
	ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode);//ImGuiDockNodeFlags_None); // Add a new node

	// Define regions for panels
	ImGuiID dock_main_id = dockspace_id; // Root node

	ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
	ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);
	ImGuiID dock_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.30f, nullptr, &dock_main_id);
	ImGuiID dock_bottom_right_id = ImGui::DockBuilderSplitNode(dock_bottom_id, ImGuiDir_Right, 0.50f, nullptr, &dock_bottom_id);

	// Set windows to specific regions
	ImGui::DockBuilderDockWindow("Hierarchy", dock_left_id);
	ImGui::DockBuilderDockWindow("Inspector", dock_right_id);
	ImGui::DockBuilderDockWindow("Console", dock_bottom_right_id);
	ImGui::DockBuilderDockWindow("Project Explorer", dock_bottom_id);
	ImGui::DockBuilderDockWindow("Scene View", dock_main_id);

	// Commit the layout
	ImGui::DockBuilderFinish(dockspace_id);
}

void Application::SetImGuiUnityStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Base adjustments for the theme
	ImGui::StyleColorsDark();

	// Colors
	colors[ImGuiCol_Text] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);  // Light gray text
	colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);  // Dark gray background
	colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.60f);  // Subtle borders
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);  // Input background
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);  // Hovered input background
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);  // Active input background
	colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);  // Dark window title background
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);  // Active window title background
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);  // Collapsed window title background
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);  // Button background
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);  // Hovered button
	colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);  // Active button
	colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);  // Popup menu background
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);  // Menu bar background
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);  // Scrollbar background
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);  // Scrollbar grab
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);  // Hovered scrollbar grab
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);  // Active scrollbar grab
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);  // Checkmark (Unity blue color)
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);  // Slider grab
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);  // Active slider grab
	colors[ImGuiCol_Header] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);  // Header background
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);  // Hovered header background
	colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);  // Active header background

	// Tab bar colors
	colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);  // Inactive tab
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);  // Hovered tab
	colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);  // Active tab
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);  // Unfocused tab
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);  // Unfocused active tab

	// Rounding and padding (matches Unity's minimal rounded corners)
	style.FrameRounding = 4.0f;
	style.GrabRounding = 4.0f;
	style.WindowRounding = 6.0f;
	style.WindowPadding = ImVec2(0.0f, 0.0f);
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
}



