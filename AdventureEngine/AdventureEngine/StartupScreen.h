#pragma once
#include "UserInterface.h"
#include "OpenGL/Texture.h"
#include "Globals.h"

class StartupScreen : public UserInterface
{
public:
	StartupScreen()
	{
	}
	void DrawUI() override;

	bool openProjectFlag = false;

private:
	aie::Texture Logo;
	char newProjectName[256] = "";
	ProjectBrowser* projectBrowser = ProjectBrowser::GetInstance();

	bool DeletionConfirmation = false;
	void Render();
	void SetupMainWindow(const ImVec2& windowSize, const ImVec2& WindowPos);

	void DrawBackground(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize);
	void DrawHeader(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize);

	void DrawLeftPanel(ImDrawList* drawList, const ImVec2& startOfArea, const ImVec2& WindowPos, const ImVec2& windowSize);
	void DrawLeftPanelButtons(const ImVec2& startOfArea, const ImVec2& WindowPos, const ImVec2& LeftPanelSize);
	void DrawCreateProjectButton(ImVec2 WindowPos, ImVec2 LeftPanelSize);

	void DrawCentreContent(ImDrawList* drawList, ImVec2 WindowPos, ImVec2 startOfArea);

	void DrawRightPanel(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize);
	void DrawRightPanelContent(ImDrawList* drawList, const ImVec2& RightPanelPos, const ImVec2& RightPanelSize);
	void DrawRightPanelButtons(ImDrawList* drawList, const ImVec2& RightPanelPos, const ImVec2& RightPanelSize);
	void DrawRightPanelHeader(ImDrawList* drawList, const ImVec2& RightPanelPos);
	void DrawDeletionConfirmation(ImDrawList* drawList, const ImVec2& Pos, const ImVec2& size);

	bool SavePrelimDataInBulk();
	void QueueProjectForSaving(Project* project);
};
