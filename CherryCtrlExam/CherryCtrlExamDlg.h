
// CherryCtrlExamDlg.h : 헤더 파일
//

#pragma once

#include "CherryDialog.h"
#include "CherryButton.h"
#include "CherryProgressCtrl.h"
#include "CherryImage.h"
#include "CherryFont.h"
#include "CherryStatic.h"
#include "CherryLinkCtrl.h"
#include "CherryImageCtrl.h"
#include "CherryAnimateImageCtrl.h"
#include "CherryEdit.h"
#include "CherryToggleButton.h"
#include "CherryCheckBox.h"
#include "CherryRadioButton.h"
#include "CherryComboBox.h"
#include "CherrySliderCtrl.h"
#include "CherryNotificationIcon.h"
#include "CherryListCtrl.h"
#include "CherryTabCtrl.h"

#include "FlyoutDlg.h"
#include "FlyoutDlg2.h"
#include "NotificationDialog.h"
#include "TestWnd.h"

#ifdef _DEBUG
#pragma comment(lib, "CherryCtrlD.lib")
#else
#pragma comment(lib, "CherryCtrl.lib")
#endif

// CCherryCtrlExamDlg 대화 상자
class CCherryCtrlExamDlg : public CCherryDialog
{
// 생성입니다.
public:
	CCherryCtrlExamDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CCherryCtrlExamDlg();

	enum { IDD = IDD_CHERRYCTRLEXAM_DIALOG };

protected:

	HICON	m_hIcon;

	int m_nCnt;

	CCherryRadioButton m_radioButton1;
	CCherryRadioButton m_radioButton2;
	CCherryRadioButton m_radioButton3;

	CCherryImage	m_defaultSizeImage;
	CCherryImage	m_stretchImage;

	CCherryButton	m_defaultSizeButton;
	CCherryButton	m_stretchImageButton;

	CCherryFont		m_font;

	CCherryStatic	m_static;

	CCherryLinkCtrl m_linkCtrl;

	CCherryProgressCtrl m_horzRightProgressCtrl;
	CCherryProgressCtrl m_horzLeftProgressCtrl;

	CCherryProgressCtrl m_vertTopProgressCtrl;
	CCherryProgressCtrl m_vertBottomProgressCtrl;

	CCherryComboBox	m_comboBox;

	CCherryImageCtrl m_imageCtrl;
	
	CCherryAnimateImageCtrl m_animateImageCtrl;

	CCherryEdit m_edit;

	CCherryCheckBox	m_checkBox;
	
	CCherryToggleButton	m_toggleButton;

	CCherrySliderCtrl m_sliderCtrl;

	CCherryNotificationIcon m_notifications;

	CFlyoutDlg2 *m_pFlyoutDlg;

	CNotificationDialog m_notiDlg;

	CCherryListCtrl m_listCtrl;

	CCherryTabCtrl m_tabCtrl;

	CTestWnd m_wnd1;
	CTestWnd m_wnd2;
	CTestWnd m_wnd3;
	CTestWnd m_wnd4;
	CTestWnd m_wnd5;

// Operations
public:
	

// Overridables
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual void OnDrawCherry(CCherryMemDC *pDC);
	virtual BOOL OnCreateCherry();

// Message Handler
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	void OnSliderThumbPositionChanged();
	void OnTabPageChanged(/*WPARAM wParam, LPARAM lParam*/);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};