#include "meow.h"
#include "meow_uiless.h"
#include "meow_textservice.h"

MeowUILessManager::MeowUILessManager(MeowTextService * _textservice) {
	reference = 1;
	textservice = _textservice;

	shown = FALSE;
}
MeowUILessManager::~MeowUILessManager() {
}

VOID MeowUILessManager::Show() {
	ITfUIElementMgr * uielementmgr;
	HRESULT hr = textservice->threadmgr->QueryInterface(IID_ITfUIElementMgr, (LPVOID*)&uielementmgr);
	BOOL toshown;
	DWORD uiid;
	uielementmgr->BeginUIElement(this, &toshown, &uiid);

	if (!toshown) {
		uielementmgr->UpdateUIElement(uiid);
	}
	else {
		
	}

}

VOID MeowUILessManager::SetDocumentMgr(ITfDocumentMgr * _documentmgr) {
	documentmgr = _documentmgr;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::QueryInterface(REFIID riid, void **ppvObj) {
	if (ppvObj == NULL) return E_INVALIDARG;
	*ppvObj = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfUIElement))
	{
		*ppvObj = (ITfUIElement *)this;
	}
	else if (IsEqualIID(riid, IID_ITfCandidateListUIElement))
	{
		*ppvObj = (ITfCandidateListUIElement *)this;
	}

	if (*ppvObj)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE MeowUILessManager::AddRef() {
	return ++reference;
}
ULONG STDMETHODCALLTYPE MeowUILessManager::Release() {
	if (--reference <= 0) delete this;
	return reference;
}

// ITfUIElement
HRESULT STDMETHODCALLTYPE MeowUILessManager::GetDescription(BSTR *pbstrDescription) {
	*pbstrDescription = SysAllocString(L"");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetGUID(GUID *pguid) {
	*pguid = Meow::GUID_UILESS_MANAGER;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::Show(BOOL bShow) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::IsShown(BOOL *pbShow) {
	*pbShow = shown;
	return S_OK;
}

// ITfCandidateListUIElement
HRESULT STDMETHODCALLTYPE MeowUILessManager::GetUpdatedFlags(DWORD *pdwFlags) {
	/*
	TF_CLUIE_DOCUMENTMGR
	TF_CLUIE_COUNT
	TF_CLUIE_SELECTION
	TF_CLUIE_STRING
	TF_CLUIE_PAGEINDEX
	TF_CLUIE_CURRENTPAGE
	*/
	*pdwFlags = TF_CLUIE_COUNT | TF_CLUIE_STRING;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetDocumentMgr(ITfDocumentMgr **ppdim) {
	*ppdim = documentmgr;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetCount(UINT *puCount) {
	*puCount = 5;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetSelection(UINT *puIndex) {
	*puIndex = 1;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetString(UINT uIndex, BSTR *pstr) {
	CString cstr = L"ºòÑ¡";
	cstr.AppendFormat(L" %d", uIndex);
	*pstr = cstr.AllocSysString();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetPageIndex(UINT *pIndex, UINT uSize, UINT *puPageCnt) {
	*puPageCnt = 1;
	if (pIndex != NULL) {
		*pIndex = 0;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::SetPageIndex(UINT *pIndex, UINT uPageCnt) {
	// NOT SUPPORT YET
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetCurrentPage(UINT *puPage) {
	*puPage = 0;
	return S_OK;
}