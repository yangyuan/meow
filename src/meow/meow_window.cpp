#include "meow.h"
#include "meow_window.h"


DWORD WINAPI MeowWindowManager::ThreadProc(LPVOID lpParam)
{
	MeowWindowManager * me = (MeowWindowManager *)lpParam;
	me->impl_status = new MeowStatusWindow();
	me->window_status = new MeowWindow(me->hinstance, me->impl_status, me);


	me->impl_composition = new MeowCompositionWindow();
	me->window_composition = new MeowWindow(me->hinstance, me->impl_composition, me);
	me->window_composition->WndHide();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

MeowWindowManager::MeowWindowManager(HINSTANCE _hinstance)  {
	hinstance = _hinstance;
	impl_status = NULL;
	impl_composition = NULL;
	window_status = NULL;
	skin = new MeowSkinDelegate(hinstance);
	HANDLE uithread = CreateThread(NULL, 0, MeowWindowManager::ThreadProc, this, 0, NULL);
};

VOID MeowWindowManager::ShowStatusWindow() {
	if (window_status != NULL) {
		window_status->WndShow();
	}
}
VOID MeowWindowManager::HideStatusWindow() {
	if (window_status != NULL) {
		window_status->WndHide();
	}
}


VOID MeowWindowManager::AdjustCompositionWindow(CONST RECT * rect, CONST TCHAR * text){
	if (impl_composition != NULL && window_composition != NULL) {
		if (rect != NULL) impl_composition->AdjustPosition(rect);
		if (text != NULL) {
			impl_composition->SetText(text);
			if (_tcslen(text) == 0) window_composition->WndHide();
			else window_composition->WndShow();
		}
	}
}


VOID MeowWindowManager::SetCandidate(UINT8 index, CONST TCHAR * _text) {
	impl_composition->SetCandidate(index, _text);
}

VOID MeowWindowManager::ClearCandidates() {
	impl_composition->ClearCandidates();
}

VOID MeowWindowManager::SetActiveCandidate(UINT8 index) {
	impl_composition->SetActiveCandidate(index);
}

VOID MeowWindowManager::RefreshCandidate() {
	impl_composition->RefreshCandidate();
}

// MeowWindow
MeowWindow::MeowWindow(HINSTANCE _hinstance, MeowWindowImpl * _impl, MeowWindowManager * _manager)
{
	hinstance = _hinstance;
	impl = _impl;
	manager = _manager;

	WNDCLASSEX wcex;
	TCHAR * tc = impl->GetWndClass();
	BOOL registered = GetClassInfoEx(hinstance, impl->GetWndClass(), &wcex);
	// 
	if (!registered) {
		memset(&wcex, 0, sizeof(wcex));
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_IME;
		wcex.hInstance = hinstance;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)COLOR_MENUHILIGHT;//(HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpfnWndProc = WndProc;
		wcex.lpszClassName = impl->GetWndClass();
		RegisterClassEx(&wcex);
	}

	hwnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST, impl->GetWndClass(), TEXT(""),
		WS_POPUP,
		0, 0, 0, 0,
		NULL, 0, hinstance, this);
	ShowWindow(hwnd, SW_SHOWNOACTIVATE);
}

MeowWindow::~MeowWindow()
{
}

VOID MeowWindow::WndShow() {
	if (hwnd != NULL) {
		ShowWindow(hwnd, SW_SHOWNOACTIVATE);
	}
}
VOID MeowWindow::WndHide() {
	if (hwnd != NULL) {
		ShowWindow(hwnd, SW_HIDE);
	}
}

