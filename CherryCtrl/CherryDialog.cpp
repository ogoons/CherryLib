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

using namespace Gdiplus;

// CCherryDialog

IMPLEMENT_DYNAMIC(CCherryDialog, CDialogEx)

CCherryDialog::CCherryDialog()
{
	m_backImageType = BACK_IMAGE_NONE;
	m_bNcActive = TRUE;

	m_hNcRgn = NULL;
	m_hNcEdgeRgn[0] = NULL;
	m_hNcEdgeRgn[1] = NULL;
	m_hNcEdgeRgn[2] = NULL;
	m_hNcEdgeRgn[3] = NULL;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m_pNcActive9PatchImage[i][j] = NULL;

	m_pNcImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
	m_pNcImage[WINDOW_ACTIVATION_INACTIVE] = NULL;

	m_pClientImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
	m_pClientImage[WINDOW_ACTIVATION_INACTIVE] = NULL;
}

CCherryDialog::CCherryDialog(UINT nIDTemplate, CWnd* pParent)
	: CDialogEx(nIDTemplate, pParent)
{
	m_backImageType = BACK_IMAGE_NONE;
	m_bNcActive = TRUE;

	m_hNcRgn = NULL;
	m_hNcEdgeRgn[0] = NULL;
	m_hNcEdgeRgn[1] = NULL;
	m_hNcEdgeRgn[2] = NULL;
	m_hNcEdgeRgn[3] = NULL;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m_pNcActive9PatchImage[i][j] = NULL;

	m_pNcImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
	m_pNcImage[WINDOW_ACTIVATION_INACTIVE] = NULL;

	m_pClientImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
	m_pClientImage[WINDOW_ACTIVATION_INACTIVE] = NULL;
}

