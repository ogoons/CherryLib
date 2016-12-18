// CherryListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryListCtrl.h"


// CCherryListCtrl

IMPLEMENT_DYNAMIC(CCherryListCtrl, CListCtrl)

CCherryListCtrl::CCherryListCtrl()
{
}

CCherryListCtrl::~CCherryListCtrl()
{
}

BEGIN_MESSAGE_MAP(CCherryListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CCherryListCtrl::OnNMCustomdraw)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CCherryListCtrl 메시지 처리기입니다.
CHERRY_RET CCherryListCtrl::Create(
	LPCTSTR lpszHeaderImagePath,
	LPCTSTR lpszItemImagePath, 
	UINT nHeaderHeight,
	UINT nItemHeight,
	DWORD dwCherryStyle, 
	DWORD dwStyle, 
	const CCherryRect &cherryRect,
	CWnd *pParentWnd, 
	UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		//dwStyle |= LVS_REPORT | LVS_OWNERDATA | LVS_AUTOARRANGE | LVS_SHAREIMAGELISTS | LVS_ICON | LVS_EX_TRACKSELECT;
		dwStyle |= LVS_REPORT | LVS_EX_TRACKSELECT;

		if (!CListCtrl::Create(dwStyle, cherryRect, pParentWnd, nID))
			throw CCherryException::ERROR_LISTCTRL_CREATE_FAIL;

		//ModifyStyle(0, LVS_OWNERDATA | LVS_AUTOARRANGE | LVS_SHAREIMAGELISTS | LVS_ICON | LVS_EX_TRACKSELECT);
		SetExtendedStyle(LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/);
		
		SetItemHeight(nItemHeight);

		// 헤더가 없는 경우
		if (dwStyle & LVS_NOCOLUMNHEADER)
		{
			cherryRet = SetItemImage(lpszItemImagePath);
		}
		else
		{
			//CHeaderCtrl *pHeaderCtrl = (CHeaderCtrl *)GetHeaderCtrl();
			m_headerCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

			if ((cherryRet = m_headerCtrl.Create(lpszHeaderImagePath, WS_VISIBLE | WS_CHILD, CRect(), this, nID + 1)) != CCherryException::ERROR_CHERRY_SUCCESS)
				throw cherryRet;

			// Initialize header height
			HDLAYOUT  headerLayout;
			WINDOWPOS windowPos;
			RECT      rect;

			// Reposition the header control so that it is placed at 
			// the top of its parent window's client area.
			m_headerCtrl.GetParent()->GetClientRect(&rect);
			headerLayout.prc = &rect;
			headerLayout.pwpos = &windowPos;
			m_headerCtrl.Layout(&headerLayout);
			windowPos.cy = nHeaderHeight; // 헤더 높이 수정

			m_headerCtrl.SetWindowPos(
				CWnd::FromHandle(windowPos.hwndInsertAfter),
				windowPos.x,
				windowPos.y,
				windowPos.cx,
				windowPos.cy,
				windowPos.flags | SWP_SHOWWINDOW);

			// 드래그로 인한 헤더 크기 변경 시 대응
			SetHeaderHeight(nHeaderHeight);

			cherryRet = SetImage(lpszHeaderImagePath, lpszItemImagePath);
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

BOOL CCherryListCtrl::SetHeaderHeight(UINT nHeight)
{
	if (!m_headerCtrl.GetSafeHwnd())
		return FALSE;
	
	m_headerCtrl.SetHeight(nHeight);

	return TRUE;
}

CHERRY_RET CCherryListCtrl::SetItemHeight(UINT nHeight)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (nHeight == 0)
			return CCherryException::ERROR_CHERRY_FAILURE;

		// 리스트 아이템 높이 조절
		CImageList gapImageList;
		gapImageList.Create(1, nHeight, ILC_COLORDDB, 0, 0); // 2번째 파라미터로 리스트 아이템 높이 조절
		SetImageList(&gapImageList, LVSIL_SMALL);
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryListCtrl::SetImage(LPCTSTR lpszHeaderImagePath, LPCTSTR lpszItemImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (!GetSafeHwnd())
			return CCherryException::ERROR_LISTCTRL_NOT_CREATED;

		if ((cherryRet = SetHeaderImage(lpszHeaderImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
			return cherryRet;

		cherryRet = SetItemImage(lpszItemImagePath);
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryListCtrl::SetHeaderImage(LPCTSTR lpszImagePath)
{
	return m_headerCtrl.SetImage(lpszImagePath);
}

CHERRY_RET CCherryListCtrl::SetItemImage(LPCTSTR lpszImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		// 원본 이미지 로드
		CCherryImage sourceImage;
		if ((cherryRet = sourceImage.LoadImage(lpszImagePath)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;

		UINT nRawWidth = sourceImage.GetWidth();
		UINT nRawHeight = sourceImage.GetHeight() / ITEM_STATUS_MAX_COUNT;

		Rect itemImageRect[ITEM_STATUS_MAX_COUNT];

		// 각 상태 이미지 잘라서 붙여넣기
		for (UINT i = ITEM_STATUS_NORMAL; i < ITEM_STATUS_MAX_COUNT; i++)
		{
			itemImageRect[i] = Gdiplus::Rect(0, nRawHeight * i, nRawWidth, nRawHeight);

			if ((cherryRet = m_itemImage[i].LoadImage(sourceImage.GetBitmap()->Clone(itemImageRect[i], PixelFormatDontCare), TRUE)) != CCherryException::ERROR_CHERRY_SUCCESS) // 캐시된 이미지
				return cherryRet;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

int CCherryListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void CCherryListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	*pResult = CDRF_DODEFAULT;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		// Report 스타일이면
		if (GetStyle() & LVS_REPORT)
		{
			// 아이템과 헤더를 제외한 client 영역이 갱신을 위한 코드
			CRect clientRect, headerRect;
			GetClientRect(&clientRect);
			GetHeaderCtrl()->GetClientRect(headerRect);

			CRgn clientRgn;
			CRgn headerRgn;
			clientRgn.CreateRectRgn(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);	// 클라이언트 영역
			headerRgn.CreateRectRgn(headerRect.left, headerRect.top, headerRect.right, headerRect.bottom);	// 헤더 영역
			clientRgn.CombineRgn(&clientRgn, &headerRgn, RGN_DIFF);	// 헤더 영역 제거

			CRect boundsRect;
			CRgn boundsRgn;
			for (int i = 0; i <= GetItemCount(); i++)	// 아이템 갯수만큼 아이템 영역을 가져와서 client영역에서 제외한다.
			{
				boundsRgn.CreateRectRgn(boundsRect.left, boundsRect.top, boundsRect.right, boundsRect.bottom);
				GetItemRect(i, &boundsRect, LVIR_BOUNDS);
				clientRgn.CombineRgn(&clientRgn, &boundsRgn, RGN_DIFF);
				boundsRgn.DeleteObject();
			}

			CDC *pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			CBrush brush(RGB(255, 255, 255)); // 바탕은 하얀색으로 채운다.
			pDC->FillRgn(&clientRgn, &brush);

			//*pResult = CDRF_NOTIFYPOSTERASE | CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYSUBITEMDRAW; // 모든 스테이지에서 메시지를 받으려면
			*pResult = CDRF_NOTIFYITEMDRAW;
		}
		break;
	/*
	case CDDS_PREERASE:
		break;
	case CDDS_POSTERASE:
		break;
	case CDDS_ITEMPREERASE:

		break;
	case CDDS_ITEMPOSTERASE:
		break;
	*/
	case CDDS_ITEMPREPAINT:
		// Report 스타일이면
		if (GetStyle() & LVS_REPORT)
		{
			CRect clientRect;
			GetClientRect(&clientRect);

			//int nItem = (int)pLVCD->nmcd.dwItemSpec;
			//CRect itemRect;
			//GetItemRect(nItem, &itemRect, LVIR_BOUNDS);

			int nItem = (int)pLVCD->nmcd.dwItemSpec;
			CRect itemRect(pLVCD->nmcd.rc);

			CDC *pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			CCherryMemDC memDC(pDC, itemRect);
			Graphics graphics(memDC.GetSafeHdc());

			LVITEM lvItem = { 0, };
			lvItem.mask = LVIF_STATE;
			lvItem.iItem = nItem; // 아이템 인덱스
			lvItem.stateMask = LVIS_SELECTED;
			GetItem(&lvItem);

			UINT nItemState = lvItem.state;

			CCherryImage *pCurItemImage = NULL;
			if (nItemState & LVIS_SELECTED)
				pCurItemImage = &m_itemImage[ITEM_STATUS_SELECTED];
			else
				pCurItemImage = &m_itemImage[ITEM_STATUS_NORMAL];

			if (pCurItemImage)
				pCurItemImage->Draw9PatchImage(&graphics, itemRect);

			// 텍스트 뿌리기
			TCHAR szItemText[256] = { 0, };
			CRect headerRect;
			CRect subItemRect;
			for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
			{
				ZeroMemory(&lvItem, sizeof(LVITEM));
				ZeroMemory(&szItemText, sizeof(szItemText));

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nItem; // 아이템 인덱스
				lvItem.iSubItem = i;
				lvItem.pszText = szItemText;
				lvItem.cchTextMax = _countof(szItemText);
				GetItem(&lvItem);

				GetHeaderCtrl()->GetItemRect(i, headerRect);
				GetSubItemRect(nItem, i, LVIR_BOUNDS, subItemRect);
				subItemRect.right = headerRect.right; // 헤더 right를 빌려서 적용한다.

				// 외곽선 그려서 영역 확인용
				//Pen pen(Color(255, 255, 0, 0));
				//graphics.DrawRectangle(&pen, Rect(subItemRect.left, subItemRect.top, subItemRect.right, subItemRect.bottom));
				
				if (nItemState & LVIS_SELECTED) // Selected
					SetCurrentFont(GetSelectedFont());
				else if (!IsWindowEnabled())	// Disbled
					SetCurrentFont(GetDisableFont());
				else							// Normal
					SetCurrentFont(GetNormalFont());

				if (subItemRect.Width() > 0 && subItemRect.Height() > 0)
					GetCurrentFont()->DrawText(&graphics, lvItem.pszText, subItemRect);
			}

			*pResult = CDRF_SKIPDEFAULT;
		}
		break;
	/*
	case CDDS_ITEMPOSTPAINT:
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		break;
	case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM:
		break;
	case CDDS_POSTPAINT:

		//*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	*/
	default:
		*pResult = CDRF_DODEFAULT;
		//OnNMCustomdraw(pNMHDR, pResult);
		break;
	}
}

BOOL CCherryListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE; // 깜박임 방지 처리

	//return CListCtrl::OnEraseBkgnd(pDC);
}

void CCherryListCtrl::PreSubclassWindow()
{
	//ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	//CHeaderCtrl *pHeaderCtrl = GetHeaderCtrl();
	//CCherryListCtrl *pHeaderCtrl = (CCherryListCtrl *)GetHeaderCtrl();

	//CHeaderCtrl *pHeaderCtrl = (CHeaderCtrl *)GetHeaderCtrl();
	//m_headerCtrl.SubclassWindow(pHeaderCtrl->m_hWnd);
	//m_headerCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	CListCtrl::PreSubclassWindow();
}

void CCherryListCtrl::SetCurrentFont(CCherryFont *pStatusFont)
{
	m_pCurrentFont = pStatusFont;
}

CCherryFont	*CCherryListCtrl::GetCurrentFont() const
{
	return m_pCurrentFont;
}

void CCherryListCtrl::GetFontByStatus(CCherryFont &normalFont, CCherryFont &selectedFont, CCherryFont &disableFont) const
{
	normalFont = m_normalFont;
	selectedFont = m_selectedFont;
	disableFont = m_disableFont;
}

CCherryFont	*CCherryListCtrl::GetNormalFont()
{
	return &m_normalFont;
}

CCherryFont	*CCherryListCtrl::GetSelectedFont()
{
	return &m_selectedFont;
}

CCherryFont	*CCherryListCtrl::GetDisableFont()
{
	return &m_disableFont;
}

void CCherryListCtrl::SetFontByStatus(CCherryFont *pNormalFont, CCherryFont *pSelectedFont, CCherryFont *pDisableFont)
{
	SetNormalFont(pNormalFont);
	SetSelectedFont(pSelectedFont);
	SetDisableFont(pDisableFont);
}

void CCherryListCtrl::SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	SetNormalFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
	SetSelectedFont(lpszFontName, fFontSize, fontColor, dwFontStyle);

	// Disable Gray 처리
	COLORREF disableColorRef = GetSysColor(COLOR_GRAYTEXT);
	Color disableColor(GetRValue(disableColorRef), GetGValue(disableColorRef), GetBValue(disableColorRef));
	SetDisableFont(lpszFontName, fFontSize, disableColor, dwFontStyle);
}

BOOL CCherryListCtrl::SetCherryFont(CCherryFont *pCherryFont)
{
	if (NULL == pCherryFont)
		return FALSE;

	SetCherryFont(pCherryFont->GetFontName(), pCherryFont->GetFontSize(), pCherryFont->GetFontColor(), pCherryFont->GetFontStyle());

	return TRUE;
}

//////////////////////////////
// Normal font
void CCherryListCtrl::SetNormalFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	m_normalFont.SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);

	// 노말 폰트 설정시 현재 폰트로 설정하여 화면에 표시함
	SetCurrentFont(&m_normalFont);
}

BOOL CCherryListCtrl::SetNormalFont(CCherryFont *pNormalFont)
{
	if (NULL == pNormalFont)
		return FALSE;

	SetNormalFont(pNormalFont->GetFontName(), pNormalFont->GetFontSize(), pNormalFont->GetFontColor(), pNormalFont->GetFontStyle());

	return TRUE;
}

void CCherryListCtrl::SetNormalFontName(LPCTSTR lpszFontName)
{
	m_normalFont.SetFontName(lpszFontName);
	SetCurrentFont(&m_normalFont);
}

void CCherryListCtrl::SetNormalFontSize(float fFontSize)
{
	m_normalFont.SetFontSize(fFontSize);
	SetCurrentFont(&m_normalFont);
}

void CCherryListCtrl::SetNormalFontColor(Color fontColor)
{
	m_normalFont.SetFontColor(fontColor);
	SetCurrentFont(&m_normalFont);
}

void CCherryListCtrl::SetNormalFontStyle(DWORD dwFontStyle)
{
	m_normalFont.SetFontStyle(dwFontStyle);
	SetCurrentFont(&m_normalFont);
}

//////////////////////////////
// Selected font
void CCherryListCtrl::SetSelectedFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	m_selectedFont.SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
}

BOOL CCherryListCtrl::SetSelectedFont(CCherryFont *pSelectedFont)
{
	if (NULL == pSelectedFont)
		return FALSE;

	SetSelectedFont(pSelectedFont->GetFontName(), pSelectedFont->GetFontSize(), pSelectedFont->GetFontColor(), pSelectedFont->GetFontStyle());

	return TRUE;
}

void CCherryListCtrl::SetSelectedFontName(LPCTSTR lpszFontName)
{
	m_selectedFont.SetFontName(lpszFontName);
}

void CCherryListCtrl::SetSelectedFontSize(float fFontSize)
{
	m_selectedFont.SetFontSize(fFontSize);
}

void CCherryListCtrl::SetSelectedFontColor(Color fontColor)
{
	m_selectedFont.SetFontColor(fontColor);
}

void CCherryListCtrl::SetSelectedFontStyle(DWORD dwFontStyle)
{
	m_selectedFont.SetFontStyle(dwFontStyle);
}

//////////////////////////////
// Disable font
void CCherryListCtrl::SetDisableFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle)
{
	m_disableFont.SetCherryFont(lpszFontName, fFontSize, fontColor, dwFontStyle);
}

BOOL CCherryListCtrl::SetDisableFont(CCherryFont *pDisableFont)
{
	if (NULL == pDisableFont)
		return FALSE;

	SetDisableFont(pDisableFont->GetFontName(), pDisableFont->GetFontSize(), pDisableFont->GetFontColor(), pDisableFont->GetFontStyle());

	return TRUE;
}

void CCherryListCtrl::SetDisableFontName(LPCTSTR lpszFontName)
{
	m_disableFont.SetFontName(lpszFontName);
}

void CCherryListCtrl::SetDisableFontSize(float fFontSize)
{
	m_disableFont.SetFontSize(fFontSize);
}

void CCherryListCtrl::SetDisableFontColor(Color fontColor)
{
	m_disableFont.SetFontColor(fontColor);
}

void CCherryListCtrl::SetDisableFontStyle(DWORD dwFontStyle)
{
	m_disableFont.SetFontStyle(dwFontStyle);
}