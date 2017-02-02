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

Description: Implementation of CMetaFsPropPage
*/

#include "stdafx.h"
#include "MetaFsPropPage.h"
#include <CommCtrl.h>

#import "..\..\MetaFSPropPage\bin\x64\Debug\MetaFSPropPage.tlb" 

WNDPROC pWndProc;

LRESULT CALLBACK CustomWndProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	FindChild* findChildWnd;
	auto retval = CallWindowProc(pWndProc, hwndDlg, msg, wParam, lParam);

	switch (msg) {
	case WM_COMMAND:
		auto selectedButton = LOWORD(wParam);
		if (selectedButton == IDOK || selectedButton == IDAPPLY || selectedButton == IDCANCEL) {
			auto enumChildProc = [](HWND hwnd, LPARAM lParam)-> BOOL {
				auto retval = TRUE;
				wchar_t buffer[MAX_PATH];
				auto lParamValues(reinterpret_cast<FindChild*>(lParam));

				if (GetClassName(hwnd, buffer, wcslen(buffer)) > 0) {
					wstring className(buffer);
					if (className.find(lParamValues->windowName) != wstring::npos) {
						lParamValues->nMatchCount++;

						if (lParamValues->FoundTargetIndex()) {
							lParamValues->hFound = hwnd;
							retval = FALSE;
						}
					}
				}
				return retval;
			};

			EnumChildWindows(hwndDlg, enumChildProc, reinterpret_cast<LPARAM>((findChildWnd = new FindChild)));

			switch (selectedButton) {
			case IDOK:
				SendMessage(findChildWnd->hFound, WM_APP, IDOK, NULL);
				break;
			case IDAPPLY:
				SendMessage(findChildWnd->hFound, WM_APP, IDAPPLY, NULL);
				break;
			case IDCANCEL:
				SendMessage(findChildWnd->hFound, WM_APP, IDCANCEL, NULL);
				break;
			}

			delete findChildWnd;
		}
		break;
	}

	return retval;
}


CMetaFsPropPage::CMetaFsPropPage() {

}


CMetaFsPropPage::~CMetaFsPropPage() {

}




HRESULT CMetaFsPropPage::Initialize(IN LPCITEMIDLIST pidlFolder, IN LPDATAOBJECT lpdobj, IN HKEY hkeyProgID) {
	HDROP hDrop;
	DWORD threadId;
	STGMEDIUM medium;
	auto retval = E_INVALIDARG;
	TCHAR szFileName[MAX_PATH];
	FORMATETC fe = {CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};

	//////////////// We pre-load .NET assemblies to reduce load time when switching to file metadata tab  ///////

	auto threadProc = [](LPVOID lpParameter) -> DWORD {
		CoInitialize(NULL);
		
		// We initialize CLR via MetaFsInterop library ////
		HINSTANCE hInstance;

		SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);

		if ((hInstance = LoadLibraryEx(InteropLibrary, NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS)) != NULL) {
			ptrInitializeClr funcPtr = (ptrInitializeClr)GetProcAddress(hInstance, "InitializeClr");
			funcPtr();
		}

		SetErrorMode(NULL);

		FreeLibrary(hInstance);
		
		//////////////////////////////////////////////////
		
		CComPtr<IDispatch> pUnk;
		pUnk.CoCreateInstance(L"MetaFSPropPage.MetaFsProps", NULL);
		CoUninitialize();

		return 0;
	};

	CloseHandle(CreateThread(NULL, NULL, threadProc, NULL, NULL, &threadId));

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (lpdobj != nullptr) {
		InitCommonControls();
		if (SUCCEEDED((retval = lpdobj->GetData(&fe, &medium)))) {
			hDrop = static_cast<HDROP>(medium.hGlobal);

			if (DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0) == 1) {
				DragQueryFile(hDrop, 0, szFileName, sizeof(szFileName));
				m_szFile.assign(szFileName);

				retval = NOERROR;
			} else retval = E_INVALIDARG;

			ReleaseStgMedium(&medium);
		}
	}
	return retval;
}


HRESULT CMetaFsPropPage::ReplacePage(IN UINT uPageID, IN LPFNADDPROPSHEETPAGE lpfnReplaceWith, IN LPARAM lParam) {
	return E_FAIL;
}


HRESULT CMetaFsPropPage::AddPages(IN LPFNADDPROPSHEETPAGE lpfnAddPage, IN LPARAM lParam) {
	HPROPSHEETPAGE hPage;
	PROPSHEETPAGE psp = {0};
	auto retval = E_INVALIDARG;
	psp.dwSize = sizeof(PROPSHEETPAGE);
	psp.dwFlags = PSP_USEREFPARENT | PSP_USETITLE | PSP_DEFAULT;
	psp.hInstance = _AtlBaseModule.GetResourceInstance();
	psp.pszTemplate = MAKEINTRESOURCE(IDD_EMPTYPAGE);
	psp.pszTitle = _T("File Metadata");
	psp.pcRefParent = NULL;
	psp.lParam = reinterpret_cast<LPARAM>(new wstring(m_szFile.data()));

	psp.pfnDlgProc = [](HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)->LRESULT {
		switch (uiMsg) {
		case WM_INITDIALOG:
			CoInitialize(NULL);
			CComPtr<IDispatch> pUnk;
			CAxWindow container;
			auto psp = reinterpret_cast<PROPSHEETPAGE*>(lParam);
			shared_ptr<wstring> selectedFile(reinterpret_cast<wstring*>(psp->lParam));
			container.Attach(hwnd);
			pUnk.CoCreateInstance(L"MetaFSPropPage.MetaFsProps", NULL);
			pUnk.PutPropertyByName(_bstr_t("SelectedFile"), &_variant_t(selectedFile->data()));
			container.AttachControl(pUnk, NULL);
			CoUninitialize();
			pWndProc = (WNDPROC)SetWindowLongPtr(GetParent(hwnd), GWLP_WNDPROC, (LONG_PTR)&CustomWndProc);
			break;
		}
		return FALSE;
	};

	if ((hPage = CreatePropertySheetPage(&psp)) != NULL) {
		if (!lpfnAddPage(hPage, lParam))
			DestroyPropertySheetPage(hPage);
		retval = NOERROR;
	}

	return retval;
}