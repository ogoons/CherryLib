#pragma once


// CFlyoutDlg2 대화 상자입니다.

class CFlyoutDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CFlyoutDlg2)

public:
	CFlyoutDlg2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFlyoutDlg2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
