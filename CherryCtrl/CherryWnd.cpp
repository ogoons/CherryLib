// CherryWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryWnd.h"

#include "CherryButton.h"
#include "CherryCheckBox.h"
#include "CherryStatic.h"
#include "CherryLinkCtrl.h"
#include "CherryImageCtrl.h"
#include "CherryAnimateImageCtrl.h"
#include "CherryProgressCtrl.h"
#include "CherryEdit.h"
#include "CherryComboBox.h"
#include "CherrySliderCtrl.h"
#include "CherryTabCtrl.h"

// CCherryWnd
IMPLEMENT_DYNAMIC(CCherryWnd, CWnd)

CCherryWnd::CCherryWnd()
{
	m_pBackMemDC = NULL;
	m_bShowed = FALSE;
}

CCherryWnd::~CCherryWnd()
{
	if (m_pBackMemDC)
		delete m_pBackMemDC;
}

BEGIN_MESSAGE_MAP(CCherryWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CCherryWnd 메시지 처리기입니다.
void CCherryWnd::OnPaint()
{
	CRect clientRect;
	GetClientRect(&clientRect);

	CPaintDC dc(this); // device context for painting
	CCherryMemDC memDC(&dc, clientRect);

	/*
	// 자식에서 생성해 주는 게 낫겠다.
	Graphics graphics(memDC.GetSafeHdc());

	graphics.SetInterpolationMode(InterpolationModeDefault);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);
	*/

	if (m_pBackMemDC)
		m_pBackMemDC->Draw(&memDC);

	// CherryWnd 용 OnPaint()
	OnDrawCherry(&memDC);

	// OnCreate > OnNcCreate > OnPaint > OnLoad
	if (FALSE == m_bShowed)
	{
		m_bShowed = TRUE;
		OnLoad();
	}

	CWnd *pChildWnd = GetWindow(GW_CHILD);

	while (pChildWnd)
	{
		if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryWnd)))
			((CCherryWnd *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryButton)))
			((CCherryButton *)pChildWnd)->SetBackground(&memDC);
		//else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryCheckBox)))	
		//((CCherryCheckBox *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryStatic)))
			((CCherryStatic *)pChildWnd)->SetBackground(&memDC);
		//else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryLinkCtrl)))
		//((CCherryLinkCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryComboBox)))
			((CCherryComboBox *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryEditBackWnd)))
			((CCherryEditBackWnd *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryImageCtrl)))
			((CCherryImageCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryAnimateImageCtrl)))
			((CCherryAnimateImageCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryProgressCtrl)))
			((CCherryProgressCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherrySliderCtrl)))
			((CCherrySliderCtrl *)pChildWnd)->SetBackground(&memDC);
		else if (pChildWnd->IsKindOf(RUNTIME_CLASS(CCherryTabCtrl)))
			((CCherryTabCtrl *)pChildWnd)->SetBackground(&memDC);

		if (pChildWnd->IsWindowVisible())
		{
			CRect windowRect;
			pChildWnd->GetWindowRect(windowRect);
			ScreenToClient(&windowRect);
			dc.ExcludeClipRect(windowRect);
		}

		pChildWnd = pChildWnd->GetNextWindow(GW_HWNDNEXT);
	}
}

void CCherryWnd::SetBackground(CDC *pDC)
{
	if (m_pBackMemDC)
		delete m_pBackMemDC;

	CRect clientRect, windowRect;

	GetClientRect(&clientRect);
	GetWindowRect(&windowRect);
	GetParent()->ScreenToClient(&windowRect);

	m_pBackMemDC = new CCherryMemDC(pDC, clientRect, FALSE);
	m_pBackMemDC->BitBlt(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(),
		pDC, windowRect.left, windowRect.top, SRCCOPY);
}

void CCherryWnd::OnLoad()
{
}