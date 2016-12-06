#pragma once

#include "CherryWnd.h"
#include "CherryMemDC.h"
#include "CherryImage.h"
#include "CherryRect.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryEditBackWnd
/// \brief		에디트 컨트롤 배경 Window
/// \author		ogoons
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
	virtual CHERRY_RET Create(LPCTSTR lpszBackImagePath, DWORD dwStyle, const CCherryRect &cherryRect, CWnd *pParentWnd, UINT nID);

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