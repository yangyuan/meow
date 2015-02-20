#include "meow.h"
#include "meow_window.h"
#include "meow_textservice.h"
#include "meow_composition.h"
#include "meow_candidate.h"

MeowTextService::MeowTextService() {
	reference = 1;
	threadmgr = NULL;

	threadmgreventsink_cookie = TF_INVALID_COOKIE;
	threadfocussink_cookie = TF_INVALID_COOKIE;
	texteditsink_cookie = TF_INVALID_COOKIE;

	window_manager  = new MeowWindowManager(Meow::hinstance);
	Meow::DllAddRef();
}
MeowTextService::~MeowTextService() {
	delete window_manager;
	Meow::DllRelease();
}

HRESULT MeowTextService::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj) {
	MeowTextService * tmp = nullptr;
	HRESULT hr = S_OK;

	if (ppvObj == nullptr) return E_INVALIDARG;
	*ppvObj = nullptr;
	if (pUnkOuter != nullptr) return CLASS_E_NOAGGREGATION;

	tmp = new MeowTextService();
	if (tmp == NULL) return E_OUTOFMEMORY;
	hr = tmp->QueryInterface(riid, ppvObj);
	tmp->Release(); // caller still holds ref if hr == S_OK
	return hr;
}
HRESULT STDMETHODCALLTYPE MeowTextService::QueryInterface(REFIID riid, void **ppvObj) {
	if (ppvObj == NULL) return E_INVALIDARG;
	*ppvObj = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfTextInputProcessor))
	{
		*ppvObj = (ITfTextInputProcessor *)this;
	}
	else if (IsEqualIID(riid, IID_ITfTextInputProcessorEx))
	{
		*ppvObj = (ITfTextInputProcessorEx *)this;
	}
	else if (IsEqualIID(riid, IID_ITfThreadMgrEventSink))
	{
		*ppvObj = (ITfThreadMgrEventSink *)this;
	}
	else if (IsEqualIID(riid, IID_ITfThreadFocusSink))
	{
		*ppvObj = (ITfThreadFocusSink *)this;
	}
	else if (IsEqualIID(riid, IID_ITfCompositionSink))
	{
		*ppvObj = (ITfCompositionSink *)this->composition_manager;
	}
	else if (IsEqualIID(riid, IID_ITfKeyEventSink))
	{
		*ppvObj = (ITfKeyEventSink *)this->composition_manager;
	}
	else if (IsEqualIID(riid, IID_ITfTextEditSink))
	{
		*ppvObj = (ITfTextEditSink *)this->composition_manager;
	}
	else if (IsEqualIID(riid, IID_ITfDisplayAttributeProvider))
	{
		*ppvObj = (ITfDisplayAttributeProvider *)this;
	}
	else if (IsEqualIID(riid, IID_ITfDisplayAttributeCollectionProvider))
	{
		// 
	}
	else {
		LPOLESTR str;
		StringFromIID(riid, &str);
		_bstr_t bs(str, false);
		CHAR * xx = bs;
	}

	if (*ppvObj)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE MeowTextService::AddRef() {
	return ++reference;
}
ULONG STDMETHODCALLTYPE MeowTextService::Release() {
	if (--reference <= 0) delete this;
	return reference;
}

HRESULT STDMETHODCALLTYPE MeowTextService::ActivateEx(ITfThreadMgr *pThreadMgr, TfClientId tfClientId, DWORD dwFlags) {
	threadmgr = pThreadMgr;
	clientid = tfClientId;
	threadmgr->AddRef();
	flags = dwFlags;

	ULONG_PTR gdiplustoken;
	Gdiplus::GdiplusStartupInput gdiplusstartupinput;
	Gdiplus::GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);

	candidate_manager = new MeowCandidateManager(this);
	{
		ITfUIElementMgr * uielementmgr;
		HRESULT hr = threadmgr->QueryInterface(IID_ITfUIElementMgr, (LPVOID*)&uielementmgr);
		if (hr == S_OK) {
			candidate_manager->SetUIElementMgr(uielementmgr);
		}
	}

	composition_manager = new MeowCompositionManager(clientid, this);

	if (!Meow::InitThreadMgrEventSink(threadmgr, this, &threadmgreventsink_cookie))
		goto ExitError;

	if (!Meow::InitThreadFocusSink(threadmgr, this, &threadfocussink_cookie))
		goto ExitError;

	if (!Meow::InitKeyEventSink(threadmgr, composition_manager, clientid))
		goto ExitError;



	/*
	ITfDocumentMgr *pDocMgrFocus;
	HRESULT hr = NULL;
	hr = threadmgr->GetFocus(&pDocMgrFocus);
	if ((hr == S_OK) && (pDocMgrFocus != NULL))
	{
	SyncDocumentMgr(pDocMgrFocus);
	pDocMgrFocus->Release();
	}
	*/

	return S_OK;
