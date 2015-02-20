#include "meow.h"
#include "meow_composition.h"
#include "meow_textservice.h"
#include "meow_candidate.h"
#include "meow_window.h"


MeowCompositionManager::MeowCompositionManager(TfClientId _clientid, MeowTextService * _textservice) {
	clientid = _clientid;
	textservice = _textservice;
	action = MEOW_CA_NULL;
	composition = NULL;
	reference = 1;
	buffer = CString(_T(""));

}
MeowCompositionManager::~MeowCompositionManager() {
}

HRESULT STDMETHODCALLTYPE MeowCompositionManager::QueryInterface(REFIID riid, void **ppvObj) {
	if (ppvObj == NULL) return E_INVALIDARG;
	*ppvObj = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfCompositionSink))
	{
		*ppvObj = (ITfCompositionSink *)this;
	}

	if (*ppvObj)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE MeowCompositionManager::AddRef() {
	return ++reference;
}
ULONG STDMETHODCALLTYPE MeowCompositionManager::Release() {
	if (--reference <= 0) delete this;
	return reference;
}



STDAPI MeowCompositionManager::OnEndEdit(ITfContext *pContext, TfEditCookie ecReadOnly, ITfEditRecord *pEditRecord)
{	// called everything when EditSession is done
	// maybe not necessary in SYNC call
	return S_OK;
}


STDAPI MeowCompositionManager::OnSetFocus(BOOL fForeground)
{
	// may need to store and resume composition here
	/*
	ITfDocumentMgr * pDocMgrFocus;
	HRESULT hr = threadmgr->GetFocus(&pDocMgrFocus);
	if ((hr == S_OK) && (pDocMgrFocus != NULL))
	{
		SyncDocumentMgr(pDocMgrFocus);
		pDocMgrFocus->Release();
	}
	*/
	return S_OK;
}

STDAPI MeowCompositionManager::OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	*pfEaten = OnTestKeyDown(pContext, wParam);
	return S_OK;
}

STDAPI MeowCompositionManager::OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	*pfEaten = OnTestKeyUp(pContext, wParam);
	return S_OK;
}

STDAPI MeowCompositionManager::OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	*pfEaten = OnKeyDown(pContext, wParam);
	return S_OK;
}

STDAPI MeowCompositionManager::OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	*pfEaten = OnKeyUp(pContext, wParam);
	return S_OK;
}

STDAPI MeowCompositionManager::OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten)
{
	/*
	if (IsEqualGUID(rguid, GUID_PRESERVEDKEY_ONOFF))
	{
	BOOL fOpen = _IsKeyboardOpen();
	_SetKeyboardOpen(fOpen ? FALSE : TRUE);
	*pfEaten = TRUE;
	}
	else
	{
	*pfEaten = FALSE;
	}
	*/
	return S_OK;
}


HRESULT STDMETHODCALLTYPE MeowCompositionManager::OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition *pComposition) {
	// windows want your composition terminated
	// maybe target app closed or sth
	// accroding to msdn, you dont need to do anything here. but IDK about the release of pComposition
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms538657(v=vs.85).aspx
	// action = MEOW_CA_NULL;
	if (pComposition == composition) {
		composition->Release();
		composition = NULL;
	}
	else {
		
	}
	return S_OK;
}


