// CherrySliderCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherrySliderCtrl.h"


// CCherrySliderCtrlThumbButton

IMPLEMENT_DYNAMIC(CCherrySliderCtrlThumbButton, CCherryButton)

CCherrySliderCtrlThumbButton::CCherrySliderCtrlThumbButton()
{
	m_bLButtonDown = FALSE;
}

CCherrySliderCtrlThumbButton::~CCherrySliderCtrlThumbButton()
{
}

BEGIN_MESSAGE_MAP(CCherrySliderCtrlThumbButton, CCherryButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CCherrySliderCtrlThumbButton::UseCachedImage(BOOL bUse)
{
	for (int i = STATUS_NORMAL; i < STATUS_MAX_COUNT; i++)
		m_images[i].UseCachedImage(bUse);
}

void CCherrySliderCtrlThumbButton::OnMouseMove(UINT nFlags, CPoint point)
{
	((CCherrySliderCtrl *)GetParent())->OnMouseMove(nFlags, point);

	//GetParent()->SendMessage(WM_MOUSEMOVE);
	CCherryButton::OnMouseMove(nFlags, point);
}

void CCherrySliderCtrlThumbButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLButtonDown = TRUE;

	CCherryButton::OnLButtonDown(nFlags, point);
}

void CCherrySliderCtrlThumbButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLButtonDown = FALSE;

	CCherryButton::OnLButtonUp(nFlags, point);
}


// CCherrySliderCtrl

IMPLEMENT_DYNAMIC(CCherrySliderCtrl, CCherryProgressCtrl)

CCherrySliderCtrl::CCherrySliderCtrl()
{
}

CCherrySliderCtrl::~CCherrySliderCtrl()
{
}

