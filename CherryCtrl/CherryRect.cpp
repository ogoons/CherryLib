#include "stdafx.h"
#include "CherryRect.h"

CCherryRect::CCherryRect()
{
}

CCherryRect::CCherryRect(int left, int top, int width, int height)
{
	CRect::SetRect(left, top, left + width, top + height);
}

CCherryRect::~CCherryRect()
{
}

void CCherryRect::SetRectangle(int left, int top, int width, int height) throw()
{
	CRect::SetRect(left, top, left + width, top + height);
}
