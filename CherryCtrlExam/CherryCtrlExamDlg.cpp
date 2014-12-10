
// CherryCtrlExamDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "CherryCtrlExam.h"
#include "CherryCtrlExamDlg.h"

#include "CherryWnd.h"
#include "TestWnd.h"

#include "CherryTabCtrl.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//class __declspec(uuid("896C2676-B055-4C91-BFB5-6C7C344B64E1")) CherryNotificationIcon;


// CCherryCtrlExamDlg 대화 상자

CCherryCtrlExamDlg::CCherryCtrlExamDlg(CWnd* pParent /*=NULL*/)
	: CCherryDialog(CCherryCtrlExamDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCherryCtrlExamDlg::~CCherryCtrlExamDlg()
{
}

void CCherryCtrlExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CCherryDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCherryCtrlExamDlg, CCherryDialog)
	ON_BN_CLICKED(1000, &CCherryCtrlExamDlg::OnBnClickedButton1)
	ON_BN_CLICKED(1001, &CCherryCtrlExamDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_WM_CHERRY_SLIDERCTRL_POS_CHANGED(1011, &CCherryCtrlExamDlg::OnSliderThumbPositionChanged)
	ON_WM_CHERRY_TABCTRL_PAGE_CHANGED(1013, &CCherryCtrlExamDlg::OnTabPageChanged)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CCherryCtrlExamDlg 메시지 처리기

BOOL CCherryCtrlExamDlg::OnCreateCherry()
{
	//_CrtSetBreakAlloc(357);
	m_comboBox.Create(_T("combotest.png"), NULL, WS_VISIBLE | WS_CHILD, CRect(150, 250, 500, 500), this, 499);
	
	m_comboBox.AddString(_T("CherryComboBox"));
	m_comboBox.AddString(_T("테스트"));
	//m_comboBox.OffsetText(10, 0);
	m_comboBox.SetCherryFont(_T("맑은 고딕"), 15, Color(255, 0, 0), CCherryFont::STYLE_CENTER);
	m_comboBox.SetDropDownListFontSize(60);
	m_comboBox.SetDropDownListFontName(_T("맑은 고딕"));
	m_comboBox.SetCurSel(0);

	//m_comboBox.SetCherryFont(_T("맑은 고딕"), 12, Color(255, 0, 0), CherryFont::STYLE_DEFAULT);


	m_toggleButton.Create(NULL, _T("wimax_on_off.png"), CCherryToggleButton::STYLE_AUTORESIZE | CCherryToggleButton::STYLE_AUTOTOGGLEBUTTON, WS_CHILD | WS_VISIBLE, CRect(250, 150, 0, 0), this, 500);

	m_radioButton1.Create(_T("라디오 버튼 1"), _T("check.png"), CCherryRadioButton::STYLE_AUTORESIZE | CCherryRadioButton::STYLE_AUTOCHECKBOX, NULL, WS_CHILD | WS_VISIBLE, CRect(300, 150, 0, 0), this, 501);
	m_radioButton2.Create(_T("라디오 버튼 2"), _T("check.png"), CCherryRadioButton::STYLE_AUTORESIZE | CCherryRadioButton::STYLE_AUTOCHECKBOX, &m_radioButton1, WS_CHILD | WS_VISIBLE, CRect(300, 180, 0, 0), this, 502);
	m_radioButton3.Create(_T("라디오 버튼 3"), _T("check.png"), CCherryRadioButton::STYLE_AUTORESIZE | CCherryRadioButton::STYLE_AUTOCHECKBOX, &m_radioButton1, WS_CHILD | WS_VISIBLE, CRect(300, 210, 0, 0), this, 503);
	m_radioButton2.SetCheck(CCherryRadioButton::STATUS_CHECKED);
	m_radioButton1.SetCherryFont(_T("맑은 고딕"), 15, Color(255, 0, 0), CCherryFont::STYLE_DEFAULT);
	
	m_defaultSizeButton.Create(_T("[CherryButton]\n기본 사이즈 버튼(&A)"), _T("button.png"), CCherryButton::STYLE_AUTORESIZE, WS_CHILD | WS_VISIBLE, CRect(10, 10, 0, 0), this, 1000);
	m_defaultSizeButton.SetCherryFont(_T("맑은 고딕"), 15, Color(0, 0, 0), CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);

	m_stretchImageButton.Create(_T("[CherryButton]\n확대된 이미지 버튼\n(보다시피 외곽선이\n뭉개지지 않는다.)"), _T("button.png"), NULL, WS_CHILD | WS_VISIBLE, CRect(500, 10, 700, 300), this, 1001);
	//m_stretchImageButton.SetWindowText(_T("[CherryButton]\n확대된 이미지 버튼\n(보다시피 외곽선이\n뭉개지지 않는다 (^^)/)"));
	m_stretchImageButton.SetNormalFont(_T("맑은 고딕"), 15, Color(255, 100, 50), CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER | CCherryFont::STYLE_BOLD);

	m_horzRightProgressCtrl.Create(_T("prg.png"), 100, CCherryProgressCtrl::STYLE_HORIZONTAL_LEFT_TO_RIGHT, WS_CHILD | WS_VISIBLE, CRect(70, 70, 0, 0), this, 1002);	
	m_horzRightProgressCtrl.SetPos(0);
	m_horzRightProgressCtrl.SetCherryFont(_T("맑은 고딕"), 15, Color(255, 255, 255), CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
	m_horzRightProgressCtrl.SetWindowText(_T("0%"));

	m_horzLeftProgressCtrl.Create(_T("prg.png"), 100, CCherryProgressCtrl::STYLE_HORIZONTAL_RIGHT_TO_LEFT, WS_CHILD | WS_VISIBLE, CRect(70, 100, 0, 0), this, 1003);	
	m_horzLeftProgressCtrl.SetPos(0);
	m_horzLeftProgressCtrl.SetCherryFont(_T("맑은 고딕"), 15, Color(255, 255, 255), CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
	m_horzLeftProgressCtrl.SetWindowText(_T("0%"));

	m_vertTopProgressCtrl.Create(_T("prgv.png"), 100, CCherryProgressCtrl::STYLE_VERTICAL_BOTTOM_TO_TOP, WS_CHILD | WS_VISIBLE, CRect(70, 200, 0, 0), this, 1004);	
	m_vertTopProgressCtrl.SetPos(0);
	m_vertTopProgressCtrl.SetCherryFont(_T("맑은 고딕"), 15, Color(255, 255, 255), CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
	m_vertTopProgressCtrl.SetWindowText(_T("0%"));

	m_vertBottomProgressCtrl.Create(_T("prgv.png"), 100, CCherryProgressCtrl::STYLE_VERTICAL_TOP_TO_BOTTOM, WS_CHILD | WS_VISIBLE, CRect(100, 200, 0, 0), this, 1005);	
	m_vertBottomProgressCtrl.SetPos(0);
	m_vertBottomProgressCtrl.SetCherryFont(_T("맑은 고딕"), 15, Color(255, 255, 255), CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
	m_vertBottomProgressCtrl.SetWindowText(_T("0%"));

	m_static.Create(_T("[CherryStatic] 화면에 텍스트 표현을\n자유 자제로 가능하게 한다.(투명 배경 지원)"), CCherryStatic::STYLE_AUTORESIZE, WS_CHILD | WS_VISIBLE, CRect(200, 550, 300, 560), this, 1006);
	m_static.SetFontStyle(CCherryFont::STYLE_BOLD);
	m_static.SetCherryFont(_T("맑은 고딕"), 20, Color(0, 255, 255), CCherryFont::STYLE_VCENTER);

	//m_static.EnableAutoResize(FALSE);
	//m_static.SetCherryFont(_T("맑은 고딕"), 40, Color(0, 255, 255), CherryFont::STYLE_CENTER | CherryFont::STYLE_VCENTER);

	m_linkCtrl.Create(_T("[CherryLinkCtrl]\n하이퍼텍스트 형식의 스테틱 컨트롤"), CCherryLinkCtrl::STYLE_AUTORESIZE, WS_CHILD | WS_VISIBLE, CRect(300, 350, 600, 450), this, 1007);

	CCherryFont linkFont[4];
	COLORREF color = GetSysColor(COLOR_HOTLIGHT);
	BYTE r = (BYTE)GetRValue(color);
	BYTE g = (BYTE)GetGValue(color);
	BYTE b = (BYTE)GetBValue(color);

	linkFont[0].SetCherryFont(_T("맑은 고딕"), 20, Color(r, g, b), CCherryFont::STYLE_LEFT);
	linkFont[1].SetCherryFont(_T("맑은 고딕"), 15, Color(50, 34, 67), CCherryFont::STYLE_LEFT);
	linkFont[2].SetCherryFont(_T("맑은 고딕"), 10, Color(50, 50, 67), CCherryFont::STYLE_LEFT | CCherryFont::STYLE_UNDERLINE);
	linkFont[3].SetCherryFont(_T("맑은 고딕"), 30, Color(20, 34, 67), CCherryFont::STYLE_LEFT);	
	
	//m_linkCtrl.SetFontByStatus(&linkFont[1], &linkFont[1], &linkFont[2], &linkFont[3]);
	//m_linkCtrl.SetCherryFont(&linkFont[1]);
	//m_linkCtrl.SetNormalFontSize(60);

	//m_linkCtrl.EnableWindow(FALSE);
	//m_linkCtrl.EnableAutoResize(TRUE);
	//linkFont[0].SetCherryFont(_T("맑은 고딕"), 50, Color(r, g, b), CherryFont::STYLE_LEFT);
	//m_linkCtrl.SetNormalFont(&linkFont[0]);
	//
	DWORD ret = m_defaultSizeImage.LoadImage(_T("11.png"));

	//m_imageCtrl.Create(NULL, WS_CHILD | WS_VISIBLE, _T("24.png"), CRect(200, 200, 250, 250), this, 1008);	

	CCherryImageList images;
	images.Add(_T("on.png"), TRUE);
	images.Add(_T("off.png"), TRUE);

	m_animateImageCtrl.Create(images, 50, 0, WS_CHILD | WS_VISIBLE, CRect(200, 200, 0, 0), this, 1008);	

	m_edit.Create(_T("edit_bg.png"), RGB(255, 255, 255), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, CRect(75, 150, 0, 0), this, 1009);
	m_edit.SetFontName(_T("맑은 고딕"));
	m_edit.SetFontSize(DEFAULT_FONT_SIZE);
	m_edit.SetFontStyle(CCherryFont::STYLE_BOLD);
	m_edit.SetFontColor(Color(255, 0, 0));


	m_checkBox.Create(_T("[CherryCheckBox] 체크박스(&C)\nadfkjlasfas\najfldjflksdf\nadlfjalkdfjla"), _T("wimax_on_off.png"), CCherryCheckBox::STYLE_AUTORESIZE | CCherryCheckBox::STYLE_AUTOCHECKBOX, WS_CHILD | WS_VISIBLE, CRect(200, 400, 0, 0), this, 1010);
	m_checkBox.EnableHoverHandCursor(TRUE);
	//CCherryFont hoverFont(_T("굴림"), 100, Color(23, 233, 56), CherryFont::STYLE_VCENTER);
	//m_checkBox.SetHoverFont(hoverFont);
	//m_checkBox.SetFontColor(Color(255, 0, 0));
	m_checkBox.SetHoverFontSize(30);
	//m_checkBox.EnableWindow(FALSE);

	m_sliderCtrl.Create(_T("slider_thumb.bmp"), _T("prg.png"), 7, CCherrySliderCtrl::STYLE_HORIZONTAL_LEFT_TO_RIGHT, WS_CHILD | WS_VISIBLE, /*CRect(200, 500, 500, 520)*/ CRect(400, 500, 0, 0), this, 1011);
	//m_sliderCtrl.SetPos(40);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_notifications.Create(m_hIcon, _T("CherryCtrl1"), IDR_MENU1, WM_APP + 5, this);
	m_notifications.SetIconList(IDI_ICON1, IDI_ICON3);
	DWORD dwRet = m_notifications.AddIcon();
	m_notifications.SetContextMenuDefaultItem(ID_TRAY_02);
	m_notifications.StartAnimation(200);

	//CNotificationDialog *pNotificationDialog = new CNotificationDialog();
	//pNotificationDialog->Create(NULL, CRect(0, 0, 200, 300), this, 0, 3);

	//m_notificationPopup.Create(pNotificationDialog, 300, 200);
	//m_notificationPopup.PopupNotificationDialog();


	// CCherryListCtrl
	m_listCtrl.Create(_T("header_.png"), _T("list.png"), 60, 50, NULL, WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_NOCOLUMNHEADER, CRect(700, 10, 1100, 200), this, 1012);

	m_listCtrl.InsertColumn(0, _T("테스트1"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(1, _T("테스트2"), LVCFMT_LEFT/*| HDF_OWNERDRAW*/, 150);
	
	int nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), _T("테스트1"));
	m_listCtrl.SetItem(nItem, 1, LVIF_TEXT, _T("하나"), NULL, NULL, NULL, NULL);
	
	nItem = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), _T("테스트2"));
	m_listCtrl.SetItem(nItem, 1, LVIF_TEXT, _T("하나"), NULL, NULL, NULL, NULL);

	m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), _T("테스트3\r\n하하"));
	m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), _T("테스트4"));

	m_listCtrl.GetCherryHeaderCtrl()->SetFontStyle(CCherryFont::STYLE_VCENTER | CCherryFont::STYLE_LEFT);
	m_listCtrl.SetNormalFontStyle(CCherryFont::STYLE_VCENTER | CCherryFont::STYLE_LEFT);
	m_listCtrl.SetSelectedFontStyle(CCherryFont::STYLE_VCENTER | CCherryFont::STYLE_LEFT);


	// CCherryTabCtrl
	m_tabCtrl.Create(_T("tab.png"), CCherryTabCtrl::STYLE_TAB_LEFT, WS_CHILD | WS_VISIBLE, CRect(700, 250, 1200, 500), this, 1013);

	m_wnd1.Create(NULL, _T("babo1"), WS_CHILD | WS_VISIBLE, CRect(), &m_tabCtrl, 1014);
	m_wnd1.SetBackColor(RGB(255, 0, 0));

	m_wnd2.Create(NULL, _T("babo2"), WS_CHILD | WS_VISIBLE, CRect(), &m_tabCtrl, 1015);
	m_wnd2.SetBackColor(RGB(0, 255, 0));

	m_wnd3.Create(NULL, _T("babo3"), WS_CHILD | WS_VISIBLE, CRect(), &m_tabCtrl, 1016);
	m_wnd3.SetBackColor(RGB(0, 0, 255));

	m_wnd4.Create(NULL, _T("babo4"), WS_CHILD | WS_VISIBLE, CRect(), &m_tabCtrl, 1017);
	m_wnd4.SetBackColor(RGB(255, 255, 255));

	m_wnd5.Create(NULL, _T("babo5"), WS_CHILD | WS_VISIBLE, CRect(), &m_tabCtrl, 1018);
	m_wnd5.SetBackColor(RGB(0, 0, 0));

	m_tabCtrl.AddPage(&m_wnd1);
	m_tabCtrl.AddPage(&m_wnd2);
	m_tabCtrl.AddPage(&m_wnd3);
	m_tabCtrl.AddPage(&m_wnd4);
	m_tabCtrl.AddPage(&m_wnd5);

	//m_tabCtrl.AddPage(NULL);
	//m_tabCtrl.SetTabButtonGap(20);

	//m_tabCtrl.GetTabButton(2)->SetNormalFontColor(Color(255, 255, 0, 0));


	return TRUE;
}