CCherryDialog::~CCherryDialog()
{
	if (NULL != m_pClientImage[WINDOW_ACTIVATION_ACTIVE])
	{
		delete m_pClientImage[WINDOW_ACTIVATION_ACTIVE];
		m_pClientImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
	}

	if (NULL != m_pClientImage[WINDOW_ACTIVATION_INACTIVE])
	{
		delete m_pClientImage[WINDOW_ACTIVATION_INACTIVE];
		m_pClientImage[WINDOW_ACTIVATION_INACTIVE] = NULL;
	}

	if (NULL != m_pNcImage[WINDOW_ACTIVATION_ACTIVE])
	{
		delete m_pNcImage[WINDOW_ACTIVATION_ACTIVE];
		m_pNcImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
	}

	if (NULL != m_pNcImage[WINDOW_ACTIVATION_INACTIVE])
	{
		delete m_pNcImage[WINDOW_ACTIVATION_INACTIVE];
		m_pNcImage[WINDOW_ACTIVATION_INACTIVE] = NULL;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (NULL != m_pNcActive9PatchImage[i][j])
			{
				delete m_pNcActive9PatchImage[i][j];
				m_pNcActive9PatchImage[i][j] = NULL;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (NULL != m_hNcEdgeRgn[i])
		{
			DeleteObject(m_hNcEdgeRgn[i]);
			m_hNcEdgeRgn[i] = NULL;
		}
	}

	if (NULL != m_hNcRgn)
	{
		DeleteObject(m_hNcRgn);
		m_hNcRgn = NULL;
	}
}

BEGIN_MESSAGE_MAP(CCherryDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCACTIVATE()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEHOVER()
	ON_WM_NCMOUSELEAVE()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_SIZING()
	ON_WM_SIZING()
	ON_WM_ACTIVATE()
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

	// Client 이미지 그리기
	Graphics graphics(memDC.GetSafeHdc());

	switch (m_backImageType)
	{
	case BACK_IMAGE_NONE:
		break;
	case BACK_IMAGE_CLIENT:
		if (NULL != m_pClientImage[WINDOW_ACTIVATION_ACTIVE])
			m_pClientImage[WINDOW_ACTIVATION_ACTIVE]->DrawImage(&graphics, clientRect);
		break;
	case BACK_IMAGE_NON_CLIENT:
		if (TRUE == m_bNcActive)
		{
			if (NULL != m_pClientImage[WINDOW_ACTIVATION_ACTIVE])
				m_pClientImage[WINDOW_ACTIVATION_ACTIVE]->DrawImage(&graphics, clientRect);
		}
		else
		{
			if (NULL != m_pClientImage[WINDOW_ACTIVATION_INACTIVE])
				m_pClientImage[WINDOW_ACTIVATION_INACTIVE]->DrawImage(&graphics, clientRect);
		}
		break;
	default:
		break;
	}

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

		if (TRUE == pChildWnd->IsWindowVisible()) 
		{			
			CRect windowRect;
			pChildWnd->GetWindowRect(windowRect);
			ScreenToClient(windowRect);				
			dc.ExcludeClipRect(windowRect); // 그리기 제외
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

CHERRY_RET CCherryDialog::SetClientImage(LPCTSTR lpszImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (NULL == lpszImagePath)
			throw CCherryException::ERROR_DIALOG_CLIENT_ACTIVE_WINDOW_IMAGE_NOT_FOUND;

		RemoveBackImage();

		m_pClientImage[WINDOW_ACTIVATION_ACTIVE] = new CCherryImage();
		
		if (CCherryException::ERROR_CHERRY_SUCCESS != (cherryRet = m_pClientImage[WINDOW_ACTIVATION_ACTIVE]->LoadImage(lpszImagePath)))
			throw cherryRet;

		m_backImageType = BACK_IMAGE_CLIENT;

		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}
	catch (CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryDialog::Make9PatchNcActiveImage()
{
	if (NULL == m_pNcImage[WINDOW_ACTIVATION_ACTIVE])
		return CCherryException::ERROR_DIALOG_NON_CLIENT_ACTIVE_WINDOW_IMAGE_NOT_FOUND;

	UINT nDivWidth = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetWidth() / 3;
	UINT nDivHeight = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetHeight() / 3;

	// Active image
	////////////////////////////////////////////////////////////////
	// 1 row
	m_pNcActive9PatchImage[0][0] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(0, 0, nDivWidth, nDivHeight));

	if (NULL == m_pNcActive9PatchImage[0][1])
		m_pNcActive9PatchImage[0][1] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(nDivWidth, 0, nDivWidth * 2, nDivHeight));

	if (NULL == m_pNcActive9PatchImage[0][2])
		m_pNcActive9PatchImage[0][2] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(nDivWidth * 2, 0, m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetWidth(), nDivHeight));

	////////////////////////////////////////////////////////////////
	// 2 row
	if (NULL == m_pNcActive9PatchImage[1][0])
		m_pNcActive9PatchImage[1][0] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(0, nDivHeight, nDivWidth, nDivHeight * 2));

	if (NULL == m_pNcActive9PatchImage[1][1])
		m_pNcActive9PatchImage[1][1] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(nDivWidth, nDivHeight, nDivWidth * 2, nDivHeight * 2));

	if (NULL == m_pNcActive9PatchImage[1][2])
		m_pNcActive9PatchImage[1][2] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(nDivWidth * 2, nDivHeight, m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetWidth(), nDivHeight * 2));

	////////////////////////////////////////////////////////////////
	// 3 row
	if (NULL == m_pNcActive9PatchImage[2][0])
		m_pNcActive9PatchImage[2][0] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(0, nDivHeight * 2, nDivWidth, m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetHeight()));

	if (NULL == m_pNcActive9PatchImage[2][1])
		m_pNcActive9PatchImage[2][1] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(nDivWidth, nDivHeight * 2, nDivWidth * 2, m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetHeight()));

	if (NULL == m_pNcActive9PatchImage[2][2])
		m_pNcActive9PatchImage[2][2] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(nDivWidth * 2, nDivHeight * 2, m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetWidth(), m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetHeight()));

	return CCherryException::ERROR_CHERRY_SUCCESS;
}

CHERRY_RET CCherryDialog::SetNcImage(LPCTSTR lpszActiveImagePath, LPCTSTR lpszInactiveImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (NULL == lpszActiveImagePath)
			throw CCherryException::ERROR_IMAGE_LOAD_FAIL;

		RemoveBackImage();

		// Active image
		m_pNcImage[WINDOW_ACTIVATION_ACTIVE] = new CCherryImage();

		if (CCherryException::ERROR_CHERRY_SUCCESS != (cherryRet = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->LoadImage(lpszActiveImagePath)))
			throw cherryRet;

		if (CCherryException::ERROR_CHERRY_SUCCESS != (cherryRet = Make9PatchNcActiveImage()))
			throw cherryRet;

		// Inactive image
		m_pNcImage[WINDOW_ACTIVATION_INACTIVE] = new CCherryImage();
		m_pNcImage[WINDOW_ACTIVATION_INACTIVE]->LoadImage(lpszInactiveImagePath); // Inactive 이미지는 없어도 동작할 수 있게 Exception skip

		m_backImageType = BACK_IMAGE_NON_CLIENT;

		RefreshNcImage();
		RefreshNcRegion();
		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;

		if (NULL != m_pNcImage[WINDOW_ACTIVATION_ACTIVE])
		{
			delete m_pNcImage[WINDOW_ACTIVATION_ACTIVE];
			m_pNcImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
		}
	}

	return cherryRet;
}

