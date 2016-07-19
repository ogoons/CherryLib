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
	
	// 배경 그리기
	if (m_images[DRAW_TYPE_BACKGROUND].GetBitmapLastStatus() == Ok)
	{
		if ((UINT)clientRect.Width() > m_images[DRAW_TYPE_BACKGROUND].GetWidth() && 
			(UINT)clientRect.Height() > m_images[DRAW_TYPE_BACKGROUND].GetHeight())
			// Source 보다 큰 경우 3x3 확대하여 출력한다.
			m_images[DRAW_TYPE_BACKGROUND].Draw9PatchImage(&graphics, clientRect);
		else
			// Source 크기보다 Client가 작거나 같은 경우는 Client 크기로 출력한다.
			m_images[DRAW_TYPE_BACKGROUND].DrawImage(&graphics, clientRect);
	}

	// 진행 상태바 그리기
	if (m_images[DRAW_TYPE_FILL].GetBitmapLastStatus() == Ok)
	{
		int nSourceLeft = 0, nSourceTop = 0;
		int nSourceWidth = 0, nSourceHeight = 0;
		CRect fillRect(clientRect);

		DWORD dwCherryStyle = GetCherryStyle();

		// 붙여질 영역 사이즈 계산
		switch (dwCherryStyle)
		{
		case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
			if ((UINT)(nSourceLeft = m_images[DRAW_TYPE_FILL].GetWidth() - ((m_images[DRAW_TYPE_FILL].GetWidth() * m_nPos) / m_nMaxPos)) >= m_images[DRAW_TYPE_FILL].GetWidth())
				goto Exit; // 0일때는 그리지 말것

			nSourceWidth = (int)(m_images[DRAW_TYPE_FILL].GetWidth() - nSourceLeft);
			nSourceHeight = (int)m_images[DRAW_TYPE_FILL].GetHeight();
			fillRect.left = (fillRect.right - ((fillRect.right * m_nPos) / m_nMaxPos));
			break;
		case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
			if ((nSourceWidth = (int)((m_images[DRAW_TYPE_FILL].GetWidth() * m_nPos) / m_nMaxPos)) <= 0)
				goto Exit; // 0일때는 그리지 말것

			nSourceHeight = (int)m_images[DRAW_TYPE_FILL].GetHeight();
			fillRect.right = (fillRect.right * m_nPos) / m_nMaxPos;
			break;

		case STYLE_VERTICAL_BOTTOM_TO_TOP:
			if ((UINT)(nSourceTop = m_images[DRAW_TYPE_FILL].GetHeight() - ((m_images[DRAW_TYPE_FILL].GetHeight() * m_nPos) / m_nMaxPos)) >= m_images[DRAW_TYPE_FILL].GetHeight())
				goto Exit; // 0일때는 그리지 말것

			nSourceWidth = (int)m_images[DRAW_TYPE_FILL].GetWidth();
			nSourceHeight = (int)(m_images[DRAW_TYPE_FILL].GetHeight() - nSourceTop);
			fillRect.top = (fillRect.bottom - ((fillRect.bottom * m_nPos) / m_nMaxPos));
			break;
		case STYLE_VERTICAL_TOP_TO_BOTTOM:
			if ((nSourceHeight = (int)((m_images[DRAW_TYPE_FILL].GetHeight() * m_nPos) / m_nMaxPos)) <= 0)
				goto Exit; // 0일때는 그리지 말것

			nSourceWidth = (int)m_images[DRAW_TYPE_FILL].GetWidth();
			fillRect.bottom = (fillRect.bottom * m_nPos) / m_nMaxPos;
			break;
		default:
			ASSERT(0);
			break;
		}

		CCherryImage fillImage(m_images[DRAW_TYPE_FILL].GetBitmap(), nSourceLeft, nSourceTop, nSourceWidth, nSourceHeight, TRUE); // 캐시된 이미지

		if ((UINT)clientRect.Width() > m_images[DRAW_TYPE_FILL].GetWidth() &&
			(UINT)clientRect.Height() > m_images[DRAW_TYPE_FILL].GetHeight())
			fillImage.Draw9PatchImage(&graphics, fillRect);
		else
			fillImage.DrawImage(&graphics, fillRect);
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
	// 최대 값 위치를 벗어나면 최대 값으로 설정
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
		cherryRet = errorRet;
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
		cherryRet = errorRet;
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

		UINT nRawWidth, nRawHeight;
		DWORD dwCherryStyle = GetCherryStyle();

		switch (dwCherryStyle)
		{
		case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
		case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
			nRawWidth = sourceImage.GetWidth();
			nRawHeight = sourceImage.GetHeight() / 2;
			break;
		case STYLE_VERTICAL_BOTTOM_TO_TOP:
		case STYLE_VERTICAL_TOP_TO_BOTTOM:
			nRawWidth = sourceImage.GetWidth() / 2;
			nRawHeight = sourceImage.GetHeight();
			break;
		default:
			ASSERT(0);
			break;
		}

		Rect rect;
		// 배경, 진행 상태 이미지 잘라서 붙여넣기
		for (UINT i = 0; i < DRAW_TYPE_COUNT; i++)
		{
			switch (dwCherryStyle)
			{
			case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
			case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
				rect = Rect(0, nRawHeight * i, nRawWidth, nRawHeight);
				break;
			case STYLE_VERTICAL_BOTTOM_TO_TOP:
			case STYLE_VERTICAL_TOP_TO_BOTTOM:
				rect = Rect(nRawWidth * i, 0, nRawWidth, nRawHeight);
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
			// SLIDER_STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
			ResizeWindow(nRawWidth, nRawHeight);
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
				// nWidth || nHeight 어느 하나라도 0이면 Auto Resizing
				ResizeWindow(nRawWidth, nRawHeight);
			}
		}
		/*
			// 값이 없다면 원본 이미지 사이즈를 사용한다.
			// nWidth, nHeight 넘어온 값이 있다면 그 사이즈를 사용한다.
			if (nWidth <= 0)
			nWidth = nRawWidth;

			if (nHeight <= 0)
			nHeight = nRawHeight;

			ResizeWindow(nWidth, nHeight);
			*/
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
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
