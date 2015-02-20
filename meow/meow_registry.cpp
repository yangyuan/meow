#include "meow.h"
#include "meow_registry.h"

//FIXME: all logic in this file should be rewriten, remove goto, add error detect

namespace MeowRegistry {
	const TCHAR RegInfo_Prefix_CLSID[] = _T("CLSID\\");
	const TCHAR RegInfo_Key_InProSvr32[] = _T("InProcServer32");
	const TCHAR RegInfo_Key_ThreadModel[] = _T("ThreadingModel");
	const int CLSID_STRLEN = 38;  // strlen("{xxxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxx}")

	// modified from sample
	LONG RecurseDeleteKey(_In_ HKEY hParentKey, _In_ LPCTSTR lpszKey)
	{
		HKEY regKeyHandle = nullptr;
		LONG res = 0;
		FILETIME time;
		WCHAR stringBuffer[256] = { '\0' };
		DWORD size = ARRAYSIZE(stringBuffer);

		if (RegOpenKey(hParentKey, lpszKey, &regKeyHandle) != ERROR_SUCCESS)
		{
			return ERROR_SUCCESS;
		}
		res = ERROR_SUCCESS;
		while (RegEnumKeyEx(regKeyHandle, 0, stringBuffer, &size, NULL, NULL, NULL, &time) == ERROR_SUCCESS)
		{
			stringBuffer[ARRAYSIZE(stringBuffer) - 1] = '\0';
			res = RecurseDeleteKey(regKeyHandle, stringBuffer);
			if (res != ERROR_SUCCESS)
			{
				break;
			}
			size = ARRAYSIZE(stringBuffer);
		}
		RegCloseKey(regKeyHandle);
		return res == ERROR_SUCCESS ? RegDeleteKey(hParentKey, lpszKey) : res;
	}

	// modified from sample, support TCHAR
	BOOL CLSIDToString(REFGUID refGUID, TCHAR *pCLSIDString)
	{
		static const BYTE GuidSymbols[] = { 3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-', 8, 9, '-', 10, 11, 12, 13, 14, 15 };
		static const TCHAR HexDigits[] = _T("0123456789ABCDEF");

		TCHAR* pTemp = pCLSIDString;
		const BYTE* pBytes = (const BYTE *)&refGUID;

		DWORD j = 0;
		pTemp[j++] = _T('{');
		for (int i = 0; i < sizeof(GuidSymbols) && j < (CLSID_STRLEN - 2); i++)
		{
			if (GuidSymbols[i] == '-')
			{
				pTemp[j++] = _T('-');
			}
			else
			{
				pTemp[j++] = HexDigits[(pBytes[GuidSymbols[i]] & 0xF0) >> 4];
				pTemp[j++] = HexDigits[(pBytes[GuidSymbols[i]] & 0x0F)];
			}
		}

		pTemp[j++] = _T('}');
		pTemp[j] = _T('\0');

		return TRUE;
	}
}

BOOL MeowRegistry::RegisterServer() {
	DWORD copiedStringLen = 0;
	HKEY regKeyHandle = nullptr;
	HKEY regSubkeyHandle = nullptr;
	BOOL ret = FALSE;
	TCHAR achIMEKey[ARRAYSIZE(RegInfo_Prefix_CLSID) + CLSID_STRLEN] = { '\0' };
	TCHAR achFileName[MAX_PATH] = { '\0' };

	if (!CLSIDToString(Meow::CLSID_TEXTSERVICE, achIMEKey + ARRAYSIZE(RegInfo_Prefix_CLSID) - 1))
	{
		return FALSE;
	}

	memcpy(achIMEKey, RegInfo_Prefix_CLSID, sizeof(RegInfo_Prefix_CLSID) - sizeof(TCHAR));

	if (RegCreateKeyEx(HKEY_CLASSES_ROOT, achIMEKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &regKeyHandle, &copiedStringLen) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(regKeyHandle, NULL, 0, REG_SZ, (const BYTE *)Meow::STRING_TEXTSERVICE_TITLE, (lstrlen(Meow::STRING_TEXTSERVICE_TITLE) + 1)*sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			goto Exit;
		}

		if (RegCreateKeyEx(regKeyHandle, RegInfo_Key_InProSvr32, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &regSubkeyHandle, &copiedStringLen) == ERROR_SUCCESS)
		{
			copiedStringLen = GetModuleFileName(Meow::hinstance, achFileName, ARRAYSIZE(achFileName));
			copiedStringLen = (copiedStringLen >= (MAX_PATH - 1)) ? MAX_PATH : (++copiedStringLen);
			if (RegSetValueEx(regSubkeyHandle, NULL, 0, REG_SZ, (const BYTE *)achFileName, (copiedStringLen)*sizeof(TCHAR)) != ERROR_SUCCESS)
			{
				goto Exit;
			}
			if (RegSetValueEx(regSubkeyHandle, RegInfo_Key_ThreadModel, 0, REG_SZ, (const BYTE *)Meow::STRING_TEXTSERVICE_THREADMODEL, (lstrlen(Meow::STRING_TEXTSERVICE_THREADMODEL) + 1)*sizeof(TCHAR) * sizeof(TCHAR)) != ERROR_SUCCESS)
			{
				goto Exit;
			}

			ret = TRUE;
		}
	}

Exit:
	if (regSubkeyHandle)
	{
		RegCloseKey(regSubkeyHandle);
		regSubkeyHandle = nullptr;
	}
	if (regKeyHandle)
	{
		RegCloseKey(regKeyHandle);
		regKeyHandle = nullptr;
	}

	return ret;
}
BOOL MeowRegistry::UnregisterServer() {
	TCHAR achIMEKey[ARRAYSIZE(RegInfo_Prefix_CLSID) + CLSID_STRLEN] = { '\0' };
	if (!CLSIDToString(Meow::CLSID_TEXTSERVICE, achIMEKey + ARRAYSIZE(RegInfo_Prefix_CLSID) - 1))
	{
		return FALSE;
	}
	memcpy(achIMEKey, RegInfo_Prefix_CLSID, sizeof(RegInfo_Prefix_CLSID) - sizeof(TCHAR));
	RecurseDeleteKey(HKEY_CLASSES_ROOT, achIMEKey);
	return TRUE;
}
BOOL MeowRegistry::RegisterProfiles() {

	// ITfInputProcessorProfileMgr is only supported after Vista
	// ITfInputProcessorProfileMgr *pITfInputProcessorProfileMgr = nullptr;
	HRESULT hr = S_FALSE;
	ITfInputProcessorProfiles *pInputProcessProfiles;

	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, CLSCTX_INPROC_SERVER,
		IID_ITfInputProcessorProfiles, (void**)&pInputProcessProfiles);

