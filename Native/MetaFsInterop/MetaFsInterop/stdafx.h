// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <iostream>
#include <memory>
#include <metahost.h>
#include <mscoree.h>

typedef struct _ExecuteArgs {
	LPCWSTR pwzAssemblyPath;
	LPCWSTR pwzTypeName;
	LPCWSTR pwzMethodName;
	LPCWSTR pwzArgument;
	DWORD pReturnValue;
} ExecuteArgs, *PExecuteArgs;


