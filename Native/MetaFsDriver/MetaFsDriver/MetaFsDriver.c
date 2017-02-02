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

Description: This is the main module of the MetaFsDriver miniFilter driver.
*/

#include <fltKernel.h>
#include <wdm.h>
#include <dontuse.h>
#include <suppress.h>

#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")

#pragma region "Const and Macros"

#define MetaFs_NT_DEVICE_NAME     L"\\Device\\MetaFsDriver"

#define MetaFs_WIN32_DEVICE_NAME  L"\\DosDevices\\MetaFsDriver"

#define ATTACH_ATTEMPT_RETRIES    5 

#define SHARED_METAFS_EVENT L"MetaFS-Kernel-User-Event"

#define IOCTL_REGISTER_EVENT			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatchPtr, field) \
    (((fastIoDispatchPtr) != NULL) && \
    (((fastIoDispatchPtr)->SizeOfFastIoDispatch) >= \
    (FIELD_OFFSET(FAST_IO_DISPATCH, field) + sizeof(void *))) && \
    ((fastIoDispatchPtr)->field != NULL))

#define ONE_MICROSECOND   (-10)
#define ONE_MILLISECOND   (ONE_MICROSECOND * 1000)
#define ONE_SECOND        (ONE_MILLISECOND * 1000)
#define THREE_SECONDS     (ONE_SECOND * 3) 


#define DEVOBJ_LIST_SIZE        32

#pragma endregion 

#pragma region "Function Declarations"

DRIVER_UNLOAD MetaFsDriverUnload;

DRIVER_INITIALIZE DriverEntry;

NTSTATUS MetaFsDispatchDeviceControl(IN PDEVICE_OBJECT pDO, IN PIRP Irp);

