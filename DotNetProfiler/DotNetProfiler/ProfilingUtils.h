#pragma once

#include <CorHdr.h>
#include <cor.h>
#include <corprof.h>
ULONG GetModuleName(ICorProfilerInfo* corProf, ModuleID id, LPWSTR szName, ULONG inSize);

