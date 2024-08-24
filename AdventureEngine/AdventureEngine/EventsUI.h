#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <vector>
#include "CurrentWindow.h"
#include "IncludeAllNodes.h"
#include "Chapter.h"
#include "UserInterface.h"
#include "InspectorType.h"

enum GMode { EDIT, PLAY };

class Application;
class EventsUI : public UserInterface
{
public:
	EventsUI() : ActiveChapter(nullptr)
	{
		
	}
	void DrawUI() override;
	void StartupUI() override;

	void text_centered(const std::string& text);


	//Rendering
	void RenderEventWindow();
	void RenderMenuBar(const char* title);
	void RenderOverlayUI();
	void RenderGraphTabBar();

	void RenderChapterList();
	void RenderChapterInspector();

	void RenderNodeInspector();

	void RenderPlayModeToolBar();

	//Utilities
	bool contains(std::vector<Chapter*> C, Chapter* value);
	void SetActiveChapter(Chapter* chapter);

	void SetMode(GMode mode) { GameMode = mode; }
	GMode GetGMode() { return GameMode; }

	static InspectorType Itype;
private:
	std::vector<Chapter*> Chapters;
	std::vector<Chapter*> OpenChapters;
	Chapter* ActiveChapter;
	int selectedItem = -1;
	int activeTabIndex = -1; // Index of the currently active tab
	
	GMode GameMode = GMode::EDIT;
	
};