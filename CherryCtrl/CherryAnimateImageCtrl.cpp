// CCherryAnimateImageCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryAnimateImageCtrl.h"


// CCherryAnimateImageCtrl

#define ID_TIMER_ANIMATE	1000

IMPLEMENT_DYNAMIC(CCherryAnimateImageCtrl, CCherryWnd)

CCherryAnimateImageCtrl::CCherryAnimateImageCtrl()
{
	m_nAnimateTimerRet = 0;
	m_nImage = 0;
	m_nInterval = 0;
	m_nAnimateCount = 0;
}

CCherryAnimateImageCtrl::~CCherryAnimateImageCtrl()
{
}

BEGIN_MESSAGE_MAP(CCherryAnimateImageCtrl, CCherryWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CCherryAnimateImageCtrl::OnDrawCherry(CCherryMemDC *pDC)
{
	if (m_imageList.GetCount() == 0)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	Graphics graphics(pDC->GetSafeHdc());

	m_imageList[m_nImage]->DrawImage(&graphics, clientRect);
}

CHERRY_RET CCherryAnimateImageCtrl::Create(
	CCherryImageList &imageList, 
	UINT nInterval, 
	UINT nAnimateNum,
	DWORD dwStyle, 
	const CCherryRect &cherryRect,
	CWnd *pParentWnd, 
	UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (!CCherryWnd::Create(NULL, NULL, dwStyle, cherryRect, pParentWnd, nID))
			throw CCherryException::ERROR_ANIMATEIMAGECTRL_CREATE_FAIL;
		
		SetImageList(imageList);
		StartAnimate(nInterval, nAnimateNum);
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

void CCherryAnimateImageCtrl::SetImageList(CCherryImageList &imageList)
{
	// Copy
	m_imageList.SetImageList(imageList);

	if (!m_imageList.IsEmpty())
	{
		UINT nRawWidth = m_imageList[0]->GetWidth();
		UINT nRawHeight = m_imageList[0]->GetHeight();

		CRect windowRect;
		GetWindowRect(&windowRect);
		GetParent()->ScreenToClient(&windowRect);

		UINT nWidth, nHeight;

		if (windowRect.Width() > 0)
			nWidth = windowRect.Width();
		else
			nWidth = nRawWidth;

		if (windowRect.Height() > 0)
			nHeight = windowRect.Height();
		else
			nHeight = nRawHeight;

		windowRect.right = windowRect.left + nWidth;
		windowRect.bottom = windowRect.top + nHeight;

		MoveWindow(windowRect);
	}
}

CHERRY_RET CCherryAnimateImageCtrl::ShowImage(INT_PTR nImage)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (m_imageList.GetCount() <= nImage)
			throw CCherryException::ERROR_ANIMATEIMAGECTRL_IMAGE_NOT_FOUND;

		m_nImage = nImage;

		Invalidate(FALSE);

	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

void CCherryAnimateImageCtrl::SetAnimateCount(UINT nAnimateNum)
{
	m_nAnimateNum = nAnimateNum;

	PauseAnimate();
	StartAnimate(m_nInterval, nAnimateNum);
}

void CCherryAnimateImageCtrl::SetAnimateInterval(UINT nInterval)
{
	m_nInterval = nInterval;

	PauseAnimate();
	StartAnimate(nInterval);
}

void CCherryAnimateImageCtrl::StartAnimate(UINT nInterval, UINT nAnimateNum)
{
	m_nImage = 0;
	m_nInterval = nInterval;
	m_nAnimateNum = nAnimateNum;
	
	StopAnimate();

	m_nAnimateTimerRet = SetTimer(ID_TIMER_ANIMATE, nInterval, NULL);
}

void CCherryAnimateImageCtrl::PauseAnimate()
{
	if (0 != m_nAnimateTimerRet)
		KillTimer(m_nAnimateTimerRet);
}

void CCherryAnimateImageCtrl::StopAnimate()
{
	m_nAnimateCount = 0;

	if (0 != m_nAnimateTimerRet)
		KillTimer(m_nAnimateTimerRet);

	ShowImage(0);
}

void CCherryAnimateImageCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0)
	{
		ShowImage(m_nImage);

		if (m_imageList.GetCount() <= ++m_nImage)
		{
			m_nImage = 0;

			if (m_nAnimateNum > 0)
			{
				if (m_nAnimateNum <= ++m_nAnimateCount)
				{
					StopAnimate();
				}
			}
		}	
	}

	CWnd::OnTimer(nIDEvent);
}
