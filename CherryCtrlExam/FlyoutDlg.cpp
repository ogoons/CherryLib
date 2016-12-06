// FlyoutDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CherryCtrlExam.h"
#include "FlyoutDlg.h"


// CFlyoutDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFlyoutDlg, CDialogEx)

CFlyoutDlg::CFlyoutDlg(CWnd* pParent /*=NULL*/)
	: CCherryNotificationIconFlyout(CFlyoutDlg::IDD, pParent)
{
	
}

CFlyoutDlg::~CFlyoutDlg()
{
}

void CFlyoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CCherryNotificationIconFlyout::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFlyoutDlg, CCherryNotificationIconFlyout)
	ON_WM_CREATE()
	ON_BN_CLICKED(1000, &CFlyoutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CFlyoutDlg 메시지 처리기입니다.

void CFlyoutDlg::PostNcDestroy()
{
	//delete this;

	CCherryNotificationIconFlyout::PostNcDestroy();
}

void CFlyoutDlg::OnDrawCherry(CCherryMemDC *pDC)
{
}

void CFlyoutDlg::OnBnClickedButton1()
{
	AfxMessageBox(_T("테스트"));
}


int CFlyoutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CCherryNotificationIconFlyout::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_cherryButton.Create(_T("[CherryButton]\n기본 사이즈 버튼(&A)"), _T("button.png"), CCherryButton::STYLE_AUTORESIZE, WS_CHILD | WS_VISIBLE, CCherryRect(0, 0, 0, 0), this, 1000);

	//m_button1.Create(_T("테스트"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 50, 50), this, 1001);

	return 0;
}
