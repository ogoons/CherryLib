#pragma once
#include "CherryWnd.h"

class CCherryTabBackWnd : public CCherryWnd
{
// Constuctors
public:
	CCherryTabBackWnd();

// Overridables
protected:
	virtual void OnDrawCherry(CCherryMemDC *pDC);

	
// Implementation
public:
	virtual ~CCherryTabBackWnd();

protected:
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};