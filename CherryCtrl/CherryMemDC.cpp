#include "stdafx.h"
#include "CherryMemDC.h"

CCherryMemDC::CCherryMemDC(CDC *pDC, BOOL bDrawOut)
{
	Initialize(pDC, NULL, bDrawOut);
}

CCherryMemDC::CCherryMemDC(CDC *pDC, CRect &rect, BOOL bDrawOut)
{
	Initialize(pDC, &rect, bDrawOut);
}

CCherryMemDC::~CCherryMemDC()
{
	if (m_bMemDC) 
	{
		if (m_bDrawOut)
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), this, m_rect.left, m_rect.top, SRCCOPY);            
		
		SelectObject(m_pOldBitmap); 
		//DeleteDC();
	} 
	else 
	{           
		m_hDC = NULL;
		m_hAttribDC = NULL;
	}     
}

void CCherryMemDC::Initialize(CDC *pDC, const CRect *pRect, BOOL bDrawOut)
{
	m_pDC = pDC;
	m_pOldBitmap = NULL;
	m_bMemDC = !pDC->IsPrinting(); // DC가 인쇄를 위하여 사용되는지 검사하는 함수
	m_bDrawOut = bDrawOut;

	if (m_bMemDC)
	{
		CreateCompatibleDC(pDC);

		if (NULL == pRect)
			pDC->GetClipBox(&m_rect);
		else
			m_rect = *pRect;

		// 메모리 비트맵을 화면 표시할 영역으로 만들고, 이를 선택합니다.
		m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
		m_pOldBitmap = SelectObject(&m_bitmap);

		SetWindowOrg(m_rect.left, m_rect.top);

		// Fill background, 배경을 전달 받은 매개변수 DC 의 배경색으로 채운다.
		FillSolidRect(m_rect, pDC->GetBkColor());	
	}
	else
	{
		m_bPrinting = pDC->m_bPrinting;
		m_hDC		= pDC->m_hDC;
		m_hAttribDC = pDC->m_hAttribDC;
	}
}

CCherryMemDC *CCherryMemDC::operator->()
{
	return this;
}   
 
CCherryMemDC::operator CCherryMemDC*() 
{
	return this;
}

BOOL CCherryMemDC::Draw(CDC *pDC)
{
	return pDC->BitBlt(0, 0, m_rect.Width(), m_rect.Height(), this, m_rect.left, m_rect.top, SRCCOPY);
}

void CCherryMemDC::EnableDrawOut(BOOL bDrawOut)
{
	m_bDrawOut = bDrawOut;
}