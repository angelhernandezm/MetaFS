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

Description:  Declaration of the CMetaFsPropPage (Property page)
*/

#pragma once
#include "resource.h"       // main symbols
#include <atlcom.h>
#include <Shobjidl.h>
#include <iostream>
#include "MetaFsExt_i.h"
#include <memory>
#include <metahost.h>
#include <mscoree.h>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace std;


/// <summary>
/// Class CMetaFsPropPage.
/// </summary>
class ATL_NO_VTABLE CMetaFsPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMetaFsPropPage, &CLSID_MetaFsPropPage>,
	public IShellExtInit,
	public IShellPropSheetExt {

private:

public:
	wstring m_szFile;
	CMetaFsPropPage();
	~CMetaFsPropPage();


	DECLARE_REGISTRY_RESOURCEID(IDR_METAFSEXT)

	BEGIN_COM_MAP(CMetaFsPropPage)
		COM_INTERFACE_ENTRY(IShellExtInit)
		COM_INTERFACE_ENTRY(IShellPropSheetExt)
	END_COM_MAP()

public:
	// IShellExtInit Methods
	STDMETHOD(Initialize)(IN LPCITEMIDLIST pidlFolder, IN LPDATAOBJECT lpdobj, IN HKEY hkeyProgID);

	//IShellPropSheetExt methods
	STDMETHODIMP AddPages(IN LPFNADDPROPSHEETPAGE lpfnAddPage, IN LPARAM lParam);

	STDMETHODIMP ReplacePage(IN UINT uPageID, IN LPFNADDPROPSHEETPAGE lpfnReplaceWith, IN LPARAM lParam);
};