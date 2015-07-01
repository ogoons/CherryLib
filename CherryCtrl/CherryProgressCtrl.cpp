#include "stdafx.h"
#include "CherryProgressCtrl.h"

IMPLEMENT_DYNAMIC(CCherryProgressCtrl, CCherryWnd)

CCherryProgressCtrl::CCherryProgressCtrl()
{
	m_nPos = 0;
	m_nMaxPos = 0;
	m_dwCherryStyle = 0;
}

CCherryProgressCtrl::~CCherryProgressCtrl()
{
}

BEGIN_MESSAGE_MAP(CCherryProgressCtrl, CCherryWnd)
END_MESSAGE_MAP()

void CCherryProgressCtrl::OnDrawCherry(CCherryMemDC *pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	Graphics graphics(pDC->GetSafeHdc());
	graphics.SetInterpolationMode(InterpolationModeDefault);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);
	
	// ��� �׸���
	if (m_images[DRAW_TYPE_BACKGROUND].GetBitmapLastStatus() == Ok)
	{
		// Source ũ�⺸�� Client�� �۰ų� ���� ���� Client ũ��� ����Ѵ�.
		if ((int)m_images[DRAW_TYPE_BACKGROUND].GetWidth() >= clientRect.Width() &&
			(int)m_images[DRAW_TYPE_BACKGROUND].GetHeight() >= clientRect.Height())
			m_images[DRAW_TYPE_BACKGROUND].DrawImage(&graphics, clientRect);
		// Source ���� ū ��� 3x3 Ȯ���Ͽ� ����Ѵ�.
		else
			m_images[DRAW_TYPE_BACKGROUND].DrawStretchImage3x3(&graphics, clientRect);
	}

	// ���� ���¹� �׸���
	if (m_images[DRAW_TYPE_FILL].GetBitmapLastStatus() == Ok)
	{
		int nSourceLeft = 0, nSourceTop = 0;
		int nSourceWidth = 0, nSourceHeight = 0;
		CRect fillRect(clientRect);

		DWORD dwCherryStyle = GetCherryStyle();

		// �ٿ��� ���� ������ ���
		switch (dwCherryStyle)
		{
		case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
			if ((UINT)(nSourceLeft = m_images[DRAW_TYPE_FILL].GetWidth() - ((m_images[DRAW_TYPE_FILL].GetWidth() * m_nPos) / m_nMaxPos)) >= m_images[DRAW_TYPE_FILL].GetWidth())
				goto Exit; // 0�϶��� �׸��� ����

			nSourceWidth = (int)(m_images[DRAW_TYPE_FILL].GetWidth() - nSourceLeft);
			nSourceHeight = (int)m_images[DRAW_TYPE_FILL].GetHeight();
			fillRect.left = (fillRect.right - ((fillRect.right * m_nPos) / m_nMaxPos));
			break;
		case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
			if ((nSourceWidth = (int)((m_images[DRAW_TYPE_FILL].GetWidth() * m_nPos) / m_nMaxPos)) <= 0)
				goto Exit; // 0�϶��� �׸��� ����

			nSourceHeight = (int)m_images[DRAW_TYPE_FILL].GetHeight();
			fillRect.right = (fillRect.right * m_nPos) / m_nMaxPos;
			break;

		case STYLE_VERTICAL_BOTTOM_TO_TOP:
			if ((UINT)(nSourceTop = m_images[DRAW_TYPE_FILL].GetHeight() - ((m_images[DRAW_TYPE_FILL].GetHeight() * m_nPos) / m_nMaxPos)) >= m_images[DRAW_TYPE_FILL].GetHeight())
				goto Exit; // 0�϶��� �׸��� ����

			nSourceWidth = (int)m_images[DRAW_TYPE_FILL].GetWidth();
			nSourceHeight = (int)(m_images[DRAW_TYPE_FILL].GetHeight() - nSourceTop);
			fillRect.top = (fillRect.bottom - ((fillRect.bottom * m_nPos) / m_nMaxPos));
			break;
		case STYLE_VERTICAL_TOP_TO_BOTTOM:
			if ((nSourceHeight = (int)((m_images[DRAW_TYPE_FILL].GetHeight() * m_nPos) / m_nMaxPos)) <= 0)
				goto Exit; // 0�϶��� �׸��� ����

			nSourceWidth = (int)m_images[DRAW_TYPE_FILL].GetWidth();
			fillRect.bottom = (fillRect.bottom * m_nPos) / m_nMaxPos;
			break;
		default:
			ASSERT(0);
			break;
		}

		CCherryImage fillImage(m_images[DRAW_TYPE_FILL].GetBitmap(), nSourceLeft, nSourceTop, nSourceWidth, nSourceHeight, TRUE); // ĳ�õ� �̹���

		// Source ũ�⺸�� Client�� �۰ų� ���� ���� Client ũ��� ����Ѵ�.
		if ((int)m_images[DRAW_TYPE_FILL].GetWidth() >= clientRect.Width() &&
			(int)m_images[DRAW_TYPE_FILL].GetHeight() >= clientRect.Height())
			fillImage.DrawImage(&graphics, fillRect);
		// Source ���� ū ��� 3x3 Ȯ���Ͽ� ����Ѵ�.
		else
			fillImage.DrawStretchImage3x3(&graphics, fillRect);
	}

Exit:
	CString strText;
	GetWindowText(strText);

	if (!strText.IsEmpty())
		DrawText(&graphics, strText, clientRect);
}

int CCherryProgressCtrl::GetPos() const
{
	return m_nPos;
}

