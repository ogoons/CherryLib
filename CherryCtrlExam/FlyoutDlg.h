#pragma once

#include "CherryNotificationIcon.h"
#include "CherryButton.h"

// CFlyoutDlg 대화 상자입니다.

class CFlyoutDlg : public CCherryNotificationIconFlyout
{
	DECLARE_DYNAMIC(CFlyoutDlg)

public:
	CFlyoutDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFlyoutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

	CButton m_button1;
	CCherryButton m_cherryButton;
public:
	virtual void OnDrawCherry(CCherryMemDC *pDC);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnBnClickedButton1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
