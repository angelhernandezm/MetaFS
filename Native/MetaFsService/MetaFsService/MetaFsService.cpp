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

	OutputDebugString(L"MetaFS Agent: Starting CLR...\n");
	InitializeClr();

	SERVICE_TABLE_ENTRY ServiceTable[] = {{SERVICE_NAME, ServiceMain}};

	if (!StartServiceCtrlDispatcher(ServiceTable)) {
		OutputDebugString(L"MetaFS Agent: Unable to start service...\n");
		WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Unable to start MetaFSAgent Service."));
		retval = GetLastError();
	}

	return retval;
}


VOID ManageDriver(DriverRequestType request) {
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) {
		OutputDebugString(L"MetaFS Agent: Failed to open Service Control Manager.\n");
		return;
	}

	OutputDebugString(L"MetaFS Agent: SCManager opened.\n");

	SC_HANDLE hService = OpenService(hSCManager, METAFS_DRIVE_NAME, SERVICE_START | SERVICE_QUERY_STATUS);
	if (!hService && request == DriverRequestType::Load) {
		// Service not found—create it
		hService = CreateService(
			hSCManager,
			METAFS_DRIVE_NAME,
			METAFS_DRIVE_NAME,
			SERVICE_START,
			SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_IGNORE,
			METAFS_DRIVER_PATH,
			NULL, NULL, NULL, NULL, NULL
		);

		if (!hService) {
			OutputDebugString(L"MetaFS Agent: Failed to create driver service.\n");
			CloseServiceHandle(hSCManager);
			return;
		}

		OutputDebugString(L"MetaFS Agent: Driver service created.\n");
	}

	if (hService && request == DriverRequestType::Load) {
		if (!StartService(hService, 0, NULL)) {
			DWORD err = GetLastError();
			if (err == ERROR_SERVICE_ALREADY_RUNNING) {
				OutputDebugString(L"MetaFS Agent: Driver already running.\n");
			}
			else {
				OutputDebugString(L"MetaFS Agent: Failed to start driver service.\n");
			}
		}
		else {
			OutputDebugString(L"MetaFS Agent: Driver service started.\n");
		}

		InitializeCommunicationWithDriver();
	}

	if (hService) {
		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hSCManager);
	OutputDebugString(L"MetaFS Agent: Driver management complete.\n");
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
				OutputDebugString(L"MetaFS Agent: Stopped...\n");
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
	g_svcHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceController);
	if (!g_svcHandle) {
		OutputDebugString(L"MetaFS Agent: Failed to register service control handler...\n");
		WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Failed to register service control handler."));
		return;
	}

	// Report SERVICE_START_PENDING
	ConfigureService(ConfigOption::START_SERVICE);
	SetServiceStatus(g_svcHandle, &g_SvcStatus);

	// Create stop notification event
	g_svcStopNotification = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!g_svcStopNotification) {
		OutputDebugString(L"MetaFS Agent: Failed to create stop notification event...\n");
		WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Failed to create stop notification event."));
		ConfigureService(ConfigOption::STOP_SERVICE);
		SetServiceStatus(g_svcHandle, &g_SvcStatus);
		return;
	}

	// Report SERVICE_RUNNING
	ConfigureService(ConfigOption::RUNNING_SERVICE);
	SetServiceStatus(g_svcHandle, &g_SvcStatus);
	OutputDebugString(L"MetaFS Agent: Service is running...\n");
	WriteEvent(EventInformation(EventType::INFORMATION_TYPE, L"MetaFS Agent Service started.\nVersion 1.0.0.0"));

	// Launch worker thread
	HANDLE hThread = CreateThread(NULL, 0, AsyncFilterWorker, NULL, 0, NULL);
	if (!hThread) {
		OutputDebugString(L"MetaFS Agent: Failed to create worker thread...\n");
		WriteEvent(EventInformation(EventType::ERROR_TYPE, L"Failed to create worker thread."));
		ConfigureService(ConfigOption::STOP_SERVICE);
		SetServiceStatus(g_svcHandle, &g_SvcStatus);
		return;
	}
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
			OutputDebugString(L"MetaFS Agent: Notification from driver...\n");
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
		OutputDebugString(L"MetaFS Agent: CLR was loaded...\n");
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

	OutputDebugString(L"MetaFS Agent: Initializing communication with driver...\n");
	if ((hFile = CreateFile(TARGET_DRIVER, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
		OutputDebugString(L"MetaFS Agent: Symlink created...\n");
		if ((g_filterEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL)) != NULL) {
			if (DeviceIoControl(hFile, IOCTL_REGISTER_EVENT, &g_filterEventHandle, sizeof(g_filterEventHandle), NULL, NULL, &cbSize, NULL))
				WriteEvent(EventInformation(EventType::INFORMATION_TYPE, L"Event successfully created...."));
			OutputDebugString(L"MetaFS Agent: Event successfully created...\n");
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