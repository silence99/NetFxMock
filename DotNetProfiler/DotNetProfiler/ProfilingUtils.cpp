#include "stdafx.h"
#include "ProfilingUtils.h"


ULONG GetModuleName(ICorProfilerInfo* corProf, ModuleID id, LPWSTR szName, ULONG inSize)
{
	if (NULL == szName) 
	{
		return 0;
	}

	ULONG ret = 0;
	corProf->GetModuleInfo(id, NULL, inSize, &ret, szName, NULL);
	return ret;
}