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
	void OverviewTab();
	void GettingStartedTab();
private:
	aie::Texture Logo;
	ProjectBrowser* projectBrowser;
};

