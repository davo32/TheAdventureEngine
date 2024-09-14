#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <vector>
#include "Core/Chapter/Chapter.h"
#include "UserInterface.h"
#include "InspectorType.h"
#include "ChapterEditor.h"
#include "Globals.h"

enum GMode { EDIT, PLAY };

class Application;
class EventsUI : public UserInterface
{
public:
	EventsUI() : ActiveChapter(nullptr)
	{
		chapterEditor = new ChapterEditor();
	}
	void DrawUI() override;
	void StartupUI() override;

	void SetupInitialChapter();

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

	void RenderConsoleLog();

	//Utilities
	bool contains(std::vector<Chapter*> C, Chapter* value);
	void SetActiveChapter(Chapter* chapter);
	Chapter* GetActiveChapter() { return ActiveChapter; }

	void SetMode(GMode mode) { GameMode = mode; }
	GMode GetGMode() { return GameMode; }

	static InspectorType Itype;
	ChapterEditor* chapterEditor;
private:
	bool openComponentsList = false;

	std::vector<Chapter*> Chapters;
	std::vector<Chapter*> OpenChapters;
	Chapter* ActiveChapter;
	int selectedItem = -1;
	int activeTabIndex = -1; // Index of the currently active tab
	
	aie::Texture PlayTexture;

	GMode GameMode = GMode::EDIT;
	
};