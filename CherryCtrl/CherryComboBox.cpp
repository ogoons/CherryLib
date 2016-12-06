// CherryComboBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryComboBox.h"

#define DEFAULT_COMBOBOX_TEXT_COLOR	RGB(0, 0, 0)

// CCherryComboBox

IMPLEMENT_DYNAMIC(CCherryComboBox, CComboBox)

CCherryComboBox::CCherryComboBox()
{
	m_pCurrentImage = NULL;
	m_pBackMemDC = NULL;
	m_bTracking = FALSE;
	m_bHover = FALSE;
	m_bDropDown = FALSE;
	m_dwCherryStyle = 0;
	m_bEnableHoverHandCursor = FALSE;

	m_nOffsetTextLeft = 0;
	m_nOffsetTextTop = 0;

	m_textColor = DEFAULT_COMBOBOX_TEXT_COLOR;

	//m_comboBoxStatus = CherryComboBox::STATUS_NORMAL;
}

CCherryComboBox::~CCherryComboBox()
{
	m_font.DeleteObject();

	if (m_pBackMemDC)
		delete m_pBackMemDC;
}

BEGIN_MESSAGE_MAP(CCherryComboBox, CComboBox)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CCherryComboBox::OnCbnDropDown)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, &CCherryComboBox::OnCbnCloseUp)
END_MESSAGE_MAP()

// CCherryComboBox 메시지 처리기입니다.

