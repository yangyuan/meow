#include "meow.h"
#include "meow_window.h"


DWORD WINAPI MeowWindowManager::ThreadProc(LPVOID lpParam)
{
	MeowWindowManager * me = (MeowWindowManager *)lpParam;
	me->impl_status = new MeowStatusWindow();
	me->window_status = new MeowWindow(me->hinstance, me->impl_status);


	me->impl_composition = new MeowCompositionWindow();
	me->window_composition = new MeowWindow(me->hinstance, me->impl_composition);
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
	window_status = NULL;

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


// MeowWindow
MeowWindow::MeowWindow(HINSTANCE _hinstance, MeowWindowImpl * _impl)
{
	hinstance = _hinstance;
	impl = _impl;

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
		EndPaint(hwnd, &ps);
	}
		break;

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
				point.x = me->pointnow.y - me->pointcaptureclient.y;
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

VOID MeowStatusWindow::WndInit(HWND hwnd, MeowWindow * me) {
	RECT rcWork;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, FALSE);

	SetWindowPos(hwnd, HWND_TOPMOST,
		rcWork.right - 128, rcWork.bottom - 40, 128, 32,
		SWP_NOACTIVATE);

	HWND button1 = CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD | BS_ICON | BS_FLAT, 0, 0, 32, 32, hwnd, (HMENU)0, NULL, NULL);
	SendMessage(button1, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadImage(me->hinstance, MAKEINTRESOURCE(IDI_ICON_MAIN), IMAGE_ICON, 32, 32, NULL));
	SetWindowSubclass(button1, MeowWindow::ButtonProc, 0, (DWORD_PTR)me);

	HWND button2 = CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD | BS_ICON | BS_FLAT, 32, 0, 32, 32, hwnd, (HMENU)0, NULL, NULL);
	SendMessage(button2, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadImage(me->hinstance, MAKEINTRESOURCE(IDI_ICON_MAIN), IMAGE_ICON, 32, 32, NULL));
	SetWindowSubclass(button2, MeowWindow::ButtonProc, 0, (DWORD_PTR)me);

	HWND button3 = CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD | BS_ICON | BS_FLAT, 64, 0, 32, 32, hwnd, (HMENU)0, NULL, NULL);
	SendMessage(button3, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadImage(me->hinstance, MAKEINTRESOURCE(IDI_ICON_MAIN), IMAGE_ICON, 32, 32, NULL));
	SetWindowSubclass(button3, MeowWindow::ButtonProc, 0, (DWORD_PTR)me);

	HWND button4 = CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD | BS_ICON | BS_FLAT, 96, 0, 32, 32, hwnd, (HMENU)0, NULL, NULL);
	SendMessage(button4, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadImage(me->hinstance, MAKEINTRESOURCE(IDI_ICON_MAIN), IMAGE_ICON, 32, 32, NULL));
	SetWindowSubclass(button4, MeowWindow::ButtonProc, 0, (DWORD_PTR)me);
}


TCHAR * MeowCompositionWindow::GetWndClass() {
	return wndclass;
}

VOID MeowCompositionWindow::WndInit(HWND _hwnd, MeowWindow * me) {
	hwnd = _hwnd;
	hwnd_edit = NULL;

	POINT point;
	GetCursorPos(&point);
	//GetCaretPos(&point);

	SetWindowPos(hwnd, HWND_TOPMOST,
		point.x, point.y, 128, 24,
		SWP_NOACTIVATE);

	hwnd_edit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_READONLY, 0, 0, 128, 24, hwnd, (HMENU)0, NULL, NULL);
	//SendMessage(button4, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadImage(me->hinstance, MAKEINTRESOURCE(IDI_ICON_MAIN), IMAGE_ICON, 32, 32, NULL));
	SetWindowSubclass(hwnd_edit, MeowWindow::ChildProc, 0, (DWORD_PTR)me);
	HFONT hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hwnd_edit, WM_SETFONT, (WPARAM)hfont, MAKELPARAM(FALSE, 0));
	SetWindowText(hwnd_edit, _T("‸"));
}


VOID MeowCompositionWindow::SetText(CONST TCHAR * w) {
	if (hwnd_edit != NULL) {
		SetWindowText(hwnd_edit, w);
	}
}


VOID MeowCompositionWindow::AdjustPosition(CONST RECT * rect) {
	POINT point;
	//GetCursorPos(&point);
	//GetCaretPos(&point);
	point.x = rect->left;
	point.y = rect->bottom + 4;
	MeowWindow::WndSetPos(hwnd, &point);
	//SetWindowPos(hwnd, HWND_TOPMOST, rect.right, rect.bottom + 4, 128, 24, SWP_NOACTIVATE);
}