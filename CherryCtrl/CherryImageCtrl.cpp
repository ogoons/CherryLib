// CherryImageCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryImageCtrl.h"


// CCherryImageCtrl

IMPLEMENT_DYNAMIC(CCherryImageCtrl, CCherryWnd)

CCherryImageCtrl::CCherryImageCtrl()
{
}

CCherryImageCtrl::~CCherryImageCtrl()
{
}


BEGIN_MESSAGE_MAP(CCherryImageCtrl, CCherryWnd)
END_MESSAGE_MAP()

CHERRY_RET CCherryImageCtrl::Create(LPCTSTR lpszImagePath, DWORD dwStyle, const RECT& rect, CWnd *pParentWnd, UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;
	
	try
	{
		if (!CCherryWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID))
			throw CCherryException::ERROR_IMAGECTRL_CREATE_FAIL;

		UINT nWidth, nHeight;

		if (rect.right == 0)
			nWidth = 0;
		else
			nWidth = rect.right - rect.left;

		if (rect.bottom == 0)
			nHeight = 0;
		else
			nHeight = rect.bottom - rect.top;

		cherryRet = SetImage(lpszImagePath, nWidth, nHeight);
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryImageCtrl::SetImage(LPCTSTR lpszImagePath, int nWidth, int nHeight)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if ((cherryRet = m_image.LoadImage(lpszImagePath, TRUE)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		// 값이 없다면 원본 이미지 사이즈를 사용한다.
		if (nWidth <= 0)
			nWidth = m_image.GetWidth();

		if (nHeight <= 0)
			nHeight = m_image.GetHeight();

		CRect windowRect;

		GetWindowRect(&windowRect);
		GetParent()->ScreenToClient(&windowRect);
		
		windowRect.right = windowRect.left + nWidth;
		windowRect.bottom = windowRect.top + nHeight;

		MoveWindow(windowRect);
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	Invalidate(FALSE);

	return cherryRet;
}

void CCherryImageCtrl::OnDrawCherry(CCherryMemDC *pDC)
{
	if (FALSE == m_image.IsLoadedImage())
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	Graphics graphics(pDC->GetSafeHdc());

	if ((UINT)clientRect.Width() > m_image.GetWidth() &&
		(UINT)clientRect.Height() > m_image.GetHeight())
		m_image.DrawStretchImage3x3(&graphics, clientRect);
	else
		m_image.DrawImage(&graphics, clientRect);

}