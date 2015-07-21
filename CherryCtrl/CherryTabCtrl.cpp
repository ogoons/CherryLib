#include "stdafx.h"
#include "CherryTabCtrl.h"

IMPLEMENT_DYNAMIC(CCherryTabCtrl, CCherryWnd)

CCherryTabCtrl::CCherryTabCtrl()
{
	m_pHeadTab = NULL;
	m_dwCherryStyle = 0;
	m_nTabGap = 0;
	m_nCtrlIDCount = 0;
}

CCherryTabCtrl::~CCherryTabCtrl()
{
	for (vector<CCherryRadioButton *>::iterator it = m_tabVector.begin(); it != m_tabVector.end(); ++it)
	{
		((CCherryRadioButton *)*it)->DestroyWindow();
		delete (CCherryRadioButton *)*it;
	}
	
	/*
	// 생성된 페이지 해제
	vector<CCherryWnd *> *pPageVector = GetPageVector();

	dwSize = pPageVector->size();

	for (vector<CCherryWnd *>::iterator it = pPageVector->begin(); it != pPageVector->end(); ++it)
	{
		(CCherryWnd *)
		//((CCherryWnd *)*it)->DestroyWindow();
		//delete (CCherryWnd *)*it;
	}
	*/
}

BEGIN_MESSAGE_MAP(CCherryTabCtrl, CCherryWnd)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
//	ON_WM_PARENTNOTIFY()
END_MESSAGE_MAP()

CHERRY_RET CCherryTabCtrl::Create(LPCTSTR lpszTabImagePath, DWORD dwCherryStyle, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (!CCherryWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID))
			throw CCherryException::ERROR_TABCTRL_CREATE_FAIL;

		if (!m_tabCtrlBackWnd.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, nID + 1))
			throw CCherryException::ERROR_TABCTRL_CREATE_FAIL;
		
		ModifyCherryStyle(0, dwCherryStyle);

		m_lpszTabImagePath = lpszTabImagePath;

		m_nCtrlIDCount = nID + 1;
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

void CCherryTabCtrl::OnDrawCherry(CCherryMemDC *pDC)
{
}

CHERRY_RET CCherryTabCtrl::AddPage(CCherryWnd *pWnd)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (!GetSafeHwnd())
			throw CCherryException::ERROR_TABCTRL_NOT_CREATED;

		if (NULL == pWnd || !pWnd->GetSafeHwnd())
			throw CCherryException::ERROR_TABCTRL_CREATE_FAIL;

		// 캡션은 윈도우에서 가져오자
		CString strCaption;
		pWnd->GetWindowText(strCaption);
		pWnd->SetParent(this);

		// 버튼 할당
		CCherryRadioButton *pTab = new CCherryRadioButton();
		if ((cherryRet = pTab->Create(
			strCaption, 
			m_lpszTabImagePath,
			CCherryRadioButton::STYLE_AUTORESIZE |
			CCherryRadioButton::STYLE_AUTOCHECKBOX |
			CCherryRadioButton::STYLE_BUTTONTYPE,
			m_pHeadTab, 
			WS_CHILD | WS_VISIBLE, 
			CRect(), 
			&m_tabCtrlBackWnd,
			++m_nCtrlIDCount)) != CCherryException::ERROR_CHERRY_SUCCESS) // 탭 제거 시 컨트롤 아이디 생성에 대한 예외처리 필요
			throw cherryRet;

		// 탭 버튼 기본 스타일 설정
		pTab->SetNormalFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
		pTab->SetHoverFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
		pTab->SetDownFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
		pTab->SetDisableFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);

		if (NULL == m_pHeadTab)	// 첫 번째 버튼을 헤드로 할당
		{
			m_pHeadTab = pTab;
			m_pHeadTab->SetCheck(CCherryRadioButton::STATUS_CHECKED); // 첫 번째 버튼 디폴트 체크

			CRect clientRect;
			GetClientRect(clientRect);

			CRect tabRect;
			m_pHeadTab->GetClientRect(tabRect);

			CRect tabBackWndRect;
			switch (m_dwCherryStyle)
			{
			case STYLE_TAB_TOP:
				tabBackWndRect.SetRect(0, 0, clientRect.Width(), tabRect.Height());
				break;
			case STYLE_TAB_BOTTOM:
				tabBackWndRect.SetRect(0, clientRect.Height() - tabRect.Height(), clientRect.Width(), clientRect.Height());
				break;
			case STYLE_TAB_LEFT:
				tabBackWndRect.SetRect(0, 0, tabRect.Width(), clientRect.Height());
				break;
			case STYLE_TAB_RIGHT:
				tabBackWndRect.SetRect(clientRect.Width() - tabRect.Width(), 0, clientRect.Width(), clientRect.Height());
				break;
			default:
				ASSERT(0);
				break;
			}

			// 탭 배경 페이지 이동
			if (m_tabCtrlBackWnd.GetSafeHwnd())
				m_tabCtrlBackWnd.MoveWindow(tabBackWndRect);

			// 첫 번째 페이지 기본으로 보임
			if (!pWnd->IsWindowVisible())
				pWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			// 나머지 페이지는 기본 숨김
			pWnd->ShowWindow(SW_HIDE);
		}

		m_tabVector.push_back(pTab);
		m_pageVector.push_back(pWnd);

		ResizeWindow();
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

