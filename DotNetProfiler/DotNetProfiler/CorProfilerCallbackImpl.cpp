#include "stdafx.h"
#include <direct.h>
#include <iostream>
#include "CorProfilerCallbackImpl.h"
#include <fstream>
#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <map>

#include "ProfilingUtils.h"


using namespace std;
using namespace log4cplus;

void CCorProfilerCallbackImpl::LogString(char *pszFmtString, ...)
{
	CHAR szBuffer[4096];

	va_list args;
	va_start(args, pszFmtString);
	vsprintf_s(szBuffer, pszFmtString, args);
	va_end(args);

	// log info
	Log_Info(szBuffer);
}

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

STDMETHODIMP CCorProfilerCallbackImpl::Shutdown()
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


void CCorProfilerCallbackImpl::Log_Trace(char* pszFmtString, ...)
{
	CHAR szBuffer[4096];
	va_list args;
	va_start(args, pszFmtString);
	vsprintf_s(szBuffer, pszFmtString, args);
	va_end(args);
	LOG4CPLUS_TRACE(_logger, szBuffer);
}
void CCorProfilerCallbackImpl::Log_Debug(char* pszFmtString, ...)
{
	CHAR szBuffer[4096];
	va_list args;
	va_start(args, pszFmtString);
	vsprintf_s(szBuffer, pszFmtString, args);
	va_end(args);
	LOG4CPLUS_DEBUG(_logger, szBuffer);
}
void CCorProfilerCallbackImpl::Log_Info(char* pszFmtString, ...)
{
	CHAR szBuffer[4096];
	va_list args;
	va_start(args, pszFmtString);
	vsprintf_s(szBuffer, pszFmtString, args);
	va_end(args);
	LOG4CPLUS_INFO(_logger, szBuffer);
}
void CCorProfilerCallbackImpl::Log_Warn(char* pszFmtString, ...)
{
	CHAR szBuffer[4096];
	va_list args;
	va_start(args, pszFmtString);
	vsprintf_s(szBuffer, pszFmtString, args);
	va_end(args);
	LOG4CPLUS_WARN(_logger, szBuffer);
}
void CCorProfilerCallbackImpl::Log_Error(char* pszFmtString, ...)
{
	CHAR szBuffer[4096];
	va_list args;
	va_start(args, pszFmtString);
	vsprintf_s(szBuffer, pszFmtString, args);
	va_end(args);
	LOG4CPLUS_ERROR(_logger, szBuffer);
}
void CCorProfilerCallbackImpl::Log_Fatal(char* pszFmtString, ...)
{
	CHAR szBuffer[4096];
	va_list args;
	va_start(args, pszFmtString);
	vsprintf_s(szBuffer, pszFmtString, args);
	va_end(args);
	LOG4CPLUS_FATAL(_logger, szBuffer);
}