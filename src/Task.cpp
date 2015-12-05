#include "Task.h"

CTask::CTask(void (*pFunc)(void*), void* arg)
:m_pFunc(pFunc)
,m_arg(arg)
{
}

CTask::~CTask(void)
{
	if (m_arg != 0) 
	{
		delete m_arg;
		m_arg=0;
	}
}

void CTask::Run()
{
	(*m_pFunc)(m_arg);
}
