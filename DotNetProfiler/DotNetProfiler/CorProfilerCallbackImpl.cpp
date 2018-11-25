#include "stdafx.h"
#include <direct.h>
#include <iostream>
#include "CorProfilerCallbackImpl.h"
#include <fstream>
#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

#include "ProfilingUtils.h"


using namespace std;
using namespace log4cplus;

void CCorProfilerCallbackImpl::LogString(char *pszFmtString, ...)
{
	CHAR szBuffer[4096];
	DWORD dwWritten = 0;

	va_list args;
	va_start(args, pszFmtString);
	vsprintf_s(szBuffer, pszFmtString, args);
	va_end(args);

	// write out to the file if the file is open
	//WriteFile(m_hLogFile, szBuffer, (DWORD)strlen(szBuffer), &dwWritten, NULL);
	ofstream o("callback.log", ios::app);
	o << "log:" << szBuffer << endl;
	o.close();
}

// creates the fully scoped name of the method in the provided buffer
HRESULT CCorProfilerCallbackImpl::GetFullMethodName(FunctionID functionID, LPWSTR wszMethod, int cMethod)
{
	IMetaDataImport* pIMetaDataImport = 0;
	HRESULT hr = S_OK;
	mdToken funcToken = 0;
	WCHAR szFunction[NAME_BUFFER_SIZE];
	WCHAR szClass[NAME_BUFFER_SIZE];

	// get the token for the function which we will use to get its name
	hr = m_pICorProfilerInfo2->GetTokenAndMetaDataFromFunction(functionID, IID_IMetaDataImport, (LPUNKNOWN *)&pIMetaDataImport, &funcToken);
	if (SUCCEEDED(hr))
	{
		mdTypeDef classTypeDef;
		ULONG cchFunction;
		ULONG cchClass;

		// retrieve the function properties based on the token
		hr = pIMetaDataImport->GetMethodProps(funcToken, &classTypeDef, szFunction, NAME_BUFFER_SIZE, &cchFunction, 0, 0, 0, 0, 0);
		if (SUCCEEDED(hr))
		{
			// get the function name
			hr = pIMetaDataImport->GetTypeDefProps(classTypeDef, szClass, NAME_BUFFER_SIZE, &cchClass, 0, 0);
			if (SUCCEEDED(hr))
			{
				// create the fully qualified name
				_snwprintf_s(wszMethod, cMethod, cMethod, L"%s.%s", szClass, szFunction);
			}
		}
		// release our reference to the metadata
		pIMetaDataImport->Release();
	}

	return hr;
}


int wchar2char(const wchar_t* wchar, char* out)
{
	return WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), out, NAME_BUFFER_SIZE, NULL, NULL);
}

CCorProfilerCallbackImpl::CCorProfilerCallbackImpl()
{
	char path[100] = { 0 };
	_getcwd(path, 100);
	cout << path << endl;
	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log.properties"));
	_logger = Logger::getRoot();
	LOG4CPLUS_FATAL(_logger, "create COM instance");
}

CCorProfilerCallbackImpl::~CCorProfilerCallbackImpl()
{
}

