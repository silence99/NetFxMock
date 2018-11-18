﻿// Profiler.cpp: CProfiler 的实现

#include "stdafx.h"
#include "Profiler.h"
#include <fstream>
using namespace std;

#pragma  once
#pragma  comment(lib,"corguids.lib")

// CProfiler

CProfiler* g_pICorProfilerCallback = NULL;

CProfiler::CProfiler()
{
	LogString("create profiler");
	m_hLogFile = INVALID_HANDLE_VALUE;
	m_callStackSize = 0;
}

HRESULT CProfiler::FinalConstruct()
{
	

	// log that we have reached FinalConstruct
	LogString("Entering FinalConstruct\r\n\r\n");

	return S_OK;
}

void CProfiler::FinalRelease()
{
	// log that we have reached FinalRelease
	LogString("\r\n\r\nEntering FinalRelease\r\n\r\n");

	// close the log file
	CloseLogFile();
}

// ----  CALLBACK FUNCTIONS ------------------

// this function simply forwards the FunctionEnter call the global profiler object
void __stdcall FunctionEnterGlobal(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO frameInfo, COR_PRF_FUNCTION_ARGUMENT_INFO *argInfo)
{
	// make sure the global reference to our profiler is valid
	if (g_pICorProfilerCallback != NULL)
		g_pICorProfilerCallback->Enter(functionID, clientData, frameInfo, argInfo);
}

// this function is called by the CLR when a function has been entered
void _declspec(naked) FunctionEnterNaked(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo)
{
	__asm
	{
		push    ebp                 // Create a standard frame
		mov     ebp, esp
		pushad                      // Preserve all registers

		mov     eax, [ebp + 0x14]      // argumentInfo
		push    eax
		mov     ecx, [ebp + 0x10]      // func
		push    ecx
		mov     edx, [ebp + 0x0C]      // clientData
		push    edx
		mov     eax, [ebp + 0x08]      // functionID
		push    eax
		call    FunctionEnterGlobal

		popad                       // Restore all registers
		pop     ebp                 // Restore EBP
		ret     16
	}
}

// this function simply forwards the FunctionLeave call the global profiler object
void __stdcall FunctionLeaveGlobal(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO frameInfo, COR_PRF_FUNCTION_ARGUMENT_RANGE *retvalRange)
{
	// make sure the global reference to our profiler is valid
	if (g_pICorProfilerCallback != NULL)
		g_pICorProfilerCallback->Leave(functionID, clientData, frameInfo, retvalRange);
}

// this function is called by the CLR when a function is exiting
void _declspec(naked) FunctionLeaveNaked(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *retvalRange)
{
	__asm
	{
		push    ebp                 // Create a standard frame
		mov     ebp, esp
		pushad                      // Preserve all registers

		mov     eax, [ebp + 0x14]      // argumentInfo
		push    eax
		mov     ecx, [ebp + 0x10]      // func
		push    ecx
		mov     edx, [ebp + 0x0C]      // clientData
		push    edx
		mov     eax, [ebp + 0x08]      // functionID
		push    eax
		call    FunctionLeaveGlobal

		popad                       // Restore all registers
		pop     ebp                 // Restore EBP
		ret     16
	}
}

// this function simply forwards the FunctionLeave call the global profiler object
void __stdcall FunctionTailcallGlobal(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO frameInfo)
{
	if (g_pICorProfilerCallback != NULL)
		g_pICorProfilerCallback->Tailcall(functionID, clientData, frameInfo);
}

// this function is called by the CLR when a tailcall occurs.  A tailcall occurs when the 
// last action of a method is a call to another method.
void _declspec(naked) FunctionTailcallNaked(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func)
{
	__asm
	{
		push    ebp                 // Create a standard frame
		mov     ebp, esp
		pushad                      // Preserve all registers

		mov     eax, [ebp + 0x14]      // argumentInfo
		push    eax
		mov     ecx, [ebp + 0x10]      // func
		push    ecx
		mov     edx, [ebp + 0x0C]      // clientData
		push    edx
		mov     eax, [ebp + 0x08]      // functionID
		push    eax
		call    FunctionTailcallGlobal

		popad                       // Restore all registers
		pop     ebp                 // Restore EBP
		ret     16
	}
}

// ----  MAPPING FUNCTIONS ------------------

// this function is called by the CLR when a function has been mapped to an ID
UINT_PTR CProfiler::FunctionMapper(FunctionID functionID, BOOL *pbHookFunction)
{
	// make sure the global reference to our profiler is valid.  Forward this
	// call to our profiler object
	if (g_pICorProfilerCallback != NULL)
		g_pICorProfilerCallback->MapFunction(functionID);

	// we must return the function ID passed as a parameter
	return (UINT_PTR)functionID;
}

// the static function called by .Net when a function has been mapped to an ID
void CProfiler::MapFunction(FunctionID functionID)
{
	
}

// ----  CALLBACK HANDLER FUNCTIONS ------------------

// our real handler for FunctionEnter notification
void CProfiler::Enter(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO frameInfo, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo)
{
	WCHAR szMethodName[NAME_BUFFER_SIZE];
	GetFullMethodName(functionID, szMethodName, NAME_BUFFER_SIZE);
	LogString("enter function:", szMethodName);
	m_callStackSize++;
}

