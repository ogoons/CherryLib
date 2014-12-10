// CherryCheckBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryCheckBox.h"


// CCherryCheckBox

IMPLEMENT_DYNAMIC(CCherryCheckBox, CCherryButton)

CCherryCheckBox::CCherryCheckBox()
{
	m_checkStatus = STATUS_UNCHECKED;
	//m_dwCherryStyle = 0;
	//m_bEnableHoverHandCursor = FALSE;
	//m_pBackMemDC = NULL;
}

CCherryCheckBox::~CCherryCheckBox()
{
}

BEGIN_MESSAGE_MAP(CCherryCheckBox, CCherryButton)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CCherryCheckBox 메시지 처리기입니다.
CHERRY_RET CCherryCheckBox::Create(LPCTSTR lpszCaption, LPCTSTR lpszImagePath, DWORD dwCherryStyle, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		dwStyle |= BS_OWNERDRAW;	// 추가

		if (!CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID))
			throw CCherryException::ERROR_CHECKBOX_CREATE_FAIL;

		// CherryCheckBox 전용 스타일 bit 플래그
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
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return SetImage(lpszImagePath);
}

CHERRY_RET CCherryCheckBox::SetImage(LPCTSTR lpszImagePath)
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

		// 체크된 이미지 각 상태 이미지 잘라서 붙여넣기
		for (UINT j = STATUS_NORMAL; j < STATUS_MAX_COUNT; j++)
		{
			if ((cherryRet = m_checkedImages[j].LoadImage(sourceImage.GetBitmap()->Clone(Rect(nOrgWidth * j, nOrgHeight, nOrgWidth, nOrgHeight), PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;
		}

		if (GetCherryStyle() & STYLE_AUTORESIZE)
		{
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
			else // nWidth || nHeight 어느 하나라도 0이면 Auto Resizing
			{
				ResizeWindow(nOrgWidth, nOrgHeight);
			}
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

void CCherryCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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

	// Down
	if (lpDrawItemStruct->itemState & ODS_SELECTED) 
		pCurrentImage = m_checkStatus == STATUS_UNCHECKED ? &m_images[STATUS_DOWN] : &m_checkedImages[STATUS_DOWN];
	// Disable
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		pCurrentImage = m_checkStatus == STATUS_UNCHECKED ? &m_images[STATUS_DISABLE] : &m_checkedImages[STATUS_DISABLE];
	// Hover
	else if (m_bHover)
		pCurrentImage = m_checkStatus == STATUS_UNCHECKED ? &m_images[STATUS_HOVER] : &m_checkedImages[STATUS_HOVER];	
	// Normal
	else
		pCurrentImage = m_checkStatus == STATUS_UNCHECKED ? &m_images[STATUS_NORMAL] : &m_checkedImages[STATUS_NORMAL];

	if (pCurrentImage->GetBitmapLastStatus() == Ok)
	{
		// Source 크기보다 Client가 작거나 같은 경우는 Client 크기로 출력한다.
		if ((int)pCurrentImage->GetWidth() >= clientRect.Width() && // 여기만 or 연산
			(int)pCurrentImage->GetHeight() >= clientRect.Height())
		{
			pCurrentImage->DrawImage(&graphics, clientRect);
		}
		// 원본 이미지 보다 큰 경우 3x3 확대하여 출력한다.
		else
		{
			int nTop = 0;
			int nWidth = 0, nHeight = 0;

			if (clientRect.Height() > static_cast<int>(pCurrentImage->GetHeight()))
				nTop = (clientRect.Height() - pCurrentImage->GetHeight()) / 2;

			// 체크 박스 스타일에 따라 이미지 크기를 결정한다.
			if (GetCherryStyle() & STYLE_BUTTONTYPE)
			{
				nWidth = clientRect.Width();
				nHeight = clientRect.Height();
			}
			else
			{
				nWidth = pCurrentImage->GetWidth();
				nHeight = pCurrentImage->GetHeight();
			}

			pCurrentImage->DrawStretchImage3x3(&graphics, 0, nTop, nWidth, nHeight);
		}
	}

	CString strText;
	GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		CRect textRect(clientRect);

		// 체크 박스 스타일에 따라 이미지 크기를 결정한다.
		if (!(GetCherryStyle() & STYLE_BUTTONTYPE))
		{
			// 이미지 오른쪽에 텍스트 표시
			int nCheckBoxWidth = m_images[STATUS_NORMAL].GetWidth();

			textRect.left += nCheckBoxWidth;
			//textRect.right -= nCheckBoxWidth;
		}

		strText.Replace(_T("&"), _T(""));
		GetCurrentFont()->DrawText(&graphics, strText, textRect);
	}
}

void CCherryCheckBox::ResizeWindow(UINT nWidth, UINT nHeight)
{
	if (GetCherryStyle() & STYLE_BUTTONTYPE)
	{
		CCherryButton::ResizeWindow(nWidth, nHeight);
	}
	else
	{
		// 각 상태 별 폰트 중에 가장 폰트 크기가 큰 상태를 기준으로 리사이징 한다.
		CCherryFont *pFonts[STATUS_MAX_COUNT], *pGreatestSizeFont;

		pFonts[STATUS_NORMAL] = GetNormalFont();
		pFonts[STATUS_HOVER] = GetHoverFont();
		pFonts[STATUS_DOWN] = GetDownFont();
		pFonts[STATUS_DISABLE] = GetDisableFont();

		pGreatestSizeFont = pFonts[STATUS_NORMAL];

		for (UINT i = STATUS_NORMAL; i < STATUS_MAX_COUNT - 1; i++)
		{
			if (pGreatestSizeFont->GetFontSize() < pFonts[i + 1]->GetFontSize())
				pGreatestSizeFont = pFonts[i + 1];
		}

		CRect textRect;
		CString strText;
		GetWindowText(strText);

		if (!strText.IsEmpty())
		{
			CPaintDC dc(this);
			Graphics graphics(dc.GetSafeHdc());

			textRect = pGreatestSizeFont->MeasureString(&graphics, strText);
		}

		CRect windowRect;
		GetWindowRect(&windowRect);
		GetParent()->ScreenToClient(&windowRect);
		windowRect.right = windowRect.left + nWidth + textRect.Width() + 3;	// 오차를 보정하기 위한 3

		// 이미지 높이가 텍스트 높이보다 크면
		if (nHeight > (UINT)textRect.Height())
			windowRect.bottom = windowRect.top + nHeight;
		else
			windowRect.bottom = windowRect.top + textRect.Height();

		MoveWindow(windowRect);
	}
}

void CCherryCheckBox::SetWindowText(LPCTSTR lpszText)
{
	CCherryButton::SetWindowText(lpszText);

	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetCherryStyle() & STYLE_AUTOCHECKBOX)
	{
		if (GetCheck() == STATUS_UNCHECKED)
			//m_checkStatus = STATUS_CHECKED;
			SetCheck(STATUS_CHECKED);
		else
			//m_checkStatus = STATUS_UNCHECKED;
			SetCheck(STATUS_UNCHECKED);
	}

	CCherryButton::OnLButtonUp(nFlags, point);
}

CCherryCheckBox::STATUS CCherryCheckBox::GetCheck() const
{
	return m_checkStatus;
}

void CCherryCheckBox::SetCheck(STATUS checkStatus)
{
	m_checkStatus = checkStatus;

	Invalidate(FALSE);

	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED));
}