CHERRY_RET CCherryDialog::SetNcImage(LPCTSTR lpszActiveInactiveMergedImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		CCherryImage rawImage;
		if (CCherryException::ERROR_CHERRY_SUCCESS != (cherryRet = rawImage.LoadImage(lpszActiveInactiveMergedImagePath)))
			throw cherryRet;

		RemoveBackImage();

		m_pNcImage[WINDOW_ACTIVATION_ACTIVE] = rawImage.ExtractImage(CRect(0, 0, rawImage.GetWidth() / 2, rawImage.GetHeight()));

		if (CCherryException::ERROR_CHERRY_SUCCESS != (cherryRet = Make9PatchNcActiveImage()))
			throw cherryRet;

		// Inactive image
		m_pNcImage[WINDOW_ACTIVATION_INACTIVE] = rawImage.ExtractImage(CRect(rawImage.GetWidth() / 2, 0, rawImage.GetWidth(), rawImage.GetHeight()));

		m_backImageType = BACK_IMAGE_NON_CLIENT;

		RefreshNcImage();
		RefreshNcRegion();
		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}
	catch (CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

void CCherryDialog::RefreshNcImage()
{
	CRect windowRect;
	GetWindowRect(&windowRect);

	CRect innerClientRect;
	GetClientRect(&innerClientRect);
	ClientToScreen(&innerClientRect);

	innerClientRect.OffsetRect(-windowRect.left, -windowRect.top);
	windowRect.OffsetRect(-windowRect.left, -windowRect.top);

	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	// Client
	if (NULL != m_pClientImage[WINDOW_ACTIVATION_ACTIVE])
	{
		delete m_pClientImage[WINDOW_ACTIVATION_ACTIVE];
		m_pClientImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
	}

	if (NULL != m_pNcImage[WINDOW_ACTIVATION_ACTIVE])
		m_pClientImage[WINDOW_ACTIVATION_ACTIVE] = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->Extract9PatchImage(windowRect, innerClientRect, 100, cherryRet); // Active

	if (NULL != m_pClientImage[WINDOW_ACTIVATION_INACTIVE])
	{
		delete m_pClientImage[WINDOW_ACTIVATION_INACTIVE];
		m_pClientImage[WINDOW_ACTIVATION_INACTIVE] = NULL;
	}

	if (NULL != m_pNcImage[WINDOW_ACTIVATION_INACTIVE])
		m_pClientImage[WINDOW_ACTIVATION_INACTIVE] = m_pNcImage[WINDOW_ACTIVATION_INACTIVE]->Extract9PatchImage(windowRect, innerClientRect, 100, cherryRet); // Inactive
}

void CCherryDialog::RemoveBackImage() // 수정 필요
{
	if (m_backImageType == BACK_IMAGE_NONE)
		return;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (NULL != m_pNcActive9PatchImage[i][j])
			{
				delete m_pNcActive9PatchImage[i][j];
				m_pNcActive9PatchImage[i][j] = NULL;
			}
		}
	}

	if (NULL != m_pNcImage[WINDOW_ACTIVATION_ACTIVE])
	{
		delete m_pNcImage[WINDOW_ACTIVATION_ACTIVE];
		m_pNcImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
	}

	if (NULL != m_pNcImage[WINDOW_ACTIVATION_INACTIVE])
	{
		delete m_pNcImage[WINDOW_ACTIVATION_INACTIVE];
		m_pNcImage[WINDOW_ACTIVATION_INACTIVE] = NULL;
	}

	if (NULL != m_pClientImage[WINDOW_ACTIVATION_ACTIVE])
	{
		delete m_pClientImage[WINDOW_ACTIVATION_ACTIVE];
		m_pClientImage[WINDOW_ACTIVATION_ACTIVE] = NULL;
	}

	if (NULL != m_pClientImage[WINDOW_ACTIVATION_INACTIVE])
	{
		delete m_pClientImage[WINDOW_ACTIVATION_INACTIVE];
		m_pClientImage[WINDOW_ACTIVATION_INACTIVE] = NULL;
	}

	m_backImageType = BACK_IMAGE_NONE;

	InitRgn();

	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}

