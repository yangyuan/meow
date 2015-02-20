#include "meow.h"
#include "meow_candidate.h"
#include "meow_textservice.h"

MeowCandidateManager::MeowCandidateManager(MeowTextService * _textservice) {
	reference = 1;
	textservice = _textservice;

	shown = FALSE;
}
MeowCandidateManager::~MeowCandidateManager() {
}

VOID MeowCandidateManager::Show() {

	if (uielementmgr != NULL) {
		BOOL toshown;
		DWORD uiid;
		uielementmgr->BeginUIElement(this, &toshown, &uiid);

		if (!toshown) {
			uielementmgr->UpdateUIElement(uiid);
		}
		else {

		}
	}
}

VOID MeowCandidateManager::SetDocumentMgr(ITfDocumentMgr * _documentmgr) {
	// FIXME: reference
	documentmgr = _documentmgr;
}

VOID MeowCandidateManager::SetUIElementMgr(ITfUIElementMgr * _uielementmgr) {
	// FIXME: reference
	uielementmgr = _uielementmgr;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::QueryInterface(REFIID riid, void **ppvObj) {
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
ULONG STDMETHODCALLTYPE MeowCandidateManager::AddRef() {
	return ++reference;
}
ULONG STDMETHODCALLTYPE MeowCandidateManager::Release() {
	if (--reference <= 0) delete this;
	return reference;
}

// ITfUIElement
HRESULT STDMETHODCALLTYPE MeowCandidateManager::GetDescription(BSTR *pbstrDescription) {
	*pbstrDescription = SysAllocString(L"");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::GetGUID(GUID *pguid) {
	*pguid = Meow::GUID_UILESS_MANAGER;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::Show(BOOL bShow) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::IsShown(BOOL *pbShow) {
	*pbShow = shown;
	return S_OK;
}

// ITfCandidateListUIElement
HRESULT STDMETHODCALLTYPE MeowCandidateManager::GetUpdatedFlags(DWORD *pdwFlags) {
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

HRESULT STDMETHODCALLTYPE MeowCandidateManager::GetDocumentMgr(ITfDocumentMgr **ppdim) {
	MessageBox(NULL, L"XX", L"XX", MB_OK);
	*ppdim = documentmgr;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::GetCount(UINT *puCount) {
	*puCount = 5;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::GetSelection(UINT *puIndex) {
	*puIndex = 1;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::GetString(UINT uIndex, BSTR *pstr) {
	CString cstr = L"ºòÑ¡";
	cstr.AppendFormat(L" %d", uIndex);
	*pstr = cstr.AllocSysString();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::GetPageIndex(UINT *pIndex, UINT uSize, UINT *puPageCnt) {
	*puPageCnt = 1;
	if (pIndex != NULL) {
		*pIndex = 0;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::SetPageIndex(UINT *pIndex, UINT uPageCnt) {
	// NOT SUPPORT YET
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowCandidateManager::GetCurrentPage(UINT *puPage) {
	*puPage = 0;
	return S_OK;
}