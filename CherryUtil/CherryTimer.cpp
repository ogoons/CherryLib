#include "stdafx.h"
#include "CherryTimer.h"

CCherryTimer::CCherryTimer(LPVOID lpCallbackFunc, LPVOID lpParam, DWORD dwDelay, DWORD dwCount)
{
#ifdef USE_STD_THREAD
	m_pThread = NULL;
#else
	m_pThread = new CWinThread(&CCherryTimer::ThreadFunc, this);	
#endif
	m_lpParam = lpParam;
	m_pCallbackFunc = (void(__cdecl *)(LPVOID))lpCallbackFunc;
	m_dwDelay = dwDelay;
	m_dwCount = dwCount;
	m_bRunning = FALSE;
}

CCherryTimer::~CCherryTimer()
{
	Stop();
}

void CCherryTimer::Run()
{
	m_bRunning = TRUE;
#ifdef USE_STD_THREAD
	m_pThread = new thread(CCherryTimer::ThreadFunc, this);
#else
	m_pThread->CreateThread();
#endif
}

void CCherryTimer::Stop()
{
	m_bRunning = FALSE;
}

#ifdef USE_STD_THREAD
void CCherryTimer::ThreadFunc(LPVOID lpParam)
#else
UINT CCherryTimer::ThreadFunc(LPVOID lpParam)
#endif
{
	CCherryTimer *pThis = (CCherryTimer *)lpParam;
	if (NULL == pThis)
#ifdef USE_STD_THREAD
		return;
#else
		return 0;
#endif

	DWORD dwDelay = pThis->m_dwDelay;
	DWORD dwCount = pThis->m_dwCount;
	while (pThis->m_bRunning)
	{
		Sleep(dwDelay);
		pThis->m_pCallbackFunc(pThis->m_lpParam);
		if (0 < dwCount) {
			if (0 == --dwCount)
				break;
		}
	}

	if (NULL != pThis) 
	{
		delete pThis;
		pThis = NULL;
	}

#ifndef USE_STD_THREAD
	return 0;
#endif
}



