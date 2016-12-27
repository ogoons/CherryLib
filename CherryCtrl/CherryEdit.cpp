// CherryEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryEdit.h"

#define DEFAULT_EDITCTRL_TEXT_COLOR	RGB(0, 0, 0)
#define DEFAULT_EDITCTRL_BACK_COLOR	RGB(255, 255, 255)

// CCherryEdit

IMPLEMENT_DYNAMIC(CCherryEdit, CEdit)

CCherryEdit::CCherryEdit()
{
	m_textColor = DEFAULT_EDITCTRL_TEXT_COLOR;
	m_backColor = DEFAULT_EDITCTRL_BACK_COLOR;
}

CCherryEdit::~CCherryEdit()
{
}

BEGIN_MESSAGE_MAP(CCherryEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CCherryEdit 메시지 처리기입니다.

void CCherryEdit::PreSubclassWindow()
{
	//ModifyStyle(0, SS_NOTIFY, SWP_FRAMECHANGED);
	CEdit::PreSubclassWindow();
}

CHERRY_RET CCherryEdit::Create(LPCTSTR lpszBackImagePath, COLORREF backColor, DWORD dwStyle, const CCherryRect &cherryRect, int nBorderMargin, CWnd *pParentWnd, UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if ((cherryRet = m_backWnd.Create(lpszBackImagePath, WS_CHILD | WS_VISIBLE, cherryRect, pParentWnd, nID)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		CRect parentClientRect;
		m_backWnd.GetClientRect(&parentClientRect);

		CRect deflatedRect(parentClientRect);
		deflatedRect.DeflateRect(nBorderMargin, nBorderMargin, nBorderMargin, nBorderMargin); // Default value

		if (!CEdit::Create(dwStyle, deflatedRect, &m_backWnd, nID))
			throw CCherryException::ERROR_EDIT_CREATE_FAIL;
		
		if (NULL == m_font.CreateFont(DEFAULT_ORG_FONT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
			, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, DEFAULT_FONT_NAME))
			throw CCherryException::ERROR_EDIT_CREATE_FONT_FAIL;

		SetFont(&m_font);
		SetBackColor(backColor);

		LOGFONT logFont;
		m_font.GetLogFont(&logFont);

		// 싱글라인 속성이면
		if (!(GetStyle() & ES_MULTILINE))
		{
			//HDC hDC = ::GetDC(::GetDesktopWindow());
			//int ret = GetDeviceCaps(hDC, LOGPIXELSY) / 72 * logFont.lfHeight;
			//::ReleaseDC(USE_MFC_CLASS, hDC);

			deflatedRect.bottom = deflatedRect.top + abs(logFont.lfHeight) + 4;
			deflatedRect.top = (parentClientRect.Height() - deflatedRect.Height()) / 2;

			MoveWindow(deflatedRect);
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

void CCherryEdit::SetBackColor(COLORREF backColor)
{
	m_backColor = backColor;
}

HBRUSH CCherryEdit::CtlColor(CDC *pDC, UINT nCtlColor)
{
	pDC->SetTextColor(m_textColor);
	pDC->SetBkColor(m_backColor);

	//return (HBRUSH)GetStockObject(NULL_BRUSH);
	return (HBRUSH)CreateSolidBrush(m_backColor);
}

void CCherryEdit::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);
}

void CCherryEdit::SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
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

	m_font.CreateFont((int)fFontSize + 3, 0, 0, 0, nWeight, byItalic, byUnderline, byStrikeOut, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, lpszFontName);
	SetFont(&m_font);

	CCherryFont::SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
}

BOOL CCherryEdit::SetCherryFont(CCherryFont *pCherryFont)
{
	SetCherryFont(pCherryFont->GetFontName(), pCherryFont->GetFontSize(), pCherryFont->GetFontColor(), pCherryFont->GetFontStyle());

	return CCherryFont::SetCherryFont(pCherryFont);
}

void CCherryEdit::SetFontName(LPCTSTR lpszFontName)
{
	LOGFONT logFont;

	m_font.GetLogFont(&logFont);
	m_font.DeleteObject();

	_tcscpy_s(logFont.lfFaceName, lpszFontName);
	m_font.CreateFontIndirect(&logFont);
	SetFont(&m_font);

	CCherryFont::SetFontName(lpszFontName);
}

void CCherryEdit::SetFontSize(float fFontSize)
{
	LOGFONT logFont;

	m_font.GetLogFont(&logFont);
	m_font.DeleteObject();

	logFont.lfHeight = (LONG)fFontSize + 3;
	m_font.CreateFontIndirect(&logFont);
	SetFont(&m_font);

	CCherryFont::SetFontSize(fFontSize);
}

void CCherryEdit::SetFontColor(Color fontColor)
{
	m_textColor = RGB(fontColor.GetR(), fontColor.GetG(), fontColor.GetB());

	CCherryFont::SetFontColor(fontColor);
}

void CCherryEdit::SetFontStyle(DWORD dwFontStyle)
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

	CCherryFont::SetFontStyle(dwFontStyle);
}

BOOL CCherryEdit::SetPadding(int nMargin)
{
	return SetPadding(nMargin, nMargin, nMargin, nMargin);
}

BOOL CCherryEdit::SetPadding(int nLeft, int nTop, int nRight, int nBottom)
{
	if (!m_backWnd.GetSafeHwnd())
		return FALSE;

	CRect parentClientRect;
	m_backWnd.GetClientRect(parentClientRect);

	CRect deflatedRect(parentClientRect);
	deflatedRect.DeflateRect(nLeft, nTop, nRight, nBottom);

	MoveWindow(deflatedRect);

	return TRUE;
}

void CCherryEdit::OnDestroy()
{
	CEdit::OnDestroy();
	m_font.DeleteObject();
}