VOID MeowCompositionManager::Switch(ITfDocumentMgr *pDocMgrFocus, ITfDocumentMgr *pDocMgrPrevFocus) {
	// one program can only have one focus ITfDocumentMgr

	if (action != MEOW_CA_NULL) {
		action = MEOW_CA_NULL; // stop all actions
	}

	if (pDocMgrPrevFocus != NULL) {

		// BUG:
		// COMMIT should only happen if there is a remain composition

		// sometimes pDocMgrPrevFocus may already released
		ITfContext * context;
		pDocMgrPrevFocus->GetTop(&context);
		// FIXME: current context may not be the top, needs to handle sth here.
		action = MEOW_CA_COMMIT;
		actioncontext = context;
		MeowEditSession * mes = new MeowEditSession(this);
		HRESULT hr = context->RequestEditSession(clientid, mes, TF_ES_SYNC | TF_ES_READWRITE, &hr);
		mes->Release();
	}





	

	// When DocumentMgr changed, we need to reset the Sinks binded to the DocumentMgr
	// clear previous Sink

	/* ITfTextEditSink is not used
	if (_pTextEditSinkContext != NULL && texteditsink_cookie != TF_INVALID_COOKIE)
	{
		Meow::UninitTextEditSink(_pTextEditSinkContext, &texteditsink_cookie);
		_pTextEditSinkContext->Release();
		_pTextEditSinkContext = NULL;
	}

	if (pDocMgr == NULL)
	{
		return TRUE; // caller just wanted to clear the previous sink
	}

	// setup a new sink advised to the topmost context of the document
	if (pDocMgr->GetTop(&_pTextEditSinkContext) == S_OK) {
		Meow::InitTextEditSink(_pTextEditSinkContext, composition_manager, &texteditsink_cookie);
		_pTextEditSinkContext->Release();
		return TRUE;
	}
	else {
		_pTextEditSinkContext = NULL;
		return FALSE;
	}
	*/
}

BOOL MeowCompositionManager::OnTestKeyDown(ITfContext * context, WPARAM vkey) {
	return OnKeyTest(vkey);
}
BOOL MeowCompositionManager::OnTestKeyUp(ITfContext * context, WPARAM vkey){
	return OnKeyTest(vkey);
}
BOOL MeowCompositionManager::OnKeyDown(ITfContext * context, WPARAM vkey){
	BOOL eaten = FALSE;
	BOOL ret = ProcessKeyStroke(context, vkey, &eaten);
	if (!ret) {
		Sleep(128);
		ret = ProcessKeyStroke(context, vkey, &eaten);
	}
	textservice->candidate_manager->Show();
	if (ret) return eaten;
	else return TRUE; // eat all if in error
}
BOOL MeowCompositionManager::OnKeyUp(ITfContext * context, WPARAM vkey){
	return OnKeyTest(vkey);
}

BOOL MeowCompositionManager::OnKeyTest(WPARAM vkey) {
	BOOL ret = FALSE;
	//EnterCriticalSection(&criticalsection);
	if (composition != NULL) {
		ret = TRUE; // eat every key in composition 
	} else {
		if (vkey >= 0x41 && vkey <= 0x5A) { // A - Z
			ret = TRUE; // a-z will triger composition
		}
	}
	//LeaveCriticalSection(&criticalsection);
	return ret;
}

BOOL MeowCompositionManager::ProcessKeyStroke(ITfContext * context, WPARAM vkey, BOOL * eaten) {
	// ProcessKeyStroke only failed when handle pending situation
	BOOL ret = FALSE;
	//EnterCriticalSection(&criticalsection);
	if (action == MEOW_CA_NULL) {
		ret = TRUE;
		if (composition != NULL) {
			if (vkey == VK_SPACE || vkey == VK_RETURN || CheckKeyType(vkey, MEOW_KT_NUMBER, TRUE)) {
				action = MEOW_CA_COMMIT;
				actioncontext = context;
				*eaten = TRUE;
			}
			else if (vkey == VK_BACK) {
				int len = buffer.GetLength();
				len = len - 1;
				if (len >= 0) {
					buffer.Delete(len, 1);
				}
				*eaten = TRUE;
				if (len == 0) {
					action = MEOW_CA_COMMIT;
				} else {
					action = MEOW_CA_SYNC;
				}
				actioncontext = context;
			}
			else {
				buffer += WCHAR(vkey);
				*eaten = TRUE;
				action = MEOW_CA_SYNC;
				actioncontext = context;
			}
		}
		else {
			if (CheckKeyType(vkey, MEOW_KT_CHAR, FALSE)) {
				*eaten = TRUE;
				action = MEOW_CA_CREATE;
				actioncontext = context;
				buffer.SetString(_T(""));
				buffer += WCHAR(vkey);
			} else {
				*eaten = FALSE;
			}
		}

		if (action != MEOW_CA_NULL) {
			MeowEditSession * mes = new MeowEditSession(this);
			HRESULT hr = context->RequestEditSession(clientid, mes, TF_ES_SYNC | TF_ES_READWRITE, &hr);
			mes->Release();
		}
	}

	return ret;
}