void CCherryProgressCtrl::SetPos(int nPos)
{
	// �ִ� �� ��ġ�� ����� �ִ� ������ ����
	if (nPos > m_nMaxPos)
		nPos = m_nMaxPos;
	else if (nPos < 0)
		nPos = 0;

	m_nPos = nPos;

	Invalidate(FALSE);
}

int CCherryProgressCtrl::GetMaxPos() const
{
	return m_nMaxPos;
}

CHERRY_RET CCherryProgressCtrl::SetMaxPos(int nMaxPos)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (nMaxPos < m_nPos)
			throw CCherryException::ERROR_PROGRESSCTRL_RANGE_CURRENT_POS_LOW;

		m_nMaxPos = nMaxPos;

		Invalidate(FALSE);

	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryProgressCtrl::Create(
	LPCTSTR lpszImagePath, 
	const short nMaxPos, 
	DWORD dwCherryStyle,
	DWORD dwStyle, 
	const RECT &rect, 
	CWnd *pParentWnd, UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (!CCherryWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID))
			throw CCherryException::ERROR_PROGRESSCTRL_CREATE_FAIL;

		UINT nWidth, nHeight;

		if (rect.right == 0)
			nWidth = 0;
		else
			nWidth = rect.right - rect.left;

		if (rect.bottom == 0)
			nHeight = 0;
		else
			nHeight = rect.bottom - rect.top;

		SetMaxPos(nMaxPos);
		ModifyCherryStyle(0, dwCherryStyle);

		cherryRet = SetImage(lpszImagePath, nWidth, nHeight);
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryProgressCtrl::SetImage(LPCTSTR lpszImagePath, int nWidth, int nHeight)
{	
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		CCherryImage sourceImage;
		if ((cherryRet = sourceImage.LoadImage(lpszImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		UINT nOrgWidth, nOrgHeight;
		DWORD dwCherryStyle = GetCherryStyle();

		switch (dwCherryStyle)
		{
		case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
		case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
			nOrgWidth = sourceImage.GetWidth();
			nOrgHeight = sourceImage.GetHeight() / 2;
			break;
		case STYLE_VERTICAL_BOTTOM_TO_TOP:
		case STYLE_VERTICAL_TOP_TO_BOTTOM:
			nOrgWidth = sourceImage.GetWidth() / 2;
			nOrgHeight = sourceImage.GetHeight();
			break;
		default:
			ASSERT(0);
			break;
		}

		Rect rect;
		// ���, ���� ���� �̹��� �߶� �ٿ��ֱ�
		for (UINT i = 0; i < DRAW_TYPE_COUNT; i++)
		{
			switch (dwCherryStyle)
			{
			case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
			case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
				rect = Rect(0, nOrgHeight * i, nOrgWidth, nOrgHeight);
				break;
			case STYLE_VERTICAL_BOTTOM_TO_TOP:
			case STYLE_VERTICAL_TOP_TO_BOTTOM:
				rect = Rect(nOrgWidth * i, 0, nOrgWidth, nOrgHeight);
				break;
			default:
				ASSERT(0);
				break;
			}

			if ((cherryRet = m_images[i].LoadImage(sourceImage.GetBitmap()->Clone(rect, PixelFormatDontCare))) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;
		}

		if (GetCherryStyle() & STYLE_AUTORESIZE)
		{
			// SLIDER_STYLE_AUTORESIZE �����Ǿ� �ִٸ� ������ Resize
			ResizeWindow(nOrgWidth, nOrgHeight);
		}
		else
		{
			CRect windowRect;
			GetWindowRect(&windowRect);
			GetParent()->ScreenToClient(&windowRect);

			UINT nWidth, nHeight;

			if (windowRect.Width() > 0)
				nWidth = windowRect.Width();
			else
				nWidth = 0;

			if (windowRect.Height() > 0)
				nHeight = windowRect.Height();
			else
				nHeight = 0;

			if (nWidth > 0 && nHeight > 0)
			{
				windowRect.right = windowRect.left + nWidth;
				windowRect.bottom = windowRect.top + nHeight;

				MoveWindow(windowRect);
			}
			else
			{
				// nWidth || nHeight ��� �ϳ��� 0�̸� Auto Resizing
				ResizeWindow(nOrgWidth, nOrgHeight);
			}
		}
		/*
			// ���� ���ٸ� ���� �̹��� ����� ����Ѵ�.
			// nWidth, nHeight �Ѿ�� ���� �ִٸ� �� ����� ����Ѵ�.
			if (nWidth <= 0)
			nWidth = nOrgWidth;

			if (nHeight <= 0)
			nHeight = nOrgHeight;

			ResizeWindow(nWidth, nHeight);
			*/
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

void CCherryProgressCtrl::ResizeWindow(UINT nWidth, UINT nHeight)
{
	CRect windowRect;
	GetWindowRect(&windowRect);
	GetParent()->ScreenToClient(&windowRect);

	windowRect.right = windowRect.left + nWidth;
	windowRect.bottom = windowRect.top + nHeight;

	MoveWindow(windowRect);
}

DWORD CCherryProgressCtrl::GetCherryStyle() const
{
	return m_dwCherryStyle;
}

void CCherryProgressCtrl::ModifyCherryStyle(DWORD dwRemove, DWORD dwAdd)
{
	m_dwCherryStyle &= ~dwRemove;
	m_dwCherryStyle |= dwAdd;
}

void CCherryProgressCtrl::EnableAutoResize(BOOL bEnable)
{
	if (bEnable)
		ModifyCherryStyle(0, STYLE_AUTORESIZE);
	else
		ModifyCherryStyle(STYLE_AUTORESIZE, 0);
}