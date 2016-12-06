#pragma once

#include "CherryButton.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryToggleButton
/// \brief		토글 버튼 
/// \author		ogoons
/// \date		2013-09-24
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryToggleButton : public CCherryButton
{
	DECLARE_DYNAMIC(CCherryToggleButton)

public:
	enum STATUS
	{
		STATUS_OFF,
		STATUS_ON,
	};

	enum STYLE
	{
		STYLE_AUTORESIZE = 0x0001,
		STYLE_AUTOTOGGLEBUTTON = 0x0002,
	};

// Constructors
public:
	CCherryToggleButton();
	virtual CHERRY_RET Create(
		LPCTSTR lpszCaption, 
		LPCTSTR lpszImagePath, 
		DWORD dwCherryStyle,
		DWORD dwStyle, 
		const CCherryRect &cherryRect,
		CWnd *pParentWnd, 
		UINT nID);
	
// Attributes
protected:
	CCherryImage m_toggleOnImages[STATUS_MAX_COUNT];
	STATUS m_toggleStatus;

// Operations
public:
	CHERRY_RET SetImage(LPCTSTR lpszImagePath);

	STATUS GetCheck() const;
	void SetCheck(STATUS toggleStatus);

// Overridables
public:
	void EnableAutoResize(BOOL bEnable);
	void EnableAutoToggle(BOOL bEnable);

protected:
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

// Implmentation
public:
	virtual ~CCherryToggleButton();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};