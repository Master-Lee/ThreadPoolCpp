#ifndef _SRC_PLATFORM_H_
#define _SRC_PLATFORM_H_

#ifdef WIN32
#include <windows.h>
#define SLEEP(x)                    Sleep(x)
#else
#include <sys/stat.h> 
#include <sys/types.h> 
#define SLEEP(x)                    usleep(1000*x)
#endif

/*
	Thread
*/
#ifdef _WIN32
#include <process.h>
#define THREAD_EXIT								              return
#define THREAD_API                          
#define THREAD_TYPE							               void*
#define DEFINE_THREAD(_thread)			               void* _thread
#define BEGIN_THREAD(_thread, _pfn, _pArg)	    _thread = (HANDLE)_beginthread(_pfn, 0, _pArg)
#define END_THREAD()							                _endthread();
#define FAILED_THREAD(_t)                                   (_t) <= 0
#define SUCCEED_THREAD(_t)                               (_t) > 0
#define WAITTHREADEND(_t)                               WaitForSingleObject(_t, INFINITE)
#else
#include <pthread.h>
#include <semaphore.h>
#define THREAD_EXIT								              pthread_exit(NULL); return NULL
#define THREAD_API								              *
#define THREAD_TYPE							              pthread_t
#define DEFINE_THREAD(_thread)		    	           pthread_t _thread
#define BEGIN_THREAD(_thread, _pfn, _pArg)     pthread_create(&_thread, NULL, _pfn, _pArg)
#define END_THREAD()                                          ;
#define FAILED_THREAD(_t)					                (_t) != 0
#define SUCCEED_THREAD(_t)				            	(_t) == 0
#define WAITTHREADEND(_t)                               pthread_join (_t, NULL)
#endif

/*
	Event/Condition
*/
#ifdef _WIN32
#define DEFINE_EVENT(_event)				                    HANDLE _event
#define INITIALIZE_EVENT(_event)			                _event = CreateEvent(NULL, FALSE, FALSE, NULL)
#define SIGNAL_EVENT(_event)			                        SetEvent(_event);
#define BROADCAST_EVENT(_event)			                SetEvent(_event);
#define WAIT_EVENT(_event,_timeout,_mutex)         WaitForSingleObject(_event, _timeout);
#define DESTROY_EVENT(_event)				                 CloseHandle(_event)
#else
#include <unistd.h>
#define DEFINE_EVENT(_event)				                     pthread_cond_t  _event;
#define INITIALIZE_EVENT(_event)			                 pthread_cond_init(&(_event), NULL)
#define SIGNAL_EVENT(_event)			                         pthread_cond_signal(&(_event));
#define BROADCAST_EVENT(_event)			                 pthread_cond_broadcast(&(_event))
#define WAIT_EVENT(_event,_timeout,_mutex)         pthread_cond_wait(&(_event),_mutex)
#define DESTROY_EVENT(_event)				                 pthread_cond_destroy(&(_event))
#endif

/*
	Mutex
*/
#ifdef _WIN32
#define	    MUTEXTYPE                      HANDLE
#define		MUTEXDEFX(x)		          HANDLE x 
#define		MUTEXINITX(x)		          x = CreateMutex (NULL, FALSE, NULL)  
#define		MUTEXLOCKX(x)		          WaitForSingleObject(x , INFINITE)
#define		MUTEXUNLOCKX(x)		  ReleaseMutex(x)
#define       MUTEXDESDROYX(x)        CloseHandle(x)
#else
#define	    MUTEXTYPE                      pthread_mutex_t
#define		MUTEXDEFX(x)	              pthread_mutex_t x
#define		MUTEXINITX(x)		          pthread_mutex_init(& x, NULL)
#define		MUTEXLOCKX(x)		          pthread_mutex_lock(& x)
#define		MUTEXUNLOCKX(x)	      pthread_mutex_unlock(& x)
#define       MUTEXDESDROYX(x)	      pthread_mutex_destroy(& x)
#endif

class CMutex
{
public:
	CMutex() { MUTEXINITX(x);}
	~CMutex() { MUTEXDESDROYX(x); }

	void lock() { MUTEXLOCKX(x); }
	void unlock() { MUTEXUNLOCKX(x); }
	MUTEXTYPE* operator &() { return &x; }

private:
	MUTEXDEFX(x);
};

class CEvent
{
public:
	CEvent() { INITIALIZE_EVENT(x); }
	~CEvent() { DESTROY_EVENT(x); }
	void signal() { SIGNAL_EVENT(x); }
	void broadcast() {BROADCAST_EVENT(x);}
	void wait(int timeout,CMutex &m) {WAIT_EVENT(x,timeout,&m);}
private:
	DEFINE_EVENT(x);
};

template<class T>
class CLock
{
public:
	explicit CLock(T& cs) :m_lock(cs) { m_lock.lock();}
	~CLock() 	{ m_lock.unlock();}

private:
	T& m_lock;
};

#endif/*_SRC_PLATFORM_H_*/
