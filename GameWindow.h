#pragma once

#include "Framework.h"
#include <chrono>

class GameWindow : public CFrameWnd {
private:
	std::chrono::high_resolution_clock::time_point start;
	Gdiplus::Bitmap* sheriffBitmap;
	Gdiplus::REAL sheriffX;
	Gdiplus::REAL sheriffY;

protected:
	DECLARE_DYNAMIC(GameWindow)
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT cs);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();

private:
	void render(Gdiplus::Graphics& graphics, float delta);
};
