#pragma once

#include "CherryNotificationDialog.h"
#include "CherryButton.h"

class CNotificationDialog : public CCherryNotificationDialog
{
	DECLARE_DYNAMIC(CNotificationDialog)
public:
	CNotificationDialog();
	virtual ~CNotificationDialog();

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();

	CCherryButton m_button;


	int m_a;
	int m_b;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