BOOL CCherryDialog::InitRgn()
{
	if (NULL != m_hNcRgn)
	{
		DeleteObject(m_hNcRgn);
		m_hNcRgn = NULL;
	}

	CRect windowRect;
	GetWindowRect(&windowRect);

	m_hNcRgn = CreateRectRgn(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);

	return (0 != SetWindowRgn(m_hNcRgn, TRUE) ? TRUE : FALSE);
}

BOOL CCherryDialog::OnEraseBkgnd(CDC* pDC)
{
	// 배경 깜박임 처리 (배경을 그리지 않음)
	return FALSE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

void CCherryDialog::OnDrawSystemButton(CCherryMemDC *pDC)
{
}

void CCherryDialog::OnNcPaint()
{
	if (!(GetStyle() & WS_CAPTION))
		return;

	if (NULL == m_pNcImage[WINDOW_ACTIVATION_ACTIVE])
		return;

	CWindowDC windowDC(this);
	windowDC.SetBkMode(TRANSPARENT);

	CRect windowRect;
	GetWindowRect(&windowRect);

	CRect clientRect;
	GetClientRect(&clientRect);

	CRect innerClientRect;
	GetClientRect(&innerClientRect);
	ClientToScreen(&innerClientRect);

	innerClientRect.OffsetRect(-windowRect.left, -windowRect.top);
	windowRect.OffsetRect(-windowRect.left, -windowRect.top);

	// Client 영역 Clipping (그리지 않기)
	windowDC.ExcludeClipRect(innerClientRect);

	CCherryMemDC memDC(&windowDC, CRect(0, 0, windowRect.Width(), windowRect.Height()));
	Graphics graphics(memDC);
	graphics.SetSmoothingMode(SmoothingModeDefault);
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeHalf);

	if (TRUE == m_bNcActive)
	{
		if (NULL != m_pNcActive9PatchImage[0][0] && NULL != m_pNcActive9PatchImage[0][1] && NULL != m_pNcActive9PatchImage[0][2] &&
			NULL != m_pNcActive9PatchImage[1][0] && NULL != m_pNcActive9PatchImage[1][1] && NULL != m_pNcActive9PatchImage[1][2] &&
			NULL != m_pNcActive9PatchImage[2][0] && NULL != m_pNcActive9PatchImage[2][1] && NULL != m_pNcActive9PatchImage[2][2])
		{
			// 1행
			m_pNcActive9PatchImage[0][0]->DrawImage(&graphics, 0, 0, m_pNcActive9PatchImage[0][0]->GetWidth(), m_pNcActive9PatchImage[0][0]->GetHeight());
			m_pNcActive9PatchImage[0][1]->DrawImage(&graphics, m_pNcActive9PatchImage[0][0]->GetWidth(), 0, windowRect.Width() - m_pNcActive9PatchImage[0][2]->GetWidth(), m_pNcActive9PatchImage[0][1]->GetHeight());
			m_pNcActive9PatchImage[0][2]->DrawImage(&graphics, windowRect.Width() - m_pNcActive9PatchImage[0][2]->GetWidth(), 0, windowRect.Width(), m_pNcActive9PatchImage[0][2]->GetHeight());

			// 2행
			m_pNcActive9PatchImage[1][0]->DrawImage(&graphics, 0, m_pNcActive9PatchImage[0][0]->GetHeight(), m_pNcActive9PatchImage[1][0]->GetWidth(), windowRect.Height() - m_pNcActive9PatchImage[2][0]->GetHeight());
			m_pNcActive9PatchImage[1][1]->DrawImage(&graphics, m_pNcActive9PatchImage[1][0]->GetWidth(), m_pNcActive9PatchImage[0][1]->GetHeight(), windowRect.Width() - m_pNcActive9PatchImage[1][2]->GetWidth(), windowRect.Height() - m_pNcActive9PatchImage[2][1]->GetHeight());
			m_pNcActive9PatchImage[1][2]->DrawImage(&graphics, windowRect.Width() - m_pNcActive9PatchImage[1][2]->GetWidth(), m_pNcActive9PatchImage[0][2]->GetHeight(), windowRect.Width(), windowRect.Height() - m_pNcActive9PatchImage[2][2]->GetHeight());

			// 3행
			m_pNcActive9PatchImage[2][0]->DrawImage(&graphics, 0, windowRect.Height() - m_pNcActive9PatchImage[2][0]->GetHeight(), m_pNcActive9PatchImage[2][0]->GetWidth(), windowRect.Height());
			m_pNcActive9PatchImage[2][1]->DrawImage(&graphics, m_pNcActive9PatchImage[2][0]->GetWidth(), windowRect.Height() - m_pNcActive9PatchImage[2][1]->GetHeight(), windowRect.Width() - m_pNcActive9PatchImage[2][2]->GetWidth(), windowRect.Height());
			m_pNcActive9PatchImage[2][2]->DrawImage(&graphics, windowRect.Width() - m_pNcActive9PatchImage[2][2]->GetWidth(), windowRect.Height() - m_pNcActive9PatchImage[2][2]->GetHeight(), windowRect.Width(), windowRect.Height());
		}
	}
	else
	{
		if (NULL != m_pNcImage[WINDOW_ACTIVATION_INACTIVE])
			m_pNcImage[WINDOW_ACTIVATION_INACTIVE]->Draw9PatchImage(&graphics, windowRect);
	}
}

