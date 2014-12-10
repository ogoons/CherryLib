#pragma once
#include "CherryWnd.h"

class CTestWnd : public CCherryWnd
{
public:
	CTestWnd();
	CTestWnd(COLORREF color);

	COLORREF m_color;

	void SetBackColor(COLORREF color);

	virtual void OnDrawCherry(CCherryMemDC *pDC);

	virtual ~CTestWnd();
};

