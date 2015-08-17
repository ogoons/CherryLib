#include "stdafx.h"
#include "CherryDialog.h"

#include "CherryButton.h"
#include "CherryCheckBox.h"
#include "CherryStatic.h"
#include "CherryLinkCtrl.h"
#include "CherryImageCtrl.h"
#include "CherryAnimateImageCtrl.h"
#include "CherryProgressCtrl.h"
#include "CherryEdit.h"
#include "CherryComboBox.h"
#include "CherrySliderCtrl.h"
#include "CherryTabCtrl.h"

// CCherryDialog

IMPLEMENT_DYNAMIC(CCherryDialog, CDialogEx)

CCherryDialog::CCherryDialog()
{
}

CCherryDialog::CCherryDialog(UINT nIDTemplate, CWnd* pParent)
: CDialogEx(nIDTemplate, pParent)
{

}

CCherryDialog::~CCherryDialog()
{
}

BEGIN_MESSAGE_MAP(CCherryDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCACTIVATE()
	ON_WM_NCDESTROY()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEHOVER()
	ON_WM_NCMOUSELEAVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CCherryDialog 메시지 처리기입니다.

BOOL CCherryDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 모든 CherryCtrl 들은 여기서 생성되도록 한다.
	if (!OnCreateCherry())
		return FALSE;
	
	return TRUE;
}

void CCherryDialog::OnPaint()
{
	CPaintDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);
	CCherryMemDC memDC(&dc, clientRect);

	// CherryDialog 용 OnPaint()
	OnDrawCherry(&memDC);

	CWnd *pChildWnd = GetWindow(GW_CHILD);

	while (pChildWnd)
	{
		if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryWnd)))
			((CCherryWnd *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryButton)))
			((CCherryButton *)pChildWnd)->SetBackground(&memDC);
		//else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryCheckBox)))	
			//((CCherryCheckBox *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryStatic)))	
			((CCherryStatic *)pChildWnd)->SetBackground(&memDC);
		//else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryLinkCtrl)))
			//((CCherryLinkCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryComboBox)))	
			((CCherryComboBox *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryEditBackWnd)))	
			((CCherryEditBackWnd *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryImageCtrl)))	
			((CCherryImageCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryAnimateImageCtrl)))	
			((CCherryAnimateImageCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryProgressCtrl)))
			((CCherryProgressCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherrySliderCtrl)))
			((CCherrySliderCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryTabCtrl)))
			((CCherryTabCtrl *)pChildWnd)->SetBackground(&memDC);

		if (pChildWnd->IsWindowVisible()) 
		{			
			CRect windowRect;
			pChildWnd->GetWindowRect(windowRect);
			ScreenToClient(windowRect);				
			dc.ExcludeClipRect(windowRect);
		}	

		pChildWnd = pChildWnd->GetNextWindow(GW_HWNDNEXT);
	}

	CDialogEx::OnPaint();
}

BOOL CCherryDialog::OnCreateCherry()
{
	DWORD dwStyle = GetStyle();

	if (WS_MAXIMIZEBOX & dwStyle)
	{

	}

	if (WS_MINIMIZEBOX & dwStyle)
	{

	}


	return TRUE;
}

BOOL CCherryDialog::OnEraseBkgnd(CDC* pDC)
{
	// 배경 깜박임 처리
	return FALSE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

void CCherryDialog::OnDrawSystemButton(CCherryMemDC *pDC)
{
	

}

BOOL CCherryDialog::OnNcActivate(BOOL bActive)
{


	return CDialogEx::OnNcActivate(bActive);
}

LRESULT CCherryDialog::OnNcHitTest(CPoint point)
{


	return CDialogEx::OnNcHitTest(point);
}


void CCherryDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{


	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


void CCherryDialog::OnNcLButtonUp(UINT nHitTest, CPoint point)
{


	CDialogEx::OnNcLButtonUp(nHitTest, point);
}


void CCherryDialog::OnNcMouseHover(UINT nFlags, CPoint point)
{


	CDialogEx::OnNcMouseHover(nFlags, point);
}


void CCherryDialog::OnNcMouseLeave()
{


	CDialogEx::OnNcMouseLeave();
}


void CCherryDialog::PreSubclassWindow()
{
	

	CDialogEx::PreSubclassWindow();
}

CHERRY_RET CCherryDialog::SetSystemButtonImage(LPCTSTR lpszMinButtonImagePath, LPCTSTR lpszMaxButtonImagePath, LPCTSTR lpszCloseButtonImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		// 원본 이미지 로드
		CCherryImage minButtonImage;
		if ((cherryRet = minButtonImage.LoadImage(lpszMinButtonImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		CCherryImage maxButtonImage;
		if ((cherryRet = maxButtonImage.LoadImage(lpszMaxButtonImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		CCherryImage closeButtonImage;
		if ((cherryRet = closeButtonImage.LoadImage(lpszCloseButtonImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		UINT nMinOrgWidth = minButtonImage.GetWidth() / STATUS_MAX_COUNT;
		UINT nMinOrgHeight = minButtonImage.GetHeight();

		UINT nMaxOrgWidth = maxButtonImage.GetWidth() / STATUS_MAX_COUNT;
		UINT nMaxOrgHeight = maxButtonImage.GetHeight();

		UINT nCloseOrgWidth = closeButtonImage.GetWidth() / STATUS_MAX_COUNT;
		UINT nCloseOrgHeight = closeButtonImage.GetHeight();

		for (UINT i = STATUS_NORMAL; i < STATUS_MAX_COUNT; i++) // 각 상태 이미지 잘라서 붙여넣기
		{
			// Min
			if ((cherryRet = m_sysMinButtonImage[i].LoadImage(minButtonImage.GetBitmap()->Clone(Rect(nMinOrgWidth * i, 0, nMinOrgWidth, nMinOrgHeight), PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;

			// Max
			if ((cherryRet = m_sysMaxButtonImage[i].LoadImage(maxButtonImage.GetBitmap()->Clone(Rect(nMaxOrgWidth * i, 0, nMaxOrgWidth, nMaxOrgHeight), PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;

			// Close
			if ((cherryRet = m_sysCloseButtonImage[i].LoadImage(closeButtonImage.GetBitmap()->Clone(Rect(nCloseOrgWidth * i, 0, nCloseOrgWidth, nCloseOrgHeight), PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}


void CCherryDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	Invalidate(FALSE);
}
