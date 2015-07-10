#pragma once

#include "CherryException.h"
#include "CherryButton.h"
#include "CherryMemDC.h"
#include "CherryImage.h"
#include "CherryProgressCtrl.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherrySliderCtrlThumbButton
/// \brief		슬라이더 컨트롤 Thumb 버튼
/// \author		ogoons
/// \date		2014-01-01
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class CCherrySliderCtrlThumbButton : public CCherryButton
{
	DECLARE_DYNAMIC(CCherrySliderCtrlThumbButton)

// Constructors
public:
	CCherrySliderCtrlThumbButton();

// Attributes
protected:
	BOOL m_bLButtonDown;

// Operations
public:
	BOOL IsLButtonDown() { return m_bLButtonDown; };
	void UseCachedImage(BOOL bUse = TRUE);

// Overridables
protected:

// Implementation
public:
	virtual ~CCherrySliderCtrlThumbButton();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherrySliderCtrl
/// \brief		슬라이더 컨트롤
/// \author		ogoons
/// \date		2013-07-10
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

// Message
#define WM_CHERRY_SLIDERCTRL_POS_CHANGED WM_USER + 1

// Register Message Map
#define ON_WM_CHERRY_SLIDERCTRL_POS_CHANGED(id, memberFxn) \
	ON_CONTROL(WM_CHERRY_SLIDERCTRL_POS_CHANGED, id, memberFxn)

class AFX_EXT_CLASS CCherrySliderCtrl : public CCherryProgressCtrl
{
	DECLARE_DYNAMIC(CCherrySliderCtrl)

public:
	// 슬라이더 스타일 정의
	enum STYLE
	{
		// 2013-07-17 ogoons: 주의!
		// CherryProgressCtrl과 style 값을 일치시킨다.
		STYLE_AUTORESIZE = 0x0001,

		// Thumb 위치 만큼 배경 이미지가 채워진다.
		STYLE_HORIZONTAL_LEFT_TO_RIGHT		= 0x0002,
		STYLE_HORIZONTAL_RIGHT_TO_LEFT		= 0x0004,
		STYLE_VERTICAL_BOTTOM_TO_TOP		= 0x0008,
		STYLE_VERTICAL_TOP_TO_BOTTOM		= 0x0010,

		// 배경 이미지가 Thumb 위치에 따라서 채워지지 않는다.
		STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW		= 0x0020,
		STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW		= 0x0040,
		STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW			= 0x0080,
		STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW			= 0x0100,
	};

// Constructors
public:
	CCherrySliderCtrl();
	virtual CHERRY_RET Create(
		LPCTSTR lpszThumbImagePath,
		LPCTSTR lpszTrackImagePath, 
		const int nMaxRange,
		DWORD dwCherryStyle, 
		DWORD dwStyle,
		const RECT &rect, 
		CWnd *pParentWnd, 
		UINT nID);	// 버튼 생성(필수 사용)
	

// Attributes
protected:
	CCherryImage					m_trackImage;
	CCherrySliderCtrlThumbButton	m_thumbButton;

// Operations
public:
	CHERRY_RET			SetThumbImage(LPCTSTR lpszImagePath);
	CHERRY_RET			SetTrackImage(LPCTSTR lpszImagePath);
	
protected:
	void									ResizeWindow(UINT nWidth, UINT nHeight);
	void									RepositionThumbButton(int nTrackWidth, int nTrackHeight);
	int										ConvertCursorPosToCurrentPos(CPoint point);
	CRect									ConvertCurrentPosToThumbButtonRect(int nCurrentPos);

// Implementation
public:
	virtual ~CCherrySliderCtrl();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()

	virtual void OnDrawCherry(CCherryMemDC *pDC);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};