VOID MeowCompositionManager::DoEditSessionAction(TfEditCookie ec) {
	// currently this function is called sync-ly
	// I think it is safer to use an action list to manager pending actions

	// CREATE -> SYNC -> SYNC -> COMMIT
	switch (action) {
	case MEOW_CA_CREATE:
		DoCompositionBegin(ec);
		DoCompositionSyncWind(ec);
		break;
	case MEOW_CA_COMMIT:
		DoCompositionSyncText(ec);
		DoCompositionEnd(ec);
		break;
	case MEOW_CA_SYNC:
		DoCompositionSyncWind(ec);
		break;
	case MEOW_CA_NULL: // should never happen
	default:
		break;
	}
	action = MEOW_CA_NULL;
}

VOID MeowCompositionManager::DoCompositionBegin(TfEditCookie ec) {

	ITfInsertAtSelection * insertatselection = NULL;
	ITfRange * range = NULL;
	ITfContextComposition * contextcomposition = NULL;

	if (actioncontext->QueryInterface(IID_ITfInsertAtSelection, (void **)&insertatselection) == S_OK) {
		if (insertatselection->InsertTextAtSelection(ec, TF_IAS_QUERYONLY, NULL, 0, &range) == S_OK) {
			if (actioncontext->QueryInterface(IID_ITfContextComposition, (void **)&contextcomposition) == S_OK) {
				HRESULT hr = contextcomposition->StartComposition(ec, range, (ITfCompositionSink *)this, &composition);
				ULONG x = 0;
				if ((hr == S_OK) && (composition != NULL))
				{
					TF_SELECTION tfSelection;
					tfSelection.range = range;
					tfSelection.style.ase = TF_AE_NONE;
					tfSelection.style.fInterimChar = FALSE;
					actioncontext->SetSelection(ec, 1, &tfSelection);
				}
				contextcomposition->Release();
			}
			range->Release();
		}
		insertatselection->Release();
	}

	// composition start finished
	// usually we need to AdjustCompositionWindow 
	if (composition != NULL) {
		ITfContextView * view;
		if (actioncontext->GetActiveView(&view) == S_OK && (view != NULL)) {
			RECT rect;
			ULONG fetched = 0;
			TF_SELECTION tfselection;
			if (actioncontext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfselection, &fetched) == S_OK && fetched != 0) {
				BOOL fClipped;
				HRESULT hrx = view->GetTextExt(ec, tfselection.range, &rect, &fClipped);
				if (hrx != S_OK) {
					
				}
				textservice->window_manager->AdjustCompositionWindow(&rect, NULL);
				tfselection.range->Release();
			}
		}
	}
}

VOID MeowCompositionManager::DoCompositionEnd(TfEditCookie ec) {
	if (composition != NULL) {
		HRESULT hr = composition->EndComposition(ec);
		if (hr != S_OK) {
			
		}
		composition->Release();
		composition = NULL;

		// bug fix for windows xp
		ULONG fetched = 0;
		TF_SELECTION tfselection;
		if (actioncontext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfselection, &fetched) == S_OK && fetched != 0) {
			tfselection.range->Collapse(ec, TF_ANCHOR_END);
			// in windows xp, Selection will not be reset, manually reset here, IDK why
			actioncontext->SetSelection(ec, 1, &tfselection);
			tfselection.range->Release();
		}
		
	}
	textservice->window_manager->AdjustCompositionWindow(NULL, _T(""));
}

VOID MeowCompositionManager::DoCompositionSync(TfEditCookie ec) {
	ITfRange * range;
	if (composition->GetRange(&range) == S_OK)
	{
		
		if (range->SetText(ec, 0, (PCTSTR)buffer, buffer.GetLength()) == S_OK) {
			textservice->window_manager->AdjustCompositionWindow(NULL, (PCTSTR)buffer);
			textservice->window_manager->RefreshCandidate();
		}
		
		/*
		ITfProperty* property = NULL;
		if (actioncontext->GetProperty(GUID_PROP_ATTRIBUTE, &property) == S_OK)
		{
			VARIANT var;
			// set the value over the range
			// the application will use this guid atom to lookup the acutal rendering information
			var.vt = VT_I4; // we're going to set a TfGuidAtom
			var.lVal = textservice->displayattribute;
			property->SetValue(ec, range, &var);
			property->Release();
		}
		*/

		range->Release();
	}
}

