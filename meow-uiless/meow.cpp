#include "meow.h"

const TCHAR * MEOWUI_TITLE = _T("Meow UIless Debugger");
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		default:
			break;
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
