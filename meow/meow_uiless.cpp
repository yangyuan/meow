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
	*pguid = GUID_UILESS_MANAGER;
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

	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetDocumentMgr(ITfDocumentMgr **ppdim) {

	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetCount(UINT *puCount) {

	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetSelection(UINT *puIndex) {

	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetString(UINT uIndex, BSTR *pstr) {

	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetPageIndex(UINT *pIndex, UINT uSize, UINT *puPageCnt) {

	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::SetPageIndex(UINT *pIndex, UINT uPageCnt) {

	return S_OK;
}

HRESULT STDMETHODCALLTYPE MeowUILessManager::GetCurrentPage(UINT *puPage) {

	return S_OK;
}