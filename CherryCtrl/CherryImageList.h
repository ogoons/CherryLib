#pragma once

#include "CherryException.h"
#include "CherryImage.h"
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryImageList
/// \brief		이미지 리스트 클래스
/// \author		오수철
/// \date		2013-07-10
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryImageList
{
// Constructors
public:
	CCherryImageList();
	CCherryImageList(const CCherryImageList& imageList);

// Attributes
protected:
	vector<CCherryImage> m_imageList;
	
// Operations
public:
	void			Add(LPCTSTR lpszImagePath, BOOL bUseCachedImage = FALSE);
	void			Add(CCherryImage &image);
	void			SetImageList(const CCherryImageList &imageList);
	CCherryImage	*Extract(INT_PTR nImage);
	INT_PTR			GetCount();
	BOOL			IsEmpty();
	void			Remove(INT_PTR nImage);	
	void			RemoveAll();

	void			operator=(const CCherryImageList &imageList);
	CCherryImage	*operator[](INT_PTR nIndex);

// Implementation
public:
	virtual ~CCherryImageList();
};
