#pragma once

#include "CherryException.h"

using namespace Gdiplus;

#define DEFAULT_FONT_NAME		_T("Segoe UI")
#define DEFAULT_FONT_SIZE		12
#define DEFAULT_ORG_FONT_SIZE	15
#define DEFAULT_FONT_STYLE		FontStyleRegular
#define DEFAULT_UNIT_PIXEL		UnitPixel
#define DEFAULT_FONT_COLOR		Color(0, 0, 0)			// black
#define DEFAULT_SHADOW_COLOR	Color(160, 160, 160);	// gray
#define DEFAULT_FONT_ALIGN		StringAlignmentCenter

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryFont
/// \brief		CherryX 에서 사용하는 모든 문자열은 이 클래스를 사용하여 표현하도록 한다.\n
///				Base 클래스 성격.
/// \author		오수철
/// \date		2012-11-09
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryFont
{
public:
	// 스타일 정의
	enum STYLE
	{
		STYLE_DEFAULT = 0x0000,	// 기본값
		STYLE_LEFT = 0x0000,	// 좌로 정렬
		STYLE_TOP = 0x0001,	// 위로 정렬
		STYLE_RIGHT = 0x0002,	// 우로 정렬
		STYLE_BOTTOM = 0x0004,	// 아래로 정렬
		STYLE_CENTER = 0x0008,	// 수평 가운데 정렬
		STYLE_VCENTER = 0x0010,	// 수직 가운데 정렬

		STYLE_BOLD = 0x0020,	// 진하게
		STYLE_ITALIC = 0x0040,	// 기울임
		STYLE_UNDERLINE = 0x0080,	// 밑줄 긋기
		STYLE_STRIKEOUT = 0x0100,	// 취소선
		STYLE_NOWRAP = 0x0200,	// 줄바꿈 안 함
	};

// Constructors
public:
	CCherryFont();
	CCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle);
	CCherryFont(const CCherryFont &cherryFont);

// Attributes
protected:
	// Text
	CString			m_strFontName;
	Color			m_fontColor;
	float			m_fFontSize;
	DWORD			m_dwFontStyle;
	CRect			m_offsetRect;				// OffsetText() 사용시에

// 	// Shadow
// 	BOOL			m_bEnableShadow;
// 	Color			m_shadowColor;
// 	int				m_nOffsetTextLeft;
// 	int				m_nOffsetTextTop;

// Operations
public:
	// Text Style
	void			SetCherryFont(LPCTSTR lpszFontName, float fFontSize, Color fontColor, DWORD dwFontStyle);
	BOOL			SetCherryFont(CCherryFont *pCherryFont);
	void			SetFontName(LPCTSTR lpszFontName);
	void			SetFontSize(float fFontSize);
	void			SetFontColor(Color fontColor);
	void			SetFontStyle(DWORD dwFontStyle);
	CRect			MeasureString(Graphics *pGraphics, LPCTSTR lpszString);

	CString			GetFontName();
	float			GetFontSize();
	Color			GetFontColor();
	DWORD			GetFontStyle();
	
	void			DrawText(Graphics *pGraphics, LPCTSTR lpszText, const CRect &rect);

// Implementation
public:
	virtual ~CCherryFont();
};
