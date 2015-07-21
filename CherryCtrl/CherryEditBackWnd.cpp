// CherryEditBackWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryEditBackWnd.h"

// CCherryEditBackWnd

IMPLEMENT_DYNAMIC(CCherryEditBackWnd, CCherryWnd)

CCherryEditBackWnd::CCherryEditBackWnd()
{
}

CCherryEditBackWnd::~CCherryEditBackWnd()
{
}

BEGIN_MESSAGE_MAP(CCherryEditBackWnd, CCherryWnd)
	//ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CCherryEditBackWnd 메시지 처리기입니다.

CHERRY_RET CCherryEditBackWnd::Create(LPCTSTR lpszBackImagePath, DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if ((cherryRet = SetImage(lpszBackImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		CRect backWndRect(rect);

		if ((rect.right - rect.left) <= 0)
			backWndRect.right = rect.left + m_backImage.GetWidth();

		if ((rect.bottom - rect.top) <= 0)
			backWndRect.bottom = rect.top + m_backImage.GetHeight();

		if (!CCherryWnd::Create(NULL, NULL, dwStyle, backWndRect, pParentWnd, nID + 1000))
			throw CCherryException::ERROR_EDITBACKWND_CREATE_FAIL;
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryEditBackWnd::SetImage(LPCTSTR lpszImagePath)
{
	return m_backImage.LoadImage(lpszImagePath);
}

void CCherryEditBackWnd::OnDrawCherry(CCherryMemDC *pDC)
{
	if (FALSE == m_backImage.IsLoadedImage())
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	Graphics graphics(pDC->GetSafeHdc());

	m_backImage.DrawImage(&graphics, clientRect);
}

//HBRUSH CCherryEditBackWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	//switch (nCtlColor)
//	//{
//	//case CTLCOLOR_EDIT:
//
//	//	//if (pWnd->GetDlgCtrlID() == this->GetDlgCtrlID() + 1000)
//	//	//{
//	//		pDC->SetTextColor(RGB(255, 255, 255));
//	//		pDC->SetBkMode(TRANSPARENT);
//	//		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
//	//	//}
//	//	break;
//	//}
//
//	return hbr;
//}
