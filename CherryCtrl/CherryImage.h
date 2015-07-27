#pragma once

#include "CherryException.h"

using namespace Gdiplus;

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryFont
/// \brief		이미지 관리 클래스\n
///				CherryX 에서 사용하는 모든 이미지는 이 클래스를 사용하여 표현하도록 한다.\n
///				직접 사용하기 보단 상속하여 사용하는 상위 and 추상 클래스 성격을 갖고있다.
/// \author		ogoons
/// \date		2012-11-09
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryImage
{
// Constructors
public:
	CCherryImage();
	CCherryImage(LPCTSTR lpszImagePath, BOOL bUseCachedImage = FALSE);
	CCherryImage(Bitmap *pBitmap, BOOL bUseCachedImage = FALSE);
	CCherryImage(Bitmap *pBitmap, int nLeft, int nTop, int nWidth, int nHeight, BOOL bUseCachedImage = FALSE);
	CCherryImage(Bitmap *pBitmap, Rect rect, BOOL bUseCachedImage = FALSE);
	CCherryImage(const CCherryImage &image);

// Attributes
protected:
	Bitmap			*m_pBitmap;
	
	CachedBitmap	*m_pCachedBitmap;
	CRect			m_cachedBitmapRect;
	
	BOOL			m_bUseCachedImage;

	static CString	m_strDefaultImagePath;
// Operations
public:
	// 이미지가 로드 되었는지
	BOOL			IsLoadedImage() const;
	
	// Bitmap 객체 포인터를 가져옴
	Bitmap			*GetBitmap() const;
	
	// 전체 이미지 사이즈를 가져옴
	CSize			GetSize() const;
	
	// 가로 크기를 가져옴
	UINT			GetWidth() const;
	
	// 세로 크기를 가져옴
	UINT			GetHeight() const;

	// 마지막 에러 상태 가져옴
	Status			GetBitmapLastStatus() const;

	// 캐시된 이미지 사용
	void									UseCachedImage(BOOL bUse = TRUE);

	// 이미지 읽어오기
	CHERRY_RET		LoadImage(LPCTSTR lpszImagePath, BOOL bUseCachedImage = FALSE);
	CHERRY_RET		LoadImage(Bitmap *pBitmap, BOOL bUseCachedImage = FALSE);
	CHERRY_RET		LoadImage(Bitmap *pBitmap, int nLeft, int nTop, int nWidth, int nHeight, BOOL bUseCachedImage);
	CHERRY_RET		LoadImage(Bitmap *pBitmap, Rect rect, BOOL bUseCachedImage = FALSE);
	CHERRY_RET		LoadImageFromResourceID(LPCTSTR lpszResourceType, UINT nResourceID, BOOL bUseCachedImage = FALSE);

	// 멤버로 등록된 이미지 그리기
	CHERRY_RET		DrawImage(Graphics *pGraphics, int nLeft, int nTop, UINT nAlphaBlendRatio = 100);
	CHERRY_RET		DrawImage(Graphics *pGraphics, int nLeft, int nTop, int nRight, int nBottom, UINT nAlphaBlendRatio = 100);
	CHERRY_RET		DrawImage(Graphics *pGraphics, CRect rect, UINT nAlphaBlendRatio = 100);

	//// 확대나 축소된 이미지 그리기(사용 안 함)
	//BOOL			DrawRatioImage(Graphics *pGraphics, int nLeft, int nTop, UINT nRatio, UINT nAlphaBlendRatio = 100);	
	//BOOL			DrawRatioImage(Graphics *pGraphics, CPoint point, UINT nRatio, UINT nAlphaBlendRatio = 100);
	//

	// 3x3 으로 분할하여 확대 그리기(외곽선 깨짐 방지용)
	CHERRY_RET		DrawStretchImage3x3(Graphics *pGraphics, int nLeft, int nTop, int nRight, int nBottom, UINT nAlphaBlendRatio = 100);
	CHERRY_RET		DrawStretchImage3x3(Graphics *pGraphics, CRect rect, UINT nAlphaBlendRatio = 100);

	// 멤버로 할당된 Bitmap 객체 삭제
	BOOL									RemoveImage();
	
	// 멤버로 할당된 캐시된 Bitmap 객체 삭제
	BOOL									RemoveCachedImage();

	static void		SetDefaultImagePath(LPCTSTR lpszPath);
	

protected:
	// 중복되는 로직을 하나로 묶어서 클래스 내부에서만 사용하는 이미지 Draw 함수
	Status			MakeImage(Graphics *pGraphics, Rect rect, UINT nAlphaBlendRatio);
	Status			MakeStretchImage3x3(Graphics *pGraphics, Rect rect, UINT nAlphaBlendRatio);

	// 이미지 속성에 투명 효과
	Status			CaculatedAlphaBlendAttributes(_In_ UINT nAlphaBlendRatio, _Out_ ImageAttributes &imageAttributes);

	// 캐시된 이미지를 업데이트
	Status			UpdateCachedImage(Graphics *pGraphics, CRect rect, UINT nAlphaBlendRatio, BOOL bUseDrawStretch3x3 = FALSE);

// Implementation
public:
	virtual	~CCherryImage();
};