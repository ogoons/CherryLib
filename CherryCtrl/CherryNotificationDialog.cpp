#include "stdafx.h"
#include "CherryNotificationDialog.h"

#include "CherryUtil.h"
#ifdef _DEBUG
#pragma comment(lib, "CherryUtilD.lib")
#else
#pragma comment(lib, "CherryUtil.lib")
#endif

#include "versionhelpers.h"

#define ID_TIMER_FADING		100
#define ID_TIMER_DURATION	101
#define SCROLLBAR_WIDTH		18

IMPLEMENT_DYNAMIC(CCherryNotificationDialog, CCherryDialog)

CCherryNotificationDialog::CCherryNotificationDialog()
:
m_bWinXPOrLater(IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0))
{
	Initialize();
}

CCherryNotificationDialog::~CCherryNotificationDialog()
{
}

BEGIN_MESSAGE_MAP(CCherryNotificationDialog, CCherryDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CCherryNotificationDialog::Initialize()
{
	m_bFadeInTurn = TRUE;
	m_nFadeCount = 0;
	m_nDurationSec = 0;
	m_nDurationSecCount = 0;

	m_nFadingTimerRet = 0;
	m_nDurationTimerRet = 0;
}

int CCherryNotificationDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CCherryDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_bWinXPOrLater)
		m_nFadingTimerRet = SetTimer(ID_TIMER_FADING, 1, NULL);

	if (m_nDurationSec > 0)
		m_nDurationTimerRet = SetTimer(ID_TIMER_DURATION, 1000, NULL);

	return 0;
}

CHERRY_RET CCherryNotificationDialog::Create(DWORD dwStyle, UINT nWidth, UINT nHeight, CWnd* pParentWnd, UINT nID, UINT nDurationSec)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (GetSafeHwnd())
		{
			if (m_nDurationTimerRet != 0)
			{
				KillTimer(m_nDurationTimerRet);
				m_nDurationTimerRet = 0;
			}

			if (m_nFadingTimerRet != 0)
			{
				KillTimer(m_nFadingTimerRet);
				m_nFadingTimerRet = 0;
			}

			DestroyWindow();
			//Initialize();
		}

		m_nDurationSec = m_nDurationSecCount = (int)nDurationSec;

		CRect rect = GetAnchorRect(nWidth, nHeight);

		dwStyle |= WS_POPUP | WS_VISIBLE;

		if (!CCherryDialog::CreateEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, AfxRegisterWndClass(0), NULL, dwStyle, rect, pParentWnd, nID))
			throw CCherryException::ERROR_NOTIFICATIONDIALOG_CREATE_FAIL;
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

void CCherryNotificationDialog::OnDrawCherry(CCherryMemDC *pDC)
{
}

BOOL CCherryNotificationDialog::FadeInWindow(USHORT nStep)
{
	if (!m_bWinXPOrLater)
		return FALSE;

	if (!(GetExStyle() & WS_EX_LAYERED))
		return FALSE;

	m_nFadeCount += nStep;

	if (m_nFadeCount >= 255)
	{
		m_nFadeCount = 255;
		m_bFadeInTurn = FALSE;

		// 마지막 증가 후 적용
		SetLayeredWindowAttributes(NULL, m_nFadeCount, LWA_ALPHA);

		return FALSE;
	}

	return SetLayeredWindowAttributes(NULL, m_nFadeCount, LWA_ALPHA);
}

BOOL CCherryNotificationDialog::FadeOutWindow(USHORT nStep)
{
	if (!m_bWinXPOrLater)
		return FALSE;

	if (!(GetExStyle() & WS_EX_LAYERED))
		return FALSE;

	m_nFadeCount -= nStep;

	if (m_nFadeCount <= 0)
	{
		m_nFadeCount = 0;
		m_bFadeInTurn = TRUE;

		// 마지막 감소 후 적용
		SetLayeredWindowAttributes(NULL, m_nFadeCount, LWA_ALPHA);

		return FALSE;
	}

	return SetLayeredWindowAttributes(NULL, m_nFadeCount, LWA_ALPHA);
}

BOOL CCherryNotificationDialog::IsFadeInWindowTurn() const
{
	return m_bFadeInTurn;
}

void CCherryNotificationDialog::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case ID_TIMER_DURATION:

		if (--m_nDurationSecCount <= 0)
		{
			m_nDurationSecCount = m_nDurationSec;

			if (m_nFadingTimerRet != 0)
			{
				KillTimer(m_nDurationTimerRet);
				m_nFadingTimerRet = 0;
			}

			m_nFadingTimerRet = SetTimer(ID_TIMER_FADING, 1, NULL);
		}

		break;
	case ID_TIMER_FADING:

		if (IsFadeInWindowTurn())
		{
			// Alpha가 증가하다가 FALSE면 타이머 제거
			if (!FadeInWindow())
			{
				if (m_nFadingTimerRet != 0)
				{
					KillTimer(m_nFadingTimerRet);
					m_nFadingTimerRet = 0;
				}
			}
		}
		else
		{
			// Alpha가 감소하다가 FALSE면 타이머 제거 후 창 닫음
			if (!FadeOutWindow())
			{
				if (m_nFadingTimerRet != 0)
				{
					KillTimer(m_nFadingTimerRet);
					m_nFadingTimerRet = 0;
				}

				DestroyWindow();

				return;
			}
		}

		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CCherryNotificationDialog::PostNcDestroy()
{
	// 자식 클래스
	delete this;

	CCherryDialog::PostNcDestroy();
}

