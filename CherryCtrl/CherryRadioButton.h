#pragma once

#include "CherryCheckBox.h"
#include "CherryRect.h"
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryRadioButton
/// \brief		라디오 버튼 클래스
/// \author		ogoons
/// \date		2013-07-10
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryRadioButton : public CCherryCheckBox
{
	DECLARE_DYNAMIC(CCherryRadioButton)

// Constructors
public:
	CCherryRadioButton();
	virtual CHERRY_RET Create(
		LPCTSTR lpszCaption, 
		LPCTSTR lpszImagePath, 
		DWORD dwCherryStyle,
		CCherryRadioButton *pHeadRadioButton,
		DWORD dwStyle, 
		const CCherryRect &cherryRect,
		CWnd *pParentWnd, 
		UINT nID);
	
// Attributes
protected:
	vector<CCherryRadioButton *> m_radioVector;
	CCherryRadioButton *m_pHeadRadioButton;

// Operations
public:
	vector<CCherryRadioButton *>	*GetRadioVector();
	vector<CCherryRadioButton *>	*GetHeadRadioVector();
	CCherryRadioButton				*GetHeadRadioButton() const;
	
	void							SetCheck(STATUS checkStatus);
	void							SetHeadRadioButton(CCherryRadioButton *pRadioButton);

protected:
	void							AddRadioVector(CCherryRadioButton *pRadioButton);

// Implementation
public:
	virtual ~CCherryRadioButton();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};