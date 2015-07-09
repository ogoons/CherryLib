#pragma once

#include "CherryButton.h"
//
//namespace CherryCheckBox
//{
//
//}

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryCheckBox
/// \brief		체크 박스 컨트롤\n
///				CCherryButton을 상속받아 구현되었다.
/// \author		오수철
/// \date		2013-07-10
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryCheckBox : public CCherryButton
{
	DECLARE_DYNAMIC(CCherryCheckBox)
public:
	enum STATUS
	{
		STATUS_UNCHECKED,
		STATUS_CHECKED,
		STATUS_INDETERMINATE, // 미구현
	};

	enum STYLE
	{
		STYLE_AUTORESIZE = 0x0001,
		STYLE_AUTOCHECKBOX = 0x0002,
		STYLE_3STATE = 0x0004,	// 미구현
		STYLE_BUTTONTYPE = 0x0008,
	};

// Constructors
public:
	CCherryCheckBox();
	virtual CHERRY_RET Create(
		LPCTSTR lpszCaption, 
		LPCTSTR lpszImagePath, 
		DWORD dwCherryStyle,
		DWORD dwStyle, 
		const RECT &rect, 
		CWnd *pParentWnd, 
		UINT nID);
	
// Attributes
protected:
	CCherryImage m_checkedImages[STATUS_MAX_COUNT];
	STATUS m_checkStatus;

// Operations
public:
	CHERRY_RET SetImage(LPCTSTR lpszImagePath);

	STATUS GetCheck() const;
	void SetCheck(STATUS checkStatus);

// Overridables
public:
	void SetWindowText(LPCTSTR lpszText);
	void EnableAutoResize(BOOL bEnable);
	void EnableAutoCheck(BOOL bEnable);

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

protected:
	void ResizeWindow(UINT nWidth, UINT nHeight);
	
// Implementation
public:
	virtual ~CCherryCheckBox();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};