BEGIN_MESSAGE_MAP(CCherrySliderCtrl, CCherryProgressCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CCherrySliderCtrl 메시지 처리기입니다.

CHERRY_RET CCherrySliderCtrl::Create(LPCTSTR lpszThumbImagePath, LPCTSTR lpszTrackImagePath, const int nMaxPos, DWORD dwCherryStyle, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{	
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		switch (dwCherryStyle)
		{
			// 2x1 이미지를 사용하는 스타일이면
		case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
		case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
		case STYLE_VERTICAL_BOTTOM_TO_TOP:
		case STYLE_VERTICAL_TOP_TO_BOTTOM:
			{
				// 기존 CherryProgressCtrl 코드를 사용한다.
				if ((cherryRet = CCherryProgressCtrl::Create(lpszTrackImagePath, nMaxPos, dwCherryStyle,
					dwStyle, rect, pParentWnd, nID)) != CCherryException::ERROR_CHERRY_SUCCESS)
					throw cherryRet;

				// 버튼 생성
				if ((cherryRet = m_thumbButton.Create(NULL, lpszThumbImagePath, STYLE_AUTORESIZE,
					WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, nID + 1)) != CCherryException::ERROR_CHERRY_SUCCESS)
					throw cherryRet;

				m_thumbButton.UseCachedImage(); // 버튼이 자주 움직이지 캐시된 이미지를 사용하자.

				CRect clientRect;
				GetClientRect(&clientRect);

				// 반드시 버튼 생성 뒤에 호출
				RepositionThumbButton(clientRect.Width(), clientRect.Height());
			}
			break;
			// 1x1 스타일의 이미지라면
		case STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW:
		case STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW:
		case STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW:
		case STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW:
			{
				if (!CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID))
					throw CCherryException::ERROR_SLIDERCTRL_CREATE_FAIL;

				SetMaxPos(nMaxPos);	// CWnd로 생성했기에 수동으로 설정
				SetPos(nMaxPos);

				// CCherrySliderCtrl 전용 스타일 bit 플래그
				ModifyCherryStyle(0, dwCherryStyle);

				// 버튼 생성
				if ((cherryRet = m_thumbButton.Create(NULL, lpszThumbImagePath, STYLE_AUTORESIZE,
					WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, nID + 1)) != CCherryException::ERROR_CHERRY_SUCCESS)
					throw cherryRet;

				if ((cherryRet = SetTrackImage(lpszTrackImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
					throw cherryRet;
			}
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherrySliderCtrl::SetThumbImage(LPCTSTR lpszImagePath)
{
	return m_thumbButton.SetImage(lpszImagePath);
}

// NOT_FOLLOW 용 이미지 셋 함수
CHERRY_RET CCherrySliderCtrl::SetTrackImage(LPCTSTR lpszImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		// 배경 이미지 로드
		if ((cherryRet = m_trackImage.LoadImage(lpszImagePath, TRUE)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		UINT nOrgWidth = m_trackImage.GetWidth();
		UINT nOrgHeight = m_trackImage.GetHeight();

		if (GetCherryStyle() & STYLE_AUTORESIZE)
		{
			// SLIDER_STYLE_AUTORESIZE 설정되어 있다면 무조건 Resize
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

				// Thumb 버튼 높이를 가운데로 이동
				RepositionThumbButton(windowRect.Width(), windowRect.Height());
			}
			else
			{
				// nWidth || nHeight 어느 하나라도 0이면 Auto Resizing
				ResizeWindow(nOrgWidth, nOrgHeight);
			}
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

void CCherrySliderCtrl::OnDrawCherry(CCherryMemDC *pDC)
{
	// 2013-07-19 ogoons: 
	// CherryProgressCtrl과 코드 중복이지만 dc를 재활용하기엔 번거로우므로
	// 그냥 따로 코드를 분리하여 오버라이드 한다.
	CRect clientRect;
	GetClientRect(&clientRect);

	Graphics graphics(pDC->GetSafeHdc());
	graphics.SetInterpolationMode(InterpolationModeDefault);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	DWORD dwCherryStyle = GetCherryStyle();

	switch (dwCherryStyle)
	{
	// 2x1 이미지를 사용하는 스타일이면
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
	case STYLE_VERTICAL_BOTTOM_TO_TOP:
	case STYLE_VERTICAL_TOP_TO_BOTTOM:
	{
		// 배경 그리기
		if (m_images[DRAW_TYPE_BACKGROUND].GetBitmapLastStatus() == Ok)
		{
			if ((UINT)clientRect.Width() > m_images[DRAW_TYPE_BACKGROUND].GetWidth() && 
				(UINT)clientRect.Height() > m_images[DRAW_TYPE_BACKGROUND].GetHeight())
				// Source 보다 큰 경우 3x3 확대하여 출력한다.
				m_images[DRAW_TYPE_BACKGROUND].DrawStretchImage3x3(&graphics, clientRect);
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

			// 붙여질 영역 사이즈 계산
			switch (dwCherryStyle)
			{
			case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
				if ((UINT)(nSourceLeft = m_images[DRAW_TYPE_FILL].GetWidth() - ((m_images[DRAW_TYPE_FILL].GetWidth() * m_nPos) / m_nMaxPos)) >= m_images[DRAW_TYPE_FILL].GetWidth())
					goto done; // 0일때는 그리지 말것

				nSourceWidth = (int)(m_images[DRAW_TYPE_FILL].GetWidth() - nSourceLeft);
				nSourceHeight = (int)m_images[DRAW_TYPE_FILL].GetHeight();
				fillRect.left = (fillRect.right - ((fillRect.right * m_nPos) / m_nMaxPos));
				break;
			case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
				if ((nSourceWidth = (int)((m_images[DRAW_TYPE_FILL].GetWidth() * m_nPos) / m_nMaxPos)) <= 0)
					goto done; // 0일때는 그리지 말것

				nSourceHeight = (int)m_images[DRAW_TYPE_FILL].GetHeight();
				fillRect.right = (fillRect.right * m_nPos) / m_nMaxPos;
				break;
			case STYLE_VERTICAL_BOTTOM_TO_TOP:
				if ((UINT)(nSourceTop = m_images[DRAW_TYPE_FILL].GetHeight() - ((m_images[DRAW_TYPE_FILL].GetHeight() * m_nPos) / m_nMaxPos)) >= m_images[DRAW_TYPE_FILL].GetHeight())
					goto done; // 0일때는 그리지 말것

				nSourceWidth = (int)m_images[DRAW_TYPE_FILL].GetWidth();
				nSourceHeight = (int)(m_images[DRAW_TYPE_FILL].GetHeight() - nSourceTop);
				fillRect.top = (fillRect.bottom - ((fillRect.bottom * m_nPos) / m_nMaxPos));
				break;
			case STYLE_VERTICAL_TOP_TO_BOTTOM:
				if ((nSourceHeight = (int)((m_images[DRAW_TYPE_FILL].GetHeight() * m_nPos) / m_nMaxPos)) <= 0)
					goto done; // 0일때는 그리지 말것

				nSourceWidth = (int)m_images[DRAW_TYPE_FILL].GetWidth();
				fillRect.bottom = (fillRect.bottom * m_nPos) / m_nMaxPos;
				break;
			default:
				ASSERT(0);
				break;
			}

			CCherryImage fillImage(m_images[DRAW_TYPE_FILL].GetBitmap(), nSourceLeft, nSourceTop, nSourceWidth, nSourceHeight, TRUE);
			
			if ((UINT)clientRect.Width() > m_images[DRAW_TYPE_FILL].GetWidth() &&
				(UINT)clientRect.Height() > m_images[DRAW_TYPE_FILL].GetHeight())
				// Source 보다 큰 경우 3x3 확대하여 출력한다.
				fillImage.DrawStretchImage3x3(&graphics, fillRect);
			else
				// Source 크기보다 Client가 작거나 같은 경우는 Client 크기로 출력한다.
				fillImage.DrawImage(&graphics, fillRect);
		}
	}
	// 1x1 스타일의 이미지라면
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW:
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW:
	case STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW:
	case STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW:
		if (m_trackImage.IsLoadedImage())
			m_trackImage.DrawStretchImage3x3(&graphics, clientRect);
		break;
	default:
		ASSERT(0);
		break;
	}

done:
	m_thumbButton.SetBackground(pDC);
}

void CCherrySliderCtrl::ResizeWindow(UINT nWidth, UINT nHeight)
{
	CCherryProgressCtrl::ResizeWindow(nWidth, nHeight);

	// Thumb 버튼의 높이를 가운데로 이동
	RepositionThumbButton(nWidth, nHeight);
}

// Pos = 0을 기준으로 Reposition한다.
void CCherrySliderCtrl::RepositionThumbButton(int nTrackWidth, int nTrackHeight)
{
	CRect thumbButtonRect;
	m_thumbButton.GetClientRect(&thumbButtonRect);

	int nThumbButtonWidth = thumbButtonRect.Width();
	int nThumbButtonHeight = thumbButtonRect.Height();

	// Horizontal
	DWORD dwCherryStyle = GetCherryStyle();

	switch (dwCherryStyle)
	{
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW:
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW:
		{
			int nThumbButtonHeight = thumbButtonRect.Height();
			LONG lTop = (nTrackHeight - nThumbButtonHeight) / 2;	// 수직 가운데 정렬

			thumbButtonRect.top = lTop;
			thumbButtonRect.bottom = lTop + nThumbButtonHeight;

			switch (dwCherryStyle)
			{
			// Left to right
			case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
			case STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW:
				thumbButtonRect.left = nTrackWidth - nThumbButtonWidth;
				thumbButtonRect.right = nTrackWidth;
				break;
			// Right to left
			case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
			case STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW:
				thumbButtonRect.left = 0;
				thumbButtonRect.right = nThumbButtonWidth;
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		break;
	// Vertical
	case STYLE_VERTICAL_BOTTOM_TO_TOP:
	case STYLE_VERTICAL_TOP_TO_BOTTOM:
	case STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW:
	case STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW:
		{
			int nThumbButtonWidth = thumbButtonRect.Width();
			LONG lLeft = (nTrackWidth - nThumbButtonWidth) / 2;

			thumbButtonRect.left = lLeft;
			thumbButtonRect.right = lLeft + nThumbButtonWidth;
			
			switch (dwCherryStyle)
			{
			// Bottom to top
			case STYLE_VERTICAL_BOTTOM_TO_TOP:
			case STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW:
				thumbButtonRect.top = nTrackHeight - nThumbButtonHeight;
				thumbButtonRect.bottom = nTrackHeight;
				break;
			// Top to bottom
			case STYLE_VERTICAL_TOP_TO_BOTTOM:
			case STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW:
				thumbButtonRect.top = 0;
				thumbButtonRect.bottom = nThumbButtonHeight;
				break;
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	m_thumbButton.MoveWindow(thumbButtonRect);
}

void CCherrySliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!IsWindowEnabled())
		return;

	CRect clientRect;
	GetClientRect(clientRect);

	int nWidth = clientRect.Width();
	int nHeight = clientRect.Height();
	int nCurrentPos = ConvertCursorPosToCurrentPos(point);

	/*
	if (nPos > GetPos())
	nPos = GetPos() + 1;
	else if (nPos < GetPos())
	nPos = GetPos() - 1;
	else
	nPos -= 1;
	*/

	CRect thumbButtonRect = ConvertCurrentPosToThumbButtonRect(nCurrentPos);

	m_thumbButton.MoveWindow(thumbButtonRect);

	// 버튼 위치 이동과 동시에 drag가 가능하도록 thumb 버튼에 WM_LBUTTONDOWN 메시지 발생 시킴
	m_thumbButton.SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(point.x, point.y));

	SetPos(nCurrentPos);

	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), WM_CHERRY_SLIDERCTRL_POS_CHANGED), (WPARAM)GetSafeHwnd());

	CCherryProgressCtrl::OnLButtonDown(nFlags, point);
}

void CCherrySliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!IsWindowEnabled())
		return;

	if (m_thumbButton.IsLButtonDown())
	{
		CPoint cursorPoint;
		GetCursorPos(&cursorPoint);
		ScreenToClient(&cursorPoint);

		int nCurrentPos = ConvertCursorPosToCurrentPos(cursorPoint);
		CRect thumbButtonRect = ConvertCurrentPosToThumbButtonRect(nCurrentPos);

		m_thumbButton.MoveWindow(thumbButtonRect, FALSE);

		if (GetPos() != nCurrentPos) // 위치에 변화가 생겼다면
		{
			SetPos(nCurrentPos);
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), WM_CHERRY_SLIDERCTRL_POS_CHANGED), (LPARAM)GetSafeHwnd());
		}
	}

	CCherryProgressCtrl::OnMouseMove(nFlags, point);
}

void CCherrySliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// thumb 버튼에 WM_LBUTTONUP 메시지 발생 시킴
	m_thumbButton.SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(point.x, point.y));

	CCherryProgressCtrl::OnLButtonUp(nFlags, point);
}

int CCherrySliderCtrl::ConvertCursorPosToCurrentPos(CPoint point)
{
	CRect clientRect;
	GetClientRect(clientRect);

	int nClientWidth = clientRect.Width();
	int nClientHeight = clientRect.Height();

	float fPos = 0.0;
	DWORD dwCherryStyle = GetCherryStyle();

	switch (dwCherryStyle)
	{
	// To left
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW:
		fPos = (((float)(nClientWidth - point.x) / nClientWidth) * GetMaxPos()) - 1;	// rect위에 커서 위치를 상대적인 현재 pos 값으로 변환
		break;
	// To right
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW:
		fPos = (((float)point.x / nClientWidth) * GetMaxPos()) + 1;						// rect위에 커서 위치를 상대적인 현재 pos 값으로 변환
		break;
	// To top
	case STYLE_VERTICAL_BOTTOM_TO_TOP:
	case STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW:
		fPos = (((float)(nClientHeight - point.y) / nClientHeight) * GetMaxPos()) - 1;	// rect위에 커서 위치를 상대적인 현재 pos 값으로 변환
		break;
	// To bottom
	case STYLE_VERTICAL_TOP_TO_BOTTOM:
	case STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW:
		fPos = (((float)point.y / nClientHeight) * GetMaxPos()) + 1;					// rect위에 커서 위치를 상대적인 현재 pos 값으로 변환
		break;
	default:
		ASSERT(0);
		break;
	}

	int nPos;

	if ((int)(fPos * 10) % 10 < 5)
		nPos = (int)floor(fPos);
	else
		nPos = (int)ceil(fPos);

	switch (dwCherryStyle)
	{
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW:
	case STYLE_VERTICAL_BOTTOM_TO_TOP:
	case STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW:
		nPos += 1;
		break;
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW:
	case STYLE_VERTICAL_TOP_TO_BOTTOM:
	case STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW:
		nPos -= 1;
		break;
	default:
		ASSERT(0);
		break;
	}

	return nPos;
}

CRect CCherrySliderCtrl::ConvertCurrentPosToThumbButtonRect(int nCurrentPos)
{
	CRect clientRect;
	GetClientRect(clientRect);

	int nClientWidth = clientRect.Width();
	int nClientHeight = clientRect.Height();

	CRect thumbButtonRect;
	m_thumbButton.GetWindowRect(thumbButtonRect);
	ScreenToClient(thumbButtonRect);

	int nThumbButtonWidth = thumbButtonRect.Width();
	int nThumbButtonHeight = thumbButtonRect.Height();

	int nThumbButtonCenter = 0;
	int nThumbButtonPos = 0;

	DWORD dwCherryStyle = GetCherryStyle();
	
	/////////////////////////////////////////////////////
	// 1. 버튼 중앙 위치 가져오기
	switch (dwCherryStyle)
	{
	// Horizontal
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW:
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW:
		nThumbButtonCenter = thumbButtonRect.Width() / 2;
		break;
	// Vertical
	case STYLE_VERTICAL_BOTTOM_TO_TOP:
	case STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW:
	case STYLE_VERTICAL_TOP_TO_BOTTOM:
	case STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW:
		nThumbButtonCenter = thumbButtonRect.Height() / 2;
		break;
	default:
		ASSERT(0);
		break;
	}

	/////////////////////////////////////////////////////
	// 2. 버튼의 위치 계산
	switch (dwCherryStyle)
	{
	// To left
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW:
		nThumbButtonPos = ((nClientWidth * (GetMaxPos() - nCurrentPos)) / GetMaxPos()) - nThumbButtonCenter;
		break;
	// To right
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW:
		nThumbButtonPos = ((nClientWidth * nCurrentPos) / GetMaxPos()) - nThumbButtonCenter;
		break;
	// To top
	case STYLE_VERTICAL_BOTTOM_TO_TOP:
	case STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW:
		nThumbButtonPos = ((nClientHeight * (GetMaxPos() - nCurrentPos)) / GetMaxPos()) - nThumbButtonCenter;
		break;
	// To bottom
	case STYLE_VERTICAL_TOP_TO_BOTTOM:
	case STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW:
		nThumbButtonPos = ((nClientHeight * nCurrentPos) / GetMaxPos()) - nThumbButtonCenter;
		break;
	default:
		ASSERT(0);
		break;
	}

	/////////////////////////////////////////////////////
	// 3. 계산된 버튼 위치로 좌표 조정
	switch(dwCherryStyle)
	{
	// Horizontal
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT:
	case STYLE_HORIZONTAL_RIGHT_TO_LEFT_NOT_FOLLOW:
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT:
	case STYLE_HORIZONTAL_LEFT_TO_RIGHT_NOT_FOLLOW:
		thumbButtonRect.MoveToX(nThumbButtonPos);

		// 왼쪽 끝을 벗어나면
		if (0 > thumbButtonRect.left)
		{
			thumbButtonRect.left = 0;
			thumbButtonRect.right = nThumbButtonWidth;
		}
		// 오른쪽 끝을 벗어나면
		else if (nClientWidth < thumbButtonRect.right)
		{
			thumbButtonRect.left = nClientWidth - nThumbButtonWidth;
			thumbButtonRect.right = nClientWidth;
		}
		break;
	// Vertical
	case STYLE_VERTICAL_BOTTOM_TO_TOP:
	case STYLE_VERTICAL_BOTTOM_TO_TOP_NOT_FOLLOW:
	case STYLE_VERTICAL_TOP_TO_BOTTOM:
	case STYLE_VERTICAL_TOP_TO_BOTTOM_NOT_FOLLOW:
		thumbButtonRect.MoveToY(nThumbButtonPos);

		// 상단 끝을 벗어나면
		if (0 > thumbButtonRect.top)
		{
			thumbButtonRect.top = 0;
			thumbButtonRect.bottom = nThumbButtonHeight;
		}
		// 하단 끝을 벗어나면
		else if (nClientHeight < thumbButtonRect.bottom)
		{
			thumbButtonRect.top = nClientHeight - nThumbButtonHeight;
			thumbButtonRect.bottom = nClientHeight;
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	return thumbButtonRect;
}