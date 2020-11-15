#pragma once

#include <afxwin.h>

class GameWindow : public CFrameWnd {
private:
	float period;
	LONGLONG start;
	CD2DBrush* blackBrush;
	CD2DBitmap* sheriffBitmap;
	float sheriffX;
	float sheriffY;

public:
	DECLARE_DYNAMIC(GameWindow)

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKeyDown(UINT key, UINT repeatCount, UINT flags);
	afx_msg int OnCreate(LPCREATESTRUCT cs);
	afx_msg LRESULT OnDraw2D(WPARAM wParam, LPARAM lParam);

	void update(float delta);
	void render(float delta, CRenderTarget* renderTarget);
};
