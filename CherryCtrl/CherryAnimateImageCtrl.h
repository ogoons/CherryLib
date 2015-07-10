#pragma once

#include "CherryWnd.h"
#include "CherryImageList.h"
#include "CherryMemDC.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryAnimateImageCtrl
/// \brief		에니메이션 이미지 클래스
/// \author		ogoons
/// \date		2013-07-10
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryAnimateImageCtrl : public CCherryWnd
{
	DECLARE_DYNAMIC(CCherryAnimateImageCtrl)

// Constructors
public:
	CCherryAnimateImageCtrl();
	virtual CHERRY_RET Create(
		CCherryImageList& imageList, 
		UINT nInterval, 
		UINT nAnimateNum,
		DWORD dwStyle,
		const RECT& rect, 
		CWnd* pParentWnd, 
		UINT nID);

// Attributes
protected:
	CCherryImageList m_imageList;	// 이미지 리스트
	INT_PTR m_nAnimateTimerRet;

	INT_PTR m_nImage;				// 현재 보여지는 이미지의 Index
	UINT m_nInterval;				// 이미지 간에 Interval
	UINT m_nAnimateNum;			// 반복 횟수 0인 경우 무한
	UINT m_nAnimateCount;				// 현재 반복한 횟수
	
// Operations
public:
	void		SetImageList(CCherryImageList& imageList);
	CHERRY_RET	ShowImage(INT_PTR nImage);
	
	void StartAnimate(UINT nInterval, UINT nAnimateNum = 0);
	void PauseAnimate();
	void StopAnimate();
	void SetAnimateInterval(UINT nInternval);
	void SetAnimateCount(UINT nAnimateNum);

// Implementation
public: 
	virtual ~CCherryAnimateImageCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void OnDrawCherry(CCherryMemDC *pDC);

	afx_msg void OnTimer(UINT_PTR nIDEvent);	
};


