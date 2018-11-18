#include "stdafx.h"
#include "FunctionInfo.h"


CFunctionInfo::CFunctionInfo(FunctionID functionID, const char* name)
{
	m_functionID = functionID;
	m_name = _strdup(name);
	m_callCount = 0;
}

CFunctionInfo::~CFunctionInfo()
{
	// free the allocated name
	free(m_name);
}

char* CFunctionInfo::GetName()
{
	return m_name;
}

FunctionID CFunctionInfo::GetFunctionID()
{
	return m_functionID;
}

long CFunctionInfo::GetCallCount()
{
	return m_callCount;
}

void CFunctionInfo::IncrementCallCount()
{
	m_callCount++;
}
