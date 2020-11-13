#include "GameWindow.h"

IMPLEMENT_DYNAMIC(GameWindow, CFrameWnd)

BEGIN_MESSAGE_MAP(GameWindow, CFrameWnd)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &GameWindow::OnDraw2D)
END_MESSAGE_MAP()

int GameWindow::OnCreate(LPCREATESTRUCT cs) {
	if (CWnd::OnCreate(cs) != 0) return -1;

	EnableD2DSupport();
	auto renderTarget = GetRenderTarget();

	blackBrush = new CD2DSolidColorBrush(renderTarget, D2D1::ColorF(D2D1::ColorF::Black));

	sheriffBitmap = new CD2DBitmap(renderTarget, IDB_SHERIFF, RT_RCDATA);
	sheriffX = 0.0F;
	sheriffY = 0.0F;

	start = std::chrono::high_resolution_clock::now();

	return 0;
}

LRESULT GameWindow::OnDraw2D(WPARAM wParam, LPARAM lParam) {
	auto now = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::duration<float>>(now - start).count();
	start = now;

	auto sheriffBitmapSize = sheriffBitmap->GetSize();

	BYTE keyState[256];

	if (GetKeyboardState(keyState) != FALSE) {
		auto dx = 0.0F;
		auto dy = 0.0F;

		if ((keyState['W'] & 0b10000000) || (keyState[VK_UP] & 0b10000000)) --dy;
		if ((keyState['A'] & 0b10000000) || (keyState[VK_LEFT] & 0b10000000)) --dx;
		if ((keyState['S'] & 0b10000000) || (keyState[VK_DOWN] & 0b10000000)) ++dy;
		if ((keyState['D'] & 0b10000000) || (keyState[VK_RIGHT] & 0b10000000)) ++dx;

		auto length = sqrtf(powf(dx, 2.0F) + powf(dy, 2.0F));

		if (length > 0.0F) {
			sheriffX += delta * ((dx / length) * sheriffBitmapSize.width * 10.0F);
			sheriffY += delta * ((dy / length) * sheriffBitmapSize.height * 10.0F);
		}
	}

	CString string;
	string.Format(TEXT("%.0f"), roundf(1.0F / delta));

	auto renderTarget = reinterpret_cast<CHwndRenderTarget*>(lParam);
	auto renderTargetSize = renderTarget->GetSize();

	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->DrawText(string, D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height), blackBrush);

	auto transform = D2D1::Matrix3x2F::Translation(roundf(sheriffX), roundf(sheriffY));
	transform = transform * D2D1::Matrix3x2F::Scale(4, 4);
	renderTarget->SetTransform(transform);

	renderTarget->DrawBitmap(
		sheriffBitmap,
		CD2DRectF(0, 0, sheriffBitmapSize.width, sheriffBitmapSize.height),
		1.0F,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
	);

	return TRUE;
}
