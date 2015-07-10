#pragma once

#include "CherryStatic.h"
#include "CherryFontByStatus.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryLinkCtrl
/// \brief		링크 컨트롤
/// \author		ogoons
/// \date		2013-07-10
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryLinkCtrl : public CCherryStatic, public CCherryFontByStatus
{
	DECLARE_DYNAMIC(CCherryLinkCtrl)

public:
	enum STYLE
	{
		STYLE_AUTORESIZE = 0x0001,
	};

// Constructors
public:
	CCherryLinkCtrl();
	virtual CHERRY_RET Create(
		LPCTSTR lpszText,
		DWORD dwCherryStyle,
		DWORD dwStyle, 
		const RECT &rect, 
		CWnd *pParentWnd, 
		UINT nID);

// Attributes
protected:
	BOOL			m_bTracking;	// 트래킹 여부
	BOOL			m_bHover;		// 마우스 Hover 상태 저장
	BOOL			m_bEnableHoverHandCursor;

// Operations
public:
	void EnableHoverHandCursor(BOOL bEnable);

// Overridables
	void EnableAutoResize(BOOL bEnable);

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
	void ResizeWindow();

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	
// Implementation
public:
	virtual ~CCherryLinkCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void	OnMouseLeave();
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnEnable(BOOL bEnable);
};