LRESULT CALLBACK MeowWindow::ButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	MeowWindow * me = (MeowWindow *)(dwRefData);
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		SendMessage(me->hwnd, WM_LBUTTONDOWN, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		SendMessage(me->hwnd, WM_MOUSEMOVE, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		SendMessage(me->hwnd, WM_LBUTTONUP, wParam, lParam);
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MeowWindow::ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	MeowWindow * me = (MeowWindow *)(dwRefData);
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		SendMessage(me->hwnd, WM_LBUTTONDOWN, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		SendMessage(me->hwnd, WM_MOUSEMOVE, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		SendMessage(me->hwnd, WM_LBUTTONUP, wParam, lParam);
	case WM_MOUSEACTIVATE:
		return MA_NOACTIVATE;
	case WM_SETFOCUS:
		return NULL;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MeowWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (uMsg == WM_CREATE)
	{
		MeowWindow * me = (MeowWindow *)((LONG_PTR)((CREATESTRUCT *)lParam)->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)me);
		me->impl->WndInit(hwnd, me);
	}

	MeowWindow * me = (MeowWindow *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (!me) return DefWindowProc(hwnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_PAINT: {
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		me->impl->WndPaint(hwnd, me, hdc, &ps);
		EndPaint(hwnd, &ps);
	}
				   break;
	case WM_DRAWITEM: {
		if (me->impl->WndDrawItem(hwnd, me, (DRAWITEMSTRUCT*)lParam)) {
			return TRUE;
		}
	}
					  break;
	case WM_ERASEBKGND:
		return TRUE;
	case WM_LBUTTONDOWN:
		SetCapture(hwnd);
		me->cursorcache = NULL;
		//save current cursor coordinate
		GetCursorPos(&me->pointcapture);
		me->pointcaptureclient = me->pointcapture;
		ScreenToClient(hwnd, &me->pointcaptureclient);
		break;
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON) {
			GetCursorPos(&me->pointnow);
			if (me->cursorcache == NULL) {
				if ((me->pointnow.x != me->pointcapture.x) || (me->pointnow.y != me->pointcapture.y)) {
					me->cursorcache = SetCursor(LoadCursor(NULL, IDC_SIZEALL));
				}
			}
			{
				POINT point;
				point.x = me->pointnow.x - me->pointcaptureclient.x;
				point.y = me->pointnow.y - me->pointcaptureclient.y;
				WndSetPos(me->hwnd, &point);
			}
		}
		break;
	case WM_LBUTTONUP:
		if (me->cursorcache != NULL) {
			SetCursor(me->cursorcache);
			me->cursorcache = NULL;
		}
		ReleaseCapture();

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}


RECT MeowWindow::rect;
RECT MeowWindow::rcwork;
VOID MeowWindow::WndSetPos(HWND hWnd, CONST POINT * point) {
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcwork, FALSE);
	GetWindowRect(hWnd, &rect);

	rect.right = rect.right - rect.left; // width
	rect.bottom = rect.bottom - rect.top; // height
	rect.left = point->x; // x
	rect.top = point->y; // y

	if (rect.left < rcwork.left) rect.left = rcwork.left;
	if (rect.top < rcwork.top) rect.top = rcwork.top;
	if ((rect.left + rect.right) > rcwork.right) rect.left = rcwork.right - rect.right;
	if ((rect.top + rect.bottom) > rcwork.bottom) rect.top = rcwork.bottom - rect.bottom;

	SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
}

TCHAR * MeowWindowImpl::GetWndClass() {
	return wndclass;
}

VOID MeowWindowImpl::WndInit(HWND hwnd, MeowWindow * me) {

	SetWindowPos(hwnd, HWND_TOPMOST,
		128, 128, 120, 80,
		SWP_NOACTIVATE);

	HWND Exit_Button = CreateWindow(L"BUTTON", L"EXIT", WS_VISIBLE | WS_CHILD | BS_ICON | BS_FLAT, 28, 8, 64, 64, hwnd, (HMENU)0, NULL, NULL);
	SendMessage(Exit_Button, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadImage(me->hinstance, MAKEINTRESOURCE(IDI_ICON_MAIN), IMAGE_ICON, 32, 32, NULL));
	SetWindowSubclass(Exit_Button, MeowWindow::ButtonProc, 0, (DWORD_PTR)me);
}

TCHAR * MeowStatusWindow::GetWndClass() {
	return wndclass;
}

LRESULT CALLBACK MeowWindow::FlowButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	MEOW_WINDOW_FLOWBUTTON * mwfb = (MEOW_WINDOW_FLOWBUTTON *)(dwRefData);
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		SendMessage(mwfb->parent->hwnd, WM_LBUTTONDOWN, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		SendMessage(mwfb->parent->hwnd, WM_MOUSEMOVE, wParam, lParam);
		if (!mwfb->mousetracking) {
			mwfb->mousetracking = true;
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
			SendMessage(hWnd, WM_SETFOCUS, NULL, NULL);
		}
		break;
	case WM_LBUTTONDBLCLK:
		SendMessage(hWnd, WM_LBUTTONDOWN, wParam, lParam);
		break;
	case WM_MOUSELEAVE:
		mwfb->mousetracking = false;
		SendMessage(hWnd, WM_KILLFOCUS, NULL, NULL);
		break;
	case WM_LBUTTONUP:
		SendMessage(mwfb->parent->hwnd, WM_LBUTTONUP, wParam, lParam);
		break;
	case WM_ERASEBKGND:
		return TRUE;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

VOID MeowStatusWindow::WndInit(HWND hwnd, MeowWindow * me) {
	RECT rcWork;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, FALSE);

	SetWindowPos(hwnd, HWND_TOPMOST,
		rcWork.right - 136, rcWork.bottom - 40, 130, 34,
		SWP_NOACTIVATE);

	MEOW_WINDOW_FLOWBUTTON mwfb;
	ZeroMemory(&mwfb, sizeof(MEOW_WINDOW_FLOWBUTTON));
	mwfb.parent = me;
	mwfb.memu = 0;
	mwfb.tooltip = L"";
	buttons.push_back(mwfb);
	mwfb.mode = 1;
	mwfb.memu = (HMENU)1;
	buttons.push_back(mwfb);
	mwfb.memu = (HMENU)2;
	buttons.push_back(mwfb);
	mwfb.memu = (HMENU)3;
	buttons.push_back(mwfb);
	unsigned int count = buttons.size();
	unsigned int size = 28;
	SetWindowPos(hwnd, NULL, 0, 0, size * count + 2, size + 2, SWP_NOMOVE | SWP_NOACTIVATE);

	for (unsigned int i = 0; i < count; i++) {
		buttons[i].hwnd = CreateWindow(L"BUTTON", mwfb.tooltip, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, size*i + 1, 1, size, size, hwnd, buttons[i].memu, NULL, NULL);
		SetWindowSubclass(buttons[i].hwnd, MeowWindow::FlowButtonProc, 0, (DWORD_PTR)&buttons[i]);
	}
}

VOID MeowStatusWindow::WndPaint(HWND hwnd, MeowWindow * me, HDC hdc, PAINTSTRUCT * ps) {
	using namespace Gdiplus;
	RECT rect;
	GetClientRect(hwnd, &rect);
	Gdiplus::Graphics graphics(hdc);
	SolidBrush brush_white(Color(255, 255, 255, 255));
	// Pen pen_frame(Color(255, 51, 153, 255));
	Pen pen_frame(Color(255, 56, 121, 217));
	Pen pen_border(Color(255, 255, 255, 255));
	//graphics.FillRectangle(&brush_white, 0, 0, rect.right, rect.bottom);
	graphics.DrawRectangle(&pen_frame, 0, 0, rect.right - 1, rect.bottom - 1);
}

BOOL MeowStatusWindow::WndDrawItem(HWND hwnd, MeowWindow * me, DRAWITEMSTRUCT * ds) {

	using namespace Gdiplus;
	MeowStatusWindow * parent = (MeowStatusWindow *)me->impl;
	MEOW_WINDOW_FLOWBUTTON * button = &parent->buttons[ds->CtlID];
	HDC memDC = ::CreateCompatibleDC(ds->hDC);
	RECT rect;
	GetClientRect(hwnd, &rect);
	HBITMAP memBitmap = ::CreateCompatibleBitmap(ds->hDC, rect.right, rect.bottom);
	SelectObject(memDC, memBitmap);
	Graphics graphics(memDC);


	Gdiplus::SolidBrush brush_blue(Color(255, 56, 121, 217));
	// Gdiplus::SolidBrush brush_blue(Color(255, 34, 142, 230));
	Gdiplus::SolidBrush brush_red(Color(255, 244, 47, 9));
	Gdiplus::SolidBrush brush_normal(Color(255, 255, 255, 255)); // 247
	Gdiplus::SolidBrush brush_hover(Color(255, 204, 223, 255));
	Gdiplus::SolidBrush brush_push(Color(255, 153, 204, 255));

	if (button->mode == 0) {
		graphics.FillRectangle(&brush_normal, 0, 0, rect.right, rect.bottom);
	}
	else if (button->mode == 1) {
		if (ds->itemState & ODS_FOCUS) {
			if (ds->itemState & ODS_SELECTED) {
				if (button->state == 1) {
					button->state = 0;
				}
				else {

					button->state = 1;
				}
				graphics.FillRectangle(&brush_push, 0, 0, rect.right, rect.bottom);
			}
			else {
				graphics.FillRectangle(&brush_hover, 0, 0, rect.right, rect.bottom);
			}
		}
		else {
			graphics.FillRectangle(&brush_normal, 0, 0, rect.right, rect.bottom);
		}
	}

	RectF dest = RectF(2, 2, 24, 24);

	Gdiplus::Image * image;
	switch (ds->CtlID) {
	case 1:{
		image = me->manager->skin->GetImageByKey(MS_IK_LANGUAGE);
		if (button->state == 0) {
			graphics.DrawImage(image, dest, 0, 0, 24, 24, UnitPixel);
		}
		else {
			graphics.DrawImage(image, dest, 24, 0, 24, 24, UnitPixel);
		}
	}
		   break;
	case 2:{
		image = me->manager->skin->GetImageByKey(MS_IK_PUNCTUATION);
		if (button->state == 0) {
			graphics.DrawImage(image, dest, 0, 0, 24, 24, UnitPixel);
		}
		else {
			graphics.DrawImage(image, dest, 24, 0, 24, 24, UnitPixel);
		}
	}
		   break;
	case 3:{
		image = me->manager->skin->GetImageByKey(MS_IK_CONFIGURE);
		graphics.DrawImage(image, 2, 2, 24, 24);

	}
		   break;
	default:{
		image = me->manager->skin->GetImageByKey(MS_IK_PINYIN);
		graphics.FillRectangle(&brush_blue, 0, 0, rect.right, rect.bottom);
		graphics.DrawImage(image, 0, 0, 28, 28); // 4, 4, 24, 24

	}
			break;
	}
	BitBlt(ds->hDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
	DeleteDC(memDC);
	DeleteObject(memBitmap);
	return true;
}


TCHAR * MeowCompositionWindow::GetWndClass() {
	return wndclass;
}

VOID MeowCompositionWindow::WndInit(HWND _hwnd, MeowWindow * me) {
	hwnd = _hwnd;

	POINT point;
	GetCursorPos(&point);
	//GetCaretPos(&point);
	wndsize.cx = 256;
	wndsize.cy = 64;

	SetWindowPos(hwnd, HWND_TOPMOST,
		point.x, point.y, wndsize.cx, wndsize.cy,
		SWP_NOACTIVATE);
}


VOID MeowCompositionWindow::SetText(CONST TCHAR * w) {
	candidates[0].SetString(w);
}

VOID MeowCompositionWindow::SetCandidate(UINT8 index, CONST TCHAR * _text) {
	candidates[index].SetString(_text);
}

VOID MeowCompositionWindow::ClearCandidates() {
	candidates[0].SetString(_T(""));
}


VOID MeowCompositionWindow::SetActiveCandidate(UINT8 index) {
	activecandidate = index;
}

VOID MeowCompositionWindow::RefreshCandidate() {
	InvalidateRect(hwnd, NULL, NULL);
}


VOID MeowCompositionWindow::AdjustPosition(CONST RECT * rect) {
	POINT point;
	point.x = rect->left;
	point.y = rect->bottom + 4;
	MeowWindow::WndSetPos(hwnd, &point);
}
VOID MeowCompositionWindow::WndPaint(HWND hwnd, MeowWindow * me, HDC hdc, PAINTSTRUCT * ps) {
	using namespace Gdiplus;
	RECT rect;
	GetClientRect(hwnd, &rect);

	// Gdiplus::Graphics graphics(hdc);
	HDC memhdc = CreateCompatibleDC(hdc);
	HBITMAP membitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	SelectObject(memhdc, membitmap);
	Graphics graphics(memhdc);




	Gdiplus::SolidBrush brush_front(Color(255, 34, 142, 230));
	Gdiplus::SolidBrush brush_front_hl(Color(255, 244, 47, 9));
	Gdiplus::SolidBrush brush_white(Gdiplus::Color(255, 255, 255, 255));
	Gdiplus::Pen      pen(Gdiplus::Color(255, 120, 180, 240));
	Gdiplus::Pen      pen2(Gdiplus::Color(255, 207, 207, 207), 2);
	//
	graphics.FillRectangle(&brush_white, 0, 0, rect.right, rect.bottom);
	graphics.DrawRectangle(&pen2, 1, 1, rect.right - 2, rect.bottom - 2);
	graphics.DrawLine(&pen2, 0, 32, rect.right, 32);
	// Create a Rect object.

	// Initialize arguments.
	Gdiplus::Font font_pinyin(L"Arial", 12, FontStyleBold);
	PointF origin(8.0f, 8.0f);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	// Draw string.
	graphics.DrawString(candidates[0], candidates[0].GetLength(), &font_pinyin, origin, &brush_front);

	graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit); // TextRenderingHintAntiAlias TextRenderingHintClearTypeGridFit

	Gdiplus::Font font_candidate(L"Microsoft Yahei", 12); // Microsoft Yahei , FontStyleBold
	origin = PointF(8.0f, 40.0f);
	RectF rectf;
	Gdiplus::StringFormat format = Gdiplus::StringFormat::GenericTypographic();

	unsigned int width = 0;
	REAL realwidth = 0;

	WCHAR string_sample[] = L"猫の输入法";
	graphics.MeasureString(string_sample, wcslen(string_sample), &font_candidate, origin, &format, &rectf);
	origin.Y -= (rectf.Height - 18);
	//graphics.DrawString(string_sample, wcslen(string_sample), &font_candidate, origin, &format, &brush_front_hl);

	WCHAR stringbuff[256] = L"";
	for (unsigned int i = 1; i < 10; i++) {
		if (candidates[i].GetLength() == 0) break;
		if (i != 1) {
			// wsprintf(stringbuff, L"  %d.%s", i, LPCWSTR(candidates[i]));
			StringCchPrintf(stringbuff, 256, L"  %d.%s", i, LPCWSTR(candidates[i]));

		}
		else {
			// wsprintf(stringbuff, L"%d.%s", i, LPCWSTR(candidates[i]));
			StringCchPrintf(stringbuff, 256, L"%d.%s", i, LPCWSTR(candidates[i]));
		}
		graphics.MeasureString(stringbuff, wcslen(stringbuff), &font_candidate, origin, &format, &rectf);
		if (activecandidate == i) {
			graphics.DrawString(stringbuff, wcslen(stringbuff), &font_candidate, origin, &format, &brush_front_hl);
		}
		else {
			graphics.DrawString(stringbuff, wcslen(stringbuff), &font_candidate, origin, &format, &brush_front);
		}
		realwidth += rectf.Width;
		origin.X += rectf.Width;
	}
	realwidth += 16;
	if (realwidth < 192) realwidth = 192;
	width = (unsigned int) realwidth;
	if (rect.right != width) {
		wndsize.cx = width;
		//wndsize.cy = height;
		SetWindowPos(hwnd, NULL,
			0, 0, wndsize.cx, wndsize.cy,
			SWP_NOMOVE | SWP_NOACTIVATE);
	}

	BitBlt(hdc, 0, 0, rect.right, rect.bottom, memhdc, 0, 0, SRCCOPY);
	DeleteDC(memhdc);
	DeleteObject(membitmap);
	return;

}


MeowSkinDelegate::MeowSkinDelegate(HINSTANCE hinstance) {
	WCHAR path[MAX_PATH] = { '\0' };
	DWORD len = GetModuleFileName(hinstance, path, ARRAYSIZE(path));
	root = L"";
	while (len--)
	{
		if (path[len] == '\\' || path[len] == '/')
		{
			root.Append(path, len);
			break;
		}
	}
	root.Append(L"\\skin\\default\\");
}
MeowSkinDelegate::~MeowSkinDelegate() {
	std::map<MEOW_SKIN_IMAGEKEY, Gdiplus::Image *>::iterator it;
	for (it = images.begin(); it != images.end(); ++it) {
		delete it->second;
	}
}


Gdiplus::Image * MeowSkinDelegate::GetImageByKey(UINT32 key) {
	std::map<MEOW_SKIN_IMAGEKEY, Gdiplus::Image *>::iterator it = images.find((MEOW_SKIN_IMAGEKEY)key);
	if (it != images.end()) {
		return it->second;
	}
	else {
		WCHAR buff[MAX_PATH];
		switch (key) {
		case MS_IK_LANGUAGE:
			GetImagePathByName(L"language.png", buff, MAX_PATH);
			break;
		case MS_IK_PUNCTUATION:
			GetImagePathByName(L"punctuation.png", buff, MAX_PATH);
			break;
		case MS_IK_CONFIGURE:
			GetImagePathByName(L"configure.png", buff, MAX_PATH);
			break;
		default:
		case MS_IK_PINYIN:
			GetImagePathByName(L"pinyin.png", buff, MAX_PATH);
			break;
		}

		Gdiplus::Image *image = new Gdiplus::Image(buff);
		images.insert(std::map<MEOW_SKIN_IMAGEKEY, Gdiplus::Image *>::value_type((MEOW_SKIN_IMAGEKEY)key, image));
		return image;
	}

}
BOOL MeowSkinDelegate::GetImagePathByName(WCHAR * name, WCHAR * buff, UINT32 length) {
	CString xx = (name);
	xx = root + xx;
	wcscpy_s(buff, length, (LPCWSTR)xx);
	return TRUE;
}