#pragma once

class MeowWindowManager;
class MeowCompositionManager;
class MeowUILessManager;

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

	// traditional ClassFactory factory callback
	static HRESULT CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj);

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// ITfTextInputProcessorEx
	HRESULT STDMETHODCALLTYPE ActivateEx(ITfThreadMgr *ptim, TfClientId tid, DWORD dwFlags);
	HRESULT STDMETHODCALLTYPE Activate(ITfThreadMgr *pThreadMgr, TfClientId tfClientId);
	HRESULT STDMETHODCALLTYPE Deactivate();

	// ITfThreadFocusSink
	HRESULT STDMETHODCALLTYPE OnSetThreadFocus();
	HRESULT STDMETHODCALLTYPE OnKillThreadFocus();

	// ITfThreadMgrEventSink
	STDMETHODIMP OnInitDocumentMgr(ITfDocumentMgr *pDocMgr);
	STDMETHODIMP OnUninitDocumentMgr(ITfDocumentMgr *pDocMgr);
	STDMETHODIMP OnSetFocus(ITfDocumentMgr *pDocMgrFocus, ITfDocumentMgr *pDocMgrPrevFocus);
	STDMETHODIMP OnPushContext(ITfContext *pContext);
	STDMETHODIMP OnPopContext(ITfContext *pContext);

	// ITfTextEditSink
	STDMETHODIMP OnEndEdit(ITfContext *pContext, TfEditCookie ecReadOnly, ITfEditRecord *pEditRecord);

	// ITfKeyEventSink
	STDMETHODIMP OnSetFocus(BOOL fForeground);
	STDMETHODIMP OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
	STDMETHODIMP OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
	STDMETHODIMP OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
	STDMETHODIMP OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
	STDMETHODIMP OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten);


	// ITfDisplayAttributeProvider
	HRESULT STDMETHODCALLTYPE EnumDisplayAttributeInfo( IEnumTfDisplayAttributeInfo **ppEnum);
	HRESULT STDMETHODCALLTYPE GetDisplayAttributeInfo( REFGUID guid, ITfDisplayAttributeInfo **ppInfo);

	MeowWindowManager * windowmanager;
	MeowCompositionManager * compositionmanager;
	MeowUILessManager * uilessmanager;
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