CHERRY_RET CCherryComboBox::Create(LPCTSTR lpszImagePath, DWORD dwCherryStyle, DWORD dwStyle, const CCherryRect &cherryRect, CWnd *pParentWnd, UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		dwStyle |= CBS_DROPDOWNLIST | CBS_HASSTRINGS;

		if (!CComboBox::Create(dwStyle, cherryRect, pParentWnd, nID))
			throw CCherryException::ERROR_COMBOBOX_CREATE_FAIL;

		m_font.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
			, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, DEFAULT_FONT_NAME);

		SetFont(&m_font);

		// CCherryComboBox 전용 스타일 bit 플래그
		ModifyCherryStyle(0, dwCherryStyle);

		// 기본 왼쪽, 세로 중앙 정렬
		GetNormalFont()->SetFontStyle(STYLE_VCENTER);
		GetHoverFont()->SetFontStyle(STYLE_VCENTER);
		GetDownFont()->SetFontStyle(STYLE_VCENTER);
		GetDisableFont()->SetFontStyle(STYLE_VCENTER);

		// 기본 비활성화 텍스트 속성
		COLORREF disableColor = GetSysColor(COLOR_GRAYTEXT);
		GetDisableFont()->SetFontColor(Color(GetRValue(disableColor), GetGValue(disableColor), GetBValue(disableColor)));

		cherryRet = SetImage(lpszImagePath);
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryComboBox::SetImage(LPCTSTR lpszImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		// 원본 이미지 로드
		CCherryImage sourceImage;
		if ((cherryRet = sourceImage.LoadImage(lpszImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		UINT nRawWidth = sourceImage.GetWidth() / STATUS_MAX_COUNT;
		UINT nRawHeight = sourceImage.GetHeight();

		// 각 상태 이미지 잘라서 붙여넣기
		for (UINT i = STATUS_NORMAL; i < STATUS_MAX_COUNT; i++)
		{
			if ((cherryRet = m_images[i].LoadImage(sourceImage.GetBitmap()->Clone(Rect(nRawWidth * i, 0, nRawWidth, nRawHeight), PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;
		}

		m_pCurrentImage = &m_images[STATUS_NORMAL];

		if (GetCherryStyle() & STYLE_AUTORESIZE)
		{
			// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
			ResizeWindow(nRawWidth, nRawHeight);
		}
		else
		{
			CRect windowRect;
			CComboBox::GetWindowRect(&windowRect);
			CComboBox::GetParent()->ScreenToClient(&windowRect);

			UINT nWidth, nHeight;

			if (windowRect.Width() > 0)
				nWidth = windowRect.Width();
			else
				nWidth = 0;

			if (windowRect.Height() > 0)
				nHeight = windowRect.Height();
			else
				nHeight = 0;

			if (nWidth > 0 && nHeight > 0)
			{
				windowRect.right = windowRect.left + nWidth;
				windowRect.bottom = windowRect.top + nHeight;
				CComboBox::MoveWindow(windowRect);

				CComboBox::GetClientRect(m_textRect);
				m_textRect.OffsetRect(m_nOffsetTextLeft, m_nOffsetTextTop);
			}
			else
			{
				// nWidth || nHeight 어느 하나라도 0이면 Auto Resizing
				ResizeWindow(nRawWidth, nRawHeight);
			}
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

void CCherryComboBox::OnLButtonDown() 
{

}

void CCherryComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT trackMouseEvent;
		trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
		trackMouseEvent.hwndTrack = GetSafeHwnd();
		trackMouseEvent.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&trackMouseEvent);
	}

	CComboBox::OnMouseMove(nFlags, point);
}

void CCherryComboBox::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bHover = TRUE;

	// Hover
	m_pCurrentImage = &m_images[STATUS_HOVER];	

	SetCurrentFont(GetHoverFont());

	if (m_bEnableHoverHandCursor)
		SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_HAND));

	Invalidate(FALSE);

	CComboBox::OnMouseHover(nFlags, point);
}

void CCherryComboBox::OnMouseLeave()
{
	COMBOBOXINFO comboBoxInfo = { sizeof(COMBOBOXINFO) };
	GetComboBoxInfo(&comboBoxInfo);

	// 버튼이 눌린 상태면
	if (/* comboBoxInfo.stateButton == STATE_SYSTEM_PRESSED || */ !m_bDropDown)
	{
		// Normal
		m_pCurrentImage = &m_images[STATUS_NORMAL];
	}

	if (m_bTracking)
	{
		TRACKMOUSEEVENT trackMouseEvent;
		trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		trackMouseEvent.dwFlags = TME_CANCEL;
		trackMouseEvent.hwndTrack = GetSafeHwnd();
		trackMouseEvent.dwHoverTime = 1;
		_TrackMouseEvent(&trackMouseEvent);

		m_bTracking = FALSE;
	}

	m_bHover = FALSE;
	SetCurrentFont(GetNormalFont());

	if (m_bEnableHoverHandCursor)
		SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	Invalidate(FALSE);

	CComboBox::OnMouseLeave();
}

void CCherryComboBox::OnCbnDropDown()
{
	m_bDropDown = TRUE;
	m_pCurrentImage = &m_images[STATUS_DOWN];
	SetCurrentFont(GetDownFont());
	Invalidate(FALSE);
}

void CCherryComboBox::OnCbnCloseUp()
{
	m_bDropDown = FALSE;
	m_pCurrentImage = &m_images[STATUS_NORMAL];	
	SetCurrentFont(GetHoverFont());
	Invalidate(FALSE);
}

void CCherryComboBox::OffsetText(int nLeft, int nTop)
{
	m_nOffsetTextLeft = nLeft;
	m_nOffsetTextTop = nTop;

	GetClientRect(m_textRect);
	m_textRect.OffsetRect(nLeft, nTop);
}

BOOL CCherryComboBox::OnEraseBkgnd(CDC *pDC)
{
	// 배경을 지운 뒤 그리는 것이 아닌 덮어서 처리하도록 한다.(깜박임 방지)
	return FALSE;
}

BOOL CCherryComboBox::PreTranslateMessage(MSG *pMsg)
{
	// 더블 클릭 메시지를 WM_LBUTTONDOWN 메시지로 바꾸어 메시지 전송한다.
	// 결과적으로 빠른 클릭이 가능하게 한다.
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CComboBox::PreTranslateMessage(pMsg);
}

void CCherryComboBox::SetBackground(CDC *pDC)
{
	if (m_pBackMemDC)
		delete m_pBackMemDC;
	
	CRect clientRect, windowRect;

	GetClientRect(&clientRect);
	GetWindowRect(&windowRect);
	GetParent()->ScreenToClient(&windowRect);

	m_pBackMemDC = new CCherryMemDC(pDC, clientRect, FALSE);
	m_pBackMemDC->BitBlt(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), 
		pDC, windowRect.left, windowRect.top, SRCCOPY);
}

void CCherryComboBox::EnableHoverHandCursor(BOOL bEnable)
{
	m_bEnableHoverHandCursor = bEnable;
}

DWORD CCherryComboBox::GetCherryStyle() const
{
	return m_dwCherryStyle;
}

void CCherryComboBox::ModifyCherryStyle(DWORD dwRemove, DWORD dwAdd)
{
	m_dwCherryStyle &= ~dwRemove;
	m_dwCherryStyle |= dwAdd;
}

void CCherryComboBox::EnableAutoResize(BOOL bEnable)
{
	if (bEnable)
		ModifyCherryStyle(0, STYLE_AUTORESIZE);
	else
		ModifyCherryStyle(STYLE_AUTORESIZE, 0);
}

void CCherryComboBox::ResizeWindow(UINT nWidth, UINT nHeight)
{
	CRect windowRect;
	GetWindowRect(&windowRect);
	GetParent()->ScreenToClient(&windowRect);

	windowRect.right = windowRect.left + nWidth;
	windowRect.bottom = windowRect.top + nHeight;

	MoveWindow(windowRect);

	GetClientRect(m_textRect);
	// Offset pixel은 항상 반영
	m_textRect.OffsetRect(m_nOffsetTextLeft, m_nOffsetTextTop);
}

void CCherryComboBox::OnEnable(BOOL bEnable)
{
	CComboBox::OnEnable(bEnable);

	if (bEnable)
	{
		m_pCurrentImage = &m_images[STATUS_NORMAL];
	}
	else
	{
		m_pCurrentImage = &m_images[STATUS_DISABLE];

		SetCurrentFont(GetDisableFont());
		Invalidate();
	}
}

// 콤보 박스 버튼의 폰트 스타일이 관리된다
void CCherryComboBox::SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	CCherryFontByStatus::SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
}

// 리스트의 폰트 스타일이 관리된다.
void CCherryComboBox::SetDropDownListFont(LPCTSTR lpszFontName, LONG lFontSize, Color fontColor, DWORD dwFontStyle)
{
	m_font.DeleteObject();

	int nWeight = FW_NORMAL;
	BYTE byItalic = FALSE;
	BYTE byUnderline = FALSE;
	BYTE byStrikeOut = FALSE;

	if (dwFontStyle & STYLE_BOLD)
		nWeight = FW_BOLD;
	if (dwFontStyle & STYLE_ITALIC)
		byItalic = TRUE;
	if (dwFontStyle & STYLE_UNDERLINE)
		byUnderline = TRUE;
	if (dwFontStyle & STYLE_STRIKEOUT)
		byStrikeOut = TRUE;

	// Color -> COLORREF
	m_textColor = RGB(fontColor.GetR(), fontColor.GetG(), fontColor.GetB());

	m_font.CreateFont(lFontSize, 0, 0, 0, nWeight, byItalic, byUnderline, byStrikeOut, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, lpszFontName);
	SetFont(&m_font);

	// SetFont() 후에 클라이언트 사이즈가 변경되기 때문에 여기서 텍스트 영역을 갱신한다.
	GetClientRect(m_textRect);
	// Offset pixel은 항상 반영
	m_textRect.OffsetRect(m_nOffsetTextLeft, m_nOffsetTextTop);

	CCherryFont::SetCherryFont(lpszFontName, static_cast<float>(lFontSize), fontColor, dwFontStyle);
}

BOOL CCherryComboBox::SetCherryFont(CCherryFont *pCherryFont)
{
	SetCherryFont(pCherryFont->GetFontName(), pCherryFont->GetFontSize(), pCherryFont->GetFontColor(), pCherryFont->GetFontStyle());

	return CCherryFontByStatus::SetCherryFont(pCherryFont);
}

BOOL CCherryComboBox::SetDropDownListFont(CCherryFont *pCherryFont)
{
	SetDropDownListFont(pCherryFont->GetFontName(), static_cast<LONG>(pCherryFont->GetFontSize()), pCherryFont->GetFontColor(), pCherryFont->GetFontStyle());

	return CCherryFont::SetCherryFont(pCherryFont);
}

void CCherryComboBox::SetDropDownListFontName(LPCTSTR lpszFontName)
{
	LOGFONT logFont;

	m_font.GetLogFont(&logFont);
	m_font.DeleteObject();

	_tcscpy_s(logFont.lfFaceName, lpszFontName);
	m_font.CreateFontIndirect(&logFont);
	SetFont(&m_font);

	GetClientRect(m_textRect);
	// Offset pixel은 항상 반영
	m_textRect.OffsetRect(m_nOffsetTextLeft, m_nOffsetTextTop);

	CCherryFont::SetFontName(lpszFontName);
}

void CCherryComboBox::SetDropDownListFontSize(LONG lFontSize)
{
	LOGFONT logFont;

	m_font.GetLogFont(&logFont);
	m_font.DeleteObject();

	logFont.lfHeight = lFontSize;
	m_font.CreateFontIndirect(&logFont);
	SetFont(&m_font);

	GetClientRect(m_textRect);
	// Offset pixel은 항상 반영
	m_textRect.OffsetRect(m_nOffsetTextLeft, m_nOffsetTextTop);

	CCherryFont::SetFontSize(static_cast<float>(lFontSize));
}

void CCherryComboBox::SetDropDownListFontColor(Color fontColor)
{
	m_textColor = RGB(fontColor.GetR(), fontColor.GetG(), fontColor.GetB());

	CCherryFont::SetFontColor(fontColor);
}

void CCherryComboBox::SetDropDownListFontStyle(DWORD dwFontStyle)
{
	LOGFONT logFont;

	m_font.GetLogFont(&logFont);
	m_font.DeleteObject();

	if (dwFontStyle & STYLE_BOLD)
		logFont.lfWeight = FW_BOLD;
	if (dwFontStyle & STYLE_ITALIC)
		logFont.lfItalic = TRUE;
	if (dwFontStyle & STYLE_UNDERLINE)
		logFont.lfUnderline = TRUE;
	if (dwFontStyle & STYLE_STRIKEOUT)
		logFont.lfStrikeOut = TRUE;

	m_font.CreateFontIndirect(&logFont);	
	SetFont(&m_font);

	GetClientRect(m_textRect);
	// Offset pixel은 항상 반영
	m_textRect.OffsetRect(m_nOffsetTextLeft, m_nOffsetTextTop);

	CCherryFont::SetFontStyle(dwFontStyle);
}

int CCherryComboBox::SetCurSel(int nSelect)
{
	Invalidate(FALSE);

	return CComboBox::SetCurSel(nSelect);
}

void CCherryComboBox::OnPaint()
{
	CRect clientRect;
	GetClientRect(clientRect);

	CPaintDC dc(this); // device context for painting
	CCherryMemDC memDC(&dc, clientRect);
	Graphics graphics(memDC.GetSafeHdc());

	graphics.SetInterpolationMode(InterpolationModeDefault);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	if (m_pBackMemDC)
		m_pBackMemDC->Draw(&memDC);

	if (m_pCurrentImage)
	{
		if (m_pCurrentImage->GetBitmapLastStatus() == Ok)
		{
			if ((UINT)clientRect.Width() > m_pCurrentImage->GetWidth() &&
				(UINT)clientRect.Height() > m_pCurrentImage->GetHeight())
				// 원본 이미지 보다 큰 경우 3x3 확대하여 출력한다.
				m_pCurrentImage->Draw9PatchImage(&graphics, clientRect);
			else
				// Source 크기보다 Client가 작거나 같은 경우는 Client 크기로 출력한다.
				m_pCurrentImage->DrawImage(&graphics, clientRect);
		}
	}

	CString strText;
	GetWindowText(strText);

	if (!strText.IsEmpty())
	{
		strText.Replace(_T("&"), _T(""));
		GetCurrentFont()->DrawText(&graphics, strText, m_textRect);
	}
}

void CCherryComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	/*
	CRect clientRect;
	GetClientRect(clientRect);

	Graphics graphics(lpDrawItemStruct->hDC);

	graphics.SetInterpolationMode(InterpolationModeDefault);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	//if (m_pBackMemDC)
		//m_pBackMemDC->Draw(&memDC);

	if (m_pCurrentImage)
	{
		if (m_pCurrentImage->GetBitmapLastStatus() == Ok)
		{
			// Source 크기보다 Client가 작거나 같은 경우는 Client 크기로 출력한다.
			if ((int)m_pCurrentImage->GetWidth() >= clientRect.Width() &&
				(int)m_pCurrentImage->GetHeight() >= clientRect.Height())
			{
				m_pCurrentImage->DrawImage(&graphics, clientRect);
			}
			// 원본 이미지 보다 큰 경우 3x3 확대하여 출력한다.
			else
			{
				m_pCurrentImage->Draw9PatchImage(&graphics, clientRect);
			}
		}
	}


	if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
		(lpDrawItemStruct->itemState  & ODS_SELECTED))
	{
		//TRACE(_T("클릭\n"));

	}

	if ((lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		static int a = 0;
		TRACE(_T("클릭%d\n"), ++a);

	}
	*/
}


void CCherryComboBox::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
{

	// TODO:  지정된 항목의 크기를 확인하는 코드를 추가합니다.
}