ExitError:
	Deactivate(); // cleanup any half-finished init
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE MeowTextService::Activate(ITfThreadMgr *pThreadMgr, TfClientId tfClientId) {
	return ActivateEx(pThreadMgr, tfClientId, 0);
}
HRESULT STDMETHODCALLTYPE MeowTextService::Deactivate() {

	Meow::UninitKeyEventSink(threadmgr, clientid);
	Meow::UninitThreadFocusSink(threadmgr, &threadfocussink_cookie);
	Meow::UninitThreadMgrEventSink(threadmgr, &threadmgreventsink_cookie);

	delete composition_manager;
	delete candidate_manager;

	if (threadmgr != NULL)
	{
		threadmgr->Release();
		threadmgr = NULL;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowTextService::OnSetThreadFocus()
{
	window_manager->ShowStatusWindow();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowTextService::OnKillThreadFocus()
{
	window_manager->HideStatusWindow();
	return S_OK;
}


STDAPI MeowTextService::OnInitDocumentMgr(ITfDocumentMgr *pDocMgr)
{
	return S_OK;
}

STDAPI MeowTextService::OnUninitDocumentMgr(ITfDocumentMgr *pDocMgr)
{
	return S_OK;
}

STDAPI MeowTextService::OnSetFocus(ITfDocumentMgr *pDocMgrFocus, ITfDocumentMgr *pDocMgrPrevFocus)
{
	// never happend in XP

	composition_manager->Switch(pDocMgrFocus, pDocMgrPrevFocus);
	candidate_manager->SetDocumentMgr(pDocMgrFocus);

	
	return S_OK;
}

STDAPI MeowTextService::OnPushContext(ITfContext *pContext)
{
	return S_OK;
}
STDAPI MeowTextService::OnPopContext(ITfContext *pContext)
{
	return S_OK;
}


STDAPI MeowTextService::EnumDisplayAttributeInfo(IEnumTfDisplayAttributeInfo **ppEnum)
{
	if (ppEnum == nullptr)
	{
		return E_INVALIDARG;
	}
	*ppEnum = nullptr;
	*ppEnum = new (std::nothrow) MeowDisplayAttributeInfoEnum();
	if (*ppEnum == nullptr)
	{
		return E_OUTOFMEMORY;
	}
	return S_OK;
}

STDAPI MeowTextService::GetDisplayAttributeInfo(REFGUID guidInfo, ITfDisplayAttributeInfo **ppInfo)
{
	if (ppInfo == nullptr)
	{
		return E_INVALIDARG;
	}
	*ppInfo = nullptr;
	if (IsEqualGUID(guidInfo, Meow::GUID_DISPLAY_ARRTIBUTE_INFO))
	{
		*ppInfo = new (std::nothrow) MeowDisplayAttributeInfo();
		if ((*ppInfo) == nullptr)
		{
			return E_OUTOFMEMORY;
		}
	}
	else
	{
		return E_INVALIDARG;
	}
	return S_OK;
}

BOOL Meow::InitThreadMgrEventSink(ITfThreadMgr * threadmgr, ITfThreadMgrEventSink * punk, DWORD * cookie)
{
	ITfSource * source = NULL;
	if (threadmgr->QueryInterface(IID_ITfSource, (void **)&source) == S_OK)
	{
		HRESULT hr;
		hr = source->AdviseSink(IID_ITfThreadMgrEventSink, punk, cookie);
		source->Release();

		if (hr != S_OK) {
			*cookie = TF_INVALID_COOKIE;
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

VOID Meow::UninitThreadMgrEventSink(ITfThreadMgr * threadmgr, DWORD * cookie)
{
	ITfSource * source;
	if (*cookie == TF_INVALID_COOKIE) { return; }
	if (threadmgr->QueryInterface(IID_ITfSource, (void **)&source) == S_OK)
	{
		source->UnadviseSink(*cookie);
		source->Release();
	}
	*cookie = TF_INVALID_COOKIE;
}

BOOL Meow::InitThreadFocusSink(ITfThreadMgr * threadmgr, ITfThreadFocusSink * punk, DWORD * cookie)
{
	ITfSource * source = NULL;
	if (threadmgr->QueryInterface(IID_ITfSource, (void **)&source) == S_OK)
	{
		HRESULT hr;
		hr = source->AdviseSink(IID_ITfThreadFocusSink, punk, cookie);
		source->Release();

		if (hr != S_OK) {
			*cookie = TF_INVALID_COOKIE;
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

VOID Meow::UninitThreadFocusSink(ITfThreadMgr * threadmgr, DWORD * cookie)
{
	ITfSource * source;
	if (*cookie == TF_INVALID_COOKIE) { return; }
	if (threadmgr->QueryInterface(IID_ITfSource, (void **)&source) == S_OK)
	{
		source->UnadviseSink(*cookie);
		source->Release();
	}
	*cookie = TF_INVALID_COOKIE;
}

BOOL Meow::InitKeyEventSink(ITfThreadMgr * threadmgr, ITfKeyEventSink * punk, TfClientId clientid)
{
	ITfKeystrokeMgr * keystrokemgr = NULL;
	if (threadmgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&keystrokemgr) == S_OK)
	{
		HRESULT hr;
		hr = keystrokemgr->AdviseKeyEventSink(clientid, punk, TRUE);
		keystrokemgr->Release();

		// hr = keystrokemgr->PreserveKey();

		return (hr == S_OK);
	}
	return FALSE;
}

VOID Meow::UninitKeyEventSink(ITfThreadMgr * threadmgr, TfClientId clientid)
{
	ITfKeystrokeMgr * keystrokemgr;
	if (threadmgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&keystrokemgr) == S_OK)
	{
		keystrokemgr->UnadviseKeyEventSink(clientid);
		// keystrokemgr->UnpreserveKey();
		keystrokemgr->Release();
	}
}

BOOL Meow::InitTextEditSink(ITfContext * context, ITfTextEditSink * punk, DWORD * cookie)
{
	ITfSource * source = NULL;
	if (context->QueryInterface(IID_ITfSource, (void **)&source) == S_OK)
	{
		HRESULT hr;
		hr = source->AdviseSink(IID_ITfTextEditSink, punk, cookie);
		source->Release();

		if (hr != S_OK) {
			*cookie = TF_INVALID_COOKIE;
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

VOID Meow::UninitTextEditSink(ITfContext * context, DWORD * cookie)
{
	ITfSource * source;
	if (*cookie == TF_INVALID_COOKIE) { return; }
	if (context->QueryInterface(IID_ITfSource, (void **)&source) == S_OK)
	{
		source->UnadviseSink(*cookie);
		source->Release();
	}
	*cookie = TF_INVALID_COOKIE;
}

























/*
BOOL InitDisplayAttributeGuidAtom()
{
// FIXME: COMLESS MODE?
ITfCategoryMgr* pCategoryMgr = nullptr;
HRESULT hr = CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, (void**)&pCategoryMgr);

if (FAILED(hr))
{
return FALSE;
}

// FIXME: GUID_DISPLAY_ARRTIBUTE_INFO unregister
// register the display attribute for input text.
hr = pCategoryMgr->RegisterGUID(Meow::GUID_DISPLAY_ARRTIBUTE_INFO, &displayattribute);
if (FAILED(hr))
{
goto Exit;
}

Exit:
pCategoryMgr->Release();

return (hr == S_OK);
}
*/

