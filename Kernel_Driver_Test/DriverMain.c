#include <ntifs.h>

/*
* OriginalApcEnvironment(default environment):APC should be executed in the target thread's original environment. 
                                              This means that the APC will execute in the context of the target thread, 
											  regardless of whether the target thread is in the suspended, waiting, or other state.

* AttachApcEnvironment:If another thread is associated with the target thread via the KeAttachProcess or KeAttachThread function,
                       and you want to execute APC during that association, then APC should execute in AttachApcEnvironment. 
					   This is often used for debugging or other situations where you need to interact deeply with another thread or process.

*CurrentApcEnvironment:The value represents the current APC environment. 
                       If the current thread is associated with another thread or process via KeAttachThread or KeAttachProcess,
					   then CurrentApcEnvironment will be AttachApcEnvironment. Otherwise, it will be OriginalApcEnvironment.

*InsertApcEnvironment:This environment is used to indicate that the APC should be executed in the environment in which it is inserted.
                      In general, this is similar to OriginalApcEnvironment, but it is specifically designed for those
					  cases where an APC needs to be inserted ina specific context.
*/
typedef enum _KAPC_ENVIRONMENT {
	OriginalApcEnvironment,	
	AttachApcEnvironment,	
	CurrentApcEnvironment,
	InsertApcEnvironment
} KAPC_ENVIRONMENT;


typedef VOID(*PKNORMAL_ROUTINE) (
	IN PVOID NormalContext,
	IN PVOID SystemArgument1,
	IN PVOID SystemArgument2
	);

typedef VOID(*PKKERNEL_ROUTINE) (
	IN struct _KAPC* Apc,
	IN OUT PKNORMAL_ROUTINE* NormalRountine,
	IN OUT PVOID* NormalContext,
	IN OUT PVOID* SystemArgument1,
	IN OUT PVOID* SystemArgument2
	);

typedef VOID(*PKRUNDOWN_ROUNTINE)(
	IN struct _KAPC* Apc
	);

VOID KeInitializeApc(
	__out PKAPC Apc,
	__in PKTHREAD Thread,
	__in KAPC_ENVIRONMENT Environment,
	__in PKKERNEL_ROUTINE KernelRoutine,
	_In_opt_ PKRUNDOWN_ROUNTINE RundownRoutine,
	_In_opt_ PKNORMAL_ROUTINE NormalRoutine,
	_In_opt_ KPROCESSOR_MODE ProcessorMode,
	_In_opt_ PVOID NormalContext
);

BOOLEAN KeInsertQueueApc(
	__inout PKAPC Apc,
	_In_opt_  PVOID SystemArgument1,
	_In_opt_ PVOID SystemArgument2,
	__in KPRIORITY Increment
);


VOID KernelApcCallBack(
	IN struct _KAPC* Apc,
	IN OUT PKNORMAL_ROUTINE* NormalRountine,
	IN OUT PVOID* NormalContext,
	IN OUT PVOID* SystemArgument1,
	IN OUT PVOID* SystemArgument2
) {

}

VOID DriverUnload(PDRIVER_OBJECT pDriver) {

}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg) {

	PKAPC pApc = ExAllocatePool(NonPagedPool, sizeof(PKAPC));
	memset(pApc, 0, sizeof(PKAPC));
	KeInitializeApc(pApc, KeGetCurrentThread(), OriginalApcEnvironment, KernelApcCallBack, NULL, NULL, KernelMode, NULL);
	KeInsertQueueApc(pApc, NULL, NULL, 0);
	return STATUS_SUCCESS;
}