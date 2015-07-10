#pragma once

#include "CherryFont.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryFontByStatus
/// \brief		4가지 상태 Font 관리 클래스\n
///				CCherryButton, CCherryCheckBox, CCherryLinkCtrl 등 마우스 메시지를 받아 처리되는\n
///				컨트롤들의 문자열 처리를 담당하는 Super 클래스이다.
/// \author		ogoons
/// \date		2013-04-06
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryFontByStatus
{
// Constructors
public:
	CCherryFontByStatus();

// Attributes
protected:
	CCherryFont		*m_pCurrentFont;
	CCherryFont		m_normalFont;
	CCherryFont		m_hoverFont;
	CCherryFont		m_downFont;
	CCherryFont		m_disableFont;

// Operations
public:
	// 모든 상태 폰트를 통일한다.(Disable 칼라는 기본 그레이 설정)
	void SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle);
	BOOL SetCherryFont(CCherryFont *pCherryFont);

	// 상태별 폰트 설정
	void SetFontByStatus(CCherryFont *pNormalFont, CCherryFont *pHoverFont, CCherryFont *pDownFont, CCherryFont *pDisableFont);
	
	// Normal 폰트 설정
	void SetNormalFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle);
	BOOL SetNormalFont(CCherryFont *pNormalFont);
	void SetNormalFontName(LPCTSTR lpszFontName);
	void SetNormalFontSize(float fFontSize);
	void SetNormalFontColor(Color fontColor);
	void SetNormalFontStyle(DWORD dwFontStyle);

	// Hover 폰트 설정
	void SetHoverFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle);
	BOOL SetHoverFont(CCherryFont *pHoverFont);
	void SetHoverFontName(LPCTSTR lpszFontName);
	void SetHoverFontSize(float fFontSize);
	void SetHoverFontColor(Color fontColor);
	void SetHoverFontStyle(DWORD dwFontStyle);

	// Down 폰트 설정
	void SetDownFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle);
	BOOL SetDownFont(CCherryFont *pDownFont);
	void SetDownFontName(LPCTSTR lpszFontName);
	void SetDownFontSize(float fFontSize);
	void SetDownFontColor(Color fontColor);
	void SetDownFontStyle(DWORD dwFontStyle);

	// Disable 폰트 설정
	void SetDisableFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle);
	BOOL SetDisableFont(CCherryFont *pDisableFont);
	void SetDisableFontName(LPCTSTR lpszFontName);
	void SetDisableFontSize(float fFontSize);
	void SetDisableFontColor(Color fontColor);
	void SetDisableFontStyle(DWORD dwFontStyle);

	void GetFontByStatus(CCherryFont &normalFont, CCherryFont &hoverFont, CCherryFont &downFont, CCherryFont &disableFont) const;

protected:
	CCherryFont	*GetNormalFont();
	CCherryFont	*GetHoverFont();
	CCherryFont	*GetDownFont();
	CCherryFont	*GetDisableFont();

	void SetCurrentFont(CCherryFont *pStatusFont);
	CCherryFont	*GetCurrentFont() const;

// Implementation
public:
	virtual ~CCherryFontByStatus();
};
