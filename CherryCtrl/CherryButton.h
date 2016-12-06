#pragma once

#include "CherryException.h"
#include "CherryMemDC.h"
#include "CherryFontByStatus.h"
#include "CherryImage.h"
#include "CherryWnd.h"
#include "CherryRect.h"

using namespace Gdiplus;


///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryButton
/// \brief		일반적인 이미지 버튼을 생성한다. (투명 지원)\n
///				버튼의 상태 = Normal, Hover, Down, Disable\n
///				1x4 형태의 이미지를 버튼에 할당하여 각 상태를 표현한다.
/// \author		ogoons
/// \date		2012-11-05
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryButton : public CButton, public CCherryFontByStatus
{
	DECLARE_DYNAMIC(CCherryButton)

	// 버튼 상태 정의
	enum STATUS
	{
		STATUS_NORMAL,
		STATUS_HOVER,
		STATUS_DOWN,
		STATUS_DISABLE,
		STATUS_MAX_COUNT
	};

	// 버튼 스타일 정의
	enum STYLE
	{
		STYLE_AUTORESIZE = 0x0001,
	};

// Constructors
public:
	CCherryButton();
	virtual CHERRY_RET Create(
		LPCTSTR lpszCaption, 
		LPCTSTR lpszImagePath, 
		DWORD dwCherryStyle, 
		DWORD dwStyle, 
		CCherryRect &cherryRect,
		CWnd *pParentWnd, 
		UINT nID);	// 버튼 생성(필수 사용)
	
// Attributes
protected:
	CCherryMemDC	*m_pBackMemDC;
	CCherryImage	m_images[STATUS_MAX_COUNT];		// 4가지 상태(Normal, Hover, Down, Disable)의 이미지 저장
	
	BOOL			m_bTracking;	// 트래킹 여부
	BOOL			m_bHover;		// 마우스 Hover 상태 저장
	BOOL			m_bEnableHoverHandCursor;
	DWORD			m_dwCherryStyle;

	CRect			m_textRect;
	int				m_nOffsetTextLeft;
	int				m_nOffsetTextTop;

// Operations
public:
	CHERRY_RET								SetImage(LPCTSTR lpszImagePath);		// 버튼 이미지 설정
	void									OffsetText(int nLeft, int nTop);		// 텍스트 위치 이동
	void									EnableHoverHandCursor(BOOL bEnable);
	void									SetBackground(CDC *pDC);

	DWORD									GetCherryStyle() const;
	void									ModifyCherryStyle(DWORD dwRemove, DWORD dwAdd);
	void									EnableAutoResize(BOOL bEnable);

protected:
	void									ResizeWindow(UINT nWidth, UINT nHeight);

// Implementation
public:
	virtual ~CCherryButton();

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	virtual void	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);	// Owner draw 시 사용되며 OnPaint는 사용되지 않는다.

	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void	OnMouseLeave();
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnEnable(BOOL bEnable);

	virtual void	OnDrawCherry(CCherryMemDC *pDC);
};
	