void CCherryTabCtrl::ResizeWindow()
{
	CRect tabBackWndRect;
	m_tabCtrlBackWnd.GetClientRect(tabBackWndRect);

	DWORD dwCount = (DWORD)m_tabVector.size();

	for (vector<CCherryRadioButton *>::iterator it = m_tabVector.begin(); it != m_tabVector.end(); ++it)
	{
		DWORD dwPos = it - m_tabVector.begin();
		CRect buttonRect;
		((CCherryRadioButton *)*it)->GetClientRect(buttonRect);

		int nLeft = 0;
		int nTop = 0;
		int nWidth = 0;
		int nHeight = buttonRect.Height();
		int nWidthGap = 0;
		int nHeightGap = 0;

		switch (m_dwCherryStyle)
		{
		case STYLE_TAB_TOP:
			nWidthGap = m_nTabGap;
			nWidth = buttonRect.Width();
			//nWidth = clientRect.Width() / dwCount;
			nLeft = nWidth * dwPos;
			break;
		case STYLE_TAB_BOTTOM:
			nWidthGap = m_nTabGap;
			nWidth = buttonRect.Width();
			//nWidth = clientRect.Width() / dwCount;
			nLeft = nWidth * dwPos;
			nTop = tabBackWndRect.bottom - nHeight;
			break;
		case STYLE_TAB_LEFT:
			nHeightGap = m_nTabGap;
			nHeight = buttonRect.Height();
			//nHeight = clientRect.Height() / dwCount;
			nTop = nHeight * dwPos;
			nWidth = buttonRect.Width();
			break;
		case STYLE_TAB_RIGHT:
			nHeightGap = m_nTabGap;
			nLeft = tabBackWndRect.Width() - buttonRect.Width();
			nHeight = buttonRect.Height();
			//nHeight = clientRect.Height() / dwCount;
			nTop = nHeight * dwPos;
			nWidth = buttonRect.Width();
			break;
		default:
			ASSERT(0);
			break;
		}

		((CCherryRadioButton *)*it)->MoveWindow(nLeft, nTop, nWidth - nWidthGap, nHeight - nHeightGap);
	}

	//if (m_tabCtrlBackWnd.GetSafeHwnd() && m_pHeadTab)
	{
		CRect clientRect;
		GetClientRect(clientRect);

		CRect pageRect;
		switch (m_dwCherryStyle)
		{
		case STYLE_TAB_TOP:
			pageRect.SetRect(0, tabBackWndRect.Height(), clientRect.Width(), clientRect.Height());
			break;
		case STYLE_TAB_BOTTOM:
			pageRect.SetRect(0, 0, clientRect.Width(), clientRect.Height() - tabBackWndRect.Height());
			break;
		case STYLE_TAB_LEFT:
			pageRect.SetRect(tabBackWndRect.Width(), 0, clientRect.Width(), clientRect.Height());
			break;
		case STYLE_TAB_RIGHT:
			pageRect.SetRect(0, 0, clientRect.Width() - tabBackWndRect.Width(), clientRect.Height());
			break;
		default:
			ASSERT(0);
			break;
		}

		for (vector<CCherryWnd *>::iterator it = m_pageVector.begin(); it != m_pageVector.end(); ++it)
			((CCherryWnd *)*it)->MoveWindow(pageRect);
	}
}

