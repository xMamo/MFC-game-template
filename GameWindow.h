#pragma once

#include "Framework.h"
#include <chrono>
#include <memory>

class GameWindow : public CFrameWnd {
private:
	std::chrono::high_resolution_clock::time_point start;
	CD2DBrush* blackBrush;
	CD2DBitmap* sheriffBitmap;
	float sheriffX;
	float sheriffY;

protected:
	DECLARE_DYNAMIC(GameWindow)
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT cs);
	afx_msg LRESULT OnDraw2D(WPARAM wParam, LPARAM lParam);
};
