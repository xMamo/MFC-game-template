#include "GameWindow.h"
#include "Resource.h"
#include "Math.h"

IMPLEMENT_DYNAMIC(GameWindow, CFrameWnd)

BEGIN_MESSAGE_MAP(GameWindow, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
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
	sheriffZ = 0.0F;

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	period = 1.0F / frequency.QuadPart;

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	start = now.QuadPart;

	return 0;
}

void GameWindow::OnKeyDown(UINT key, UINT repeatCount, UINT flags) {
	switch (key) {
	case VK_ESCAPE:
		DestroyWindow();
		break;
	default:
		CFrameWnd::OnKeyDown(key, repeatCount, flags);
		break;
	}
}

LRESULT GameWindow::OnDraw2D(WPARAM wParam, LPARAM lParam) {
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	auto delta = period * (now.QuadPart - start);
	start = now.QuadPart;

	auto renderTarget = reinterpret_cast<CRenderTarget*>(lParam);
	update(delta);
	render(delta, renderTarget);

	return TRUE;
}

void GameWindow::update(float delta) {
	BYTE keyState[256];

	if (GetKeyboardState(keyState) != FALSE) {
		auto dx = 0.0F;
		auto dy = 0.0F;

		if ((keyState['W'] & 0x80) || (keyState[VK_UP] & 0x80)) --dy;
		if ((keyState['A'] & 0x80) || (keyState[VK_LEFT] & 0x80)) --dx;
		if ((keyState['S'] & 0x80) || (keyState[VK_DOWN] & 0x80)) ++dy;
		if ((keyState['D'] & 0x80) || (keyState[VK_RIGHT] & 0x80)) ++dx;

		auto length = sqrtf(powf(dx, 2.0F) + powf(dy, 2.0F));

		if (length > 0.0F) {
			auto sheriffBitmapSize = sheriffBitmap->GetSize();
			sheriffX += delta * ((dx / length) * sheriffBitmapSize.width * 10.0F);
			sheriffY += delta * ((dy / length) * sheriffBitmapSize.height * 10.0F);
		}
	}

	sheriffZ += delta * 30.0F;
}

void GameWindow::render(float delta, CRenderTarget* renderTarget) {
	auto renderTargetSize = renderTarget->GetSize();
	auto renderTargetWidth = renderTargetSize.width;
	auto renderTargetHeight = renderTargetSize.height;

	auto sheriffSize = sheriffBitmap->GetSize();
	auto sheriffWidth = sheriffSize.width;
	auto sheriffHeight = sheriffSize.height;

	CString string;
	string.Format(_T("%.0f"), round(1.0F / delta));

	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	auto transform = rotation(sheriffZ, D2D1::Point2F(sheriffWidth / 2.0F, sheriffHeight / 2.0F));
	transform = transform * D2D1::Matrix3x2F::Translation(round(sheriffX), round(sheriffY));
	transform = transform * D2D1::Matrix3x2F::Scale(4.0F, 4.0F);
	renderTarget->SetTransform(transform);

	renderTarget->DrawBitmap(
		sheriffBitmap,
		CD2DRectF(0.0F, 0.0F, sheriffWidth, sheriffHeight),
		1.0F,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
	);

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->DrawText(string, D2D1::RectF(0.0F, 0.0F, renderTargetWidth, renderTargetHeight), blackBrush);
}
