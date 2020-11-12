#pragma once

#include "Framework.h"
#include "GameWindow.h"

class GameApp : CWinApp {
public:
	BOOL InitInstance() override;
	int ExitInstance() override;
	BOOL OnIdle(LONG count) override;

protected:
	DECLARE_MESSAGE_MAP()

private:
	ULONG_PTR gdiPlusToken;
	GameWindow* gameWindow;
};

extern GameApp gameApp;
