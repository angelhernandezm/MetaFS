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

Description: MetaFSAgent Service (Interaction between kernel and user mode)
*/

#include "stdafx.h"

// Global variables
SERVICE_STATUS        g_SvcStatus = {0};
SERVICE_STATUS_HANDLE g_svcHandle = NULL;
HANDLE				  g_filterEventHandle = INVALID_HANDLE_VALUE;
HANDLE                g_svcStopNotification = INVALID_HANDLE_VALUE;


/// <summary>
/// _tmains the specified argc.
/// </summary>
/// <param name="argc">The argc.</param>
/// <param name="argv">The argv.</param>
/// <returns>int.</returns>
int _tmain(int argc, _TCHAR* argv[]) {
	auto retval = 0;

	InitializeClr();

	SERVICE_TABLE_ENTRY ServiceTable[] = {{SERVICE_NAME, ServiceMain}};

	if (!StartServiceCtrlDispatcher(ServiceTable)) {
		WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Unable to start MetaFSAgent Service."));
		retval = GetLastError();
	}

	return retval;
}


VOID ManageDriver(DriverRequestType request) {
	SERVICE_STATUS ss;
	SC_HANDLE hService;
	SC_HANDLE hSCManager;

	if ((hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE)) != NULL) {
		if (request == DriverRequestType::Load) {
			if ((hService = CreateService(hSCManager, METAFS_DRIVE_NAME, METAFS_DRIVE_NAME, SERVICE_START | DELETE | SERVICE_STOP,
				SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE,
				METAFS_DRIVER_PATH, NULL, NULL, NULL, NULL, NULL)) == NULL) {
				hService = OpenService(hSCManager, METAFS_DRIVE_NAME, SERVICE_START | DELETE | SERVICE_STOP);
			}

			if (hService != NULL) {
				StartService(hService, NULL, NULL);
				InitializeCommunicationWithDriver();
				CloseHandle(hService);
			}
		} else {
			if ((hService = OpenService(hSCManager, METAFS_DRIVE_NAME, SERVICE_START | DELETE | SERVICE_STOP)) != NULL) {
				ControlService(hService, SERVICE_CONTROL_STOP, &ss);
				CloseServiceHandle(hService);
				DeleteService(hService);
			}
		}
		CloseServiceHandle(hSCManager);
	}

}

/// <summary>
/// Services the controller.
/// </summary>
/// <param name="ctlCode">The control code.</param>
VOID WINAPI ServiceController(DWORD ctlCode) {
	switch (ctlCode) {
	case SERVICE_CONTROL_STOP:
		if (g_SvcStatus.dwCurrentState == SERVICE_RUNNING) {
			ConfigureService(ConfigOption::STOP_SERVICE);
			if (SetServiceStatus(g_svcHandle, &g_SvcStatus)) {
				SetEvent(g_filterEventHandle);
				SetEvent(g_svcStopNotification);
				WriteEvent(EventInformation(EventType::INFORMATION_TYPE, L"MetaFS Agent Service stopped.\nVersion 1.0.0.0.\n\nChanges made to metadata on existing filesystem objects will not be monitored"));
			} else WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Unable to change status of MetaFSAgent Service (Stop Pending)."));
		}
		break;
	}
}

/// <summary>
/// Services the main.
/// </summary>
/// <param name="argc">The argc.</param>
/// <param name="argv">The argv.</param>
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv) {
	HANDLE hThread;

	if ((g_svcHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceController)) != NULL) {
		ConfigureService(ConfigOption::START_SERVICE);
		if (SetServiceStatus(g_svcHandle, &g_SvcStatus)) {
			if ((g_svcStopNotification = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL) {
				ConfigureService(ConfigOption::RUNNING_SERVICE);
				if (SetServiceStatus(g_svcHandle, &g_SvcStatus)) {
					WriteEvent(EventInformation(EventType::INFORMATION_TYPE, L"MetaFS Agent Service started.\nVersion 1.0.0.0"));
					hThread = CreateThread(NULL, NULL, AsyncFilterWorker, NULL, NULL, NULL);
					WaitForSingleObject(hThread, INFINITE);
					CloseHandle(g_svcStopNotification);
					ConfigureService(ConfigOption::STOPPED_SERVICE);
					SetServiceStatus(g_svcHandle, &g_SvcStatus);
					CloseHandle(hThread);
				} else WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Unable to change status of MetaFSAgent Service (Running)."));
			} else {
				WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Unable to CreateEvent(g_ServiceStopEvent) for MetaFSAgent Service."));
				ConfigureService(ConfigOption::STOP_SERVICE);
				SetServiceStatus(g_svcHandle, &g_SvcStatus);
			}
		} else WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Unable to change status of MetaFSAgent Service (Started)."));
	} else WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Unable to start MetaFSAgent Service."));
}

