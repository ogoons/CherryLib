#pragma once
#define USE_STD_THREAD

#include <thread>
using namespace std;

class CCherryTimer
{
public:
	CCherryTimer(LPVOID lpCallbackFunc, LPVOID lpParam, DWORD dwDelay, DWORD dwCount = 0);
	
protected:
	void		(*m_pCallbackFunc)(LPVOID);
	LPVOID		m_lpParam;
#ifdef USE_STD_THREAD
	thread		*m_pThread;
#else
	CWinThread	*m_pThread;
#endif
	DWORD		m_dwDelay;
	DWORD		m_dwCount;
	BOOL		m_bRunning;
	
public:
	void Run();
	void Stop();

protected:
#ifdef USE_STD_THREAD
	static void ThreadFunc(LPVOID lpParam);
#else
	static UINT ThreadFunc(LPVOID lpParam);
#endif
public:
	virtual ~CCherryTimer();
};

