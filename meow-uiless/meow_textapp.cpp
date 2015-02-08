#include "meow.h"
#include "meow_textapp.h"


MeowTextApp::MeowTextApp(HWND _hwnd)
{
	hwnd = _hwnd;
	reference = 1;

	CoInitialize(NULL);
	HRESULT hr;
	
	hr= CoCreateInstance(CLSID_TF_ThreadMgr, NULL, CLSCTX_INPROC_SERVER, IID_ITfThreadMgr, (void**)&threadmgr);

	// active
	hr = threadmgr->QueryInterface(IID_ITfThreadMgrEx, (LPVOID*)&threadmgrex);
	hr = threadmgrex->ActivateEx(&clientid, TF_TMAE_UIELEMENTENABLEDONLY);

	// ITfUIElementSink
	hr = threadmgr->QueryInterface(IID_ITfUIElementMgr, (LPVOID*)&uielementmgr);
	CComPtr<ITfSource> source;
	hr = uielementmgr->QueryInterface(IID_ITfSource, (LPVOID*)&source);
	hr = source->AdviseSink(IID_ITfUIElementSink, (ITfUIElementSink*)this, &uielementsinkcookie);

	CComPtr<ITfDocumentMgr> documentmgr;
	CComPtr<ITfContext> context;
	hr = threadmgr->CreateDocumentMgr(&documentmgr);
	documentmgr->CreateContext(clientid, 0, (ITfContextOwnerCompositionSink*)this, &context, &editcookie);
	documentmgr->Push(context);
	threadmgr->SetFocus(documentmgr);
}


MeowTextApp::~MeowTextApp()
{
	HRESULT hr;
	CComPtr<ITfSource> source;
	hr = uielementmgr->QueryInterface(IID_ITfSource, (LPVOID*)&source);
	source->UnadviseSink(uielementsinkcookie);

	uielementmgr->Release();

	threadmgrex->Release();
	threadmgr->Release();
}


HRESULT STDMETHODCALLTYPE MeowTextApp::QueryInterface(REFIID riid, void **ppvObj) {
	if (ppvObj == NULL) return E_INVALIDARG;
	*ppvObj = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfUIElementSink))
	{
		*ppvObj = (ITfUIElementSink *)this;
	}
	else if (IsEqualIID(riid, IID_ITfContextOwnerCompositionSink))
	{
		AddRef();

		*ppvObj = (ITfContextOwnerCompositionSink*)this;

		return S_OK;
	}

	if (*ppvObj)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE MeowTextApp::AddRef() {
	return ++reference;
}
ULONG STDMETHODCALLTYPE MeowTextApp::Release() {
	if (--reference <= 0) delete this;
	return reference;
}


HRESULT MeowTextApp::BeginUIElement(DWORD dwUIElementId, BOOL *pbShow)
{
 	if (pbShow)
	{
		*pbShow = FALSE;
		return S_OK;
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT MeowTextApp::UpdateUIElement(DWORD dwUIElementId)
{
	CComPtr<ITfUIElement> uiElement;
	if (SUCCEEDED(uielementmgr->GetUIElement(dwUIElementId, &uiElement)))
	{
		CComPtr<ITfCandidateListUIElement> candidatelistuielement;
		if (SUCCEEDED(uiElement->QueryInterface(IID_ITfCandidateListUIElement, (LPVOID*)&candidatelistuielement)))
		{
			candidatelist.clear();

			UINT count;
			if (SUCCEEDED(candidatelistuielement->GetCount(&count)))
			{
				for (UINT i = 0; i<count; ++i)
				{
					CComBSTR candidate;
					if (SUCCEEDED(candidatelistuielement->GetString(i, &candidate)))
					{
						LPWSTR text = candidate;

						candidatelist.push_back(text);
					}
				}

			}
			InvalidateRect(hwnd, NULL, NULL);
			candidatelistuielement->GetSelection(&m_candidateSelection);
		}
	}

	return S_OK;
}

HRESULT MeowTextApp::EndUIElement(DWORD dwUIElementId)
{
	CComPtr<ITfUIElement> uielement;
	if (SUCCEEDED(uielementmgr->GetUIElement(dwUIElementId, &uielement)))
	{
		CComPtr<ITfCandidateListUIElement> candidatelistuielement;
		if (SUCCEEDED(uielement->QueryInterface(IID_ITfCandidateListUIElement, (LPVOID*)&candidatelistuielement)))
		{
			candidatelist.clear();
		}
	}
	return S_OK;
}

HRESULT MeowTextApp::OnStartComposition(ITfCompositionView *pComposition, BOOL *pfOk)
{
	*pfOk = TRUE;
	return S_OK;
}

HRESULT MeowTextApp::OnUpdateComposition(ITfCompositionView *pComposition, ITfRange *pRangeNew)
{
	CComPtr<ITfRange> compRange = pRangeNew;
	if (compRange == NULL) {
		pComposition->GetRange(&compRange);
	}
	return S_OK;
}

HRESULT MeowTextApp::OnEndComposition(ITfCompositionView *pComposition)
{
	return S_OK;
}
