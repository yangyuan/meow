#pragma once

class MeowWindowManager;
class MeowCompositionManager;
class MeowCandidateManager;

class MeowTextService : 
	public ITfTextInputProcessorEx,
	public ITfThreadMgrEventSink,
	public ITfThreadFocusSink,
	public ITfTextEditSink,
	public ITfKeyEventSink,
	public ITfDisplayAttributeProvider
{
public:
	MeowTextService();
	~MeowTextService();

	// ClassFactory factory callback
	static HRESULT CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj);

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// ITfTextInputProcessorEx
	HRESULT STDMETHODCALLTYPE ActivateEx(ITfThreadMgr *ptim, TfClientId tid, DWORD dwFlags);

	// ITfTextInputProcessor
	HRESULT STDMETHODCALLTYPE Activate(ITfThreadMgr *ptim, TfClientId tid);
	HRESULT STDMETHODCALLTYPE Deactivate(void);

	// ITfThreadFocusSink
	HRESULT STDMETHODCALLTYPE OnSetThreadFocus(void);
	HRESULT STDMETHODCALLTYPE OnKillThreadFocus(void);

	// ITfThreadMgrEventSink
	HRESULT STDMETHODCALLTYPE OnInitDocumentMgr(ITfDocumentMgr *pdim);
	HRESULT STDMETHODCALLTYPE OnUninitDocumentMgr(ITfDocumentMgr *pdim);
	HRESULT STDMETHODCALLTYPE OnSetFocus(ITfDocumentMgr *pdimFocus, ITfDocumentMgr *pdimPrevFocus);
	HRESULT STDMETHODCALLTYPE OnPushContext(ITfContext *pic);
	HRESULT STDMETHODCALLTYPE OnPopContext(ITfContext *pic);

	// ITfTextEditSink
	HRESULT STDMETHODCALLTYPE OnEndEdit(ITfContext *pic, TfEditCookie ecReadOnly, ITfEditRecord *pEditRecord);

	// ITfKeyEventSink
	HRESULT STDMETHODCALLTYPE OnSetFocus(BOOL fForeground);
	HRESULT STDMETHODCALLTYPE OnTestKeyDown(ITfContext *pic, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
	HRESULT STDMETHODCALLTYPE OnTestKeyUp(ITfContext *pic, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
	HRESULT STDMETHODCALLTYPE OnKeyDown(ITfContext *pic, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
	HRESULT STDMETHODCALLTYPE OnKeyUp(ITfContext *pic, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
	HRESULT STDMETHODCALLTYPE OnPreservedKey(ITfContext *pic, REFGUID rguid, BOOL *pfEaten);

	// ITfDisplayAttributeProvider
	HRESULT STDMETHODCALLTYPE EnumDisplayAttributeInfo(IEnumTfDisplayAttributeInfo **ppEnum);
	HRESULT STDMETHODCALLTYPE GetDisplayAttributeInfo(REFGUID guid, ITfDisplayAttributeInfo **ppInfo);

	MeowWindowManager *      window_manager;
	MeowCompositionManager * composition_manager;
	MeowCandidateManager *   candidate_manager;
	ITfThreadMgr * threadmgr;

	TfGuidAtom displayattribute;
private:
	ULONG reference;
	TfClientId clientid;
	DWORD flags;

	DWORD _dwThreadMgrEventSinkCookie;
	BOOL _InitThreadMgrEventSink();
	void _UninitThreadMgrEventSink();

	DWORD _dwThreadFocusSinkCookie;
	BOOL _InitThreadFocusSink();
	VOID _UninitThreadFocusSink();


	// initialize TextEditSink.
	ITfContext   *_pTextEditSinkContext;
	DWORD _dwTextEditSinkCookie;
	BOOL SyncDocumentMgr(ITfDocumentMgr *pDocMgr);

	// initialize and uninitialize KeyEventSink.
	BOOL _InitKeyEventSink();
	void _UninitKeyEventSink();
	BOOL _InitPreservedKey();
	void _UninitPreservedKey();

	BOOL _InitDisplayAttributeGuidAtom();
};

