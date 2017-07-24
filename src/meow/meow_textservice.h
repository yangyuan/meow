#pragma once

class MeowWindowManager;
class MeowCompositionManager;
class MeowCandidateManager;

class MeowTextService: 
	public ITfTextInputProcessorEx,
	public ITfThreadMgrEventSink,
	public ITfThreadFocusSink,
	public ITfDisplayAttributeProvider
{
public:
	MeowTextService();
	~MeowTextService();

	// ClassFactory Callback
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

	// ITfDisplayAttributeProvider
	HRESULT STDMETHODCALLTYPE EnumDisplayAttributeInfo(IEnumTfDisplayAttributeInfo **ppEnum);
	HRESULT STDMETHODCALLTYPE GetDisplayAttributeInfo(REFGUID guid, ITfDisplayAttributeInfo **ppInfo);

	// Managers
	MeowWindowManager *      window_manager;
	MeowCompositionManager * composition_manager;
	MeowCandidateManager *   candidate_manager;

private:
	// 
	ULONG reference;

	ITfThreadMgr * threadmgr;
	TfClientId     clientid;
	DWORD          flags;

	DWORD threadmgreventsink_cookie;
	DWORD threadfocussink_cookie;
	DWORD texteditsink_cookie;
};

// TfGuidAtom displayattribute;
// BOOL InitDisplayAttributeGuidAtom();

namespace Meow {
	// ThreadMgrEventSink
	BOOL InitThreadMgrEventSink(ITfThreadMgr * threadmgr, ITfThreadMgrEventSink * punk, DWORD * cookie);
	VOID UninitThreadMgrEventSink(ITfThreadMgr * threadmgr, DWORD * cookie);
	// ThreadFocusSink
	BOOL InitThreadFocusSink(ITfThreadMgr * threadmgr, ITfThreadFocusSink * punk, DWORD * cookie);
	VOID UninitThreadFocusSink(ITfThreadMgr * threadmgr, DWORD * cookie);
	// KeyEventSink
	BOOL InitKeyEventSink(ITfThreadMgr * threadmgr, ITfKeyEventSink * punk, TfClientId clientid);
	VOID UninitKeyEventSink(ITfThreadMgr * threadmgr, TfClientId clientid);
	// TextEditSink
	BOOL InitTextEditSink(ITfContext * context, ITfTextEditSink * punk, DWORD * cookie);
	VOID UninitTextEditSink(ITfContext * context, DWORD * cookie);
}

