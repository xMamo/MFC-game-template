#pragma once

#include "GameWindow.h"

class GameApp : CWinApp {
public:
	BOOL InitInstance() override;
	BOOL OnIdle(LONG count) override;

protected:
	DECLARE_MESSAGE_MAP()

private:
	GameWindow* gameWindow;
};

extern GameApp gameApp;
