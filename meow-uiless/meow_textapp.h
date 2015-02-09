#pragma once
class MeowTextApp:
	public ITfContextOwnerCompositionSink,
	public ITfUIElementSink
{
public:
	MeowTextApp(HWND);
	~MeowTextApp();

	HRESULT	STDMETHODCALLTYPE	QueryInterface(REFIID, LPVOID*);
	DWORD	STDMETHODCALLTYPE	AddRef();
	DWORD	STDMETHODCALLTYPE	Release();

	/* ITfContextOwnerCompositionSink */
	HRESULT STDMETHODCALLTYPE	OnStartComposition(ITfCompositionView *pComposition, BOOL *pfOk);
	HRESULT STDMETHODCALLTYPE	OnUpdateComposition(ITfCompositionView *pComposition, ITfRange *pRangeNew);
	HRESULT STDMETHODCALLTYPE	OnEndComposition(ITfCompositionView *pComposition);

	/* ITfUIElementSink */
	HRESULT	STDMETHODCALLTYPE	BeginUIElement(DWORD dwUIElementId, BOOL *pbShow);
	HRESULT	STDMETHODCALLTYPE	UpdateUIElement(DWORD dwUIElementId);
	HRESULT	STDMETHODCALLTYPE	EndUIElement(DWORD dwUIElementId);

	std::vector<std::wstring> candidatelist;
	VOID SetFocus();

private:
	HWND hwnd;
	ULONG reference;
	DWORD clientid;
	ITfThreadMgr * threadmgr;
	ITfThreadMgrEx * threadmgrex;
	ITfUIElementMgr * uielementmgr;

	DWORD uielementsinkcookie;
	TfEditCookie editcookie;


	UINT m_candidateSelection;
};