VOID MeowCompositionManager::DoCompositionSyncWind(TfEditCookie ec) {
	ITfRange * range;
	if (composition->GetRange(&range) == S_OK)
	{
		if (range->SetText(ec, 0, (PCTSTR)buffer, 0) == S_OK) {
			textservice->window_manager->AdjustCompositionWindow(NULL, (PCTSTR)buffer);
			textservice->window_manager->RefreshCandidate();
		}
		range->Release();
	}
}

VOID MeowCompositionManager::DoCompositionSyncText(TfEditCookie ec) {
	ITfRange * range;
	if (composition->GetRange(&range) == S_OK)
	{
		if (range->SetText(ec, 0, (PCTSTR)buffer, buffer.GetLength()) == S_OK) {
			textservice->window_manager->AdjustCompositionWindow(NULL, (PCTSTR)buffer);
			textservice->window_manager->RefreshCandidate();
		}
		range->Release();
	}

}



VOID MeowCompositionManager::DoCompositionInsert(TfEditCookie ec, const WCHAR * text) {
/* // IsRangeCovered can be used to dicide to "end current composition" or not
BOOL IsRangeCovered(TfEditCookie ec, ITfRange *pRangeTest, ITfRange *pRangeCover)
{
	LONG lResult;
	if (pRangeCover->CompareStart(ec, pRangeTest, TF_ANCHOR_START, &lResult) != S_OK || lResult > 0) { return FALSE; }
	if (pRangeCover->CompareEnd(ec, pRangeTest, TF_ANCHOR_END, &lResult) != S_OK || lResult < 0) { return FALSE; }
	return TRUE;
} */
	TF_SELECTION selection;
	ULONG fetched;
	if (actioncontext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &selection, &fetched) != S_OK) { return; }
	// TF_SELECTION.range should be release when success
	if (fetched == 1) {
		selection.range->SetText(ec, 0, text, wcslen(text));
		actioncontext->SetSelection(ec, 1, &selection);
	}
	selection.range->Release();
}

BOOL MeowCompositionManager::CheckKeyType(WPARAM vkey, MEOW_COMPOSITION_KEYTYPE type, BOOL incomposition) {
	switch (type)
	{
	case MEOW_KT_CHAR:
		if (vkey >= 0x41 && vkey <= 0x5A) return TRUE;
		break;
	case MEOW_KT_NUMBER:
		if (vkey >= 0x30 && vkey <= 0x39) return TRUE;
		break;
	default:
		break;
	}

	return FALSE;
}


MeowEditSession::MeowEditSession(MeowCompositionManager * _compositionmanager)
{
	compositionmanager = _compositionmanager;
	reference = 1;
}


MeowEditSession::~MeowEditSession()
{
}

HRESULT STDMETHODCALLTYPE MeowEditSession::QueryInterface(REFIID riid, void **ppvObj) {
	if (ppvObj == NULL) return E_INVALIDARG;
	*ppvObj = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfEditSession))
	{
		*ppvObj = (ITfEditSession *)this;
	}

	if (*ppvObj)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE MeowEditSession::AddRef() {
	return ++reference;
}
ULONG STDMETHODCALLTYPE MeowEditSession::Release() {
	if (--reference <= 0) delete this;
	return reference;
}

STDMETHODIMP MeowEditSession::DoEditSession(TfEditCookie ec) {
	compositionmanager->DoEditSessionAction(ec);
	return S_OK;
}

// TF_CT_NONE app defaulr color
TF_DISPLAYATTRIBUTE MeowDisplayAttributeInfo::defaultdisplayattribute = {
	{ TF_CT_COLORREF, RGB(255, 0, 0) }, // text color
	{ TF_CT_COLORREF, RGB(255, 255, 255) },  // background color (TF_CT_NONE => app default) { TF_CT_COLORREF, RGB(0, 255, 255) }, 
	TF_LS_NONE,                             // underline style
	FALSE,                                  // underline boldness
	{ TF_CT_NONE, 0 },                      // underline color { TF_CT_COLORREF, RGB(0, 103,206) }, 
	TF_ATTR_INPUT                // attribute info
};