	if (FAILED(hr))
	{
		return FALSE;
	}

	TCHAR achIconFile[MAX_PATH] = { '\0' };
	DWORD cchA = 0;
	cchA = GetModuleFileName(Meow::hinstance, achIconFile, MAX_PATH);
	cchA = cchA >= MAX_PATH ? (MAX_PATH - 1) : cchA;
	achIconFile[cchA] = '\0';

	size_t lenOfDesc = 0;
	hr = StringCchLength(Meow::STRING_TEXTSERVICE_TITLE, STRSAFE_MAX_CCH, &lenOfDesc);
	if (hr != S_OK)
	{
		goto Exit;
	}
	hr = pInputProcessProfiles->AddLanguageProfile(Meow::CLSID_TEXTSERVICE,
		Meow::LANGID_TEXTSERVICE,
		Meow::GUID_PROFILE,
		Meow::STRING_TEXTSERVICE_TITLE,
		static_cast<ULONG>(lenOfDesc),
		achIconFile,
		cchA,
		(UINT)Meow::ICONINDEX_TEXTSERVICE);

	if (FAILED(hr))
	{
		goto Exit;
	}

Exit:
	if (pInputProcessProfiles)
	{
		pInputProcessProfiles->Release();
	}

	return (hr == S_OK);
}
BOOL MeowRegistry::UnregisterProfiles() {
	HRESULT hr = S_OK;

	ITfInputProcessorProfiles *pInputProcessProfiles = nullptr;
	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, CLSCTX_INPROC_SERVER,
		IID_ITfInputProcessorProfiles, (void**)&pInputProcessProfiles);
	if (FAILED(hr))
	{
		goto Exit;
	}

	hr = pInputProcessProfiles->Unregister(Meow::CLSID_TEXTSERVICE);
	if (FAILED(hr))
	{
		goto Exit;
	}

Exit:
	if (pInputProcessProfiles)
	{
		pInputProcessProfiles->Release();
	}

	return TRUE;
}

VOID UnregisterProfilesEx() {
	HRESULT hr = S_OK;

	ITfInputProcessorProfileMgr *pITfInputProcessorProfileMgr = nullptr;
	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, CLSCTX_INPROC_SERVER,
		IID_ITfInputProcessorProfileMgr, (void**)&pITfInputProcessorProfileMgr);
	if (FAILED(hr))
	{
		goto Exit;
	}

	hr = pITfInputProcessorProfileMgr->UnregisterProfile(Meow::CLSID_TEXTSERVICE, Meow::LANGID_TEXTSERVICE, Meow::GUID_PROFILE, 0);
	if (FAILED(hr))
	{
		goto Exit;
	}

Exit:
	if (pITfInputProcessorProfileMgr)
	{
		pITfInputProcessorProfileMgr->Release();
	}

	return;
}
BOOL MeowRegistry::RegisterCategories() {
	ITfCategoryMgr* pCategoryMgr = nullptr;
	HRESULT hr = S_OK;

	hr = CoCreateInstance(CLSID_TF_CategoryMgr, NULL, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, (void**)&pCategoryMgr);
	if (FAILED(hr))
	{
		return FALSE;
	}


	for each(GUID guid in Meow::SupportCategories)
	{
		hr = pCategoryMgr->RegisterCategory(Meow::CLSID_TEXTSERVICE, guid, Meow::CLSID_TEXTSERVICE);
	}

	pCategoryMgr->Release();

	return (hr == S_OK);
}
BOOL MeowRegistry::UnregisterCategories() {

	ITfCategoryMgr* pCategoryMgr = S_OK;
	HRESULT hr = S_OK;

	hr = CoCreateInstance(CLSID_TF_CategoryMgr, NULL, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, (void**)&pCategoryMgr);
	if (FAILED(hr))
	{
		return FALSE;
	}

	for each(GUID guid in Meow::SupportCategories)
	{
		pCategoryMgr->UnregisterCategory(Meow::CLSID_TEXTSERVICE, guid, Meow::CLSID_TEXTSERVICE);
	}

	pCategoryMgr->Release();

	return TRUE;
}
