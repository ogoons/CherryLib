#pragma once

#include "CherryImage.h"
#include "CherryFont.h"

// CCherryHeaderCtrl

class CCherryHeaderCtrl : public CHeaderCtrl, public CCherryFont
{
	DECLARE_DYNAMIC(CCherryHeaderCtrl)

	enum HEADER_STATUS
	{
		HEADER_STATUS_NORMAL,
		HEADER_STATUS_HOVER,
		HEADER_STATUS_DOWN,
		HEADER_STATUS_DISABLED,
		HEADER_STATUS_MAX_COUNT
	};

// Constructors
public:
	CCherryHeaderCtrl();
	virtual CHERRY_RET Create(
		LPCTSTR lpszImagePath,
		DWORD dwStyle, 
		const RECT &rect, 
		CWnd *pParentWnd, 
		UINT nID);

// Attributes
protected:
	UINT m_nHeight; // 헤더 높이

	CCherryImage m_headerImage;
	
	//CCherryFont		m_font;

// Operations
public:
	CHERRY_RET SetImage(LPCTSTR lpszImagePath);
	void SetHeight(UINT nHeight);

// Implementation
public:
	virtual ~CCherryHeaderCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnLayout(WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();

};


