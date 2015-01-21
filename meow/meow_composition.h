#pragma once

typedef enum {
	MEOW_CA_NULL,
	MEOW_CA_CREATE,
	MEOW_CA_COMMIT,
	MEOW_CA_SYNC
} MEOW_COMPOSITION_ACTION;

typedef enum {
	MEOW_KT_CHAR,
	MEOW_KT_NUMBER,
	MEOW_KT_OTHERS
} MEOW_COMPOSITION_KEYTYPE;


class MeowTextService;
// MeowCompositionManager is used to manage a Composition Session
// MeowCompositionManager will decide which key to eat, which key to release
// if ITfContext changed, MeowCompositionManager should be released and recreated
class MeowCompositionManager:
	public ITfCompositionSink {
public:
	MeowCompositionManager(TfClientId, MeowTextService *);
	~MeowCompositionManager();

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	HRESULT STDMETHODCALLTYPE OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition *pComposition);
	HRESULT STDMETHODCALLTYPE OnEndEdit(ITfContext *pContext, TfEditCookie ecReadOnly, ITfEditRecord *pEditRecord);

	VOID Switch(ITfDocumentMgr *pDocMgrFocus, ITfDocumentMgr *pDocMgrPrevFocus);

	BOOL OnTestKeyDown(ITfContext * context, WPARAM vkey);
	BOOL OnTestKeyUp(ITfContext * context, WPARAM vkey);
	BOOL OnKeyDown(ITfContext * context, WPARAM vkey);
	BOOL OnKeyUp(ITfContext * context, WPARAM vkey);

	// callback for MeowEditSession
	VOID DoEditSessionAction(TfEditCookie ec);

	// handle composition, windowmanager but not action
	// buffer should be set before this
	VOID DoCompositionBegin(TfEditCookie ec);
	VOID DoCompositionEnd(TfEditCookie ec);
	VOID DoCompositionSync(TfEditCookie ec);
	VOID DoCompositionSyncWind(TfEditCookie ec); // will only sync CompositionWindow
	VOID DoCompositionSyncText(TfEditCookie ec); // will only sync composition text
	// actually this is not a composition action, but composition status will affect this 
	VOID DoCompositionInsert(TfEditCookie ec, const WCHAR * text);
private:
	ULONG reference;
	TfClientId clientid;
	MeowTextService * textservice;

	MEOW_COMPOSITION_ACTION action;
	ITfContext * actioncontext; // actioncontext combined to action, should be set everytime when action not NULL
	ITfComposition * composition;
	CString buffer;

	BOOL ProcessKeyStroke(ITfContext * context, WPARAM vkey, BOOL * eaten);
	BOOL OnKeyTest(WPARAM vkey);
	static BOOL CheckKeyType(WPARAM vkey, MEOW_COMPOSITION_KEYTYPE type, BOOL incomposition);
};



class MeowEditSession :
	public ITfEditSession
{
public:
	MeowEditSession(MeowCompositionManager * compositionmanager);
	~MeowEditSession();

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	STDMETHODIMP DoEditSession(TfEditCookie ec);
private:
	ULONG reference;
	MeowCompositionManager * compositionmanager;
};

class MeowDisplayAttributeInfoEnum : public IEnumTfDisplayAttributeInfo {
public:
	MeowDisplayAttributeInfoEnum() {
		reference = 1;
		index = 0;
	}
	~MeowDisplayAttributeInfoEnum()  {

	}

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj) {
		if (ppvObj == NULL) return E_INVALIDARG;
		*ppvObj = NULL;
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IEnumTfDisplayAttributeInfo))
		{
			*ppvObj = (IEnumTfDisplayAttributeInfo *)this;
		}
		if (*ppvObj) { AddRef(); return S_OK; }
		return E_NOINTERFACE;
	}
	ULONG STDMETHODCALLTYPE AddRef() {
		return ++reference;
	}
	ULONG STDMETHODCALLTYPE Release() {
		if (--reference <= 0) delete this;
		return reference;
	}

	// IEnumTfDisplayAttributeInfo
	STDMETHODIMP Clone(_Out_ IEnumTfDisplayAttributeInfo **ppEnum);
	STDMETHODIMP Next(ULONG ulCount, __RPC__out_ecount_part(ulCount, *pcFetched) ITfDisplayAttributeInfo **rgInfo, __RPC__out ULONG *pcFetched);
	STDMETHODIMP Reset();
	STDMETHODIMP Skip(ULONG ulCount);

private:
	ULONG reference;
	LONG index;
	const unsigned int count = 1;
};
class MeowDisplayAttributeInfo: public ITfDisplayAttributeInfo {
public:
	MeowDisplayAttributeInfo() {
		reference = 1;
		displayattribute = defaultdisplayattribute;
	}
	~MeowDisplayAttributeInfo()  {

	}

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj) {
		if (ppvObj == NULL) return E_INVALIDARG;
		*ppvObj = NULL;
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfDisplayAttributeInfo))
		{
			*ppvObj = (ITfDisplayAttributeInfo *)this;
		}
		if (*ppvObj) { AddRef(); return S_OK; }
		return E_NOINTERFACE;
	}
	ULONG STDMETHODCALLTYPE AddRef() {
		return ++reference;
	}
	ULONG STDMETHODCALLTYPE Release() {
		if (--reference <= 0) delete this;
		return reference;
	}

	// ITfDisplayAttributeInfo
	STDMETHODIMP GetGUID(_Out_ GUID *pguid);
	STDMETHODIMP GetDescription(_Out_ BSTR *pbstrDesc);
	STDMETHODIMP GetAttributeInfo(_Out_ TF_DISPLAYATTRIBUTE *pTSFDisplayAttr);
	STDMETHODIMP SetAttributeInfo(_In_ const TF_DISPLAYATTRIBUTE *ptfDisplayAttr);
	STDMETHODIMP Reset();

private:
	ULONG reference;
	TCHAR * description = _T("MEOW_DAI_DEFAULT");
	static TF_DISPLAYATTRIBUTE defaultdisplayattribute;
	TF_DISPLAYATTRIBUTE displayattribute;
};

