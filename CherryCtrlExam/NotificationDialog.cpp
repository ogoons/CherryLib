#include "stdAfx.h"
#include "NotificationDialog.h"

IMPLEMENT_DYNAMIC(CNotificationDialog, CCherryNotificationDialog)

CNotificationDialog::CNotificationDialog()
{
	m_a = 1;
	m_b = 2;
}

CNotificationDialog::~CNotificationDialog()
{
}

BEGIN_MESSAGE_MAP(CNotificationDialog, CCherryNotificationDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CNotificationDialog::OnInitDialog()
{
	CCherryNotificationDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNotificationDialog::PostNcDestroy()
{
	//delete this;

	CCherryNotificationDialog::PostNcDestroy();
}

int CNotificationDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CCherryNotificationDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_button.Create(_T("[CherryButton]\n기본 사이즈 버튼(&A)"), _T("button.png"), CCherryButton::STYLE_AUTORESIZE, WS_CHILD | WS_VISIBLE, CCherryRect(0, 0, 0, 0), this, 1000);

	return 0;
}