void CCherryCheckBox::EnableAutoResize(BOOL bEnable)
{
	if (bEnable)
		ModifyCherryStyle(0, STYLE_AUTORESIZE);
	else
		ModifyCherryStyle(STYLE_AUTORESIZE, 0);
}

void CCherryCheckBox::EnableAutoCheck(BOOL bEnable)
{
	if (bEnable)
		ModifyCherryStyle(0, STYLE_AUTOCHECKBOX);
	else
		ModifyCherryStyle(STYLE_AUTOCHECKBOX, 0);
}

void CCherryCheckBox::SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	CCherryFontByStatus::SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

BOOL CCherryCheckBox::SetCherryFont(CCherryFont *pCherryFont)
{
	if (!CCherryFontByStatus::SetCherryFont(pCherryFont))
		return FALSE;

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}

	return TRUE;
}

void CCherryCheckBox::SetNormalFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	CCherryFontByStatus::SetNormalFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
		
	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

BOOL CCherryCheckBox::SetNormalFont(CCherryFont *pNormalFont)
{
	if (!CCherryFontByStatus::SetNormalFont(pNormalFont))
		return FALSE;

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}

	return TRUE;
}

void CCherryCheckBox::SetNormalFontName(LPCTSTR lpszFontName)
{
	CCherryFontByStatus::SetNormalFontName(lpszFontName);
	
	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetNormalFontSize(float fFontSize)
{
	CCherryFontByStatus::SetNormalFontSize(fFontSize);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetNormalFontColor(Color fontColor)
{
	CCherryFontByStatus::SetNormalFontColor(fontColor);
	// 색상은 사이즈 변경이 이루어지지 않음으로 리사이즈 할 필요가 없다.
}

void CCherryCheckBox::SetNormalFontStyle(DWORD dwFontStyle)
{
	CCherryFontByStatus::SetNormalFontStyle(dwFontStyle);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetHoverFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	CCherryFontByStatus::GetHoverFont()->SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
		
	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

BOOL CCherryCheckBox::SetHoverFont(CCherryFont *pHoverFont)
{
	if (!CCherryFontByStatus::SetHoverFont(pHoverFont))
		return FALSE;

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}

	return TRUE;
}

void CCherryCheckBox::SetHoverFontName(LPCTSTR lpszFontName)
{
	CCherryFontByStatus::SetHoverFontName(lpszFontName);
	
	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetHoverFontSize(float fFontSize)
{
	CCherryFontByStatus::SetHoverFontSize(fFontSize);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetHoverFontColor(Color fontColor)
{
	CCherryFontByStatus::SetHoverFontColor(fontColor);
	// 색상은 사이즈 변경이 이루어지지 않음으로 리사이즈 할 필요가 없다.
}

void CCherryCheckBox::SetHoverFontStyle(DWORD dwFontStyle)
{
	CCherryFontByStatus::SetHoverFontStyle(dwFontStyle);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetDownFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	CCherryFontByStatus::SetDownFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
		
	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

BOOL CCherryCheckBox::SetDownFont(CCherryFont *pDownFont)
{
	if (!CCherryFontByStatus::SetDownFont(pDownFont))
		return FALSE;

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}

	return TRUE;
}

void CCherryCheckBox::SetDownFontName(LPCTSTR lpszFontName)
{
	CCherryFontByStatus::SetDownFontName(lpszFontName);
	
	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetDownFontSize(float fFontSize)
{
	CCherryFontByStatus::SetDownFontSize(fFontSize);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetDownFontColor(Color fontColor)
{
	CCherryFontByStatus::SetDownFontColor(fontColor);
	// 색상은 사이즈 변경이 이루어지지 않음으로 리사이즈 할 필요가 없다.
}

void CCherryCheckBox::SetDownFontStyle(DWORD dwFontStyle)
{
	CCherryFontByStatus::SetDownFontStyle(dwFontStyle);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetDisableFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	CCherryFontByStatus::SetDisableFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
		
	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

BOOL CCherryCheckBox::SetDisableFont(CCherryFont *pDisableFont)
{
	if (!CCherryFontByStatus::SetDisableFont(pDisableFont))
		return FALSE;

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}

	return TRUE;
}

void CCherryCheckBox::SetDisableFontName(LPCTSTR lpszFontName)
{
	CCherryFontByStatus::SetDisableFontName(lpszFontName);
	
	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetDisableFontSize(float fFontSize)
{
	CCherryFontByStatus::SetDisableFontSize(fFontSize);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}

void CCherryCheckBox::SetDisableFontColor(Color fontColor)
{
	CCherryFontByStatus::SetDisableFontColor(fontColor);
	// 색상은 사이즈 변경이 이루어지지 않음으로 리사이즈 할 필요가 없다.
}

void CCherryCheckBox::SetDisableFontStyle(DWORD dwFontStyle)
{
	CCherryFontByStatus::SetDisableFontStyle(dwFontStyle);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		if (m_images[STATUS_NORMAL].IsLoadedImage())
			ResizeWindow(m_images[STATUS_NORMAL].GetWidth(), m_images[STATUS_NORMAL].GetHeight());
	}
}