DWORD CCherryTabCtrl::GetCurPage()
{
	for (vector<CCherryRadioButton *>::iterator it = m_tabVector.begin(); it != m_tabVector.end(); ++it)
	{
		DWORD dwPos = it - m_tabVector.begin();
		if (((CCherryRadioButton *)*it)->GetCheck() == CCherryRadioButton::STATUS_CHECKED)
			return dwPos;
	}

	return 0;
}

void CCherryTabCtrl::SetCurPage(DWORD dwIndex)
{
	if (m_tabVector.empty() || m_tabVector.size() < dwIndex)
		return;

	// 탭 버튼 체크
	m_tabVector.at(dwIndex)->SetCheck(CCherryRadioButton::STATUS_CHECKED);
	
	// 해당 윈도우 보이기
	for (vector<CCherryWnd *>::iterator it = m_pageVector.begin(); it != m_pageVector.end(); ++it)
	{
		DWORD dwPos = it - m_pageVector.begin();
		if (dwIndex == dwPos)
		{
			((CCherryWnd *)*it)->ShowWindow(SW_SHOW);

			continue;
		}

		((CCherryWnd *)*it)->ShowWindow(SW_HIDE);
	}

	// 점검 필요
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(dwIndex, WM_CHERRY_TABCTRL_PAGE_CHANGED), (LPARAM)GetSafeHwnd());
}

DWORD CCherryTabCtrl::GetCherryStyle() const	
{
	return m_dwCherryStyle;
}

void CCherryTabCtrl::ModifyCherryStyle(DWORD dwRemove, DWORD dwAdd)
{
	m_dwCherryStyle &= ~dwRemove;
	m_dwCherryStyle |= dwAdd;
}

void CCherryTabCtrl::SetTabGap(UINT nGap)
{
	m_nTabGap = nGap;
	ResizeWindow();
}

vector<CCherryRadioButton *> *CCherryTabCtrl::GetTabVector()
{
	return &m_tabVector;
}

vector<CCherryWnd *> *CCherryTabCtrl::GetPageVector()
{
	return &m_pageVector;
}

CCherryRadioButton *CCherryTabCtrl::GetTab(UINT nIndex) const
{
	if (m_tabVector.empty())
		return NULL;

	return m_tabVector.at(nIndex);
}

CCherryWnd *CCherryTabCtrl::GetPage(UINT nIndex) const
{
	if (m_pageVector.empty())
		return NULL;

	return m_pageVector.at(nIndex);
}

DWORD CCherryTabCtrl::GetPageCount() const
{
	return (DWORD)m_pageVector.size();
}

void CCherryTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CCherryWnd::OnSize(nType, cx, cy);

	if (m_tabCtrlBackWnd.GetSafeHwnd())
		ResizeWindow();
}

LRESULT CCherryTabCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == message && BN_CLICKED == HIWORD(wParam))
	{
		UINT nCtrlID = LOWORD(wParam);
		for (vector<CCherryRadioButton *>::iterator it = m_tabVector.begin(); it != m_tabVector.end(); ++it)
		{
			if (((CCherryRadioButton *)*it)->GetSafeHwnd())
			{
				if (nCtrlID == ((CCherryRadioButton *)*it)->GetDlgCtrlID())
				{
					DWORD dwPos = it - m_tabVector.begin();
					SetCurPage(dwPos);
				}
			}
		}
	}

	return CCherryWnd::DefWindowProc(message, wParam, lParam);
}

DWORD CCherryTabCtrl::DeleteTabPage(DWORD dwIndex)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (m_pageVector.empty() || (m_pageVector.size() - 1) < dwIndex)
			throw CCherryException::ERROR_TABCTRL_OUT_OF_BOUND;

		// 탭 삭제
		vector<CCherryRadioButton *>::iterator it = m_tabVector.begin() + dwIndex;
		((CCherryRadioButton *)*it)->DestroyWindow();
		delete (CCherryRadioButton *)*it;
		m_tabVector.erase(it);

		// 페이지 삭제
		m_pageVector.at(dwIndex)->DestroyWindow();
		m_pageVector.erase(m_pageVector.begin() + dwIndex);

		// 탭 그룹에서 그룹장 새로 갱신
		if (m_pageVector.empty())
			m_pHeadTab = NULL;
		else
			m_pHeadTab = (CCherryRadioButton *)*m_tabVector.begin();

		// 최종적으로 체크된 탭 페이지를 선택
		DWORD dwCurPage = GetCurPage();
		SetCurPage(dwCurPage);

		ResizeWindow();
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

