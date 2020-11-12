#include "GameWindow.h"
#include <string>
#include <cmath>

IMPLEMENT_DYNAMIC(GameWindow, CFrameWnd)

BEGIN_MESSAGE_MAP(GameWindow, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

int GameWindow::OnCreate(LPCREATESTRUCT cs) {
	auto result = CWnd::OnCreate(cs);
	if (result != 0) return result;

	auto sheriffResourceInfo = FindResource(NULL, MAKEINTRESOURCE(IDB_SHERIFF), RT_RCDATA);
	if (sheriffResourceInfo == NULL) return -1;

	auto sheriffResourceData = LoadResource(NULL, sheriffResourceInfo);
	if (sheriffResourceData == NULL) return -1;

	sheriffBitmap = Gdiplus::Bitmap::FromStream(SHCreateMemStream(
		static_cast<BYTE*>(LockResource(sheriffResourceData)),
		SizeofResource(NULL, sheriffResourceInfo)
	));

	CRect rect;
	GetClientRect(&rect);

	sheriffX = (rect.Width() / 8.0F - sheriffBitmap->GetWidth()) / 2.0F;
	sheriffY = (rect.Height() / 8.0F - sheriffBitmap->GetHeight()) / 2.0F;

	start = std::chrono::high_resolution_clock::now();

	return 0;
}

void GameWindow::OnDestroy() {
	delete sheriffBitmap;
	CWnd::OnDestroy();
}

void GameWindow::OnPaint() {
	auto now = std::chrono::high_resolution_clock::now();
	auto delta = now - start;
	start = now;

	CRect rect;
	GetClientRect(&rect);

	CPaintDC dc(this);
	dc.SetStretchBltMode(HALFTONE);

	CBitmap buffer;
	if (buffer.CreateCompatibleBitmap(&dc, rect.Width() / 8, rect.Height() / 8) == FALSE) return;

	CDC mdc;
	if (mdc.CreateCompatibleDC(&dc) == FALSE) return;
	if (mdc.SelectObject(buffer) == NULL) return;

	Gdiplus::Graphics graphics(mdc);
	render(graphics, std::chrono::duration_cast<std::chrono::duration<float>>(delta).count());

	dc.StretchBlt(
		rect.left,
		rect.top,
		rect.Width(),
		rect.Height(),
		&mdc,
		0,
		0,
		rect.Width() / 8,
		rect.Height() / 8,
		SRCCOPY
	);
}

void GameWindow::render(Gdiplus::Graphics& graphics, float delta) {
	BYTE keyState[256];

	if (GetKeyboardState(keyState) != FALSE) {
		Gdiplus::REAL dx = 0.0F;
		Gdiplus::REAL dy = 0.0F;

		if ((keyState['W'] & 0b10000000) || (keyState[VK_UP] & 0b10000000)) --dy;
		if ((keyState['A'] & 0b10000000) || (keyState[VK_LEFT] & 0b10000000)) --dx;
		if ((keyState['S'] & 0b10000000) || (keyState[VK_DOWN] & 0b10000000)) ++dy;
		if ((keyState['D'] & 0b10000000) || (keyState[VK_RIGHT] & 0b10000000)) ++dx;

		auto length = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));

		if (length > 0) {
			sheriffX += delta * (dx / length) * 2.0F * sheriffBitmap->GetWidth();
			sheriffY += delta * (dy / length) * 2.0F * sheriffBitmap->GetHeight();
		}
	}

	graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintSingleBitPerPixelGridFit);

	graphics.Clear(Gdiplus::Color::White);

	graphics.DrawImage(
		sheriffBitmap,
		sheriffX,
		sheriffY,
		static_cast<Gdiplus::REAL>(sheriffBitmap->GetWidth()),
		static_cast<Gdiplus::REAL>(sheriffBitmap->GetWidth())
	);
}
