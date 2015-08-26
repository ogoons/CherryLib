#include "stdafx.h"
#include "CherryFont.h"

CCherryFont::CCherryFont()
{
	SetCherryFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, DEFAULT_FONT_COLOR, DEFAULT_FONT_STYLE);
// 	m_bEnableShadow = FALSE;
// 	m_shadowColor = DEFAULT_SHADOW_COLOR;
// 
// 	m_nOffsetTextLeft = 0;
// 	m_nOffsetTextTop = 0;
}

CCherryFont::CCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle) 
{
	SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
}

CCherryFont::CCherryFont(const CCherryFont &cherryFont) 
{
	SetFontName(cherryFont.m_strFontName);
	SetFontSize(cherryFont.m_fFontSize);
	SetFontColor(cherryFont.m_fontColor);
	SetFontStyle(cherryFont.m_dwFontStyle);	
}

CCherryFont::~CCherryFont()
{
}

void CCherryFont::SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	SetFontName(lpszFontName);
	SetFontSize(fFontSize);
	SetFontColor(fontColor);
	SetFontStyle(dwFontStyle);	
}

BOOL CCherryFont::SetCherryFont(CCherryFont *pCherryFont)
{
	if (NULL == pCherryFont)
		return FALSE;

	SetCherryFont(pCherryFont->GetFontName(), pCherryFont->GetFontSize(), pCherryFont->GetFontColor(), pCherryFont->GetFontStyle());

	return TRUE;
}

void CCherryFont::SetFontName(LPCTSTR lpszFontName)
{
	m_strFontName = lpszFontName;
}

void CCherryFont::SetFontSize(float fFontSize)
{
	m_fFontSize = fFontSize;
}

void CCherryFont::SetFontStyle(DWORD dwFontStyle)
{
	m_dwFontStyle = dwFontStyle;
}

void CCherryFont::SetFontColor(Color fontColor)
{
	m_fontColor = fontColor;
}

//void CCherryFont::EnableShadow(BOOL bEnable)
//{
//	m_bEnableShadow = bEnable;
//}
//
//void CCherryFont::SetShadowColor(Color shadowColor)
//{
//	m_shadowColor = shadowColor;
//}
//
//void CCherryFont::OffsetShadow(int nLeft, int nTop)
//{
//	m_nOffsetTextLeft = nLeft;
//	m_nOffsetTextTop = nTop;
//}

CString CCherryFont::GetFontName()
{
	return m_strFontName;
}

float CCherryFont::GetFontSize()
{
	return m_fFontSize;
}

Color CCherryFont::GetFontColor()
{
	return m_fontColor;
}

DWORD CCherryFont::GetFontStyle()
{
	return m_dwFontStyle;
}

void CCherryFont::DrawText(Graphics *pGraphics, LPCTSTR lpszText, const CRect &rect)
{	
	UINT nFonyStyle = DEFAULT_FONT_STYLE;

	if (m_dwFontStyle & STYLE_BOLD)
		nFonyStyle += FontStyleBold;
	if (m_dwFontStyle & STYLE_ITALIC)
		nFonyStyle += FontStyleItalic;
	if (m_dwFontStyle & STYLE_UNDERLINE)
		nFonyStyle += FontStyleUnderline;
	if (m_dwFontStyle & STYLE_STRIKEOUT)
		nFonyStyle += FontStyleStrikeout;	

	StringFormat stringFormat;
	stringFormat.SetAlignment(StringAlignmentNear);
	stringFormat.SetLineAlignment(StringAlignmentNear);

	if (m_dwFontStyle & STYLE_LEFT)
		stringFormat.SetAlignment(StringAlignmentNear);
	else if (m_dwFontStyle & STYLE_CENTER)
		stringFormat.SetAlignment(StringAlignmentCenter);
	else if (m_dwFontStyle & STYLE_RIGHT)
		stringFormat.SetAlignment(StringAlignmentFar);
	
	if (m_dwFontStyle & STYLE_TOP)
		stringFormat.SetLineAlignment(StringAlignmentNear);
	else if (m_dwFontStyle & STYLE_VCENTER)
		stringFormat.SetLineAlignment(StringAlignmentCenter);
	else if (m_dwFontStyle & STYLE_BOTTOM)
		stringFormat.SetLineAlignment(StringAlignmentFar);

	// 개행 방지
	if (m_dwFontStyle & STYLE_NOWRAP)
		stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	
	// 범위 초과시 ... 표시
	stringFormat.SetTrimming(StringTrimmingEllipsisCharacter);

	Gdiplus::Font font(m_strFontName, m_fFontSize, nFonyStyle, DEFAULT_UNIT_PIXEL);

	//if (m_bEnableShadow)
	//{
	//	RectF shadowRect(textRect);
	//	shadowRect.Offset(1 + (REAL)m_nOffsetTextLeft, 1 + (REAL)m_nOffsetTextTop);
	//	pGraphics->DrawString(lpszText, -1, &font, shadowRect, &stringFormat, &solidBrush);
	//}

	pGraphics->DrawString(lpszText, _tcslen(lpszText), &font, RectF((REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(), (REAL)rect.Height()), &stringFormat, &SolidBrush(m_fontColor));
}

CRect CCherryFont::MeasureString(Graphics *pGraphics, LPCTSTR lpszString)
{
	Gdiplus::Font font(m_strFontName, m_fFontSize, m_dwFontStyle, DEFAULT_UNIT_PIXEL);
	RectF stringRectF;

	pGraphics->MeasureString(lpszString, static_cast<int>(_tcslen(lpszString)), &font, PointF(0, 0), &stringRectF);

	return CRect(static_cast<int>(stringRectF.GetLeft()), static_cast<int>(stringRectF.GetTop()), static_cast<int>(stringRectF.GetRight()), static_cast<int>(stringRectF.GetBottom()));
}

