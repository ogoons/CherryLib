#include "stdafx.h"
#include "TestWnd.h"


CTestWnd::CTestWnd()
{
}

CTestWnd::CTestWnd(COLORREF color)
{
	m_color = color;
}


CTestWnd::~CTestWnd()
{
}

void CTestWnd::OnDrawCherry(CCherryMemDC *pDC)
{
	CRect clientRect;
	GetClientRect(clientRect);

	
	pDC->FillSolidRect(clientRect, m_color);

}

void CTestWnd::SetBackColor(COLORREF color)
{
	m_color = color;

	Invalidate(FALSE);
}