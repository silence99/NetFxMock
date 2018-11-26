#pragma once
#include "resource.h"
#include <CorHdr.h>
#include <cor.h>
#include <corprof.h>
#include <log4cplus/logger.h>
#include <map>

using namespace ATL;
using namespace log4cplus;

#define NAME_BUFFER_SIZE 1024

extern "C" class CCorProfilerCallbackImpl: public ICorProfilerCallback2
{
protected:
	// container for ICorProfilerInfo reference
	CComQIPtr<ICorProfilerInfo> m_pICorProfilerInfo;
	// container for ICorProfilerInfo2 reference
	CComQIPtr<ICorProfilerInfo2> m_pICorProfilerInfo2;

	// gets the full method name given a function ID
	HRESULT GetFullMethodName(FunctionID functionId, LPWSTR wszMethod, int cMethod);
	void LogString(char *pszFmtString, ...);

	Logger _logger;
	const char* DEFAULT_MOCK_MODULE = "MOCK.dll";
	const char* DEFAULT_MOCK_SET_FUNCTION = "SetMock";
	char _mock_module[200] = { 0 };
	char _mock_set_function[100] = { 0 };
	ModuleID _mock_module_id;
	FunctionID _mock_function_id;
	std::map<FunctionID, std::map<OBJECTID, FunctionID>> _mock_registry;

public:
	CCorProfilerCallbackImpl();
	~CCorProfilerCallbackImpl();

	// ICorProfilerCallback interface implementation
	// STARTUP/SHUTDOWN EVENTS
	STDMETHOD(Shutdown)();
	// APPLICATION DOMAIN EVENTS
	STDMETHOD(AppDomainCreationStarted)(AppDomainID appDomainID)
	{
		return S_OK;
	}
	STDMETHOD(AppDomainCreationFinished)(AppDomainID appDomainID, HRESULT hrStatus)
	{
		return S_OK;
	}
	STDMETHOD(AppDomainShutdownStarted)(AppDomainID appDomainID)
	{
		return S_OK;
	}
	STDMETHOD(AppDomainShutdownFinished)(AppDomainID appDomainID, HRESULT hrStatus)
	{
		return S_OK;
	}
	// ASSEMBLY EVENTS
	STDMETHOD(AssemblyLoadStarted)(AssemblyID assemblyID)
	{
		return S_OK;
	}
	STDMETHOD(AssemblyLoadFinished)(AssemblyID assemblyID, HRESULT hrStatus)
	{
		return S_OK;
	}
	STDMETHOD(AssemblyUnloadStarted)(AssemblyID assemblyID)
	{
		return S_OK;
	}
	STDMETHOD(AssemblyUnloadFinished)(AssemblyID assemblyID, HRESULT hrStatus)
	{
		return S_OK;
	}
	// MODULE EVENTS
	STDMETHOD(ModuleLoadStarted)(ModuleID moduleID);
	STDMETHOD(ModuleLoadFinished)(ModuleID moduleID, HRESULT hrStatus);
	STDMETHOD(ModuleUnloadStarted)(ModuleID moduleID);
	STDMETHOD(ModuleUnloadFinished)(ModuleID moduleID, HRESULT hrStatus);
	STDMETHOD(ModuleAttachedToAssembly)(ModuleID moduleID, AssemblyID assemblyID)
	{
		return S_OK;
	}
	// CLASS EVENTS
	STDMETHOD(ClassLoadStarted)(ClassID classID)
	{
		return S_OK;
	}
	STDMETHOD(ClassLoadFinished)(ClassID classID, HRESULT hrStatus)
	{
		return S_OK;
	}
	STDMETHOD(ClassUnloadStarted)(ClassID classID)
	{
		return S_OK;
	}
	STDMETHOD(ClassUnloadFinished)(ClassID classID, HRESULT hrStatus)
	{
		return S_OK;
	}
	STDMETHOD(FunctionUnloadStarted)(FunctionID functionID)
	{
		return S_OK;
	}
	// JIT EVENTS
	STDMETHOD(JITCompilationStarted)(FunctionID functionID, BOOL fIsSafeToBlock);
	STDMETHOD(JITCompilationFinished)(FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock);
	STDMETHOD(JITCachedFunctionSearchStarted)(FunctionID functionID, BOOL *pbUseCachedFunction)
	{
		return S_OK;
	}
	STDMETHOD(JITCachedFunctionSearchFinished)(FunctionID functionID, COR_PRF_JIT_CACHE result)
	{
		return S_OK;
	}
	STDMETHOD(JITFunctionPitched)(FunctionID functionID)
	{
		return S_OK;
	}
	STDMETHOD(JITInlining)(FunctionID callerID, FunctionID calleeID, BOOL *pfShouldInline)
	{
		return S_OK;
	}
	// THREAD EVENTS
	STDMETHOD(ThreadCreated)(ThreadID threadID)
	{
		return S_OK;
	}
	STDMETHOD(ThreadDestroyed)(ThreadID threadID)
	{
		return S_OK;
	}
	STDMETHOD(ThreadAssignedToOSThread)(ThreadID managedThreadID, DWORD osThreadID)
	{
		return S_OK;
	}
	// REMOTING EVENTS
	// Client-side events
	STDMETHOD(RemotingClientInvocationStarted)()
	{
		return S_OK;
	}
	STDMETHOD(RemotingClientSendingMessage)(GUID *pCookie, BOOL fIsAsync)
	{
		return S_OK;
	}
	STDMETHOD(RemotingClientReceivingReply)(GUID *pCookie, BOOL fIsAsync)
	{
		return S_OK;
	}
	STDMETHOD(RemotingClientInvocationFinished)()
	{
		return S_OK;
	}
	// Server-side events
	STDMETHOD(RemotingServerReceivingMessage)(GUID *pCookie, BOOL fIsAsync)
	{
		return S_OK;
	}
	STDMETHOD(RemotingServerInvocationStarted)()
	{
		return S_OK;
	}
	STDMETHOD(RemotingServerInvocationReturned)()
	{
		return S_OK;
	}
	STDMETHOD(RemotingServerSendingReply)(GUID *pCookie, BOOL fIsAsync)
	{
		return S_OK;
	}
	// CONTEXT EVENTS
	STDMETHOD(UnmanagedToManagedTransition)(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
	{
		return S_OK;
	}
	STDMETHOD(ManagedToUnmanagedTransition)(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
	{
		return S_OK;
	}
	// SUSPENSION EVENTS
	STDMETHOD(RuntimeSuspendStarted)(COR_PRF_SUSPEND_REASON suspendReason)
	{
		return S_OK;
	}
	STDMETHOD(RuntimeSuspendFinished)()
	{
		return S_OK;
	}
	STDMETHOD(RuntimeSuspendAborted)()
	{
		return S_OK;
	}
	STDMETHOD(RuntimeResumeStarted)()
	{
		return S_OK;
	}
	STDMETHOD(RuntimeResumeFinished)()
	{
		return S_OK;
	}
	STDMETHOD(RuntimeThreadSuspended)(ThreadID threadid)
	{
		return S_OK;
	}
	STDMETHOD(RuntimeThreadResumed)(ThreadID threadid)
	{
		return S_OK;
	}
	// GC EVENTS
	STDMETHOD(MovedReferences)(ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
	{
		return S_OK;
	}
	STDMETHOD(ObjectAllocated)(ObjectID objectID, ClassID classID)
	{
		return S_OK;
	}
	STDMETHOD(ObjectsAllocatedByClass)(ULONG classCount, ClassID classIDs[], ULONG objects[])
	{
		return S_OK;
	}
	STDMETHOD(ObjectReferences)(ObjectID objectID, ClassID classID, ULONG cObjectRefs, ObjectID objectRefIDs[])
	{
		return S_OK;
	}
	STDMETHOD(RootReferences)(ULONG cRootRefs, ObjectID rootRefIDs[])
	{
		return S_OK;
	}
	// EXCEPTION EVENTS
	// Exception creation
	STDMETHOD(ExceptionThrown)(ObjectID thrownObjectID)
	{
		return S_OK;
	}
	// Search phase
	STDMETHOD(ExceptionSearchFunctionEnter)(FunctionID functionID)
	{
		return S_OK;
	}
	STDMETHOD(ExceptionSearchFunctionLeave)()
	{
		return S_OK;
	}
	STDMETHOD(ExceptionSearchFilterEnter)(FunctionID functionID)
	{
		return S_OK;
	}
	STDMETHOD(ExceptionSearchFilterLeave)()
	{
		return S_OK;
	}
	STDMETHOD(ExceptionSearchCatcherFound)(FunctionID functionID)
	{
		return S_OK;
	}
	STDMETHOD(ExceptionCLRCatcherFound)()
	{
		return S_OK;
	}
	STDMETHOD(ExceptionCLRCatcherExecute)()
	{
		return S_OK;
	}
	STDMETHOD(ExceptionOSHandlerEnter)(FunctionID functionID)
	{
		return S_OK;
	}
	STDMETHOD(ExceptionOSHandlerLeave)(FunctionID functionID)
	{
		return S_OK;
	}
	// Unwind phase
	STDMETHOD(ExceptionUnwindFunctionEnter)(FunctionID functionID)
	{
		return S_OK;
	}
	STDMETHOD(ExceptionUnwindFunctionLeave)()
	{
		return S_OK;
	}
	STDMETHOD(ExceptionUnwindFinallyEnter)(FunctionID functionID)
	{
		return S_OK;
	}
	STDMETHOD(ExceptionUnwindFinallyLeave)()
	{
		return S_OK;
	}
	STDMETHOD(ExceptionCatcherEnter)(FunctionID functionID, ObjectID objectID)
	{
		return S_OK;
	}
	STDMETHOD(ExceptionCatcherLeave)()
	{
		return S_OK;
	}
	// COM CLASSIC VTable
	STDMETHOD(COMClassicVTableCreated)(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable, ULONG cSlots)
	{
		return S_OK;
	}
	STDMETHOD(COMClassicVTableDestroyed)(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable)
	{
		return S_OK;
	}
	// End of ICorProfilerCallback interface implementation

	// ICorProfilerCallback2 interface implementation
	STDMETHOD(ThreadNameChanged)(ThreadID threadId, ULONG cchName, WCHAR name[])
	{
		return S_OK;
	}
	STDMETHOD(GarbageCollectionStarted)(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
	{
		return S_OK;
	}
	STDMETHOD(SurvivingReferences)(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
	{
		return S_OK;
	}
	STDMETHOD(GarbageCollectionFinished)()
	{
		return S_OK;
	}
	STDMETHOD(FinalizeableObjectQueued)(DWORD finalizerFlags, ObjectID objectID)
	{
		return S_OK;
	}
	STDMETHOD(RootReferences2)(ULONG cRootRefs, ObjectID rootRefIds[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIds[])
	{
		return S_OK;
	}
	STDMETHOD(HandleCreated)(GCHandleID handleId, ObjectID initialObjectId)
	{
		return S_OK;
	}
	STDMETHOD(HandleDestroyed)(GCHandleID handleId)
	{
		return S_OK;
	}
	// End of ICorProfilerCallback2 interface implementation
	void Log_Trace(char* pszFmtString, ...);
	void Log_Debug(char* pszFmtString, ...);
	void Log_Info(char* pszFmtString, ...);
	void Log_Warn(char* pszFmtString, ...);
	void Log_Error(char* pszFmtString, ...);
	void Log_Fatal(char* pszFmtString, ...);
};