// our real handler for FunctionLeave notification
void CProfiler::Leave(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO frameInfo, COR_PRF_FUNCTION_ARGUMENT_RANGE *argumentRange)
{
	// decrement the call stack size
	if (m_callStackSize > 0)
		m_callStackSize--;
}

// our real handler for the FunctionTailcall notification
void CProfiler::Tailcall(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO frameInfo)
{
	// decrement the call stack size
	if (m_callStackSize > 0)
		m_callStackSize--;
}

// ----  ICorProfilerCallback IMPLEMENTATION ------------------

// called when the profiling object is created by the CLR
STDMETHODIMP CProfiler::Initialize(IUnknown *pICorProfilerInfoUnk)
{
	// set up our global access pointer
	g_pICorProfilerCallback = this;

	// log that we are initializing
	LogString("Initializing...\r\n\r\n");

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

	// Indicate which events we're interested in.
	hr = SetEventMask();
	if (FAILED(hr))
		LogString("Error setting the event mask\r\n\r\n");

	
		// note that we are casting our functions to the definitions for the callbacks
		hr = m_pICorProfilerInfo->SetEnterLeaveFunctionHooks((FunctionEnter*)&FunctionEnterNaked, (FunctionLeave*)&FunctionLeaveNaked, (FunctionTailcall*)&FunctionTailcallNaked);
		if (SUCCEEDED(hr))
			hr = m_pICorProfilerInfo->SetFunctionIDMapper((FunctionIDMapper*)&FunctionMapper);
	
	// report our success or failure to the log file
	if (FAILED(hr))
		LogString("Error setting the enter, leave and tailcall hooks\r\n\r\n");
	else
		LogString("Successfully initialized profiling\r\n\r\n");

	return S_OK;
}

// called when the profiler is being terminated by the CLR
STDMETHODIMP CProfiler::Shutdown()
{
	// log the we're shutting down
	LogString("\r\n\r\nShutdown... writing function list\r\n\r\n");

	// write the function names and call counts to the output file
	std::map<FunctionID, CFunctionInfo*>::iterator iter;
	for (iter = m_functionMap.begin(); iter != m_functionMap.end(); iter++)
	{
		// log the function's info
		CFunctionInfo* functionInfo = iter->second;
		LogString("%s : call count = %d\r\n", functionInfo->GetName(), functionInfo->GetCallCount());
		// free the memory for the object
		delete iter->second;
	}
	// clear the map
	m_functionMap.clear();

	// tear down our global access pointers
	g_pICorProfilerCallback = NULL;

	return S_OK;
}

// Creates the log file.  It uses the LOG_FILENAME environment variable if it 
// exists, otherwise it creates the file "ICorProfilerCallback Log.log" in the 
// executing directory.  This function doesn't report success or not because 
// LogString always checks for a valid file handle whenever the file is written
// to.
void CProfiler::CreateLogFile()
{
	
}

// Closes the log file
void CProfiler::CloseLogFile()
{
	// close the log file
	if (m_hLogFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hLogFile);
		m_hLogFile = INVALID_HANDLE_VALUE;
	}
}

// Writes a string to the log file.  Uses the same calling convention as printf.
void CProfiler::LogString(char *pszFmtString, ...)
{
	CHAR szBuffer[4096]; 
	DWORD dwWritten = 0;

	va_list args;
	va_start(args, pszFmtString);
	vsprintf_s(szBuffer, pszFmtString, args);
	va_end(args);

	// write out to the file if the file is open
	//WriteFile(m_hLogFile, szBuffer, (DWORD)strlen(szBuffer), &dwWritten, NULL);
	ofstream o("D:\\work\\code\\DotNetProfiler\\Debug\\test.log", ios::app);
	o << "log:"<< szBuffer << endl;
	o.close();
}

///<summary>
// We are monitoring events that are interesting for determining
// the hot spots of a managed CLR program (profilee). This includes
// thread related events, function enter/leave events, exception 
// related events, and unmanaged/managed transition events. Note 
// that we disable inlining. Although this does indeed affect the 
// execution time, it provides better accuracy for determining
// hot spots.
//
// If the system does not support high precision counters, then
// do not profile anything. This is determined in the constructor.
///</summary>
HRESULT CProfiler::SetEventMask()
{
	LogString("Set Event mask");
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
	DWORD eventMask = (DWORD)(COR_PRF_MONITOR_JIT_COMPILATION);
	return m_pICorProfilerInfo->SetEventMask(eventMask);
}

// creates the fully scoped name of the method in the provided buffer
HRESULT CProfiler::GetFullMethodName(FunctionID functionID, LPWSTR wszMethod, int cMethod)
{
	IMetaDataImport* pIMetaDataImport = 0;
	HRESULT hr = S_OK;
	mdToken funcToken = 0;
	WCHAR szFunction[NAME_BUFFER_SIZE];
	WCHAR szClass[NAME_BUFFER_SIZE];

	// get the token for the function which we will use to get its name
	hr = m_pICorProfilerInfo->GetTokenAndMetaDataFromFunction(functionID, IID_IMetaDataImport, (LPUNKNOWN *)&pIMetaDataImport, &funcToken);
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