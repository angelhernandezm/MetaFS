// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <winioctl.h>

using namespace std;

// Callbacks
typedef void(*ptrInitializeClr)();

// Macros and Constants
#define SERVICE_NAME  _T("MetaFSAgent")
#define INTEROP_LIBRARY L"MetaFsInterop.dll"
#define TARGET_DRIVER L"\\\\.\\MetaFsDriver"
#define METAFS_DRIVE_NAME L"MetaFSDriver"
#define METAFS_DRIVER_PATH L"c:\\windows\\system32\\drivers\\metafsdriver.sys"


#define SHARED_EVENT  L"MetaFS-Kernel-User-Event"
#define IOCTL_REGISTER_EVENT	  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)


// Enums
typedef enum _EventType {
	SUCCESS = 0x0000,
	ERROR_TYPE = 0x0001,
	WARNING_TYPE = 0x0002,
	INFORMATION_TYPE = 0x0004,
	AUDIT_SUCCESS = 0x0008,
	AUDIT_FAILURE = 0x0010
} EventType;

typedef enum _ConfigOption {
	START_SERVICE,
	STOP_SERVICE,
	RUNNING_SERVICE,
	STOPPED_SERVICE
} ConfigOption;



// Structs
/// <summary>
/// Struct _EventInformation
/// </summary>
typedef struct _EventInformation {
public:
	EventType Type;
	std::wstring Message;

	_EventInformation() {

	}

	_EventInformation(EventType type, std::wstring message): _EventInformation() {
		Type = type;
		Message = message;
	}

} EventInformation, *pEventInformation;


typedef enum _DriverActionRequest {Load, Unload } DriverRequestType;

VOID ManageDriver(DriverRequestType request);

// Function definitions
/// <summary>
/// Writes the event.
/// </summary>
/// <param name="ei">The ei.</param>
VOID WriteEvent(const EventInformation& ei);

/// <summary>
/// Services the controller.
/// </summary>
/// <param name="ctlCode">The control code.</param>
VOID WINAPI ServiceController(DWORD ctlCode);

/// <summary>
/// Services the main.
/// </summary>
/// <param name="argc">The argc.</param>
/// <param name="argv">The argv.</param>
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);


/// <summary>
/// Asynchronouses the filter worker.
/// </summary>
/// <param name="lpParam">The lp parameter.</param>
/// <returns>DWORD.</returns>
DWORD WINAPI AsyncFilterWorker(LPVOID lpParam);


/// <summary>
/// Configures the service.
/// </summary>
/// <param name="option">The option.</param>
VOID ConfigureService(ConfigOption option);


/// <summary>
/// Initializes the CLR.
/// </summary>
VOID InitializeClr();


/// <summary>
/// Initializes communication with driver
/// </summary>
VOID InitializeCommunicationWithDriver();

