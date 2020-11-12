#include "GameApp.h"

GameApp theApp;

BEGIN_MESSAGE_MAP(GameApp, CWinApp)
END_MESSAGE_MAP()

BOOL GameApp::InitInstance() {
	if (CWinApp::InitInstance() == FALSE)
		return FALSE;

	if (Gdiplus::GdiplusStartup(&gdiPlusToken, &Gdiplus::GdiplusStartupInput(), NULL) != Gdiplus::Ok)
		return FALSE;

	gameWindow = new GameWindow();

	if (gameWindow->Create(AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW)), NULL) == FALSE)
		return FALSE;

	gameWindow->ShowWindow(m_nCmdShow);
	m_pMainWnd = gameWindow;

	return TRUE;
}

int GameApp::ExitInstance() {
	Gdiplus::GdiplusShutdown(gdiPlusToken);
	return CWinApp::ExitInstance();
}

BOOL GameApp::OnIdle(LONG count) {
	CWinApp::OnIdle(count);
	gameWindow->RedrawWindow();
	return TRUE;
}
