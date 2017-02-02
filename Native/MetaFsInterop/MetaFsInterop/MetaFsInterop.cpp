// Copyright (C) 2015 Angel Hernandez Matos / Bonafide Ideas.
// You can redistribute this software and/or modify it under the terms of the 
// Microsoft Reciprocal License (Ms-RL).  This program is distributed in the hope 
// that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
// See License.txt for more details. 

/* C++ compiler   : Microsoft (R) C/C++ Optimizing Compiler Version 18.00.31101 for x64
Creation date     : 09/06/2015
Developer         : Angel Hernandez Matos
e-m@il            : angel@bonafideideas.com
Website           : http://www.bonafideideas.com

Description: Implementation of MetaFsInterop class
*/

#include "stdafx.h"
#include "MetaFsInterop.h"


/// <summary>
/// Initializes a new instance of the <see cref="CMetaFsInterop"/> class.
/// </summary>
CMetaFsInterop::CMetaFsInterop() {

}


/// <summary>
/// Initializes the CLR.
/// </summary>
void CMetaFsInterop::InitializeClr() {
	CoInitialize(NULL);

	HANDLE hProcess;
	ExecuteArgs args;
	auto isLoaded = FALSE;
	CComPtr<ICLRMetaHost> pMetaHost;
	CComPtr<ICLRRuntimeInfo> pRuntimeInfo;
	CComPtr<ICLRRuntimeHost> pRuntimeHost;
	CComPtr<IEnumUnknown> pEnumerator;

	args.pwzAssemblyPath = L"Core.dll";
	args.pwzTypeName = L"Core.Concrete.DataOperations";
	args.pwzMethodName = L"PreloadEF";
	args.pwzArgument = L"";
	args.pReturnValue = 0;

	if ((hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId())) != NULL) {
		if (SUCCEEDED(CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&pMetaHost))) {
			isLoaded = SUCCEEDED(pMetaHost->EnumerateLoadedRuntimes(hProcess, &pEnumerator)) && CheckIfClrIsLoaded(pEnumerator);
			if (!isLoaded &&  SUCCEEDED(pMetaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&pRuntimeInfo))) {
				if (SUCCEEDED(pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&pRuntimeHost))) {
					pRuntimeHost->Start();
					pRuntimeHost->ExecuteInDefaultAppDomain(args.pwzAssemblyPath, args.pwzTypeName,
						args.pwzMethodName, args.pwzArgument, &args.pReturnValue);
				}
			}
		}
	}

	CloseHandle(hProcess);

	CoUninitialize();
}


/// <summary>
/// Checks if color is loaded.
/// </summary>
/// <param name="pEnumerator">The p enumerator.</param>
/// <returns>BOOL.</returns>
BOOL CMetaFsInterop::CheckIfClrIsLoaded(const CComPtr<IEnumUnknown>& pEnumerator) {
	ULONG fetched = 0;
	DWORD bufferSize;
	auto retval = FALSE;
	wchar_t buffer[MAX_PATH];
	CComPtr<ICLRRuntimeInfo> pRuntimeInfo;

	while (SUCCEEDED(pEnumerator->Next(1, (IUnknown **)&pRuntimeInfo, &fetched)) && fetched > 0) {
		if ((SUCCEEDED(pRuntimeInfo->GetVersionString(buffer, &bufferSize))))
			if (wcscmp(buffer, L"v4.0.30319") == 0) {
			retval = TRUE;
			break;
			}
	}

	return retval;
}

/*****************************/
// Exported functions        //    
/*****************************/

/// <summary>
/// Initializes the CLR.
/// </summary>
void InitializeClr() {
	CMetaFsInterop metaFsInterop;
	metaFsInterop.InitializeClr();
}
