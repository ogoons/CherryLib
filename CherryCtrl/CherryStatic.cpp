#include "stdafx.h"
#include "CherryStatic.h"

IMPLEMENT_DYNAMIC(CCherryStatic, CStatic)

CCherryStatic::CCherryStatic()
{
	m_bHover = FALSE;
	m_bTracking = FALSE;
	m_bDown = FALSE;
	m_pBackMemDC = NULL;
}

CCherryStatic::~CCherryStatic()
{
	if (m_pBackMemDC)
		delete m_pBackMemDC;
}

BEGIN_MESSAGE_MAP(CCherryStatic, CStatic)
END_MESSAGE_MAP()

CHERRY_RET CCherryStatic::Create(LPCTSTR lpszText, DWORD dwCherryStyle, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		dwStyle |= SS_OWNERDRAW | SS_NOTIFY;

		if (!CStatic::Create(NULL, dwStyle, rect, pParentWnd, nID))
			throw CCherryException::ERROR_STATIC_CREATE_FAIL;

		// CherryCheckBox 전용 스타일 bit 플래그
		ModifyCherryStyle(0, dwCherryStyle);

		SetWindowText(lpszText);
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

void CCherryStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CCherryMemDC memDC(pDC, clientRect);
	Graphics graphics(memDC.GetSafeHdc());

	if (m_pBackMemDC)
		m_pBackMemDC->Draw(&memDC);

	CString strText;
	GetWindowText(strText);

	if (!strText.IsEmpty())
		DrawText(&graphics, strText, clientRect);
}

void CCherryStatic::SetWindowText(LPCTSTR lpszText)
{
	if (GetCherryStyle() & STYLE_AUTORESIZE)
	{
		// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
		ResizeWindow();	
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

		if (nWidth > 0 && nHeight > 0)
		{
			windowRect.right = windowRect.left + nWidth;
			windowRect.bottom = windowRect.top + nHeight;
			MoveWindow(windowRect);
		}
		else
		{
			// nWidth || nHeight 어느 하나라도 0이면 Auto Resizing
			ResizeWindow();	
		}
	}

	CStatic::SetWindowText(lpszText);
}

void CCherryStatic::SetBackground(CDC *pDC)
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

void CCherryStatic::ResizeWindow()
{
	CRect textRect;
	CString strText;
	
	GetWindowText(strText);

	if (!strText.IsEmpty())
	{
		CPaintDC dc(this);
		Graphics graphics(dc.GetSafeHdc());

		textRect = MeasureString(&graphics, strText);
	}

	CRect windowRect;
	GetWindowRect(&windowRect);
	GetParent()->ScreenToClient(&windowRect);

	windowRect.right = windowRect.left + textRect.Width() + 3;	// 오차를 보정하기 위한 3
	windowRect.bottom = windowRect.top + textRect.Height();

	MoveWindow(windowRect);
}

//void CCherryStatic::operator=(CCherryFont cherryFont)
//{	
//	CCherryFont::SetCherryFont(cherryFont);
//
//	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
//	if (GetCherryStyle() & CherryStatic::STYLE_AUTORESIZE)
//		ResizeWindow();	
//}

void CCherryStatic::SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	CCherryFont::SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
		ResizeWindow();	
}

BOOL CCherryStatic::SetCherryFont(CCherryFont *pCherryFont)
{
	if (!CCherryFont::SetCherryFont(pCherryFont))
		return FALSE;

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
		ResizeWindow();	

	return TRUE;
}

void CCherryStatic::SetFontName(LPCTSTR lpszFontName)
{
	CCherryFont::SetFontName(lpszFontName);
	
	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
		ResizeWindow();	
}

void CCherryStatic::SetFontSize(float fFontSize)
{
	CCherryFont::SetFontSize(fFontSize);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
		ResizeWindow();	
}

void CCherryStatic::SetFontStyle(DWORD dwFontStyle)
{
	CCherryFont::SetFontStyle(dwFontStyle);

	// CherryCheckBox::STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
	if (GetCherryStyle() & STYLE_AUTORESIZE)
		ResizeWindow();	
}

DWORD CCherryStatic::GetCherryStyle() const
{
	return m_dwCherryStyle;
}

void CCherryStatic::ModifyCherryStyle(DWORD dwRemove, DWORD dwAdd)
{
	m_dwCherryStyle &= ~dwRemove;
	m_dwCherryStyle |= dwAdd;
}

void CCherryStatic::EnableAutoResize(BOOL bEnable)
{
	if (bEnable)
		ModifyCherryStyle(0, STYLE_AUTORESIZE);
	else
		ModifyCherryStyle(STYLE_AUTORESIZE, 0);
}