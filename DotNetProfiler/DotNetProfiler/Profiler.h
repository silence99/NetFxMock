// Profiler.h: CProfiler 的声明

#pragma once
#include "resource.h"       // 主符号

#include "CorProfilerCallbackImpl.h"
#include "FunctionInfo.h"

#include "DotNetProfiler_i.h"

#include <map>



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

#define ASSERT_HR(x) _ASSERT(SUCCEEDED(x))
#define NAME_BUFFER_SIZE 1024

// CProfiler

class ATL_NO_VTABLE CProfiler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProfiler, &CLSID_Profiler>,
	public IDispatchImpl<IProfiler, &IID_IProfiler, &LIBID_DotNetProfilerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CCorProfilerCallbackImpl
{
public:
	CProfiler();

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CProfiler)
	COM_INTERFACE_ENTRY(IProfiler)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICorProfilerCallback)
	COM_INTERFACE_ENTRY(ICorProfilerCallback2)
END_COM_MAP()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	// overridden implementations of FinalConstruct and FinalRelease
	HRESULT FinalConstruct();
	void FinalRelease();

	STDMETHOD(Shutdown)();

	// callback functions
	void Enter(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO frameInfo, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo);
	void Leave(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO frameInfo, COR_PRF_FUNCTION_ARGUMENT_RANGE *argumentRange);
	void Tailcall(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO frameInfo);

	// mapping functions
	static UINT_PTR _stdcall FunctionMapper(FunctionID functionId, BOOL *pbHookFunction);
	void MapFunction(FunctionID);

	// logging function
	void LogString(char* pszFmtString, ...);

private:
	
	// STL map for our hashed functions
	std::map<FunctionID, CFunctionInfo*> m_functionMap;
	// the number of levels deep we are in the call stack
	int m_callStackSize;
	// handle and filename of log file
	HANDLE m_hLogFile;
	TCHAR m_logFileName[_MAX_PATH];
	// function to set up our event mask
	// creates the log file
	void CreateLogFile();
	// closes the log file ;)
	void CloseLogFile();

public:



};

OBJECT_ENTRY_AUTO(__uuidof(Profiler), CProfiler)