CRect CCherryNotificationDialog::GetAnchorRect(UINT nWidth, UINT nHeight)
{
	CRect rect;

	APPBARDATA appBarData;
	appBarData.cbSize = sizeof(APPBARDATA);

	if (!SHAppBarMessage(ABM_GETTASKBARPOS, &appBarData))
		return rect;

	// TASKBAR 가 위치한 모니터의 핸들 얻어옴
	HMONITOR hMonitor = MonitorFromRect(&appBarData.rc, MONITOR_DEFAULTTOPRIMARY);

	MONITORINFO monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFO);

	GetMonitorInfo(hMonitor, &monitorInfo);
	CRect workRect(monitorInfo.rcWork);

	CPoint point;

	switch (appBarData.uEdge)
	{
	case ABE_LEFT:
		rect.SetRect(
			appBarData.rc.right,
			appBarData.rc.bottom - SCROLLBAR_WIDTH - nHeight,
			appBarData.rc.right + nWidth,
			appBarData.rc.bottom - SCROLLBAR_WIDTH);
		break;
	case ABE_TOP:
		rect.SetRect(
			appBarData.rc.right - SCROLLBAR_WIDTH - nWidth,
			appBarData.rc.bottom,
			appBarData.rc.right - SCROLLBAR_WIDTH,
			appBarData.rc.bottom + nHeight);
		break;
	case ABE_RIGHT:
		rect.SetRect(
			appBarData.rc.left - nWidth,
			appBarData.rc.bottom - SCROLLBAR_WIDTH - nHeight,
			appBarData.rc.left,
			appBarData.rc.bottom - SCROLLBAR_WIDTH);
		break;
	case ABE_BOTTOM:
		rect.SetRect(
			appBarData.rc.right - SCROLLBAR_WIDTH - nWidth,
			appBarData.rc.top - nHeight,
			appBarData.rc.right - SCROLLBAR_WIDTH,
			appBarData.rc.top);
		break;
	}

	return rect;
}

//////////////////////////////////////////////////////////////////////////
// CCherryNotificationPopup

//#define SCROLLBAR_WIDTH 18
/*
CCherryNotificationPopup::CCherryNotificationPopup()
{
	Initialize();
}

CCherryNotificationPopup::CCherryNotificationPopup(CCherryNotificationDialog *pNotificationDialog, UINT nWidth, UINT nHeight)
{
	
}

CCherryNotificationPopup::~CCherryNotificationPopup()
{
	Destroy();
}

void CCherryNotificationPopup::Initialize()
{
	m_pNotificationDialog = NULL;

	m_nWidth = 0;
	m_nHeight = 0;
}

 DWORD CCherryNotificationPopup::Create(CCherryNotificationDialog *pNotificationDialog, UINT nWidth, UINT nHeight)
 {
 	m_pNotificationDialog = pNotificationDialog;
 
 	m_nWidth = nWidth;
 	m_nHeight = nHeight;
 
 	return CCherryException::ERROR_CHERRY_SUCCESS;
 }

BOOL CCherryNotificationPopup::PopupNotificationDialog()
{
	if (!m_pNotificationDialog || !m_pNotificationDialog->GetSafeHwnd())
		return FALSE;

	CRect anchorRect = GetAnchorRect();

	m_pNotificationDialog->MoveWindow(anchorRect);
	m_pNotificationDialog->ShowWindow(SW_SHOWNORMAL);

	return TRUE;
}

CRect CCherryNotificationPopup::GetAnchorRect()
{
	CRect rect;

	APPBARDATA appBarData;
	appBarData.cbSize = sizeof(APPBARDATA);

	if (!SHAppBarMessage(ABM_GETTASKBARPOS, &appBarData))
		return rect;

	// TASKBAR 가 위치한 모니터의 핸들 얻어옴
	HMONITOR hMonitor = MonitorFromRect(&appBarData.rc, MONITOR_DEFAULTTOPRIMARY);

	MONITORINFO monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFO);
	
	GetMonitorInfo(hMonitor, &monitorInfo);
	CRect workRect(monitorInfo.rcWork);

	CPoint point;

	switch (appBarData.uEdge)
	{
	case ABE_LEFT:
		rect.SetRect(
			appBarData.rc.right, 
			appBarData.rc.bottom - SCROLLBAR_WIDTH - m_nHeight, 
			appBarData.rc.right + m_nWidth,
			appBarData.rc.bottom - SCROLLBAR_WIDTH);
		break;
	case ABE_TOP:
		rect.SetRect(
			appBarData.rc.right - SCROLLBAR_WIDTH - m_nWidth,
			appBarData.rc.bottom,
			appBarData.rc.right - SCROLLBAR_WIDTH,
			appBarData.rc.bottom + m_nHeight);
		break;
	case ABE_RIGHT:
		rect.SetRect(
			appBarData.rc.left - m_nWidth,
			appBarData.rc.bottom - SCROLLBAR_WIDTH - m_nHeight,
			appBarData.rc.left,
			appBarData.rc.bottom - SCROLLBAR_WIDTH);
		break;
	case ABE_BOTTOM:
		rect.SetRect(
			appBarData.rc.right - SCROLLBAR_WIDTH - m_nWidth, 
			appBarData.rc.top - m_nHeight,
			appBarData.rc.right - SCROLLBAR_WIDTH,
			appBarData.rc.top);
		break;
	}	

	return rect;
}

void CCherryNotificationPopup::Destroy()
{
}
*/