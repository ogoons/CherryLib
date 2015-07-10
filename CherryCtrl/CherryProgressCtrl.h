#pragma once

#include "CherryException.h"
#include "CherryImage.h"
#include "CherryWnd.h"
#include "CherryFont.h"
#include "CherryMemDC.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryProgressCtrl
/// \brief		Progress 컨트롤 클래스\n
///				스킨을 입힐 수 있는 Progress 컨트롤이다.\n
///				2x1 형태의 프로그레스 이미지를 할당하여 배경과 진행 상태를 각각 표현한다.\n
///				1x1에 해당하는 이미지는 진행 상태 이미지 1x2는 배경 이미지를 가지고 있어야 한다.\n
///				Status: 4가지의 방향과 프로그레스 이미지 진행 방향을 설정할 수 있다.\n
///				Range: 1 ~ 32768 (short)
/// \author		ogoons
/// \date		2013-09-24
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryProgressCtrl : public CCherryWnd, public CCherryFont
{
	DECLARE_DYNAMIC(CCherryProgressCtrl)

public:
	enum DRAW_TYPE
	{
		DRAW_TYPE_FILL,
		DRAW_TYPE_BACKGROUND,
		DRAW_TYPE_COUNT
	};

	enum STYLE
	{
		STYLE_AUTORESIZE = 0x0001,

		STYLE_HORIZONTAL_LEFT_TO_RIGHT = 0x0002,
		STYLE_HORIZONTAL_RIGHT_TO_LEFT = 0x0004,
		STYLE_VERTICAL_BOTTOM_TO_TOP = 0x0008,
		STYLE_VERTICAL_TOP_TO_BOTTOM = 0x0010,
	};

// Constructors
public:
	CCherryProgressCtrl();
	virtual CHERRY_RET Create(
		LPCTSTR lpszImagePath, 
		const short nMaxRange, 
		DWORD dwCherryStyle,
		DWORD dwStyle, 
		const RECT &rect, 
		CWnd *pParentWnd, UINT nID);

// Attributes
protected:
	CCherryImage	m_images[DRAW_TYPE_COUNT];
	
	int m_nMaxPos;
	int m_nPos;

	DWORD m_dwCherryStyle;
	
// Operations
public:
	CHERRY_RET			SetImage(LPCTSTR lpszImagePath, int nWidth, int nHeight);
	void				SetPos(int nPos);
	CHERRY_RET			SetMaxPos(int nMaxPos);

	//void								SetStyle(CherryProgressCtrl::STYLE style);

	DWORD								GetCherryStyle() const;
	void								ModifyCherryStyle(DWORD dwRemove, DWORD dwAdd);
	void								EnableAutoResize(BOOL bEnable);

	int									GetPos() const;
	int									GetMaxPos() const;

protected:
	void								ResizeWindow(UINT nWidth, UINT nHeight);

// Implementation
public:
	virtual ~CCherryProgressCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void OnDrawCherry(CCherryMemDC *pDC);
};
