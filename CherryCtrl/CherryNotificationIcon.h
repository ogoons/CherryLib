#pragma once

#include "CherryException.h"
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////
///
/// \class    CherryNotificationIcon
/// \brief    트레이 아이콘 관리
/// \author   ogoons
/// \date     2013-09-05
/// \remark   
/// \section  
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryNotificationIcon : public CWnd // CWnd is for timer
{
	DECLARE_DYNAMIC(CCherryNotificationIcon)

// Constructors
public:
	CCherryNotificationIcon();
	CCherryNotificationIcon(HICON hIcon, LPCTSTR lpszToolTip, UINT nContextMenuID, UINT nCallbackMessage, CWnd *pParentWnd, UINT nID = 0);
	CHERRY_RET Create(HICON hIcon, LPCTSTR lpszToolTip, UINT nContextMenuID, UINT nCallbackMessage, CWnd *pParentWnd, UINT nID = 0);

// Attributes
public:
	NOTIFYICONDATA		m_notifyIconData;			// NOTIFYICONDATA 구조체

	CWnd				*m_pParentWnd;				// 부모 윈도우
	HICON				m_hIcon;					// 기본 아이콘
	vector<HICON>		m_hIconList;				// 아이콘 리스트

	int					m_nAddedID;					// 중복 아이디 처리용
	UINT				m_nContextMenuID;			// Context Menu 용 아이디
	UINT_PTR			m_nTimerID;					// 에니메이션용 타이머 아이디 (타이머 실행 여부)
	UINT				m_nCount;					// 에니메이션용 타이머 카운트
	const size_t		m_notifyIconDataSize;		// NOTIFYICONDATA 구조체 사이즈 저장 (한 번만 초기화)

	// Flags
	const BOOL			m_bWinXPOrLater;			// Win2k 이상인지
	BOOL				m_bEnabled;					// Create 여부
	BOOL				m_bAdded;					// Add 여부
	BOOL				m_bVisible;					// 트레이에 보여지고 있는 지의 여부
	
	// Context Menu
	UINT				m_nContextMenuDefaultItem;	// Default 메뉴 저장
	BOOL				m_bContextMenuDefaultByPos;	// Default 메뉴를 pos로 할 것인지 T/F F이면 m_nContextMenuDefaultItem를 사용

	static const UINT m_nTaskbarCreatedMessage;

// Operations
public:
	CHERRY_RET	AddIcon();
	HICON		GetIcon() const;						// 현재 화면에 표시되는 아이콘 가져오기
	HICON		GetDefaultIcon() const;					// 에니메이션으로 표시되는 아이콘 외에 기본으로 지정된 아이콘 가져오기
	HICON		GetIconFromList(INT_PTR nPos) const;	// 한 개의 에니메이션 아이콘 가져오기
	CHERRY_RET	SetIcon(HICON hIcon);
	CHERRY_RET	SetIcon(LPCTSTR lpszIconPath);
	CHERRY_RET	SetIcon(UINT nID);
	CHERRY_RET	SetIconFromList(UINT nPos);
	CHERRY_RET	RemoveIcon();
	CHERRY_RET	ShowIcon();
	CHERRY_RET	HideIcon();
	CHERRY_RET	SetFocus();
	CHERRY_RET	Destroy();

	// NOTIFYICONDATA
	NOTIFYICONDATA*					GetNotifyIconData();

	// Menu
	CHERRY_RET	SetContextMenu(UINT nContextMenuID);
	void		GetContextMenuDefaultItem(UINT &nItem, BOOL &bByPos);
	CHERRY_RET	SetContextMenuDefaultItem(UINT nItem, BOOL bByPos = FALSE);

	// Status
	BOOL		IsVisible() const;	// 아이콘이 보여지고 있는 지
	BOOL		IsEnabled() const;	// 아이콘(객체) 생성 여부

	// ToolTip
	CString		GetToolTipText() const;
	CHERRY_RET	SetToolTipText(LPCTSTR lpszToolTip);

	// Callback Message
	UINT		GetCallbackMessage() const;
	DWORD		SetCallbackMessage(UINT nCallbackMessage);

	// Animation
	CHERRY_RET	StartAnimation(UINT nElapse);
	CHERRY_RET	StepAnimation();
	CHERRY_RET	PauseAnimation();
	CHERRY_RET	StopAnimation();
	CHERRY_RET	SetIconList(UINT nFirstIconID, UINT nLastIconID);

	// Balloon
	CHERRY_RET	ShowBalloon(LPCTSTR lpszText, LPCTSTR lpszTitle, DWORD dwIcon);
	CHERRY_RET	ShowBalloon(LPCTSTR lpszText, LPCTSTR lpszTitle, DWORD dwIcon, UINT nTimeout, HICON hBalloonIcon);

	// Flyout
	//DWORD	ShowFlyout(CCherryNotificationIconFlyout *pFlyout, UINT nWidth, UINT nHeight);
	//void							HideFlyout();
	//CWnd							*GetFlyout();
	//CRect							GetFlyoutRect(UINT nWidth, UINT nHeight);

protected:
	void	Initialize();
	size_t	GetNotifyIconDataSize() const;		// NOTIFYICONDATA 구조체의 사이즈를 가져온다.

// Overridables
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Implementation
public:
	virtual ~CCherryNotificationIcon();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void		OnTimer(UINT_PTR nIDEvent);
	LRESULT				OnNotificationIcon(WPARAM wParam, LPARAM lParam);
	LRESULT				OnTaskbarCreated(WPARAM wParam, LPARAM lParam);
};

#include "CherryDialog.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryNotificationIconFlyout
/// \brief		트레이 아이콘 쪽에서 날아오는 Window
/// \author		ogoons
/// \date		2013-10-04
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryNotificationIconFlyout : public CCherryDialog
{
	DECLARE_DYNAMIC(CCherryNotificationIconFlyout)

// Constructors
public:
	CCherryNotificationIconFlyout();
	CCherryNotificationIconFlyout(UINT nIDTemplate, CWnd *pParent);
	CHERRY_RET Create(CCherryNotificationIcon* pNotificationIcon, UINT nWidth, UINT nHeight);
	
// Attributes
protected:

// Operations
	CRect GetAnchorRect(CCherryNotificationIcon *pNotificationIcon, UINT nWidth, UINT nHeight);
	CRect GetAnchorRect(NOTIFYICONDATA *pNotifyIconData, UINT nWidth, UINT nHeight);

// Overridables
protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCreateCherry();
	virtual void OnDrawCherry(CCherryMemDC *pDC);

// Implementation
public:
	virtual ~CCherryNotificationIconFlyout();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	virtual void PostNcDestroy();
};