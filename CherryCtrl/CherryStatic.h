#pragma once

#include "CherryException.h"
#include "CherryFont.h"
#include "CherryMemDC.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryStatic
/// \brief		윈도우를 가지고 있는 Static 컨트롤
/// \author		오수철
/// \date		2013-09-24
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryStatic : public CStatic, public CCherryFont
{
	DECLARE_DYNAMIC(CCherryStatic)

public:
	enum STYLE
	{
		STYLE_AUTORESIZE = 0x0001,
	};

// Constructors
public:
	CCherryStatic();
	virtual CHERRY_RET Create(LPCTSTR lpszText, DWORD dwCherryStyle, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
	
// Attributes
protected:
	CCherryMemDC	*m_pBackMemDC;

	BOOL			m_bTracking;
	BOOL			m_bHover;
	BOOL			m_bDown;

	DWORD			m_dwCherryStyle;

// Operations
public:
	void	SetBackground(CDC *pDC);
	void	EnableAutoResize(BOOL bEnable);
	DWORD	GetCherryStyle() const;
	void	ModifyCherryStyle(DWORD dwRemove, DWORD dwAdd);
	
// Overridables
public:
	void SetWindowText(LPCTSTR lpszString);
	void SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle);
	BOOL SetCherryFont(CCherryFont *pCherryFont);
	void SetFontName(LPCTSTR lpszFontName);
	void SetFontSize(float fFontSize);
	void SetFontStyle(DWORD dwFontStyle);
	//void operator=(CCherryFont font);

protected:
	void ResizeWindow();	

// Implementation
public:
	virtual ~CCherryStatic();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};
