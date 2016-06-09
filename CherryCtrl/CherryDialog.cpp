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
	m_bNcActive = TRUE;

	m_hNcRgn = NULL;
	m_hNcEdgeRgn[0] = NULL;
	m_hNcEdgeRgn[1] = NULL;
	m_hNcEdgeRgn[2] = NULL;
	m_hNcEdgeRgn[3] = NULL;
}

CCherryDialog::CCherryDialog(UINT nIDTemplate, CWnd* pParent)
: CDialogEx(nIDTemplate, pParent)
{
	m_bNcActive = TRUE;

	m_hNcRgn = NULL;
	m_hNcEdgeRgn[0] = NULL;
	m_hNcEdgeRgn[1] = NULL;
	m_hNcEdgeRgn[2] = NULL;
	m_hNcEdgeRgn[3] = NULL;
}

CCherryDialog::~CCherryDialog()
{
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

	CRect windowRect;
	GetWindowRect(&windowRect);

	CRect clientRect;
	GetClientRect(&clientRect);

	CRect innerClientRect(clientRect);

	CCherryMemDC memDC(&dc, innerClientRect);
	ClientToScreen(&innerClientRect);

	innerClientRect.OffsetRect(-windowRect.left, -windowRect.top);
	windowRect.OffsetRect(-windowRect.left, -windowRect.top);

	// Client 이미지 그리기
	Graphics graphics(memDC.GetSafeHdc());
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;
	CCherryImage *pClientImage = NULL;
	
	if (TRUE == m_bNcActive)
		pClientImage = m_ncImage[WINDOW_ACTIVATION_ACTIVE].ExtractStretchImage(windowRect, innerClientRect, 100, cherryRet); // Active
	else
		pClientImage = m_ncImage[WINDOW_ACTIVATION_INACTIVE].ExtractStretchImage(windowRect, innerClientRect, 100, cherryRet); // Inactive

	if (NULL != pClientImage && CCherryException::ERROR_CHERRY_SUCCESS == cherryRet)
	{
		pClientImage->DrawImage(&graphics, clientRect);
		delete pClientImage;
		pClientImage = NULL;
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

		if (pChildWnd->IsWindowVisible()) 
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

void CCherryDialog::SetClientImage(CCherryImage clientImage)
{
}

void CCherryDialog::SetClientImage(Bitmap clientBitmap)
{
	m_clientImage[WINDOW_ACTIVATION_ACTIVE].LoadImage(&clientBitmap);
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		현재 프로세스의 실행 파일 위치를 반환
/// \author		ogoons
/// \date		2015-01-20
/// \param		
/// \return		CString
/// \remark		2016-06-08 ogoons: NC영역 스킨 기능 추가 완료
///				다만 Client 영역 draw 대하여 Move/Resize/Active/Inactive에 대한 윈도우 상태 변경시 깜빡이는 현상이 있다.
///				이미지 처리 방식에 따른 퍼포먼스 문제이나 추후에 성능 개선을 위한 코드 개선이 필요하다.
/// \section	
///
///////////////////////////////////////////////////////////////////////////

CHERRY_RET CCherryDialog::SetNcImage(LPCTSTR lpszActiveImagePath, LPCTSTR lpszInactiveImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (NULL == lpszActiveImagePath)
			throw CCherryException::ERROR_IMAGE_LOAD_FAIL;

		if (CCherryException::ERROR_CHERRY_SUCCESS != (cherryRet = m_ncImage[WINDOW_ACTIVATION_ACTIVE].LoadImage(lpszActiveImagePath, TRUE)))
			throw cherryRet;

		m_ncImage[WINDOW_ACTIVATION_INACTIVE].LoadImage(lpszInactiveImagePath, TRUE); // Inactive 이미지는 없어도 동작할 수 있게 Exception skip

		UpdateNcRegion();
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
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

		CCherryImage *pActiveImage = rawImage.ExtractImage(CRect(0, 0, rawImage.GetWidth() / 2, rawImage.GetHeight()));
		m_ncImage[WINDOW_ACTIVATION_ACTIVE].LoadImage(pActiveImage, TRUE);

		/*
		// 2016-06-09 ogoons: 
		// 퍼포먼스 증가를 위한 리팩토링 코드 (테스트 중)
		CRect windowRect;
		GetWindowRect(&windowRect);

		CRect innerClientRect;
		GetClientRect(&innerClientRect);
		ClientToScreen(&innerClientRect);

		innerClientRect.OffsetRect(-windowRect.left, -windowRect.top);
		innerClientRect.right = (pActiveImage->GetWidth() / 2) - innerClientRect.left;
		innerClientRect.bottom = pActiveImage->GetHeight() - innerClientRect.left;
		
		Bitmap *pActiveClientBitmap = pActiveImage->ExtractBitmap(innerClientRect);
		m_clientImage_.LoadImage(pActiveClientBitmap, TRUE);
		delete pActiveClientBitmap;
		pActiveClientBitmap = NULL;
		*/

		delete pActiveImage;
		pActiveImage = NULL;

		CCherryImage *pInactiveImage = rawImage.ExtractImage(CRect(rawImage.GetWidth() / 2, 0, rawImage.GetWidth(), rawImage.GetHeight()));
		m_ncImage[WINDOW_ACTIVATION_INACTIVE].LoadImage(pInactiveImage, TRUE);
		delete pInactiveImage;
		pInactiveImage = NULL;
	}
	catch (CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
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

void CCherryDialog::OnNcPaint()
{
	if (!(GetStyle() & WS_CAPTION))
		return;

	if (FALSE == m_ncImage[WINDOW_ACTIVATION_ACTIVE].IsLoadedImage())
		return;

	CWindowDC dc(this);
	dc.SetBkMode(TRANSPARENT);

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
	dc.ExcludeClipRect(innerClientRect);

	CCherryMemDC memDC(&dc, CRect(0, 0, windowRect.Width(), windowRect.Height()));
	Graphics graphics(memDC.GetSafeHdc());

	if (TRUE == m_bNcActive)
		m_ncImage[WINDOW_ACTIVATION_ACTIVE].DrawStretchImage3x3(&graphics, windowRect);
	else
		m_ncImage[WINDOW_ACTIVATION_INACTIVE].DrawStretchImage3x3(&graphics, windowRect);

	// 그리기 메시지에 대해서는 CDialogEx::OnNcPaint()을(를) 호출하지 마십시오.
}

BOOL CCherryDialog::OnNcActivate(BOOL bActive)
{
	if (FALSE == m_ncImage[WINDOW_ACTIVATION_ACTIVE].IsLoadedImage())
		return CDialogEx::OnNcActivate(bActive);

	m_bNcActive = bActive;

	UpdateNcRegion();
	OnNcPaint();
	Invalidate(FALSE);

	return TRUE;
}

LRESULT CCherryDialog::OnNcHitTest(CPoint point)
{
	OnNcPaint();

	return CDialogEx::OnNcHitTest(point);
}


void CCherryDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	OnNcPaint();

	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


void CCherryDialog::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	OnNcPaint();

	CDialogEx::OnNcLButtonUp(nHitTest, point);
}


void CCherryDialog::OnNcMouseHover(UINT nFlags, CPoint point)
{
	OnNcPaint();

	CDialogEx::OnNcMouseHover(nFlags, point);
}


void CCherryDialog::OnNcMouseLeave()
{
	OnNcPaint();

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

	UpdateNcRegion();
	OnNcPaint();
	Invalidate(FALSE);
}

void CCherryDialog::UpdateNcRegion()
{
	if (FALSE == m_ncImage[WINDOW_ACTIVATION_ACTIVE].IsLoadedImage())
		return;

	UINT nNcWidth = m_ncImage[WINDOW_ACTIVATION_ACTIVE].GetWidth();
	UINT nNcHeight = m_ncImage[WINDOW_ACTIVATION_ACTIVE].GetHeight();

	if (NULL == m_hNcEdgeRgn[0])
	{
		// Left, Top
		CCherryImage *pNcEdgeImage = m_ncImage[WINDOW_ACTIVATION_ACTIVE].ExtractImage(CRect(0, 0, nNcWidth / 3, nNcHeight / 3));
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
		CCherryImage *pNcEdgeImage = m_ncImage[WINDOW_ACTIVATION_ACTIVE].ExtractImage(CRect((nNcWidth / 3) * 2, 0, nNcWidth, nNcHeight / 3));
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
		CCherryImage *pNcEdgeImage = m_ncImage[WINDOW_ACTIVATION_ACTIVE].ExtractImage(CRect(0, (nNcHeight / 3) * 2, nNcWidth / 3, nNcHeight));
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
		CCherryImage *pNcEdgeImage = m_ncImage[WINDOW_ACTIVATION_ACTIVE].ExtractImage(CRect((nNcWidth / 3) * 2, (nNcHeight / 3) * 2, nNcWidth, nNcHeight));
		m_hNcEdgeRgn[3] = pNcEdgeImage->GetHRGN(CCherryImage::RGN_TYPE_INVISIBLE_THRESHOLD, 160);

		if (NULL != pNcEdgeImage)
		{
			delete pNcEdgeImage;
			pNcEdgeImage = NULL;ㅗ
		}
	}

	if (NULL != m_hNcRgn)
	{
		DeleteObject(m_hNcRgn);
		m_hNcRgn = NULL;
	}

	CRect windowRect;
	GetWindowRect(&windowRect);

	m_hNcRgn = CreateRectRgn(0, 0, windowRect.Width(), windowRect.Height());

	// Left, Top merge
	CombineRgn(m_hNcRgn, m_hNcRgn, m_hNcEdgeRgn[0], RGN_XOR);

	// Right, Top merge
	int nOffsetX = windowRect.Width() - (nNcWidth / 3);
	int nOffsetY = windowRect.Height() - (nNcHeight / 3);
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

	if (TRUE == IsZoomed()) 
	{
		// 구현 필요
	}

	SetWindowRgn(m_hNcRgn, TRUE);
}