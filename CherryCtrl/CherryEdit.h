#pragma once

#include "CherryWnd.h"
#include "CherryMemDC.h"
#include "CherryImage.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryEditBackWnd
/// \brief		에디트 컨트롤 배경 Window
/// \author		오수철
/// \date		2013-07-10
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class CCherryEditBackWnd : public CCherryWnd
{
	DECLARE_DYNAMIC(CCherryEditBackWnd)

// Constructors
public:
	CCherryEditBackWnd();
	virtual CHERRY_RET Create(LPCTSTR lpszBackImagePath, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
	
// Attributes
protected:
	CCherryMemDC *m_pBackMemDC;
	CCherryImage m_backImage;

// Operations
public:
	CHERRY_RET SetImage(LPCTSTR lpszImagePath);
	
// Implementation
public:
	virtual ~CCherryEditBackWnd();

protected:
	DECLARE_MESSAGE_MAP()
	
	void OnDrawCherry(CCherryMemDC *pDC);

	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	
};

#include "CherryFont.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryEdit
/// \brief		에디트 컨트롤
/// \author		오수철
/// \date		2013-07-10
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryEdit : public CEdit, public CCherryFont
{
	DECLARE_DYNAMIC(CCherryEdit)

// Constructors
public:
	CCherryEdit();
	CHERRY_RET Create(LPCTSTR lpszBackImagePath, COLORREF backColor, DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);
	

// Attributes
protected:
	CCherryEditBackWnd	m_backWnd;
	CFont				m_font;

	COLORREF m_textColor;
	COLORREF m_backColor;

// Operations
public:
	void SetBackColor(COLORREF backColor);

	void SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle);
	BOOL SetCherryFont(CCherryFont *pCherryFont);
	void SetFontName(LPCTSTR lpszFontName);
	void SetFontSize(float fFontSize);
	void SetFontColor(Color fontColor);
	void SetFontStyle(DWORD dwFontStyle);

// Overridables
protected:
	virtual void PreSubclassWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);

// Implementation
public:
	virtual ~CCherryEdit();

protected:
	DECLARE_MESSAGE_MAP()
};