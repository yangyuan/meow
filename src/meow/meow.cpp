#include "meow.h"
#include "meow_registry.h"
#include "meow_textservice.h"
#include "meow_window.h"

namespace Meow {

	const TCHAR * STRING_TEXTSERVICE_TITLE = _T("Meow IME");
	const TCHAR * STRING_TEXTSERVICE_THREADMODEL = _T("Apartment");
	const WORD    LANGID_TEXTSERVICE = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	const ULONG   ICONINDEX_TEXTSERVICE = -IDI_ICON_MAIN;


	// {E6DC8188-1479-4248-BCAD-85E21C4AEE57}
	const GUID CLSID_TEXTSERVICE = { 0xe6dc8188, 0x1479, 0x4248, { 0xbc, 0xad, 0x85, 0xe2, 0x1c, 0x4a, 0xee, 0x57 } };

	// {8D369779-D42D-4C44-8FC1-FFB17E48870B}
	const GUID GUID_PROFILE = { 0x8d369779, 0xd42d, 0x4c44, { 0x8f, 0xc1, 0xff, 0xb1, 0x7e, 0x48, 0x87, 0xb } };

	// {3FB44A20-052B-48EE-8B2E-56D9E696F51D}
	const GUID GUID_DISPLAY_ARRTIBUTE_INFO = { 0x3fb44a20, 0x52b, 0x48ee, { 0x8b, 0x2e, 0x56, 0xd9, 0xe6, 0x96, 0xf5, 0x1d } };

	// {F3DEE8FF-43C9-4174-AEC1-B22B48704B37}
	const GUID GUID_UILESS_MANAGER = { 0xf3dee8ff, 0x43c9, 0x4174, { 0xae, 0xc1, 0xb2, 0x2b, 0x48, 0x70, 0x4b, 0x37 } };

	// {6B551DEB-912F-4610-91DB-4074FC52FB88}
	const GUID clsid_meow_1 = { 0x6b551deb, 0x912f, 0x4610, { 0x91, 0xdb, 0x40, 0x74, 0xfc, 0x52, 0xfb, 0x88 } };

	// {A38B72C9-2D6C-480B-9FE7-30909879EE88}
	const GUID clsid_meow_0 = { 0xa38b72c9, 0x2d6c, 0x480b, { 0x9f, 0xe7, 0x30, 0x90, 0x98, 0x79, 0xee, 0x88 } };

}

// used to create instance and count references
// both IClassFactory and IUnknown need
// almost copied from official sample
class MeowClassFactory : public IClassFactory
{
public:
	// Constructor
	MeowClassFactory(REFCLSID rclsid, HRESULT(*pfnCreateInstance)(IUnknown *pUnkOuter, REFIID riid, void **ppvObj)) :rclsid(rclsid)
	{
		MeowCreateInstance = pfnCreateInstance;
	}

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	// IClassFactory
	HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj);
	HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock);
	REFCLSID rclsid;

private:
	HRESULT(*MeowCreateInstance)(IUnknown *pUnkOuter, REFIID riid, void **ppvObj);
};


namespace Meow {
	HINSTANCE hinstance;
	CRITICAL_SECTION criticalsection;
	LONG reference;
	MeowClassFactory * factory;
}

// STA (COM Thread Model)
// Only handle Instance and CriticalSection.
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		Meow::hinstance = hinstDLL;
		if (!InitializeCriticalSectionAndSpinCount(&Meow::criticalsection, 0)) return FALSE;
		break;
	case DLL_PROCESS_DETACH:
		DeleteCriticalSection(&Meow::criticalsection);
		break;
	}
	return TRUE;
}

HRESULT __stdcall DllRegisterServer(void)
{
	if ((!MeowRegistry::RegisterServer()) || (!MeowRegistry::RegisterProfiles()) || (!MeowRegistry::RegisterCategories()))
	{
		DllUnregisterServer();
		return E_FAIL;
	}
	return S_OK;
}

HRESULT __stdcall DllUnregisterServer(void)
{
	// simply unregister everything
	MeowRegistry::UnregisterProfiles();
	MeowRegistry::UnregisterCategories();
	MeowRegistry::UnregisterServer();

	return S_OK;
}


HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
	if (Meow::factory == NULL)
	{
		EnterCriticalSection(&Meow::criticalsection);
		if (Meow::factory == NULL)
		{
			Meow::factory = new MeowClassFactory(Meow::CLSID_TEXTSERVICE, MeowTextService::CreateInstance);
		}
		LeaveCriticalSection(&Meow::criticalsection);
	}

	if (IsEqualIID(riid, IID_IClassFactory) || IsEqualIID(riid, IID_IUnknown))
	{
		// make sure it wants our factory for text service
		if (NULL != Meow::factory && IsEqualGUID(rclsid, Meow::factory->rclsid))
		{
			*ppv = (void *)Meow::factory;
			Meow::DllAddRef();
			return NOERROR;
		}
	}
	*ppv = NULL;
	return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT __stdcall DllCanUnloadNow(void)
{
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms690368.aspx
	if (Meow::reference >= 0) // -1 with no refs
		return S_FALSE;

	return S_OK;
}

VOID Meow::DllAddRef()
{
	InterlockedIncrement(&Meow::reference);
}
VOID Meow::DllRelease()
{
	LONG value = InterlockedDecrement(&Meow::reference);
	if (value < 0) // no reference, destory ClassFactory, maybe not necessary
	{
		EnterCriticalSection(&Meow::criticalsection);
		if (nullptr != Meow::factory)
		{
			delete Meow::factory;
			Meow::factory = NULL;
		}
		LeaveCriticalSection(&Meow::criticalsection);
	}
}


HRESULT STDMETHODCALLTYPE MeowClassFactory::QueryInterface(REFIID riid, void **ppvObj)
{
	// only return self if quering IID_IClassFactory or IID_IUnknown
	if (IsEqualIID(riid, IID_IClassFactory) || IsEqualIID(riid, IID_IUnknown))
	{
		*ppvObj = this;
		Meow::DllAddRef();
		return NOERROR;
	}
	*ppvObj = nullptr;
	return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE MeowClassFactory::AddRef()
{
	Meow::DllAddRef();
	return (Meow::reference + 1); // FIXME: why +1 
}
ULONG STDMETHODCALLTYPE MeowClassFactory::Release()
{
	Meow::DllRelease();
	return (Meow::reference + 1); // FIXME: why +1 
}
HRESULT STDMETHODCALLTYPE MeowClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj)
{
	return MeowCreateInstance(pUnkOuter, riid, ppvObj);
}
HRESULT STDMETHODCALLTYPE MeowClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
	{
		Meow::DllAddRef();
	}
	else
	{
		Meow::DllRelease();
	}
	return S_OK;
}
