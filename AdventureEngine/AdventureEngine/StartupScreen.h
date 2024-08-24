#pragma once
#include "UserInterface.h"
#include "Texture.h"
#include "ProjectBrowser.h"


class StartupScreen : public UserInterface
{
public:
	StartupScreen()
	{
		projectBrowser = new ProjectBrowser("../UserProjects");
	}
	void DrawUI() override;
	



private:
	aie::Texture Logo;
	ProjectBrowser* projectBrowser;
	
	void SetupMainWindow(const ImVec2& windowSize, const ImVec2& WindowPos);

	void DrawBackground(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize);
	void DrawHeader(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize);

	void DrawLeftPanel(ImDrawList* drawList, const ImVec2& startOfArea, const ImVec2& WindowPos, const ImVec2& windowSize);
	void DrawLeftPanelButtons(const ImVec2& startOfArea, const ImVec2& WindowPos, const ImVec2& LeftPanelSize);
	void DrawCreateProjectButton(ImVec2 WindowPos, ImVec2 LeftPanelSize);
	
	void DrawCentreContent(ImDrawList* drawList, ImVec2 WindowPos, ImVec2 startOfArea);

	void DrawRightPanel(ImDrawList* drawList, const ImVec2& WindowPos, const ImVec2& windowSize);
	void DrawRightPanelContent(ImDrawList* drawList, const ImVec2& RightPanelPos, const ImVec2& RightPanelSize);
	void DrawRightPanelButtons(const ImVec2& RightPanelPos, const ImVec2& RightPanelSize);
	void DrawRightPanelHeader(ImDrawList* drawList, const ImVec2& RightPanelPos);

};

