#include "stdafx.h"
#include "resource.h"
#include "dllmain.h"
#include "MetaFsExt_i.h"

/// <summary>	The module. </summary>
CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_MetaFsPropPage, CMetaFsPropPage)
END_OBJECT_MAP()


extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		_Module.Init(ObjectMap, hInstance);
		DisableThreadLibraryCalls(hInstance);
	} else if (dwReason == DLL_PROCESS_DETACH)
		_Module.Term();

	return TRUE;
}

STDAPI DllCanUnloadNow() {
	return (_Module.GetLockCount() == 0) ? S_OK : S_FALSE;
}


STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
	return _Module.GetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void) {
	return _Module.RegisterServer(TRUE);
}

STDAPI DllUnregisterServer(void) {
	_Module.UnregisterServer();

	HKEY hKey = 0;
	LONG lErr = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE, ApprovedExtensionRegKey, NULL, KEY_ALL_ACCESS, &hKey);

	if (ERROR_SUCCESS == lErr) {
		RegDeleteValue(hKey, CLSID_MetaFsClass);
		RegCloseKey(hKey);
	}

	return S_OK;
}