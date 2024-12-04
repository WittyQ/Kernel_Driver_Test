#include <ntifs.h>


VOID dpcCallback(
	_In_ struct _KDPC* Dpc,
	_In_opt_ PVOID DeferredContext,
	_In_opt_ PVOID SystemArgument1,
	_In_opt_ PVOID SystemArgument2
) {
	ExFreePool(Dpc);
	DbgPrintEx(77, 0, "[db]:%s\n", __FUNCTION__);
}


VOID DriverUnload(PDRIVER_OBJECT pDriver) {

}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg) {

	KDPC* kdpc = ExAllocatePool(NonPagedPool, sizeof(KDPC));
	KeInitializeDpc(kdpc, &dpcCallback, NULL);
	KeInsertQueueDpc(kdpc, NULL, NULL);
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}