STDMETHODIMP MeowDisplayAttributeInfoEnum::Clone(_Out_ IEnumTfDisplayAttributeInfo **ppEnum) {
	MeowDisplayAttributeInfoEnum * pClone = nullptr;
	if (ppEnum == nullptr)
	{
		return E_INVALIDARG;
	}
	*ppEnum = nullptr;
	pClone = new (std::nothrow) MeowDisplayAttributeInfoEnum();
	if ((pClone) == nullptr)
	{
		return E_OUTOFMEMORY;
	}
	pClone->index = index;
	*ppEnum = pClone;
	return S_OK;
}
STDMETHODIMP MeowDisplayAttributeInfoEnum::Next(ULONG ulCount, __RPC__out_ecount_part(ulCount, *pcFetched) ITfDisplayAttributeInfo **rgInfo, __RPC__out ULONG *pcFetched) {
	ULONG fetched;

	fetched = 0;

	if (ulCount == 0)
	{
		return S_OK;
	}
	if (rgInfo == nullptr)
	{
		return E_INVALIDARG;
	}
	*rgInfo = nullptr;

	while (fetched < ulCount)
	{
		ITfDisplayAttributeInfo* pDisplayAttributeInfo = nullptr;

		if (index == 0)
		{
			pDisplayAttributeInfo = new (std::nothrow) MeowDisplayAttributeInfo();
			if ((pDisplayAttributeInfo) == nullptr)
			{
				return E_OUTOFMEMORY;
			}
		}
		else
		{
			break;
		}

		*rgInfo = pDisplayAttributeInfo;
		rgInfo++;
		fetched++;
		index++;
	}

	if (pcFetched != nullptr)
	{
		// technically this is only legal if ulCount == 1, but we won't check
		*pcFetched = fetched;
	}

	return (fetched == ulCount) ? S_OK : S_FALSE;
}
STDMETHODIMP MeowDisplayAttributeInfoEnum::Reset() {
	index = 0;
	return S_OK;
}
STDMETHODIMP MeowDisplayAttributeInfoEnum::Skip(ULONG ulCount) {
	if ((ulCount + index) > count || (ulCount + index) < ulCount)
	{
		index = count;
		return S_FALSE;
	}
	index += ulCount;
	return S_OK;
}





// ITfDisplayAttributeInfo
STDMETHODIMP MeowDisplayAttributeInfo::GetGUID(_Out_ GUID *pguid) {
	if (pguid == nullptr) return E_INVALIDARG;
	*pguid = Meow::GUID_DISPLAY_ARRTIBUTE_INFO;
	return S_OK;
}
STDMETHODIMP MeowDisplayAttributeInfo::GetDescription(_Out_ BSTR *pbstrDesc) {
	// this func is require to use SysAllocString
	BSTR tmp;
	if (pbstrDesc == nullptr) return E_INVALIDARG;
	*pbstrDesc = nullptr;
	if ((tmp = SysAllocString(description)) == nullptr)
	{
		return E_OUTOFMEMORY;
	}
	*pbstrDesc = tmp;
	return S_OK;
}
STDMETHODIMP MeowDisplayAttributeInfo::GetAttributeInfo(_Out_ TF_DISPLAYATTRIBUTE *pda) {
	if (pda == nullptr)
	{
		return E_INVALIDARG;
	}
	*pda = displayattribute;
	return S_OK;
}
STDMETHODIMP MeowDisplayAttributeInfo::SetAttributeInfo(_In_ const TF_DISPLAYATTRIBUTE * pda) {
	if (pda == nullptr)
	{
		return E_INVALIDARG;
	}
	displayattribute = *pda;
	return S_OK;
}
STDMETHODIMP MeowDisplayAttributeInfo::Reset() {
	return SetAttributeInfo(&defaultdisplayattribute);
}