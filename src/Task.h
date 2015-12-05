#ifndef _SRC_TASK_CPP_H_
#define _SRC_TASK_CPP_H_

class CTask
{
public:
	CTask(void (*pFunc)(void*), void* arg);
	~CTask(void);

	void Run();

private:
	void (*m_pFunc)(void*);
	void* m_arg;
};

#endif/*_SRC_TASK_CPP_H_*/