STDMETHODIMP CCorProfilerCallbackImpl::Initialize(IUnknown *pICorProfilerInfoUnk)
{
	// log that we are initializing
	LOG4CPLUS_FATAL(_logger, "Initializing...");

	// get the ICorProfilerInfo interface
	HRESULT hr = pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo, (LPVOID*)&m_pICorProfilerInfo);
	if (FAILED(hr))
		return E_FAIL;
	// determine if this object implements ICorProfilerInfo2
	hr = pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo2, (LPVOID*)&m_pICorProfilerInfo2);
	if (FAILED(hr))
	{
		// we still want to work if this call fails, might be an older .NET version
		m_pICorProfilerInfo2.p = NULL;
	}
	// report our success or failure to the log file
	if (FAILED(hr))
		LOG4CPLUS_FATAL(_logger, "Error setting the enter, leave and tailcall hooks\r\n\r\n");
	else
		LOG4CPLUS_FATAL(_logger, "Successfully initialized profiling\r\n\r\n");

	SetEvent();

	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::Shutdown()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::AppDomainCreationStarted(AppDomainID appDomainID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::AppDomainCreationFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::AppDomainShutdownStarted(AppDomainID appDomainID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::AppDomainShutdownFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::AssemblyLoadStarted(AssemblyID assemblyID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::AssemblyLoadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::AssemblyUnloadStarted(AssemblyID assemblyID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::AssemblyUnloadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ModuleLoadStarted(ModuleID moduleID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ModuleLoadFinished(ModuleID moduleID, HRESULT hrStatus)
{
	WCHAR szName[NAME_BUFFER_SIZE] = { 0 };
	ULONG length =  GetModuleName(this->m_pICorProfilerInfo, moduleID, szName, NAME_BUFFER_SIZE);
	char name[NAME_BUFFER_SIZE] = { 0 };
	wchar2char(szName, name);
	LOG4CPLUS_INFO(_logger, name);
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ModuleUnloadStarted(ModuleID moduleID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ModuleUnloadFinished(ModuleID moduleID, HRESULT hrStatus)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ModuleAttachedToAssembly(ModuleID moduleID, AssemblyID assemblyID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ClassLoadStarted(ClassID classID)
{
	LogString("Class load");
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ClassLoadFinished(ClassID classID, HRESULT hrStatus)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ClassUnloadStarted(ClassID classID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ClassUnloadFinished(ClassID classID, HRESULT hrStatus)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::FunctionUnloadStarted(FunctionID functionID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::JITCompilationStarted(FunctionID functionID, BOOL fIsSafeToBlock)
{
	LogString("Compile begin");
	WCHAR name[NAME_BUFFER_SIZE];
	GetFullMethodName(functionID, name, NAME_BUFFER_SIZE);

	char * m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, name, wcslen(name), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, name, wcslen(name), m_char, len, NULL, NULL);
	LogString("function: ", m_char);
	ClassID *pClass = 0;
	ModuleID *pModule = 0;
	mdToken *pToken = 0;

	m_pICorProfilerInfo->GetFunctionInfo(functionID, pClass, pModule, pToken);
	LogString("OK");
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::JITCompilationFinished(FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock)
{
	LogString("Compile finish");
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::JITCachedFunctionSearchStarted(FunctionID functionID, BOOL *pbUseCachedFunction)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::JITCachedFunctionSearchFinished(FunctionID functionID, COR_PRF_JIT_CACHE result)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::JITFunctionPitched(FunctionID functionID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::JITInlining(FunctionID callerID, FunctionID calleeID, BOOL *pfShouldInline)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::UnmanagedToManagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ManagedToUnmanagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ThreadCreated(ThreadID threadID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ThreadDestroyed(ThreadID threadID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ThreadAssignedToOSThread(ThreadID managedThreadID, DWORD osThreadID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RemotingClientInvocationStarted()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RemotingClientSendingMessage(GUID *pCookie, BOOL fIsAsync)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RemotingClientReceivingReply(GUID *pCookie, BOOL fIsAsync)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RemotingClientInvocationFinished()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RemotingServerReceivingMessage(GUID *pCookie, BOOL fIsAsync)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RemotingServerInvocationStarted()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RemotingServerInvocationReturned()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RemotingServerSendingReply(GUID *pCookie, BOOL fIsAsync)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RuntimeSuspendFinished()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RuntimeSuspendAborted()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RuntimeResumeStarted()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RuntimeResumeFinished()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RuntimeThreadSuspended(ThreadID threadID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RuntimeThreadResumed(ThreadID threadID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::MovedReferences(ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ObjectAllocated(ObjectID objectID, ClassID classID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ObjectsAllocatedByClass(ULONG classCount, ClassID classIDs[], ULONG objects[])
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ObjectReferences(ObjectID objectID, ClassID classID, ULONG objectRefs, ObjectID objectRefIDs[])
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RootReferences(ULONG rootRefs, ObjectID rootRefIDs[])
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionThrown(ObjectID thrownObjectID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionUnwindFunctionEnter(FunctionID functionID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionUnwindFunctionLeave()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionSearchFunctionEnter(FunctionID functionID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionSearchFunctionLeave()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionSearchFilterEnter(FunctionID functionID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionSearchFilterLeave()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionSearchCatcherFound(FunctionID functionID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionCLRCatcherFound()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionCLRCatcherExecute()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionOSHandlerEnter(FunctionID functionID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionOSHandlerLeave(FunctionID functionID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionUnwindFinallyEnter(FunctionID functionID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionUnwindFinallyLeave()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionCatcherEnter(FunctionID functionID,
	ObjectID objectID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ExceptionCatcherLeave()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable, ULONG cSlots)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::ThreadNameChanged(ThreadID threadID, ULONG cchName, WCHAR name[])
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::SurvivingReferences(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::GarbageCollectionFinished()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::RootReferences2(ULONG cRootRefs, ObjectID rootRefIDs[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIDs[])
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::HandleCreated(GCHandleID handleID, ObjectID initialObjectID)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImpl::HandleDestroyed(GCHandleID handleID)
{
	return S_OK;
}

HRESULT CCorProfilerCallbackImpl::SetEvent()
{
	//COR_PRF_MONITOR_NONE	= 0,
	//COR_PRF_MONITOR_FUNCTION_UNLOADS	= 0x1,
	//COR_PRF_MONITOR_CLASS_LOADS	= 0x2,
	//COR_PRF_MONITOR_MODULE_LOADS	= 0x4,
	//COR_PRF_MONITOR_ASSEMBLY_LOADS	= 0x8,
	//COR_PRF_MONITOR_APPDOMAIN_LOADS	= 0x10,
	//COR_PRF_MONITOR_JIT_COMPILATION	= 0x20,
	//COR_PRF_MONITOR_EXCEPTIONS	= 0x40,
	//COR_PRF_MONITOR_GC	= 0x80,
	//COR_PRF_MONITOR_OBJECT_ALLOCATED	= 0x100,
	//COR_PRF_MONITOR_THREADS	= 0x200,
	//COR_PRF_MONITOR_REMOTING	= 0x400,
	//COR_PRF_MONITOR_CODE_TRANSITIONS	= 0x800,
	//COR_PRF_MONITOR_ENTERLEAVE	= 0x1000,
	//COR_PRF_MONITOR_CCW	= 0x2000,
	//COR_PRF_MONITOR_REMOTING_COOKIE	= 0x4000 | COR_PRF_MONITOR_REMOTING,
	//COR_PRF_MONITOR_REMOTING_ASYNC	= 0x8000 | COR_PRF_MONITOR_REMOTING,
	//COR_PRF_MONITOR_SUSPENDS	= 0x10000,
	//COR_PRF_MONITOR_CACHE_SEARCHES	= 0x20000,
	//COR_PRF_MONITOR_CLR_EXCEPTIONS	= 0x1000000,
	//COR_PRF_MONITOR_ALL	= 0x107ffff,
	//COR_PRF_ENABLE_REJIT	= 0x40000,
	//COR_PRF_ENABLE_INPROC_DEBUGGING	= 0x80000,
	//COR_PRF_ENABLE_JIT_MAPS	= 0x100000,
	//COR_PRF_DISABLE_INLINING	= 0x200000,
	//COR_PRF_DISABLE_OPTIMIZATIONS	= 0x400000,
	//COR_PRF_ENABLE_OBJECT_ALLOCATED	= 0x800000,
	// New in VS2005
	//	COR_PRF_ENABLE_FUNCTION_ARGS	= 0x2000000,
	//	COR_PRF_ENABLE_FUNCTION_RETVAL	= 0x4000000,
	//  COR_PRF_ENABLE_FRAME_INFO	= 0x8000000,
	//  COR_PRF_ENABLE_STACK_SNAPSHOT	= 0x10000000,
	//  COR_PRF_USE_PROFILE_IMAGES	= 0x20000000,
	// End New in VS2005
	//COR_PRF_ALL	= 0x3fffffff,
	//COR_PRF_MONITOR_IMMUTABLE	= COR_PRF_MONITOR_CODE_TRANSITIONS | COR_PRF_MONITOR_REMOTING | COR_PRF_MONITOR_REMOTING_COOKIE | COR_PRF_MONITOR_REMOTING_ASYNC | COR_PRF_MONITOR_GC | COR_PRF_ENABLE_REJIT | COR_PRF_ENABLE_INPROC_DEBUGGING | COR_PRF_ENABLE_JIT_MAPS | COR_PRF_DISABLE_OPTIMIZATIONS | COR_PRF_DISABLE_INLINING | COR_PRF_ENABLE_OBJECT_ALLOCATED | COR_PRF_ENABLE_FUNCTION_ARGS | COR_PRF_ENABLE_FUNCTION_RETVAL | COR_PRF_ENABLE_FRAME_INFO | COR_PRF_ENABLE_STACK_SNAPSHOT | COR_PRF_USE_PROFILE_IMAGES

	// set the event mask 
	DWORD eventMask = (DWORD)(COR_PRF_MONITOR_JIT_COMPILATION | COR_PRF_MONITOR_MODULE_LOADS);
	return m_pICorProfilerInfo->SetEventMask(eventMask);
}
