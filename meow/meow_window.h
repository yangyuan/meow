#pragma once

class MeowWindowImpl;
// General Purpose IME UI Window
// It should be copied, not inheried
class MeowWindow
{
public:
	MeowWindow(HINSTANCE hinstance, MeowWindowImpl *);
	~MeowWindow();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static LRESULT CALLBACK ButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static VOID WndSetPos(HWND hWnd, CONST POINT * point);

	VOID WndShow();
	VOID WndHide();

	HINSTANCE hinstance;
	HWND hwnd;
	MeowWindowImpl * impl;
private:
	POINT pointcapture;
	POINT pointcaptureclient;
	POINT pointnow;
	HCURSOR cursorcache;

	// used for window moving
	static RECT rect;
	static RECT rcwork;
};


class MeowWindowImpl
{
public:
	virtual TCHAR * GetWndClass();
	virtual VOID WndInit(HWND hwnd, MeowWindow * me);
private:
	TCHAR * wndclass = _T("MEOW_WINDOW");
};

class MeowStatusWindow : public MeowWindowImpl
{
public:
	TCHAR * GetWndClass();
	VOID WndInit(HWND hwnd, MeowWindow * me);
private:
	TCHAR * wndclass = _T("MEOW_STATUS");
};

class MeowCompositionWindow : public MeowWindowImpl
{
public:
	TCHAR * GetWndClass();
	VOID WndInit(HWND hwnd, MeowWindow * me);
	VOID SetText(CONST TCHAR *);
	VOID AdjustPosition(CONST RECT *);
private:
	TCHAR * wndclass = _T("MEOW_COMPOSITION");
	HWND hwnd;
	HWND hwnd_edit;
};


// IME UI Window Manager, Handle and Manage the UI Thread.
// Maybe Will Handle UI Skin in Future
/*
MeowWindowManager is the bridge between IME Window and TSF Service.
Both use MeowWindowManager directly to send messages.
MeowWindowManager also need to manage the thread of IME Window(s)
Most of the members in MeowWindowManager are public, but should be use carefully, everything should be treated as unstable.
MeowWindowManager also used as the data source of
*/
class MeowWindowManager {
public:
	MeowWindowManager(HINSTANCE hinstance);
	HINSTANCE hinstance;
	HANDLE uithread;

	MeowStatusWindow * impl_status;
	MeowWindow * window_status;
	VOID ShowStatusWindow();
	VOID HideStatusWindow();
	VOID AdjustCompositionWindow(CONST RECT * rect, CONST TCHAR * text);

	MeowCompositionWindow * impl_composition;
	MeowWindow * window_composition;

private:
	static DWORD WINAPI ThreadProc(LPVOID lpParam);
};