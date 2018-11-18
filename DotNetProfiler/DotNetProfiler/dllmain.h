// dllmain.h: 模块类的声明。

class CDotNetProfilerModule : public ATL::CAtlDllModuleT< CDotNetProfilerModule >
{
public :
	DECLARE_LIBID(LIBID_DotNetProfilerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DOTNETPROFILER, "{87afb43d-b241-4db3-b23a-e8da8b625bfc}")
};

extern class CDotNetProfilerModule _AtlModule;
