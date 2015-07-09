#pragma once

#include "CherryMemDC.h"

// CCherryWnd

class AFX_EXT_CLASS CCherryWnd : public CWnd
{
	DECLARE_DYNAMIC(CCherryWnd)

// Constructors
public:
	CCherryWnd();

// Attributes
protected:
	CCherryMemDC *m_pBackMemDC;
	BOOL m_bShowed;

// Operations
public:
	void SetBackground(CDC *pDC);

// Implementation
public:
	virtual ~CCherryWnd();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();

	// 모든 그리기 작업은 여기서 한다. (OnPaint 오버라이드 금지)
	virtual void OnDrawCherry(CCherryMemDC *pDC) = 0; // 자식 클래스에서 구현하도록 한다.

	// OnCreate 다음에 컨트롤을 생성하는 작업이 있을 경우 상속, 구현하여 사용한다. OnInitDialog() 함수와 비슷함.
	virtual void OnLoad();
};