BOOL CCherryCtrlExamDlg::OnInitDialog()
{
	CCherryDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//DWORD dwTest;

	//dwTest = 0x0001 | 0x0002
	//dwTest &= ~0x0002;

	//dwTest &= ~0x0080;	

	//if(dwTest & 0x0001)
	//	AfxMessageBox(_T("0x0001 존재함"));

	//if(dwTest & 0x0002)
	//	AfxMessageBox(_T("0x0002 존재함"));

	//if(dwTest & 0X0080)
	//	AfxMessageBox(_T("0X0080 존재함"));

	//int a = 10;
	//
	//CString ss = _T("");

	//CString str = StringFormat(_T("num:%s"), a);	

	//AfxMessageBox(str);

	//int a = 10;

	//CString str2;

	//str2.Format(_T("num %d")); 

	//AfxMessageBox(str2);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

//void CCherryCtrlExamDlg::OnSysCommand(UINT nID, LPARAM lParam)
//{
//	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
//	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
//	}
//	else
//	{
//		CCherryDialog::OnSysCommand(nID, lParam);
//	}
//}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

//void CCherryCtrlExamDlg::OnPaint()
//{
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트
//
//		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
//
//		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;
//
//		// 아이콘을 그립니다.
//		dc.DrawIcon(x, y, m_hIcon);
//	}
//	else
//	{
//		CDialog::OnPaint();
//	}
//}


void CCherryCtrlExamDlg::OnDrawCherry(CCherryMemDC *pDC)
{
	Graphics grs(pDC->GetSafeHdc());

	CRect clientRect;
	GetClientRect(clientRect);

	m_defaultSizeImage.DrawImage(&grs, clientRect);
	m_font.DrawText(&grs, _T("CCherryFont 테스트 입니다."), CRect(10, 10, 100, 20));
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
//HCURSOR CCherryCtrlExamDlg::OnQueryDragIcon()
//{
//	return static_cast<HCURSOR>(m_hIcon);
//}
//BOOL CCherryCtrlExamDlg::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	return FALSE;
//	//return CDialog::OnEraseBkgnd(pDC);
//}

void CCherryCtrlExamDlg::OnBnClickedButton1()
{

	//m_tabCtrl.DeleteTabPage(0);
	//return;
	//m_notifications.StartAnimation(1000);

	//m_checkBox.SetCheck(BST_CHECKED);

	TRACE("%d\n", m_checkBox.GetCheck());

	//m_animateImageCtrl.StartAnimate(500, 2);

	m_nCnt = 0;
	SetTimer(1, 1, NULL);

	//m_notifications.ShowBalloon(_T("테스트내용"), _T("테스트제목"), NIIF_WARNING | NIIF_LARGE_ICON);
	CFlyoutDlg *pDlg = new CFlyoutDlg();
	pDlg->Create(&m_notifications, 300, 100);
	//m_notifications.SetFlyoutWindowAlpha(200);

	//m_defaultSizeButton.EnableWindow(FALSE);
	//m_linkCtrl.EnableWindow(FALSE);

	//m_notiDlg.Create(NULL, 600, 250, this, 0, 3);
}

void CCherryCtrlExamDlg::OnBnClickedButton2()
{
	/*
	CTestWnd *pWnd = new CTestWnd(RGB(255, 0, 0));
	pWnd->Create(NULL, _T("babo5"), WS_CHILD | WS_VISIBLE, CRect(), &m_tabCtrl, 10111);


	m_tabCtrl.AddPage(pWnd);
	return;

	*/

	//m_comboBox.SetCurSel(0);
	//CNotificationDialog dlg;
	//m_notiDlg.Create(NULL, 300, 250, this, 0, 3);

	//
	CNotificationDialog *pNotificationDialog = new CNotificationDialog();
	pNotificationDialog->Create(NULL, 200, 300, this, 0, 3);

	//m_notificationPopup.Create(pNotificationDialog, 400, 300);
	//m_notificationPopup.PopupNotificationDialog();
}


//BOOL CCherryCtrlExamDlg::OnNcActivate(BOOL bActive)
//{
//	//Invalidate();
//
//	return CCherryDialog::OnNcActivate(bActive);
//}
//
//void CCherryCtrlExamDlg::OnSize(UINT nType, int cx, int cy)
//{
//	CCherryDialog::OnSize(nType, cx, cy);
//
//	
//
//	//Invalidate();
//}

void CCherryCtrlExamDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		if (m_nCnt >= m_vertBottomProgressCtrl.GetMaxPos())
		{
			m_nCnt = 0;
		}

		CString str;
		str.Format(_T("%d%%"), m_nCnt);
 		m_horzLeftProgressCtrl.SetPos(m_nCnt);
 		m_horzLeftProgressCtrl.SetWindowText(str);
 		m_horzRightProgressCtrl.SetPos(m_nCnt);
 		m_horzRightProgressCtrl.SetWindowText(str);
 		m_vertTopProgressCtrl.SetPos(m_nCnt);
 		m_vertTopProgressCtrl.SetWindowText(str);
		m_vertBottomProgressCtrl.SetPos(m_nCnt);
		m_vertBottomProgressCtrl.SetWindowText(str);

		m_nCnt++;
	}

	CCherryDialog::OnTimer(nIDEvent);
}

void CCherryCtrlExamDlg::OnSliderThumbPositionChanged()
{
	//TRACE("wParam %d\n", wParam);
	//TRACE("lParam %d\n", lParam);
	TRACE("SliderCtrl changed %d\n", m_sliderCtrl.GetPos());
}

void CCherryCtrlExamDlg::OnTabPageChanged(/*WPARAM wParam, LPARAM lParam*/)
{
	TRACE("TabCtrl changed %d\n", m_tabCtrl.GetCurPage());


	//return 0;
}



void CCherryCtrlExamDlg::OnSize(UINT nType, int cx, int cy)
{
	CCherryDialog::OnSize(nType, cx, cy);

	if (m_tabCtrl.GetSafeHwnd())
	{
		CRect rect;
		m_tabCtrl.GetWindowRect(rect);
		ScreenToClient(rect);

		m_tabCtrl.MoveWindow(rect.left, rect.top, cx - 800, cy - 500);
	}
}
