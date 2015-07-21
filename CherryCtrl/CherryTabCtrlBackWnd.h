#pragma once
#include "CherryWnd.h"

class CCherryTabCtrlBackWnd : public CCherryWnd
{
// Constuctors
public:
	CCherryTabCtrlBackWnd();

// Overridables
protected:
	virtual void OnDrawCherry(CCherryMemDC *pDC);

	
// Implementation
public:
	virtual ~CCherryTabCtrlBackWnd();

protected:
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};