/// <summary>
/// Asynchronouses the filter worker.
/// </summary>
/// <param name="lpParam">The lp parameter.</param>
/// <returns>DWORD.</returns>
DWORD WINAPI AsyncFilterWorker(LPVOID lpParam) {
	while (WaitForSingleObject(g_svcStopNotification, INFINITE) != WAIT_OBJECT_0) {
		// Has there been changes to the filesystem?
		if (WaitForSingleObject(g_filterEventHandle, INFINITE) != WAIT_OBJECT_0) {
			WriteEvent(EventInformation(EventType::INFORMATION_TYPE, L"Notification from driver..."));
			SetEvent(g_filterEventHandle);
		}
	}
	return ERROR_SUCCESS;
}

/// <summary>
/// Configures the service.
/// </summary>
/// <param name="option">The option.</param>
VOID ConfigureService(ConfigOption option) {
	switch (option) {
	case ConfigOption::START_SERVICE:
		ManageDriver(DriverRequestType::Load);
		ZeroMemory(&g_SvcStatus, sizeof(g_SvcStatus));
		g_SvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		g_SvcStatus.dwControlsAccepted = 0;
		g_SvcStatus.dwCurrentState = SERVICE_START_PENDING;
		g_SvcStatus.dwWin32ExitCode = 0;
		g_SvcStatus.dwServiceSpecificExitCode = 0;
		g_SvcStatus.dwCheckPoint = 0;
		break;

	case ConfigOption::STOP_SERVICE:
		ManageDriver(DriverRequestType::Unload);
		g_SvcStatus.dwControlsAccepted = 0;
		g_SvcStatus.dwCurrentState = SERVICE_STOPPED;
		g_SvcStatus.dwWin32ExitCode = GetLastError();
		g_SvcStatus.dwCheckPoint = 1;
		break;

	case ConfigOption::RUNNING_SERVICE:
		g_SvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
		g_SvcStatus.dwCurrentState = SERVICE_RUNNING;
		g_SvcStatus.dwWin32ExitCode = 0;
		g_SvcStatus.dwCheckPoint = 0;
		break;

	case ConfigOption::STOPPED_SERVICE:
		g_SvcStatus.dwControlsAccepted = 0;
		g_SvcStatus.dwCurrentState = SERVICE_STOPPED;
		g_SvcStatus.dwWin32ExitCode = 0;
		g_SvcStatus.dwCheckPoint = 3;
	}
}

/// <summary>
/// Initializes the CLR.
/// </summary>
VOID InitializeClr() {
	HINSTANCE hInstance;

	SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);

	if ((hInstance = LoadLibraryEx(INTEROP_LIBRARY, NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS)) != NULL) {
		ptrInitializeClr funcPtr = (ptrInitializeClr)GetProcAddress(hInstance, "InitializeClr");
		funcPtr();
	}

	SetErrorMode(NULL);

	FreeLibrary(hInstance);
}

/// <summary>
/// Initializes communication with driver
/// </summary>
VOID InitializeCommunicationWithDriver() {
	HANDLE hFile;
	DWORD cbSize;

	if ((hFile = CreateFile(TARGET_DRIVER, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {

		if ((g_filterEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL)) != NULL) {
			if (DeviceIoControl(hFile, IOCTL_REGISTER_EVENT, &g_filterEventHandle, sizeof(g_filterEventHandle), NULL, NULL, &cbSize, NULL))
				WriteEvent(EventInformation(EventType::INFORMATION_TYPE, L"Event successfully created...."));
		}
		CloseHandle(hFile);
	}
}

/// <summary>
/// Writes the event.
/// </summary>
/// <param name="ei">The ei.</param>
VOID WriteEvent(const EventInformation& ei) {
	HANDLE hEventLog;
	LPWSTR messages[1] = {(WCHAR*)ei.Message.c_str()};

	if ((hEventLog = OpenEventLog(NULL, L"Application")) != NULL) {
		ReportEvent(hEventLog, ei.Type, 0x1, 0x1, NULL, 1, NULL, (LPCWSTR*)messages, NULL);
		CloseEventLog(hEventLog);
	}
}