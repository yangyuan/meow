#pragma once
#include <windows.h>
#include <msctf.h>
#include <ctffunc.h>
#include <tchar.h>
#include <comdef.h>

#include <atlstr.h>

#include <Commctrl.h>
#pragma comment(lib,"comctl32.lib")

#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#include <map>
#include <vector>


#include <strsafe.h>
#include "hack.h"
#include "resource.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
HRESULT __stdcall DllRegisterServer(void);
HRESULT __stdcall DllUnregisterServer(void);
HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv);
HRESULT __stdcall DllCanUnloadNow(void);

namespace Meow {
	VOID DllAddRef();
	VOID DllRelease();

	extern HINSTANCE hinstance;

	// CONSTs
	extern const TCHAR * STRING_TEXTSERVICE_TITLE;
	extern const TCHAR * STRING_TEXTSERVICE_THREADMODEL;
	extern const WORD    LANGID_TEXTSERVICE;
	extern const ULONG   ICONINDEX_TEXTSERVICE;

	extern const CLSID CLSID_TEXTSERVICE;
	extern const GUID  GUID_PROFILE;
	extern const GUID  GUID_DISPLAY_ARRTIBUTE_INFO;
	extern const GUID  GUID_UILESS_MANAGER;

	static const GUID SupportCategories[] = {
		GUID_TFCAT_TIP_KEYBOARD,
		GUID_TFCAT_DISPLAYATTRIBUTEPROVIDER,
		GUID_TFCAT_TIPCAP_UIELEMENTENABLED,
		GUID_TFCAT_TIPCAP_SECUREMODE,
		GUID_TFCAT_TIPCAP_COMLESS,
		GUID_TFCAT_TIPCAP_INPUTMODECOMPARTMENT,
		GUID_TFCAT_TIPCAP_IMMERSIVESUPPORT, // WIN8 ONLY
		GUID_TFCAT_TIPCAP_SYSTRAYSUPPORT, // WIN8 ONLY
	};
}



class MeowUnknown : public IUnknown {
public:
	MeowUnknown() { reference = 1; }
	~MeowUnknown() { }
	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj) {
		if (ppvObj == NULL) return E_INVALIDARG;
		*ppvObj = NULL;
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IUnknown)) // FIXME: IID_IUnknown
		{
			*ppvObj = (IUnknown *)this; // FIXME: IUnknown
		}
		if (*ppvObj) { AddRef(); return S_OK; }
		return E_NOINTERFACE;
	}
	ULONG STDMETHODCALLTYPE AddRef() {
		return ++reference;
	}
	ULONG STDMETHODCALLTYPE Release() {
		if (--reference <= 0) delete this;
		return reference;
	}
private:
	ULONG reference;
};