BOOLEAN MetaFsFastIoCheckIfPossible(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in BOOLEAN            wait,
	__in ULONG              lockKey,
	__in BOOLEAN            chkOperation,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoRead(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in BOOLEAN            wait,
	__in ULONG              lockKey,
	__out PVOID             buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoWrite(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in BOOLEAN            wait,
	__in ULONG              lockKey,
	__in PVOID              buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoQueryBasicInfo(
	__in PFILE_OBJECT       fileObj,
	__in BOOLEAN            wait,
	__out PFILE_BASIC_INFORMATION buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoQueryStandardInfo(
	__in PFILE_OBJECT       fileObj,
	__in BOOLEAN            wait,
	__out PFILE_STANDARD_INFORMATION buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoLock(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in PLARGE_INTEGER     length,
	__in PEPROCESS          pId,
	__in ULONG              key,
	__in BOOLEAN            failOnStart,
	__in BOOLEAN            exclusiveLock,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoUnlockSingle(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in PLARGE_INTEGER     length,
	__in PEPROCESS          pId,
	__in ULONG              key,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoUnlockAll(
	__in PFILE_OBJECT       fileObj,
	__in PEPROCESS          pId,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoUnlockAllBykey(
	__in PFILE_OBJECT       fileObj,
	__in PVOID              pId,
	__in ULONG              key,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoDeviceControl(
	__in PFILE_OBJECT       fileObj,
	__in BOOLEAN            wait,
	__in_opt PVOID          inBuffer,
	__in ULONG              inBufferLength,
	__out_opt PVOID         outBuffer,
	__in ULONG              outBufferLength,
	__in ULONG              ioControlCode,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

VOID MetaFsFastIoDetachDevice(
	__in PDEVICE_OBJECT     sourceDevice,
	__in PDEVICE_OBJECT     targetDevice
	);

BOOLEAN MetaFsFastIoQueryNetworkOpenInfo(
	__in PFILE_OBJECT       fileObj,
	__in BOOLEAN            wait,
	__out PFILE_NETWORK_OPEN_INFORMATION buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoMdlRead(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in ULONG              lockKey,
	__out PMDL*             mdlChain,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoMdlReadComplete(
	__in PFILE_OBJECT       fileObj,
	__in PMDL               mdlChain,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoPrepareMdlWrite(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in ULONG              lockKey,
	__out PMDL*             mdlChain,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoMdlWriteComplete(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in PMDL               mdlChain,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoReadCompressed(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in ULONG              lockKey,
	__out PVOID             buffer,
	__out PMDL*             mdlChain,
	__out PIO_STATUS_BLOCK  ioStatus,
	__out struct _COMPRESSED_DATA_INFO* zippedDataInfo,
	__in ULONG              zippedDataLength,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoWriteCompressed(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in ULONG              lockKey,
	__in PVOID              buffer,
	__out PMDL*             MdlChain,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in struct _COMPRESSED_DATA_INFO*  zippedDataInfo,
	__in ULONG              zippedDataLength,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoMdlReadCompleteCompressed(
	__in PFILE_OBJECT       fileObj,
	__in PMDL               mdlChain,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoMdlWriteCompleteCompressed(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in PMDL               mdlChain,
	__in PDEVICE_OBJECT     deviceObj
	);

BOOLEAN MetaFsFastIoQueryOpen(
	__in PIRP               irp,
	__out PFILE_NETWORK_OPEN_INFORMATION networkInfo,
	__in PDEVICE_OBJECT     deviceObj
	);


NTSTATUS MetaFsDispatchPassThrough(
	__in PDEVICE_OBJECT deviceObj,
	__in PIRP           irp
	);

NTSTATUS MetaFsDispatchCreate(
	__in PDEVICE_OBJECT deviceObj,
	__in PIRP           irp
	);

NTSTATUS MetaFsAttachToDevice(
	__in PDEVICE_OBJECT         DeviceObject,
	__out_opt PDEVICE_OBJECT*   pFilterDeviceObject
	);

VOID MetaFsDetachFromDevice(__in PDEVICE_OBJECT deviceObj);

BOOLEAN MetaFsIsAttachedToDevice(__in PDEVICE_OBJECT deviceObj);

BOOLEAN MetaFsIsMyDeviceObject(__in PDEVICE_OBJECT deviceObj);

VOID MetaFsNotificationCallback(__in PDEVICE_OBJECT deviceObj, __in BOOLEAN isActive);

NTSTATUS MetaFsAttachToFileSystemDevice(__in PDEVICE_OBJECT deviceObj);

VOID MetaFsDetachFromFileSystemDevice(__in PDEVICE_OBJECT deviceObj);

NTSTATUS MetaFsEnumerateFileSystemVolumes(__in PDEVICE_OBJECT deviceObj);

BOOLEAN MetaFsFastIoUnlockAllByKey(
	__in PFILE_OBJECT       fileObj,
	__in PVOID              pId,
	__in ULONG              key,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	);

#pragma endregion

#pragma region "Structs"

typedef struct {
	PDEVICE_OBJECT AttachedToDeviceObject;
} FSFILTER_DEVICE_EXTENSION, *PFSFILTER_DEVICE_EXTENSION;

#pragma endregion

#pragma region "Global variables"

PKEVENT g_SharedEvent = NULL;

HANDLE g_SharedEventHandle = NULL;

PDRIVER_OBJECT g_FilterDriverObject;

FAST_IO_DISPATCH g_FastIoDispatch =
{
	sizeof(FAST_IO_DISPATCH),
	MetaFsFastIoCheckIfPossible,
	MetaFsFastIoRead,
	MetaFsFastIoWrite,
	MetaFsFastIoQueryBasicInfo,
	MetaFsFastIoQueryStandardInfo,
	MetaFsFastIoLock,
	MetaFsFastIoUnlockSingle,
	MetaFsFastIoUnlockAll,
	MetaFsFastIoUnlockAllByKey,
	MetaFsFastIoDeviceControl,
	NULL,
	NULL,
	MetaFsFastIoDetachDevice,
	MetaFsFastIoQueryNetworkOpenInfo,
	NULL,
	MetaFsFastIoMdlRead,
	MetaFsFastIoMdlReadComplete,
	MetaFsFastIoPrepareMdlWrite,
	MetaFsFastIoMdlWriteComplete,
	MetaFsFastIoReadCompressed,
	MetaFsFastIoWriteCompressed,
	MetaFsFastIoMdlReadCompleteCompressed,
	MetaFsFastIoMdlWriteCompleteCompressed,
	MetaFsFastIoQueryOpen,
	NULL,
	NULL,
	NULL,
};

#pragma endregion

#pragma region "Assign text sections for each routine"

#ifdef ALLOC_PRAGMA

#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, MetaFsDriverUnload)
#pragma alloc_text(PAGE, MetaFsFastIoCheckIfPossible)
#pragma alloc_text(PAGE, MetaFsFastIoRead)
#pragma alloc_text(PAGE, MetaFsFastIoWrite)
#pragma alloc_text(PAGE, MetaFsFastIoQueryBasicInfo)
#pragma alloc_text(PAGE, MetaFsFastIoQueryStandardInfo)
#pragma alloc_text(PAGE, MetaFsFastIoLock)
#pragma alloc_text(PAGE, MetaFsFastIoUnlockSingle)
#pragma alloc_text(PAGE, MetaFsFastIoUnlockAll)
#pragma alloc_text(PAGE, MetaFsFastIoUnlockAllByKey)
#pragma alloc_text(PAGE, MetaFsFastIoDeviceControl)
#pragma alloc_text(PAGE, MetaFsFastIoDetachDevice)
#pragma alloc_text(PAGE, MetaFsFastIoQueryNetworkOpenInfo)
#pragma alloc_text(PAGE, MetaFsFastIoMdlRead)
#pragma alloc_text(PAGE, MetaFsFastIoMdlReadComplete)
#pragma alloc_text(PAGE, MetaFsFastIoPrepareMdlWrite)
#pragma alloc_text(PAGE, MetaFsFastIoMdlWriteComplete)
#pragma alloc_text(PAGE, MetaFsFastIoReadCompressed)
#pragma alloc_text(PAGE, MetaFsFastIoWriteCompressed)
#pragma alloc_text(PAGE, MetaFsFastIoMdlReadCompleteCompressed)
#pragma alloc_text(PAGE, MetaFsFastIoMdlWriteCompleteCompressed)
#pragma alloc_text(PAGE, MetaFsFastIoQueryOpen)
#pragma alloc_text(PAGE, MetaFsDispatchDeviceControl)

#endif

#pragma endregion

#pragma region "Initialization and unload routines"

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	NTSTATUS retval = STATUS_SUCCESS;
	UNICODE_STRING DeviceName;
	UNICODE_STRING LinkName;
	PDEVICE_OBJECT EventDrvDeviceObject;
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint(("MetaFsDriver::DriverEntry - Entered.\n"));

	g_FilterDriverObject = DriverObject;
	// Create Dispatch Entry Points.  
	for (int x = 0; x <= IRP_MJ_MAXIMUM_FUNCTION; ++x)
		DriverObject->MajorFunction[x] = MetaFsDispatchPassThrough;

	DriverObject->DriverUnload = MetaFsDriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = MetaFsDispatchCreate;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = MetaFsDispatchDeviceControl;

	// Create device object
	RtlInitUnicodeString(&DeviceName, MetaFs_NT_DEVICE_NAME);

	retval = IoCreateDevice(
		DriverObject,
		0,
		&DeviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&EventDrvDeviceObject);

	if (!NT_SUCCESS(retval)) {
		return retval;
	}

	KdPrint(("MetaFsDriver::DriverEntry - Device Created.\n"));

	// Create symbolic links
	RtlInitUnicodeString(&LinkName, MetaFs_WIN32_DEVICE_NAME);
	retval = IoCreateSymbolicLink(&LinkName, &DeviceName);

	if (!NT_SUCCESS(retval)) {
		IoDeleteDevice(EventDrvDeviceObject);
		return retval;
	}

	KdPrint(("MetaFsDriver::DriverEntry - Symbolic link created.\n"));

	// Set FastIO dispatch table
	DriverObject->FastIoDispatch = &g_FastIoDispatch;

	retval = IoRegisterFsRegistrationChange(DriverObject, MetaFsNotificationCallback);

	KdPrint(("MetaFsDriver::DriverEntry - Exited.\n"));

	return retval;
}

VOID MetaFsDriverUnload(IN PDRIVER_OBJECT DriverObject) {
	PDEVICE_OBJECT DevObj;
	UNICODE_STRING LinkName;
	ULONG           numDevices = 0;
	ULONG           x = 0;
	PDEVICE_OBJECT  devList[DEVOBJ_LIST_SIZE];

	KdPrint(("MetaFsDriver::MetaFsDriverUnload - Before unloading driver.\n"));

	PAGED_CODE();

	__try {
		//  Unregister callback routine for file system changes.
		IoUnregisterFsRegistrationChange(DriverObject, MetaFsNotificationCallback);

		// Detach devices
		IoEnumerateDeviceObjectList(DriverObject, devList, sizeof(devList), &numDevices);

		KdPrint(("MetaFsDriver::MetaFsDriverUnload - Unloading.\n"));

		for (x = 0; x < numDevices; ++x) {
			MetaFsDetachFromDevice(devList[x]);
			ObDereferenceObject(devList[x]);
		}

		// Get pointer to Device object
		DevObj = DriverObject->DeviceObject;

		// Form the Win32 symbolic link name
		RtlInitUnicodeString(&LinkName, MetaFs_WIN32_DEVICE_NAME);

		// Remove symbolic link from Object
		IoDeleteSymbolicLink(&LinkName);

		// Unload the callbacks from the kernel to this driver
		IoDeleteDevice(DevObj);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("MetaFsDriverUnload - Exception is 0x%x\n", GetExceptionCode()));
	}

	KdPrint(("MetaFsDriver::MetaFsDriverUnload - Unloaded.\n"));
}

#pragma endregion

#pragma  region "Utility functions"


#pragma endregion

#pragma region "Callback functions"

BOOLEAN MetaFsFastIoCheckIfPossible(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in BOOLEAN            wait,
	__in ULONG              lockKey,
	__in BOOLEAN            chkOperation,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT  nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoCheckIfPossible))
				retval = (fastIoDispatch->FastIoCheckIfPossible)(fileObj, fileOffset, length, wait, lockKey, chkOperation, ioStatus, nextDeviceObject);

		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoCheckIfPossible - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}


BOOLEAN MetaFsFastIoRead(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in BOOLEAN            wait,
	__in ULONG              lockKey,
	__out PVOID             buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT  nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoRead))
				retval = (fastIoDispatch->FastIoRead)(fileObj, fileOffset, length, wait, lockKey, buffer, ioStatus, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsIsAttachedToDevice - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoWrite(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in BOOLEAN            wait,
	__in ULONG              lockKey,
	__in PVOID              buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoWrite))
				retval = (fastIoDispatch->FastIoWrite)(fileObj, fileOffset, length, wait, lockKey, buffer, ioStatus, nextDeviceObject);

		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoWrite - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoQueryBasicInfo(
	__in PFILE_OBJECT       fileObj,
	__in BOOLEAN            wait,
	__out PFILE_BASIC_INFORMATION buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoQueryBasicInfo))
				retval = (fastIoDispatch->FastIoQueryBasicInfo)(fileObj, wait, buffer, ioStatus, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoQueryBasicInfo - Exception is 0x%x\n", GetExceptionCode()));
		}

	}

	return retval;
}

BOOLEAN MetaFsFastIoQueryStandardInfo(
	__in PFILE_OBJECT       fileObj,
	__in BOOLEAN            wait,
	__out PFILE_STANDARD_INFORMATION buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoQueryStandardInfo))
				retval = (fastIoDispatch->FastIoQueryStandardInfo)(fileObj, wait, buffer, ioStatus, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoQueryStandardInfo - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}


NTSTATUS MetaFsDispatchDeviceControl(IN PDEVICE_OBJECT pDO, IN PIRP Irp) {
	UNICODE_STRING eventName;
	NTSTATUS Status = STATUS_SUCCESS;

	PAGED_CODE();

	UNREFERENCED_PARAMETER(pDO);

	__try {
		PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(Irp);
		ULONG ControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;
		Irp->IoStatus.Information = irpStack->Parameters.DeviceIoControl.OutputBufferLength;

		switch (ControlCode) {
		case IOCTL_REGISTER_EVENT:
			//KdPrint(("MetaFsDispatchDeviceControl: Received IOCTL_OPEN_EVENT\n"));
			RtlInitUnicodeString(&eventName, SHARED_METAFS_EVENT);

			KdPrint(("MetaFsDispatchDeviceControl - event name", eventName));

			if ((g_SharedEvent = IoCreateNotificationEvent(&eventName, &g_SharedEventHandle)) != NULL) {
				KdPrint(("MetaFsDispatchDeviceControl - Notification creada"));
				ObReferenceObject(g_SharedEvent);
				KdPrint(("MetaFsDispatchDeviceControl - ObReferenceObject"));
				Status = STATUS_SUCCESS;
				IoStartTimer(pDO);
				KdPrint(("MetaFsDispatchDeviceControl - Todo Ok"));
			}

			Irp->IoStatus.Status = Status;
			IoCompleteRequest(Irp, IO_NO_INCREMENT);

			break;
		}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		Status = STATUS_FAIL_FAST_EXCEPTION;
		KdPrint(("MetaFsDispatchDeviceControl - Exception is 0x%x\n", GetExceptionCode()));
	}

	return Status;
}


BOOLEAN MetaFsFastIoLock(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in PLARGE_INTEGER     length,
	__in PEPROCESS          pId,
	__in ULONG              key,
	__in BOOLEAN            failOnStart,
	__in BOOLEAN            exclusiveLock,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL && deviceObj->CurrentIrp != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoLock))
				retval = (fastIoDispatch->FastIoLock)(fileObj, fileOffset, length, pId, key, failOnStart, exclusiveLock, ioStatus, deviceObj);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoLock - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoUnlockSingle(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in PLARGE_INTEGER     length,
	__in PEPROCESS          pId,
	__in ULONG              key,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT  nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoUnlockSingle))
				retval = (fastIoDispatch->FastIoUnlockSingle)(fileObj, fileOffset, length, pId, key, ioStatus, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoUnlockSingle - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoUnlockAll(
	__in PFILE_OBJECT       fileObj,
	__in PEPROCESS          pId,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoUnlockAll))
				retval = (fastIoDispatch->FastIoUnlockAll)(fileObj, pId, ioStatus, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoUnlockAll - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoUnlockAllBykey(
	__in PFILE_OBJECT       fileObj,
	__in PVOID              pId,
	__in ULONG              key,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoUnlockAllByKey))
				retval = (fastIoDispatch->FastIoUnlockAllByKey)(fileObj, pId, key, ioStatus, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoUnlockAllBykey - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}



BOOLEAN MetaFsFastIoDeviceControl(
	__in PFILE_OBJECT       fileObj,
	__in BOOLEAN            wait,
	__in_opt PVOID          inBuffer,
	__in ULONG              inBufferLength,
	__out_opt PVOID         outBuffer,
	__in ULONG              outBufferLength,
	__in ULONG              ioControlCode,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoDeviceControl))
				retval = (fastIoDispatch->FastIoDeviceControl)(fileObj, wait, inBuffer, inBufferLength, outBuffer, outBufferLength,
				ioControlCode, ioStatus, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoDeviceControl - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

VOID MetaFsFastIoDetachDevice(
	__in PDEVICE_OBJECT     sourceDevice,
	__in PDEVICE_OBJECT     targetDevice
	) {

	__try {
		IoDetachDevice(targetDevice);
		IoDeleteDevice(sourceDevice);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("MetaFsFastIoDetachDevice - Exception is 0x%x\n", GetExceptionCode()));
	}
}


BOOLEAN MetaFsFastIoQueryNetworkOpenInfo(
	__in PFILE_OBJECT       fileObj,
	__in BOOLEAN            wait,
	__out PFILE_NETWORK_OPEN_INFORMATION buffer,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoQueryNetworkOpenInfo))
				retval = (fastIoDispatch->FastIoQueryNetworkOpenInfo)(fileObj, wait, buffer, ioStatus, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoQueryNetworkOpenInfo - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoMdlRead(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in ULONG              lockKey,
	__out PMDL*             mdlChain,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, MdlRead))
				retval = (fastIoDispatch->MdlRead)(fileObj, fileOffset, length, lockKey, mdlChain, ioStatus, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoMdlRead - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoMdlReadComplete(
	__in PFILE_OBJECT       fileObj,
	__in PMDL               mdlChain,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, MdlReadComplete))
				retval = (fastIoDispatch->MdlReadComplete)(fileObj, mdlChain, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoMdlReadComplete - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoPrepareMdlWrite(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in ULONG              lockKey,
	__out PMDL*             mdlChain,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, PrepareMdlWrite))
				retval = (fastIoDispatch->PrepareMdlWrite)(fileObj, fileOffset, length, lockKey, mdlChain, ioStatus, nextDeviceObject);

		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoPrepareMdlWrite - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoMdlWriteComplete(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in PMDL               mdlChain,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, MdlWriteComplete))
				retval = (fastIoDispatch->MdlWriteComplete)(fileObj, fileOffset, mdlChain, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoMdlWriteComplete - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}


BOOLEAN MetaFsFastIoReadCompressed(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in ULONG              lockKey,
	__out PVOID             buffer,
	__out PMDL*             mdlChain,
	__out PIO_STATUS_BLOCK  ioStatus,
	__out struct _COMPRESSED_DATA_INFO* zippedDataInfo,
	__in ULONG              zippedDataLength,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoReadCompressed))
				retval = (fastIoDispatch->FastIoReadCompressed)(fileObj, fileOffset, length, lockKey, buffer, mdlChain, ioStatus,
				zippedDataInfo, zippedDataLength, nextDeviceObject);

		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoReadCompressed - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoWriteCompressed(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in ULONG              length,
	__in ULONG              lockKey,
	__in PVOID              buffer,
	__out PMDL*             MdlChain,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in struct _COMPRESSED_DATA_INFO*  zippedDataInfo,
	__in ULONG              zippedDataLength,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoWriteCompressed))
				retval = (fastIoDispatch->FastIoWriteCompressed)(fileObj, fileOffset, length, lockKey, buffer, MdlChain, ioStatus,
				zippedDataInfo, zippedDataLength, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoWriteCompressed - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoMdlReadCompleteCompressed(
	__in PFILE_OBJECT       fileObj,
	__in PMDL               mdlChain,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, MdlReadCompleteCompressed))
				retval = (fastIoDispatch->MdlReadCompleteCompressed)(fileObj, mdlChain, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoMdlReadCompleteCompressed - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoMdlWriteCompleteCompressed(
	__in PFILE_OBJECT       fileObj,
	__in PLARGE_INTEGER     fileOffset,
	__in PMDL               mdlChain,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, MdlWriteCompleteCompressed))
				retval = (fastIoDispatch->MdlWriteCompleteCompressed)(fileObj, fileOffset, mdlChain, nextDeviceObject);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoMdlWriteCompleteCompressed - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

BOOLEAN MetaFsFastIoQueryOpen(
	__in PIRP               irp,
	__out PFILE_NETWORK_OPEN_INFORMATION networkInfo,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;
	PIO_STACK_LOCATION irpSp;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
			PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

			if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoQueryOpen)) {
				irpSp = IoGetCurrentIrpStackLocation(irp);

				irpSp->DeviceObject = nextDeviceObject;

				retval = (fastIoDispatch->FastIoQueryOpen)(irp, networkInfo, nextDeviceObject);

				irpSp->DeviceObject = deviceObj;
			}
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsFastIoQueryOpen - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

NTSTATUS MetaFsDispatchPassThrough(
	__in PDEVICE_OBJECT deviceObj,
	__in PIRP           irp
	) {

	NTSTATUS retval = STATUS_SUCCESS;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PFSFILTER_DEVICE_EXTENSION pDevExt = (PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension;
			IoSkipCurrentIrpStackLocation(irp);
			retval = IoCallDriver(pDevExt->AttachedToDeviceObject, irp);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsDispatchPassThrough - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

NTSTATUS MetaFsDispatchCreate(
	__in PDEVICE_OBJECT deviceObj,
	__in PIRP           irp
	) {

	NTSTATUS retval = STATUS_SUCCESS;

	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(irp);
			PFILE_OBJECT pFileObject = irpStack->FileObject;
			pFileObject->Size; // To avoid warning - error
			//  &pFileObject->FileName
			retval = MetaFsDispatchPassThrough(deviceObj, irp);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsDispatchCreate - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return retval;
}

NTSTATUS MetaFsAttachToDevice(__in PDEVICE_OBJECT  deviceObj, __out_opt PDEVICE_OBJECT*  pFilterDeviceObject) {
	NTSTATUS  retval = STATUS_SUCCESS;
	PDEVICE_OBJECT filterDevObj = NULL;
	PFSFILTER_DEVICE_EXTENSION  devExt = NULL;

	__try {
		if (NT_SUCCESS(retval = IoCreateDevice(g_FilterDriverObject, sizeof(FSFILTER_DEVICE_EXTENSION), NULL,
			deviceObj->DeviceType, 0, FALSE, &filterDevObj))) {

			devExt = (PFSFILTER_DEVICE_EXTENSION)filterDevObj->DeviceExtension;

			if (FlagOn(deviceObj->Flags, DO_BUFFERED_IO))
				SetFlag(filterDevObj->Flags, DO_BUFFERED_IO);

			if (FlagOn(deviceObj->Flags, DO_DIRECT_IO))
				SetFlag(filterDevObj->Flags, DO_DIRECT_IO);

			if (FlagOn(deviceObj->Characteristics, FILE_DEVICE_SECURE_OPEN))
				SetFlag(filterDevObj->Characteristics, FILE_DEVICE_SECURE_OPEN);

			for (ULONG x = 0; x < ATTACH_ATTEMPT_RETRIES; ++x) {
				LARGE_INTEGER interval;
				retval = IoAttachDeviceToDeviceStackSafe(filterDevObj, deviceObj, &devExt->AttachedToDeviceObject);

				if (NT_SUCCESS(retval))
					break;

				interval.QuadPart = (250 * ONE_MILLISECOND);
				KeDelayExecutionThread(KernelMode, FALSE, &interval);
			}

			if (!NT_SUCCESS(retval)) {
				IoDeleteDevice(filterDevObj);
				filterDevObj = NULL;
			} else {
				ClearFlag(filterDevObj->Flags, DO_DEVICE_INITIALIZING);

				if (pFilterDeviceObject != NULL)
					*pFilterDeviceObject = filterDevObj;
			}
		}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("MetaFsAttachToDevice - Exception is 0x%x\n", GetExceptionCode()));
	}

	return retval;
}


VOID MetaFsDetachFromDevice(__in PDEVICE_OBJECT deviceObj) {
	if (deviceObj != NULL && deviceObj->DeviceExtension != NULL) {
		__try {
			PFSFILTER_DEVICE_EXTENSION devExt = (PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension;
			IoDetachDevice(devExt->AttachedToDeviceObject);
			IoDeleteDevice(deviceObj);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsDetachFromDevice - Exception is 0x%x\n", GetExceptionCode()));
		}
	}
}

BOOLEAN MetaFsIsAttachedToDevice(__in PDEVICE_OBJECT deviceObj) {
	if (deviceObj != NULL) {
		__try {
			PDEVICE_OBJECT nextDevObj = NULL;
			PDEVICE_OBJECT currentDevObj = IoGetAttachedDeviceReference(deviceObj);

			do {
				if (MetaFsIsMyDeviceObject(currentDevObj)) {
					ObDereferenceObject(currentDevObj);
					return TRUE;
				}
				nextDevObj = IoGetLowerDeviceObject(currentDevObj);
				ObDereferenceObject(currentDevObj);
				currentDevObj = nextDevObj;
			}
			while (currentDevObj != NULL);

		} __except (EXCEPTION_EXECUTE_HANDLER) {
			KdPrint(("MetaFsIsAttachedToDevice - Exception is 0x%x\n", GetExceptionCode()));
		}
	}

	return FALSE;
}


BOOLEAN MetaFsIsMyDeviceObject(__in PDEVICE_OBJECT deviceObj) {
	return (deviceObj != NULL && deviceObj->DriverObject == g_FilterDriverObject);
}

VOID MetaFsNotificationCallback(__in PDEVICE_OBJECT deviceObj, __in BOOLEAN isActive) {
	__try {
		if (isActive)
			MetaFsAttachToFileSystemDevice(deviceObj);
		else
			MetaFsDetachFromFileSystemDevice(deviceObj);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("MetaFsNotificationCallback - Exception is 0x%x\n", GetExceptionCode()));
	}
}

NTSTATUS MetaFsAttachToFileSystemDevice(__in PDEVICE_OBJECT deviceObj) {
	NTSTATUS  retval = STATUS_SUCCESS;
	PDEVICE_OBJECT  pDeviceObj = NULL;

	__try {
		if (!MetaFsIsAttachedToDevice(deviceObj)) {
			if (NT_SUCCESS(retval = MetaFsAttachToDevice(deviceObj, &pDeviceObj))) {
				if (!NT_SUCCESS(retval = MetaFsEnumerateFileSystemVolumes(deviceObj)))
					MetaFsDetachFromDevice(pDeviceObj);
			}
		}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("MetaFsAttachToFileSystemDevice - Exception is 0x%x\n", GetExceptionCode()));
	}

	return retval;
}

VOID MetaFsDetachFromFileSystemDevice(__in PDEVICE_OBJECT deviceObj) {
	PDEVICE_OBJECT device = NULL;

	__try {
		for (device = deviceObj->AttachedDevice; NULL != device; device = device->AttachedDevice) {
			if (MetaFsIsMyDeviceObject(device)) {
				MetaFsDetachFromDevice(device);
				break;
			}
		}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("MetaFsDetachFromFileSystemDevice - Exception is 0x%x\n", GetExceptionCode()));
	}
}

