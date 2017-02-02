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

Description: Interoperability with CLR
*/


#ifdef METAFSINTEROP_EXPORTS
#define METAFSINTEROP_API __declspec(dllexport)
#else
#define METAFSINTEROP_API __declspec(dllimport)
#endif

// This class is exported from the MetaFsInterop.dll
class METAFSINTEROP_API CMetaFsInterop {
public:
	CMetaFsInterop(void);
	void InitializeClr();


private:
	BOOL CheckIfClrIsLoaded(const CComPtr<IEnumUnknown>& pEnumerator);
};


extern "C"{
	METAFSINTEROP_API void InitializeClr();
}
