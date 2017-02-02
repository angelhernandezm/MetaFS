// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <iostream>

/// <summary>	A macro that defines clsid meta file system class. </summary>
#define CLSID_MetaFsClass _T("{7930DCFD-ECEF-492D-98C9-5DBC6F4C10CF}")
/// <summary>	The approved extension register key. </summary>
#define ApprovedExtensionRegKey _T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved")

#define IDAPPLY 0x3021

#define InteropLibrary L"MetaFsInterop.dll"


// Callbacks
typedef void(*ptrInitializeClr)();

LRESULT CALLBACK CustomWndProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct _ExecuteArgs {
	LPCWSTR pwzAssemblyPath;
	LPCWSTR pwzTypeName;
	LPCWSTR pwzMethodName;
	LPCWSTR pwzArgument;
	DWORD pReturnValue;
} ExecuteArgs, *PExecuteArgs;


class FindChild {
private:
	int targetCount;

public:
	HWND hFound;
	int nMatchCount;
	std::wstring windowName;

	FindChild() : FindChild(L"WindowsForms10.Window", 1) {

	}

	FindChild(const wchar_t* targetWnd, int targetCnt) {
		nMatchCount = 0;
		hFound = nullptr;
		targetCount = targetCnt;
		windowName.assign(targetWnd);
	}

	BOOL FoundTargetIndex() {
		return (nMatchCount == targetCount);
	}
};