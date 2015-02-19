#include "stdafx.h"
#include "CherryTabBackWnd.h"
#include "CherryTabCtrl.h"

CCherryTabBackWnd::CCherryTabBackWnd()
{
}

CCherryTabBackWnd::~CCherryTabBackWnd()
{
}

void CCherryTabBackWnd::OnDrawCherry(CCherryMemDC *pDC)
{
}

LRESULT CCherryTabBackWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//CCherryTabCtrl *pTabCtrl = (CCherryTabCtrl *)GetParent();
	if (WM_COMMAND == message && BN_CLICKED == HIWORD(wParam))
		GetParent()->SendMessage(message, wParam, lParam);
	//GetParent()->PostMessage(message, wParam, lParam);
	return CCherryWnd::DefWindowProc(message, wParam, lParam);
}