BOOL CCherryDialog::OnNcActivate(BOOL bActive)
{
	m_bNcActive = bActive;

	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

	return TRUE;
	//return CDialogEx::OnNcActivate(bActive);
}

LRESULT CCherryDialog::OnNcHitTest(CPoint point)
{
	//OnNcPaint();

	return CDialogEx::OnNcHitTest(point);
}


void CCherryDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	//OnNcPaint();

	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


void CCherryDialog::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	//OnNcPaint();

	CDialogEx::OnNcLButtonUp(nHitTest, point);
}


void CCherryDialog::OnNcMouseHover(UINT nFlags, CPoint point)
{
	//OnNcPaint();

	CDialogEx::OnNcMouseHover(nFlags, point);
}


void CCherryDialog::OnNcMouseLeave()
{
	//OnNcPaint();

	CDialogEx::OnNcMouseLeave();
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

		UINT nMinRawWidth = minButtonImage.GetWidth() / STATUS_MAX_COUNT;
		UINT nMinRawHeight = minButtonImage.GetHeight();

		UINT nMaxOrgWidth = maxButtonImage.GetWidth() / STATUS_MAX_COUNT;
		UINT nMaxOrgHeight = maxButtonImage.GetHeight();

		UINT nCloseOrgWidth = closeButtonImage.GetWidth() / STATUS_MAX_COUNT;
		UINT nCloseOrgHeight = closeButtonImage.GetHeight();

		for (UINT i = STATUS_NORMAL; i < STATUS_MAX_COUNT; i++) // 각 상태 이미지 잘라서 붙여넣기
		{
			// Min
			if ((cherryRet = m_sysMinButtonImage[i].LoadImage(minButtonImage.GetBitmap()->Clone(Rect(nMinRawWidth * i, 0, nMinRawWidth, nMinRawHeight), PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
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

	RefreshNcImage();
	RefreshNcRegion();
	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}

void CCherryDialog::RefreshNcRegion()
{
	if (NULL == m_pNcImage[WINDOW_ACTIVATION_ACTIVE])
		return;

	CRect windowRect;
	GetWindowRect(&windowRect);

	// 최대화한 경우 전체화면을 꽉 채워 설정한다.
	if (TRUE == IsZoomed())
	{
		if (NULL != m_hNcRgn)
		{
			DeleteObject(m_hNcRgn);
			m_hNcRgn = NULL;
		}

		m_hNcRgn = CreateRectRgn(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
	}
	else
	{
		UINT nRawWidth = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetWidth();
		UINT nRawHeight = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetWidth();

		UINT nDivWidth = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetWidth() / 3;
		UINT nDivHeight = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->GetHeight() / 3;

		if (NULL == m_hNcEdgeRgn[0])
		{
			// Left, Top
			CCherryImage *pNcEdgeImage = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(0, 0, nDivWidth, nDivHeight));
			
			if (NULL != m_hNcEdgeRgn[0])
			{
				DeleteObject(m_hNcEdgeRgn[0]);
				m_hNcEdgeRgn[0] = NULL;
			}

			m_hNcEdgeRgn[0] = pNcEdgeImage->GetHRGN(CCherryImage::RGN_TYPE_INVISIBLE_THRESHOLD, 160);

			if (NULL != pNcEdgeImage)
			{
				delete pNcEdgeImage;
				pNcEdgeImage = NULL;
			}
		}

		if (NULL == m_hNcEdgeRgn[1])
		{
			// Right, Top
			CCherryImage *pNcEdgeImage = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(nDivWidth * 2, 0, nRawWidth, nDivHeight));
			
			if (NULL != m_hNcEdgeRgn[1])
			{
				DeleteObject(m_hNcEdgeRgn[1]);
				m_hNcEdgeRgn[1] = NULL;
			}
			
			m_hNcEdgeRgn[1] = pNcEdgeImage->GetHRGN(CCherryImage::RGN_TYPE_INVISIBLE_THRESHOLD, 160);

			if (NULL != pNcEdgeImage)
			{
				delete pNcEdgeImage;
				pNcEdgeImage = NULL;
			}
		}

		if (NULL == m_hNcEdgeRgn[2])
		{
			// Left, Bottom
			CCherryImage *pNcEdgeImage = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(0, nDivHeight * 2, nDivWidth, nRawHeight));
			
			if (NULL != m_hNcEdgeRgn[2])
			{
				DeleteObject(m_hNcEdgeRgn[2]);
				m_hNcEdgeRgn[2] = NULL;
			}
			
			m_hNcEdgeRgn[2] = pNcEdgeImage->GetHRGN(CCherryImage::RGN_TYPE_INVISIBLE_THRESHOLD, 160);

			if (NULL != pNcEdgeImage)
			{
				delete pNcEdgeImage;
				pNcEdgeImage = NULL;
			}
		}

		if (NULL == m_hNcEdgeRgn[3])
		{
			// Right, Bottom
			CCherryImage *pNcEdgeImage = m_pNcImage[WINDOW_ACTIVATION_ACTIVE]->ExtractImage(CRect(nDivWidth * 2, nDivHeight * 2, nRawWidth, nRawHeight));
			
			if (NULL != m_hNcEdgeRgn[3])
			{
				DeleteObject(m_hNcEdgeRgn[3]);
				m_hNcEdgeRgn[3] = NULL;
			}
			
			m_hNcEdgeRgn[3] = pNcEdgeImage->GetHRGN(CCherryImage::RGN_TYPE_INVISIBLE_THRESHOLD, 160);

			if (NULL != pNcEdgeImage)
			{
				delete pNcEdgeImage;
				pNcEdgeImage = NULL;
			}
		}

		if (NULL != m_hNcRgn)
		{
			DeleteObject(m_hNcRgn);
			m_hNcRgn = NULL;
		}

		m_hNcRgn = CreateRectRgn(0, 0, windowRect.Width(), windowRect.Height());

		// Left, Top merge
		CombineRgn(m_hNcRgn, m_hNcRgn, m_hNcEdgeRgn[0], RGN_XOR);

		// Right, Top merge
		int nOffsetX = windowRect.Width() - nDivWidth;
		int nOffsetY = windowRect.Height() - nDivHeight;
		OffsetRgn(m_hNcEdgeRgn[1], nOffsetX, 0);
		CombineRgn(m_hNcRgn, m_hNcRgn, m_hNcEdgeRgn[1], RGN_XOR);
		OffsetRgn(m_hNcEdgeRgn[1], -nOffsetX, 0);

		// Left, Bottom merge
		OffsetRgn(m_hNcEdgeRgn[2], 0, nOffsetY);
		CombineRgn(m_hNcRgn, m_hNcRgn, m_hNcEdgeRgn[2], RGN_XOR);
		OffsetRgn(m_hNcEdgeRgn[2], 0, -nOffsetY);

		// Right, Bottom merge
		OffsetRgn(m_hNcEdgeRgn[3], nOffsetX, nOffsetY);
		CombineRgn(m_hNcRgn, m_hNcRgn, m_hNcEdgeRgn[3], RGN_XOR);
		OffsetRgn(m_hNcEdgeRgn[3], -nOffsetX, -nOffsetY);
	}

	SetWindowRgn(m_hNcRgn, TRUE);
}