NTSTATUS MetaFsEnumerateFileSystemVolumes(__in PDEVICE_OBJECT deviceObj) {
	ULONG numDevices = 0, x = 0;
	NTSTATUS  retval = STATUS_SUCCESS;
	PDEVICE_OBJECT  devList[DEVOBJ_LIST_SIZE];

	__try {
		retval = IoEnumerateDeviceObjectList(deviceObj->DriverObject, devList, sizeof(devList), &numDevices);

		if (!NT_SUCCESS(retval))
			return retval;

		numDevices = min(numDevices, RTL_NUMBER_OF(devList));

		for (x = 0; x < numDevices; ++x) {
			if (devList[x] != deviceObj &&	devList[x]->DeviceType == deviceObj->DeviceType &&!MetaFsIsAttachedToDevice(devList[x]))
				retval = MetaFsAttachToDevice(devList[x], NULL);

			ObDereferenceObject(devList[x]);
		}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("MetaFsEnumerateFileSystemVolumes - Exception is 0x%x\n", GetExceptionCode()));
	}

	return retval;
}


BOOLEAN MetaFsFastIoUnlockAllByKey(
	__in PFILE_OBJECT       fileObj,
	__in PVOID              pId,
	__in ULONG              key,
	__out PIO_STATUS_BLOCK  ioStatus,
	__in PDEVICE_OBJECT     deviceObj
	) {

	BOOLEAN retval = FALSE;

	__try {
		PDEVICE_OBJECT    nextDeviceObject = ((PFSFILTER_DEVICE_EXTENSION)deviceObj->DeviceExtension)->AttachedToDeviceObject;
		PFAST_IO_DISPATCH fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

		if (VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoUnlockAllByKey))
			retval = (fastIoDispatch->FastIoUnlockAllByKey)(fileObj, pId, key, ioStatus, nextDeviceObject);

	} __except (EXCEPTION_EXECUTE_HANDLER) {
		KdPrint(("MetaFsFastIoUnlockAllByKey - Exception is 0x%x\n", GetExceptionCode()));
	}

	return retval;
}

#pragma endregion