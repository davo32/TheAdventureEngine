#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <vector>
#include "CurrentWindow.h"
#include "IncludeAllNodes.h"
#include "Chapter.h"
#include "UserInterface.h"
#include "InspectorType.h"


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

	//Utilities
	bool contains(std::vector<Chapter*> C, Chapter* value);
	void SetActiveChapter(Chapter* chapter);

	static InspectorType Itype;
private:
	std::vector<Chapter*> Chapters;
	std::vector<Chapter*> OpenChapters;
	Chapter* ActiveChapter;
	int selectedItem = -1;
	int activeTabIndex = -1; // Index of the currently active tab


};