DWORD CCherryTabCtrl::DeleteTabPageAll()
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (!GetSafeHwnd())
			throw CCherryException::ERROR_TABCTRL_NOT_CREATED;

		for (vector<CCherryRadioButton *>::iterator it = m_tabVector.begin(); it != m_tabVector.end(); ++it)
		{
			// 버튼 제거
			((CCherryRadioButton *)*it)->DestroyWindow();
			delete (CCherryRadioButton *)*it;
		}

		m_tabVector.clear();

		for (vector<CCherryWnd *>::iterator it = m_pageVector.begin(); it != m_pageVector.end(); ++it)
		{
			// 페이지 제거
			((CCherryWnd *)*it)->DestroyWindow();
		}

		m_pageVector.clear();

		m_pHeadTab = NULL;

		ResizeWindow();
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

void CCherryTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{


	CCherryWnd::OnMouseMove(nFlags, point);
}

BOOL CCherryTabCtrl::PreTranslateMessage(MSG* pMsg)
{
	// 메시지 가로채기
	CPoint clientPoint(pMsg->pt);
	ScreenToClient(&clientPoint);

	CRect clientRect;
	GetClientRect(clientRect);

	if (WM_MOUSEMOVE == pMsg->message && 
		clientRect.PtInRect(clientPoint) && 
		m_pHeadTab)
	{
		CRect buttonRect;
		m_pHeadTab->GetClientRect(buttonRect);

		CRect firstRect(clientRect);	// 첫 번째 영역 구성을 위한 객체
		CRect secondRect(clientRect);	// 두 번째 영역 구성을 위한 객체

		const int nActionRect = 5;

		switch (m_dwCherryStyle)
		{
		case STYLE_TAB_TOP:
			firstRect.bottom = buttonRect.Height();
			firstRect.right = clientRect.Width() * nActionRect / 100;

			secondRect.bottom = buttonRect.Height();
			secondRect.left = clientRect.Width() - (clientRect.Width() * nActionRect / 100);
			break;
		case STYLE_TAB_BOTTOM:
			firstRect.top = clientRect.Height() - buttonRect.Height();
			firstRect.right = clientRect.Width() * nActionRect / 100;

			secondRect.top = clientRect.Height() - buttonRect.Height();
			secondRect.left = clientRect.Width() - (clientRect.Width() * nActionRect / 100);
			break;
		case STYLE_TAB_LEFT:
			firstRect.right = buttonRect.Width();
			firstRect.bottom = clientRect.Height() * nActionRect / 100;

			secondRect.right = buttonRect.Width();
			secondRect.top = clientRect.Height() - (clientRect.Height() * nActionRect / 100);
			break;
		case STYLE_TAB_RIGHT:
			firstRect.left = clientRect.Width() - buttonRect.Width();
			firstRect.bottom = clientRect.Height() * nActionRect / 100;

			secondRect.left = clientRect.Width() - buttonRect.Width();
			secondRect.top = clientRect.Height() - (clientRect.Height() * nActionRect / 100);
			break;
		default:
			ASSERT(0);
			break;
		}

		// First rect
		if (TRUE == firstRect.PtInRect(clientPoint))
		{
			AfxMessageBox(_T("first"));
		}
		// Second rect
		else if (TRUE == secondRect.PtInRect(clientPoint))
		{
			AfxMessageBox(_T("second"));
		}
	}

	return CCherryWnd::PreTranslateMessage(pMsg);
}

LRESULT CCherryTabCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	/*
	if (WM_PARENTNOTIFY == message && BN_CLICKED == HIWORD(wParam))
	{
		CPoint pos;
		GetCursorPos(&pos);

		CWnd *pWnd = WindowFromPoint(pos);
		::GetDlgCtrlID(pWnd->GetSafeHwnd());

		UINT nCtrlID = ::GetDlgCtrlID(pWnd->GetSafeHwnd());
		//UINT nCtrlID = LOWORD(wParam);
		for (vector<CCherryRadioButton *>::iterator it = m_tabVector.begin(); it != m_tabVector.end(); ++it)
		{
			if (((CCherryRadioButton *)*it)->GetSafeHwnd())
			{
				if (nCtrlID == ((CCherryRadioButton *)*it)->GetDlgCtrlID())
				{
					DWORD dwPos = it - m_tabVector.begin();
					SetCurPage(dwPos);
				}
			}
		}
	}
	*/

	return CCherryWnd::WindowProc(message, wParam, lParam);
}
