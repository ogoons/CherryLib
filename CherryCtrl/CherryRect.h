#pragma once

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryRect
/// \brief		width, height 계산을 편하게 하기 위한 하위 클래스
///				CRect을 상속받아 구현되었다.
/// \author		ogoons
/// \date		2013-12-06
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryRect : public CRect
{
public:
	CCherryRect();
	CCherryRect(int left, int top, int width, int height);
	//CCherryRect(const CCherryRect &cherryRect);
	virtual ~CCherryRect();

	void SetRectangle(int left, int top, int width, int height) throw();
};

