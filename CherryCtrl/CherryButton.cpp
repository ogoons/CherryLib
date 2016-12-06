// ButtonEx.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryButton.h"

// CCherryButton

IMPLEMENT_DYNAMIC(CCherryButton, CButton)

CCherryButton::CCherryButton() 
{
	m_bHover = FALSE;
	m_bTracking = FALSE;
	m_dwCherryStyle = 0;
	m_bEnableHoverHandCursor = FALSE;
	m_pBackMemDC = NULL;

	m_nOffsetTextLeft = 0;
	m_nOffsetTextTop = 0;
}

CCherryButton::~CCherryButton()
{	
	if (m_pBackMemDC)
	{
		delete m_pBackMemDC;
		m_pBackMemDC = NULL;
	}
}

BEGIN_MESSAGE_MAP(CCherryButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

CHERRY_RET CCherryButton::Create(LPCTSTR lpszCaption, LPCTSTR lpszImagePath, DWORD dwCherryStyle, DWORD dwStyle, CCherryRect &cherryRect, CWnd *pParentWnd, UINT nID)
{	
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		dwStyle |= BS_OWNERDRAW;

		if (!CButton::Create(lpszCaption, dwStyle, cherryRect, pParentWnd, nID))
			throw CCherryException::ERROR_BUTTON_CREATE_FAIL;

		// CCherryButton 전용 스타일 bit 플래그
		ModifyCherryStyle(0, dwCherryStyle);

		// 기본 중앙 정렬
		GetNormalFont()->SetFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
		GetHoverFont()->SetFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
		GetDownFont()->SetFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
		GetDisableFont()->SetFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);

		// 기본 비활성화 텍스트 속성
		COLORREF disableColor = GetSysColor(COLOR_GRAYTEXT);
		GetDisableFont()->SetFontColor(Color(GetRValue(disableColor), GetGValue(disableColor), GetBValue(disableColor)));

		// 텍스트 설정
		//SetWindowText(lpszCaption);

		cherryRet = SetImage(lpszImagePath);
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryButton::SetImage(LPCTSTR lpszImagePath)
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

		//Rect rect[STATUS_MAX_COUNT];
		for (UINT i = STATUS_NORMAL; i < STATUS_MAX_COUNT; i++) // 각 상태 이미지 잘라서 붙여넣기
		{
			//rect[i] = Rect(nRawWidth * i, 0, nRawWidth, nRawHeight);

			if ((cherryRet = m_images[i].LoadImage(sourceImage.GetBitmap()->Clone(Rect(nRawWidth * i, 0, nRawWidth, nRawHeight), PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;
		}

		if (GetCherryStyle() & STYLE_AUTORESIZE)
		{
			// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
			ResizeWindow(nRawWidth, nRawHeight);
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
				nWidth = 0;

			if (windowRect.Height() > 0)
				nHeight = windowRect.Height();
			else
				nHeight = 0;

			// 수동 리사이즈
			if (nWidth > 0 && nHeight > 0)
			{
				windowRect.right = windowRect.left + nWidth;
				windowRect.bottom = windowRect.top + nHeight;
				MoveWindow(windowRect);

				GetClientRect(m_textRect);
				m_textRect.OffsetRect(m_nOffsetTextLeft, m_nOffsetTextTop);
			}
			// 자동 리사이즈
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

// 버튼의 모든 이미지는 여기서 그린다.
void CCherryButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
		pCurrentImage = &m_images[STATUS_DOWN];
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		// Disable
		pCurrentImage = &m_images[STATUS_DISABLE];
	else if (m_bHover)
		// Hover
		pCurrentImage = &m_images[STATUS_HOVER];	
	else
		// Normal
		pCurrentImage = &m_images[STATUS_NORMAL];

	if (pCurrentImage->GetBitmapLastStatus() == Ok)
	{
		if ((UINT)clientRect.Width() > pCurrentImage->GetWidth() &&
			(UINT)clientRect.Height() > pCurrentImage->GetHeight())
			// 원본 이미지 보다 큰 경우 3x3 확대하여 출력한다.
			pCurrentImage->Draw9PatchImage(&graphics, clientRect);
		else
			// Source 크기보다 Client가 작거나 경우는 Client 크기로 출력한다.
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

void CCherryButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT trackMouseEvent;
		trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
		trackMouseEvent.hwndTrack = m_hWnd;
		trackMouseEvent.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&trackMouseEvent);
	}

	CButton::OnMouseMove(nFlags, point);
}

void CCherryButton::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bHover = TRUE;
	SetCurrentFont(GetHoverFont());

	if (m_bEnableHoverHandCursor)
		SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_HAND));

	Invalidate(FALSE);

	CButton::OnMouseHover(nFlags, point);
}

void CCherryButton::OnMouseLeave()
{
	if (m_bTracking)
	{
		TRACKMOUSEEVENT trackMouseEvent;
		trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		trackMouseEvent.dwFlags = TME_CANCEL;
		trackMouseEvent.hwndTrack = m_hWnd;
		trackMouseEvent.dwHoverTime = 1;
		_TrackMouseEvent(&trackMouseEvent);

		m_bTracking = FALSE;
	}
	
	m_bHover = FALSE;
	SetCurrentFont(GetNormalFont());

	if (m_bEnableHoverHandCursor)
		SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	Invalidate(FALSE);

	CButton::OnMouseLeave();
}

void CCherryButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCurrentFont(GetDownFont());
	Invalidate(FALSE);

	CButton::OnLButtonDown(nFlags, point);
}

void CCherryButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetCurrentFont(GetHoverFont());
	Invalidate(FALSE);

	CButton::OnLButtonUp(nFlags, point);
}

void CCherryButton::OffsetText(int nLeft, int nTop)
{
	m_nOffsetTextLeft = nLeft;
	m_nOffsetTextTop = nTop;

	GetClientRect(m_textRect);
	m_textRect.OffsetRect(nLeft, nTop);
	
}

BOOL CCherryButton::OnEraseBkgnd(CDC *pDC)
{
	// 배경을 지운 뒤 그리는 것이 아닌 덮어서 처리하도록 한다.(깜박임 방지)
	return FALSE;
}

BOOL CCherryButton::PreTranslateMessage(MSG *pMsg)
{
	// 더블 클릭 메시지를 WM_LBUTTONDOWN 메시지로 바꾸어 메시지 전송한다.
	// 결과적으로 빠른 클릭이 가능하게 한다.
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CButton::PreTranslateMessage(pMsg);
}

void CCherryButton::OnDrawCherry(CCherryMemDC *pDC)
{
}

void CCherryButton::SetBackground(CDC *pDC)
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


void CCherryButton::EnableHoverHandCursor(BOOL bEnable)
{
	m_bEnableHoverHandCursor = bEnable;
}

DWORD CCherryButton::GetCherryStyle() const
{
	return m_dwCherryStyle;
}

void CCherryButton::ModifyCherryStyle(DWORD dwRemove, DWORD dwAdd)
{
	m_dwCherryStyle &= ~dwRemove;
	m_dwCherryStyle |= dwAdd;
}

void CCherryButton::EnableAutoResize(BOOL bEnable)
{
	if (bEnable)
		ModifyCherryStyle(0, STYLE_AUTORESIZE);
	else
		ModifyCherryStyle(STYLE_AUTORESIZE, 0);
}

void CCherryButton::ResizeWindow(UINT nWidth, UINT nHeight)
{
	CRect windowRect;
	GetWindowRect(&windowRect);
	GetParent()->ScreenToClient(&windowRect);

	windowRect.right = windowRect.left + nWidth;
	windowRect.bottom = windowRect.top + nHeight;

	MoveWindow(windowRect);

	GetClientRect(m_textRect);
	m_textRect.OffsetRect(m_nOffsetTextLeft, m_nOffsetTextTop);
}

void CCherryButton::OnEnable(BOOL bEnable)
{
	CButton::OnEnable(bEnable);

	if (!bEnable)
	{
		SetCurrentFont(GetDisableFont());
		Invalidate();
	}
}