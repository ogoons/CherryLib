// CherryCheckBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryToggleButton.h"


// CCherryToggleButton

IMPLEMENT_DYNAMIC(CCherryToggleButton, CCherryButton)

CCherryToggleButton::CCherryToggleButton()
{
	m_toggleStatus = STATUS_OFF;
}

CCherryToggleButton::~CCherryToggleButton()
{
}

BEGIN_MESSAGE_MAP(CCherryToggleButton, CCherryButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CCherryToggleButton 메시지 처리기입니다.
CHERRY_RET CCherryToggleButton::Create(LPCTSTR lpszCaption, LPCTSTR lpszImagePath, DWORD dwCherryStyle, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	dwStyle |= BS_OWNERDRAW;	// 추가

	if (!CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID))
		return CCherryException::ERROR_TOGGLEBUTTON_CREATE_FAIL;

	// CherryToggleButton 전용 스타일 bit 플래그
	ModifyCherryStyle(0, dwCherryStyle);

	// 기본 수직 정렬
	GetNormalFont()->SetFontStyle(CCherryFont::STYLE_VCENTER);
	GetHoverFont()->SetFontStyle(CCherryFont::STYLE_VCENTER);
	GetDownFont()->SetFontStyle(CCherryFont::STYLE_VCENTER);
	GetDisableFont()->SetFontStyle(CCherryFont::STYLE_VCENTER);

	// 기본 비활성화 텍스트 속성
	COLORREF disableColor = GetSysColor(COLOR_GRAYTEXT);
	GetDisableFont()->SetFontColor(Color(GetRValue(disableColor), GetGValue(disableColor), GetBValue(disableColor)));

	//SetWindowText(lpszCaption);

	return SetImage(lpszImagePath);
}

CHERRY_RET CCherryToggleButton::SetImage(LPCTSTR lpszImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		// 원본 이미지 로드
		CCherryImage sourceImage;
		if ((cherryRet = sourceImage.LoadImage(lpszImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		UINT nOrgWidth = sourceImage.GetWidth() / STATUS_MAX_COUNT;
		UINT nOrgHeight = sourceImage.GetHeight() / 2;

		// 체크되지 않은 이미지 각 상태 이미지 잘라서 붙여넣기
		for (UINT i = STATUS_NORMAL; i < STATUS_MAX_COUNT; i++)
		{
			if ((cherryRet = m_images[i].LoadImage(sourceImage.GetBitmap()->Clone(Rect(nOrgWidth * i, 0, nOrgWidth, nOrgHeight), PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;
		}

		// 토글된 이미지 각 상태 이미지 잘라서 붙여넣기
		for (UINT j = STATUS_NORMAL; j < STATUS_MAX_COUNT; j++)
		{
			if ((cherryRet = m_toggleOnImages[j].LoadImage(sourceImage.GetBitmap()->Clone(Rect(nOrgWidth * j, nOrgHeight, nOrgWidth, nOrgHeight), PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;
		}

		if (GetCherryStyle() & STYLE_AUTORESIZE)
		{
			// CherryToggleButton::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
			ResizeWindow(nOrgWidth, nOrgHeight);
		}
		else
		{
			CRect windowRect;
			GetWindowRect(&windowRect);
			GetParent()->ScreenToClient(&windowRect);

			UINT nWidth, nHeight;

			if (windowRect.Width() > 0)
				nWidth = windowRect.Width();
			else
				nWidth = nOrgWidth;

			if (windowRect.Height() > 0)
				nHeight = windowRect.Height();
			else
				nHeight = nOrgHeight;

			if (nWidth > 0 && nHeight > 0)
			{
				windowRect.right = windowRect.left + nWidth;
				windowRect.bottom = windowRect.top + nHeight;
				MoveWindow(windowRect);
			}
			else
			{
				// nWidth || nHeight 어느 하나라도 0이면 Auto Resizing
				ResizeWindow(nOrgWidth, nOrgHeight);
			}
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return CCherryException::ERROR_CHERRY_SUCCESS;
}

void CCherryToggleButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CCherryMemDC memDC(pDC, clientRect);
	Graphics graphics(memDC.GetSafeHdc());

	graphics.SetInterpolationMode(InterpolationModeDefault);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	if (m_pBackMemDC)
		m_pBackMemDC->Draw(&memDC);

	CCherryImage *pCurrentImage = NULL;

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		// Down
		pCurrentImage = m_toggleStatus == STATUS_OFF ? &m_images[STATUS_DOWN] : &m_toggleOnImages[STATUS_DOWN];
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		// Disable
		pCurrentImage = m_toggleStatus == STATUS_OFF ? &m_images[STATUS_DISABLE] : &m_toggleOnImages[STATUS_DISABLE];
	else if (m_bHover)
		// Hover
		pCurrentImage = m_toggleStatus == STATUS_OFF ? &m_images[STATUS_HOVER] : &m_toggleOnImages[STATUS_HOVER];	
	else
		// Normal
		pCurrentImage = m_toggleStatus == STATUS_OFF ? &m_images[STATUS_NORMAL] : &m_toggleOnImages[STATUS_NORMAL];

	if (pCurrentImage->GetBitmapLastStatus() == Ok)
	{
		if ((UINT)clientRect.Width() > pCurrentImage->GetWidth() &&
			(UINT)clientRect.Height() > pCurrentImage->GetHeight())
			// 원본 이미지 보다 큰 경우 3x3 확대하여 출력한다.
			pCurrentImage->DrawStretchImage3x3(&graphics, clientRect);
		else
			// Source 크기보다 Client가 작거나 같은 경우는 Client 크기로 출력한다.
			pCurrentImage->DrawImage(&graphics, clientRect);
	}
	CString strText;
	GetWindowText(strText);

	if (!strText.IsEmpty())
	{
		strText.Replace(_T("&"), _T(""));
		GetCurrentFont()->DrawText(&graphics, strText, m_textRect);
	}
}

void CCherryToggleButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetCherryStyle() & STYLE_AUTOTOGGLEBUTTON)
	{
		if (GetCheck() == STATUS_OFF)
			SetCheck(STATUS_ON);
		else
			SetCheck(STATUS_OFF);
	}

	CCherryButton::OnLButtonUp(nFlags, point);
}

CCherryToggleButton::STATUS CCherryToggleButton::GetCheck() const
{
	return m_toggleStatus;
}

void CCherryToggleButton::SetCheck(STATUS toggleStatus)
{
	m_toggleStatus = toggleStatus;

	Invalidate(FALSE);

	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED));
}

void CCherryToggleButton::EnableAutoResize(BOOL bEnable)
{
	if (bEnable)
		ModifyCherryStyle(0, STYLE_AUTORESIZE);
	else
		ModifyCherryStyle(STYLE_AUTORESIZE, 0);
}

void CCherryToggleButton::EnableAutoToggle(BOOL bEnable)
{
	if (bEnable)
		ModifyCherryStyle(0, STYLE_AUTOTOGGLEBUTTON);
	else
		ModifyCherryStyle(STYLE_AUTOTOGGLEBUTTON, 0);
}