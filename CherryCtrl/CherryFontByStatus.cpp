#include "stdafx.h"
#include "CherryFontByStatus.h"

CCherryFontByStatus::CCherryFontByStatus()
{
	// 기본 Current 폰트는 노말 폰트로 한다.
	m_pCurrentFont = &m_normalFont;

	// Disable Gray 처리
	COLORREF disableColorRef = GetSysColor(COLOR_GRAYTEXT);
	Color disableColor(GetRValue(disableColorRef), GetGValue(disableColorRef), GetBValue(disableColorRef));
	GetDisableFont()->SetFontColor(disableColor);
}

CCherryFontByStatus::~CCherryFontByStatus()
{
}

void CCherryFontByStatus::SetCurrentFont(CCherryFont *pStatusFont)
{
	m_pCurrentFont = pStatusFont;
}

CCherryFont	*CCherryFontByStatus::GetCurrentFont() const
{
	return m_pCurrentFont;
}

void CCherryFontByStatus::GetFontByStatus(CCherryFont &normalFont, CCherryFont &hoverFont, CCherryFont &downFont, CCherryFont &disableFont) const
{
	normalFont = m_normalFont;
	hoverFont = m_hoverFont;
	downFont = m_downFont;
	disableFont = m_disableFont;
}

CCherryFont	*CCherryFontByStatus::GetNormalFont()
{
	return &m_normalFont;
}

CCherryFont	*CCherryFontByStatus::GetHoverFont()
{
	return &m_hoverFont;
}

CCherryFont	*CCherryFontByStatus::GetDownFont()
{
	return &m_downFont;
}

CCherryFont	*CCherryFontByStatus::GetDisableFont()
{
	return &m_disableFont;
}

void CCherryFontByStatus::SetFontByStatus(CCherryFont *pNormalFont, CCherryFont *pHoverFont, CCherryFont *pDownFont, CCherryFont *pDisableFont)
{
	SetNormalFont(pNormalFont);
	SetHoverFont(pHoverFont);
	SetDownFont(pDownFont);
	SetDisableFont(pDisableFont);
}

void CCherryFontByStatus::SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	SetNormalFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
	SetHoverFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
	SetDownFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
	
	// Disable Gray 처리
	COLORREF disableColorRef = GetSysColor(COLOR_GRAYTEXT);
	Color disableColor(GetRValue(disableColorRef), GetGValue(disableColorRef), GetBValue(disableColorRef));
	SetDisableFont(lpszFontName, fFontSize, disableColor, dwFontStyle);
}

BOOL CCherryFontByStatus::SetCherryFont(CCherryFont *pCherryFont)
{
	if (pCherryFont == NULL)
		return FALSE;

	SetCherryFont(pCherryFont->GetFontName(), pCherryFont->GetFontSize(), pCherryFont->GetFontColor(), pCherryFont->GetFontStyle());

	return TRUE;
}

//////////////////////////////
// Normal font
void CCherryFontByStatus::SetNormalFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	m_normalFont.SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
	
	// 노말 폰트 설정시 현재 폰트로 설정하여 화면에 표시함
	SetCurrentFont(&m_normalFont);
}

BOOL CCherryFontByStatus::SetNormalFont(CCherryFont *pNormalFont)
{
	if (pNormalFont == NULL)
		return FALSE;

	SetNormalFont(pNormalFont->GetFontName(), pNormalFont->GetFontSize(), pNormalFont->GetFontColor(), pNormalFont->GetFontStyle());

	return TRUE;
}

void CCherryFontByStatus::SetNormalFontName(LPCTSTR lpszFontName)
{
	m_normalFont.SetFontName(lpszFontName);
	SetCurrentFont(&m_normalFont);
}

void CCherryFontByStatus::SetNormalFontSize(float fFontSize)
{
	m_normalFont.SetFontSize(fFontSize);
	SetCurrentFont(&m_normalFont);
}

void CCherryFontByStatus::SetNormalFontColor(Color fontColor)
{
	m_normalFont.SetFontColor(fontColor);
	SetCurrentFont(&m_normalFont);
}

void CCherryFontByStatus::SetNormalFontStyle(DWORD dwFontStyle)
{
	m_normalFont.SetFontStyle(dwFontStyle);
	SetCurrentFont(&m_normalFont);
}

//////////////////////////////
// Hover font
void CCherryFontByStatus::SetHoverFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	m_hoverFont.SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
}

BOOL CCherryFontByStatus::SetHoverFont(CCherryFont *pHoverFont)
{
	if (pHoverFont == NULL)
		return FALSE;

	SetHoverFont(pHoverFont->GetFontName(), pHoverFont->GetFontSize(), pHoverFont->GetFontColor(), pHoverFont->GetFontStyle());

	return TRUE;
}

void CCherryFontByStatus::SetHoverFontName(LPCTSTR lpszFontName)
{
	m_hoverFont.SetFontName(lpszFontName);
}

void CCherryFontByStatus::SetHoverFontSize(float fFontSize)
{
	m_hoverFont.SetFontSize(fFontSize);
}

void CCherryFontByStatus::SetHoverFontColor(Color fontColor)
{
	m_hoverFont.SetFontColor(fontColor);
}

void CCherryFontByStatus::SetHoverFontStyle(DWORD dwFontStyle)
{
	m_hoverFont.SetFontStyle(dwFontStyle);
}

//////////////////////////////
// Down font
void CCherryFontByStatus::SetDownFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	m_downFont.SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
}

BOOL CCherryFontByStatus::SetDownFont(CCherryFont *pDownFont)
{
	if (pDownFont == NULL)
		return FALSE;

	SetDownFont(pDownFont->GetFontName(), pDownFont->GetFontSize(), pDownFont->GetFontColor(), pDownFont->GetFontStyle());

	return TRUE;
}

void CCherryFontByStatus::SetDownFontName(LPCTSTR lpszFontName)
{
	m_downFont.SetFontName(lpszFontName);
}

void CCherryFontByStatus::SetDownFontSize(float fFontSize)
{
	m_downFont.SetFontSize(fFontSize);
}

void CCherryFontByStatus::SetDownFontColor(Color fontColor)
{
	m_downFont.SetFontColor(fontColor);
}

void CCherryFontByStatus::SetDownFontStyle(DWORD dwFontStyle)
{
	m_downFont.SetFontStyle(dwFontStyle);
}

//////////////////////////////
// Disable font
void CCherryFontByStatus::SetDisableFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	m_disableFont.SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
}

BOOL CCherryFontByStatus::SetDisableFont(CCherryFont *pDisableFont)
{
	if (pDisableFont == NULL)
		return FALSE;

	SetDisableFont(pDisableFont->GetFontName(), pDisableFont->GetFontSize(), pDisableFont->GetFontColor(), pDisableFont->GetFontStyle());

	return TRUE;
}

void CCherryFontByStatus::SetDisableFontName(LPCTSTR lpszFontName)
{
	m_disableFont.SetFontName(lpszFontName);
}

void CCherryFontByStatus::SetDisableFontSize(float fFontSize)
{
	m_disableFont.SetFontSize(fFontSize);
}

void CCherryFontByStatus::SetDisableFontColor(Color fontColor)
{
	m_disableFont.SetFontColor(fontColor);
}

void CCherryFontByStatus::SetDisableFontStyle(DWORD dwFontStyle)
{
	m_disableFont.SetFontStyle(dwFontStyle);
}