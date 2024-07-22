#include "gl_core_4_4.h"
#include "Application.h"
#include <iostream>
#include "Gizmos.h"
#include "ImGuiSetup.h"
#include "MonitorInfo.h"

bool Application::isShuttingdown = false;
int Application::UICounter = 1;

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

	window = glfwCreateWindow(MonitorInfo::GetMode()->width, MonitorInfo::GetMode()->height, "Adventure Engine", nullptr, nullptr);

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

	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	


	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// update IMGUI input flags
	ImGuiIO& io = ImGui::GetIO();

	//TEST
	//UImanager.GetEventUI()->Test();
	////////////////////////////////
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
	if (ImGui::Begin("Background Window", (bool*)1, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar))
	{
		//MenuBar
		UImanager.DrawUIByIndex(0);
		UImanager.DrawUIByIndex(UICounter);
	}
	ImGui::EndChild();
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
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




