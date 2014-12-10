// CherryHeaderCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryHeaderCtrl.h"
#include "CherryException.h"
#include "CherryMemDC.h"


// CCherryHeaderCtrl

IMPLEMENT_DYNAMIC(CCherryHeaderCtrl, CHeaderCtrl)

CCherryHeaderCtrl::CCherryHeaderCtrl()
{
	m_nHeight = 0;
}

CCherryHeaderCtrl::~CCherryHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CCherryHeaderCtrl, CHeaderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CCherryHeaderCtrl::OnNMCustomdraw)
	ON_MESSAGE(HDM_LAYOUT, &CCherryHeaderCtrl::OnLayout)

	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CCherryHeaderCtrl 메시지 처리기입니다.

void CCherryHeaderCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	*pResult = 0;
	*/

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	//LPNMHEADER pHeader = reinterpret_cast<LPNMHEADER>(pNMHDR);


	//if (1 == pHeader->iItem)
		//AfxMessageBox(_T("1"));

	*pResult = CDRF_DODEFAULT;

	// 아이템 인덱스
	int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		// 모든 스테이지에서 메시지를 받으려면
		*pResult = CDRF_NOTIFYPOSTERASE | CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYSUBITEMDRAW;
		//*pResult = CDRF_NOTIFYSUBITEMDRAW;;
		break;
	case CDDS_PREERASE:
		break;
	case CDDS_POSTERASE:
		break;
	case CDDS_ITEMPREERASE:
		break;
	case CDDS_ITEMPOSTERASE:
		break;
	case CDDS_ITEMPREPAINT:
		// Report 스타일이면
		if (GetParent()->GetStyle() & LVS_REPORT && !(GetParent()->GetStyle() & LVS_NOCOLUMNHEADER))
		{
			CRect itemRect(pLVCD->nmcd.rc);
			//GetItemRect(nItem, &itemRect);

			CDC *pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			CCherryMemDC memDC(pDC, itemRect);
			Graphics graphics(memDC.GetSafeHdc());

			graphics.SetInterpolationMode(InterpolationModeDefault);
			graphics.SetPixelOffsetMode(PixelOffsetModeNone);

			if (m_headerImage.IsLoadedImage())
				m_headerImage.DrawStretchImage3x3(&graphics, itemRect);

			TCHAR szHeaderText[256] = { 0 };
			HDITEM hdItem = { 0 };
			hdItem.mask = HDI_TEXT;
			hdItem.pszText = szHeaderText;
			hdItem.cchTextMax = _countof(szHeaderText);
			GetItem(nItem, &hdItem);

			/*
			// Disable Gray 처리
			if (!GetParent()->IsWindowEnabled())
			{
				COLORREF disableColor = GetSysColor(COLOR_GRAYTEXT);
				SetFontColor(Color(GetRValue(disableColor), GetGValue(disableColor), GetBValue(disableColor)));
			}
			*/

			DrawText(&graphics, hdItem.pszText, itemRect);

			*pResult = CDRF_SKIPDEFAULT;
		}
		break;
	case CDDS_ITEMPOSTPAINT:
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		break;
	case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM:
		break;
	case CDDS_POSTPAINT:
		break;
	default:
		*pResult = CDRF_DODEFAULT;
		//OnNMCustomdraw(pNMHDR, pResult);
		break;
	}
}

void CCherryHeaderCtrl::SetHeight(UINT nHeight)
{
	m_nHeight = nHeight;
}

LRESULT CCherryHeaderCtrl::OnLayout(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRet = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);

	HD_LAYOUT &hdl = *(HD_LAYOUT *)lParam;
	RECT *pRect = hdl.prc;
	WINDOWPOS *pWindowPos = hdl.pwpos;

	if (m_nHeight > 0)
	{
		pWindowPos->cy = (int)m_nHeight;
		pRect->top = (LONG)m_nHeight;	
	}

	// 컬럼 사이즈 변경 시 리스트 갱신
	GetParent()->Invalidate(FALSE);

	return lRet;
}

CHERRY_RET CCherryHeaderCtrl::Create(LPCTSTR lpszImagePath, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	//if (!CHeaderCtrl::Create(dwStyle, rect, pParentWnd, nID))
		//return CCherryException::ERROR_HEADERCTRL_CREATE_FAIL;
	return SetImage(lpszImagePath);
}

CHERRY_RET CCherryHeaderCtrl::SetImage(LPCTSTR lpszImagePath)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if ((cherryRet = m_headerImage.LoadImage(lpszImagePath, TRUE)) != CCherryException::ERROR_CHERRY_SUCCESS)
			throw cherryRet;
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

void CCherryHeaderCtrl::PreSubclassWindow()
{
	CHeaderCtrl::PreSubclassWindow();
}


void CCherryHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CHeaderCtrl::OnLButtonDown(nFlags, point);
}


void CCherryHeaderCtrl::OnDestroy()
{
	CHeaderCtrl::OnDestroy();
}
