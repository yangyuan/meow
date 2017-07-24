#include "meow.h"
#include "meow_window.h"

const TCHAR * MEOWUI_TITLE = _T("Meow GUI Debugger");
const TCHAR * MEOWUI_WINDOWCLASS_MAIN = _T("MEOW_MAIN");

HINSTANCE hinstance;
HWND hwndmain;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	hinstance = hInstance;

	ULONG_PTR gdiplustoken;
	Gdiplus::GdiplusStartupInput gdiplusstartupinput;
	Gdiplus::GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);
	wcex.lpszClassName = MEOWUI_WINDOWCLASS_MAIN;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	RegisterClassEx(&wcex);

	hwndmain = CreateWindowEx(NULL,
		MEOWUI_WINDOWCLASS_MAIN, MEOWUI_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 400, 300,
		NULL, NULL, hInstance, NULL);

	if (!hwndmain)
	{
		return 0;
	}
	
	ShowWindow(hwndmain, nCmdShow);
	UpdateWindow(hwndmain);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

MeowWindowManager * mwm;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		mwm = new MeowWindowManager(hinstance);
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_DEBUG_SHOWSTATUSWINDOW:
			mwm->ShowStatusWindow();
			break;
		case ID_DEBUG_HIDESTATUSWINDOW:
			mwm->HideStatusWindow();
			break;
		case ID_DEBUG_SETCOMPOSITIONTEXT:
		{
			RECT rect;
			GetWindowRect(hWnd, &rect);
			rect.left += 16;
			rect.bottom = rect.top + 72;
			mwm->ClearCandidates();
			mwm->AdjustCompositionWindow(&rect, _T("pin'yin'shu'ru'fa"));
			mwm->SetCandidate(1, L"拼音输入法");
			mwm->SetCandidate(2, L"拼音");
			mwm->SetCandidate(3, L"猫の输入法");
			mwm->SetCandidate(4, L"我是候选");
			mwm->SetCandidate(5, L"我也是");
			mwm->SetActiveCandidate(1);
			mwm->RefreshCandidate();
		}
		break;
		case ID_DEBUG_APPLENDCOMPOSITIONTEXT:
		{
			mwm->AdjustCompositionWindow(NULL, _T("pin'yin'shu'ru"));
			mwm->SetActiveCandidate(0);
			mwm->RefreshCandidate();
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/*







// WS_EX_LAYERED
RECT wndRect;
::GetWindowRect(hwndmain, &wndRect);
SIZE wndSize = { wndRect.right - wndRect.left, wndRect.bottom - wndRect.top };
HDC hdc = ::GetDC(hwndmain);
HDC memDC = ::CreateCompatibleDC(hdc);
HBITMAP memBitmap = ::CreateCompatibleBitmap(hdc, wndSize.cx, wndSize.cy);
::SelectObject(memDC, memBitmap);
Gdiplus::Image image(L"C:\\pic.png");
Gdiplus::Graphics graphics(memDC);
graphics.DrawImage(&image, 0, 0, wndSize.cx, wndSize.cy);

HDC screenDC = GetDC(NULL);
POINT ptSrc = { 0, 0 };
BLENDFUNCTION blendFunction;
blendFunction.AlphaFormat = AC_SRC_ALPHA;
blendFunction.BlendFlags = 0;
blendFunction.BlendOp = AC_SRC_OVER;
blendFunction.SourceConstantAlpha = 255;
UpdateLayeredWindow(hwndmain, screenDC, &ptSrc, &wndSize, memDC, &ptSrc, 0, &blendFunction, 2);
//SetLayeredWindowAttributes(hwndmain, RGB(255, 0, 0), 0, LWA_COLORKEY);
::DeleteDC(memDC);
::DeleteObject(memBitmap);

*/