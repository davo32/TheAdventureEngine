#pragma once
#include "UserInterface.h"
#include "Texture.h"

class StartupScreen : public UserInterface
{
public:
	void DrawUI() override;
	void OverviewTab();
	void GettingStartedTab();
private:
	aie::Texture Logo;
};

