// Profiler.cpp: CProfiler 的实现

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

