#include "stdafx.h"
#include "CherryTabCtrlBackWnd.h"
#include "CherryTabCtrl.h"

CCherryTabCtrlBackWnd::CCherryTabCtrlBackWnd()
{
}

CCherryTabCtrlBackWnd::~CCherryTabCtrlBackWnd()
{
}

void CCherryTabCtrlBackWnd::OnDrawCherry(CCherryMemDC *pDC)
{
}

LRESULT CCherryTabCtrlBackWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//CCherryTabCtrl *pTabCtrl = (CCherryTabCtrl *)GetParent();
	if (WM_COMMAND == message && BN_CLICKED == HIWORD(wParam))
		GetParent()->SendMessage(message, wParam, lParam);
	//GetParent()->PostMessage(message, wParam, lParam);
	return CCherryWnd::DefWindowProc(message, wParam, lParam);
}