#include "stdafx.h"
#include "CherryImage.h"

// CCherryImage

CString CCherryImage::m_strDefaultImagePath;

CCherryImage::CCherryImage()
{
	m_pBitmap = NULL;
	m_pCachedBitmap = NULL;
	m_bUseCachedImage = FALSE;
}

CCherryImage::CCherryImage(LPCTSTR lpszImagePath, BOOL bUseCachedImage)
{
	m_pBitmap = NULL;
	m_pCachedBitmap = NULL;

	// 새로 할당
	LoadImage(lpszImagePath, bUseCachedImage);
}

CCherryImage::CCherryImage(Bitmap *pBitmap, BOOL bUseCachedImage)
{
	m_pBitmap = NULL;
	m_pCachedBitmap = NULL;
	
	// 깊은 복사
	LoadImage(pBitmap, bUseCachedImage);
}

CCherryImage::CCherryImage(Bitmap *pBitmap, int nLeft, int nTop, int nWidth, int nHeight, BOOL bUseCachedImage)
{
	m_pBitmap = NULL;
	m_pCachedBitmap = NULL;
	
	// 깊은 복사
	LoadImage(pBitmap, nLeft, nTop, nWidth, nHeight, bUseCachedImage);
}

CCherryImage::CCherryImage(Bitmap *pBitmap, Rect rect, BOOL bUseCachedImage)
{
	m_pBitmap = NULL;
	m_pCachedBitmap = NULL;
	
	// 깊은 복사
	LoadImage(pBitmap, rect, bUseCachedImage);
}

CCherryImage::CCherryImage(const CCherryImage &image)
{
	m_pBitmap = NULL;
	m_pCachedBitmap = NULL;
	
	// 깊은 복사
	LoadImage(image.m_pBitmap, image.m_bUseCachedImage);	
}

CCherryImage::~CCherryImage()
{
	RemoveImage();
	RemoveCachedImage();
}

CHERRY_RET CCherryImage::LoadImage(LPCTSTR lpszImagePath, BOOL bUseCachedImage)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		CString strImagePath;
		if (m_strDefaultImagePath.IsEmpty())
			strImagePath = lpszImagePath;
		else
			strImagePath.Format(_T("%s\\%s"), m_strDefaultImagePath, lpszImagePath);

		if (!PathFileExists(strImagePath))
			throw CCherryException::ERROR_IMAGE_FILE_NOT_FOUND;

		RemoveImage();
		RemoveCachedImage();

		m_pBitmap = new Bitmap(strImagePath);

		if (m_pBitmap->GetLastStatus() != Ok)
		{
			RemoveImage();

			throw CCherryException::ERROR_IMAGE_LOAD_FAIL;
		}

		m_bUseCachedImage = bUseCachedImage;
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryImage::LoadImage(Bitmap *pBitmap, BOOL bUseCachedImage)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		RemoveImage();
		RemoveCachedImage();

		m_pBitmap = pBitmap->Clone(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), PixelFormatDontCare);

		if (m_pBitmap->GetLastStatus() != Ok)
		{
			RemoveImage();

			throw CCherryException::ERROR_IMAGE_LOAD_FAIL;
		}

		m_bUseCachedImage = bUseCachedImage;
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryImage::LoadImage(Bitmap *pBitmap, int nLeft, int nTop, int nWidth, int nHeight, BOOL bUseCachedImage)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		RemoveImage();
		RemoveCachedImage();

		// 깊은 복사
		m_pBitmap = pBitmap->Clone(nLeft, nTop, nWidth, nHeight, PixelFormatDontCare);

		if (m_pBitmap->GetLastStatus() != Ok)
		{
			RemoveImage();

			throw CCherryException::ERROR_IMAGE_LOAD_FAIL;
		}

		m_bUseCachedImage = bUseCachedImage;
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryImage::LoadImage(Bitmap *pBitmap, Rect rect, BOOL bUseCachedImage)
{
	return LoadImage(pBitmap, rect.GetLeft(), rect.GetTop(), rect.Width, rect.Height, bUseCachedImage);
}

CHERRY_RET CCherryImage::LoadImageFromResourceID(LPCTSTR lpszResourceType, UINT nResourceID, BOOL bUseCachedImage)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;
	
	HGLOBAL hBuffer = NULL;

	try
	{
		RemoveImage();
		RemoveCachedImage();

		HMODULE hModule = AfxGetInstanceHandle();
		HRSRC hRsrc = FindResource(hModule, MAKEINTRESOURCE(nResourceID), lpszResourceType);
		HGLOBAL hGlobal = LoadResource(hModule, hRsrc);
		PVOID pResourceData = LockResource(hGlobal);
		DWORD dwSize = SizeofResource(hModule, hRsrc);

		hBuffer = GlobalAlloc(GMEM_MOVEABLE, dwSize);
		LPVOID lpBuffer = GlobalLock(hBuffer);
		CopyMemory(lpBuffer, pResourceData, dwSize);
		GlobalUnlock(hBuffer);

		IStream *pStream = NULL;
		HRESULT hResult = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

		if (FAILED(hResult))
			throw CCherryException::ERROR_IMAGE_LOAD_FAIL;

		m_pBitmap = Bitmap::FromStream(pStream);

		if (Ok != m_pBitmap->GetLastStatus())
		{
			RemoveImage();

			throw CCherryException::ERROR_IMAGE_LOAD_FAIL;
		}

		m_bUseCachedImage = bUseCachedImage;
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	if (hBuffer)
		GlobalFree(hBuffer);

	return cherryRet;
}

BOOL CCherryImage::IsLoadedImage() const
{
	if (NULL == m_pBitmap)
		return FALSE;

	return TRUE;
}

Bitmap *CCherryImage::GetBitmap() const
{
	if (NULL == m_pBitmap)
		return NULL;

	return m_pBitmap;
}

CSize CCherryImage::GetSize() const
{
	return CSize(m_pBitmap->GetWidth(), m_pBitmap->GetHeight());
}

// 내부에서 사용하는 함수는 CherryException을 사용하지 않는다.
Status CCherryImage::MakeImage(Graphics *pGraphics, Rect rect, UINT nAlphaBlendRatio)
{
	Status statusRet = Ok;

	try
	{
		if (NULL == m_pBitmap)
			throw GenericError;

		ImageAttributes imageAttributes;
		if ((statusRet = CaculatedAlphaBlendAttributes(nAlphaBlendRatio, imageAttributes)) != Ok)
			throw statusRet;

		statusRet = pGraphics->DrawImage(m_pBitmap, rect, 0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight(), UnitPixel, &imageAttributes);
	}
	catch (const Status &e)
	{
		statusRet = e;
	}

	return statusRet;
}

CHERRY_RET CCherryImage::DrawImage(Graphics *pGraphics, int nLeft, int nTop, UINT nAlphaBlendRatio)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (NULL == m_pBitmap)
			throw CCherryException::ERROR_IMAGE_NOT_LOADED;

		if (m_bUseCachedImage)
		{
			// 크기가 변경되었을 때만 업데이트 하게 되어있다.
			if (UpdateCachedImage(pGraphics, CRect(nLeft, nTop, nLeft + m_pBitmap->GetWidth(), nTop + m_pBitmap->GetHeight()), nAlphaBlendRatio) != Ok)
				throw CCherryException::ERROR_IMAGE_UPDATE_CACHED_IMAGE_FAIL;

			if (m_pCachedBitmap == NULL)
				throw CCherryException::ERROR_IMAGE_CACHED_IMAGE_NOT_LOADED;

			if (pGraphics->DrawCachedBitmap(m_pCachedBitmap, nLeft, nTop) != Ok)
				throw CCherryException::ERROR_IMAGE_DRAW_CACHED_IMAGE_FAIL;
		}
		else
		{
			if (MakeImage(pGraphics, Rect(nLeft, nTop, nLeft + m_pBitmap->GetWidth(), nTop + m_pBitmap->GetHeight()), nAlphaBlendRatio) != Ok)
				throw CCherryException::ERROR_IMAGE_MAKE_IMAGE_FAIL;
		}

	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryImage::DrawImage(Graphics *pGraphics, int nLeft, int nTop, int nRight, int nBottom, UINT nAlphaBlendRatio)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (NULL == m_pBitmap)
			throw CCherryException::ERROR_IMAGE_NOT_LOADED;

		// 넓이나 높이가 0 이하로 넘어올 경우 원본 크기로 변경
		if (nRight - nLeft <= 0)
			nRight = nLeft + m_pBitmap->GetWidth();

		if (nBottom - nTop <= 0)
			nBottom = nTop + m_pBitmap->GetHeight();

		if (m_bUseCachedImage)
		{
			if (UpdateCachedImage(pGraphics, CRect(nLeft, nTop, nRight, nBottom), nAlphaBlendRatio) != Ok)
				throw CCherryException::ERROR_IMAGE_UPDATE_CACHED_IMAGE_FAIL;

			if (m_pCachedBitmap == NULL)
				throw CCherryException::ERROR_IMAGE_CACHED_IMAGE_NOT_LOADED;

			if (pGraphics->DrawCachedBitmap(m_pCachedBitmap, nLeft, nTop) != Ok)
				throw CCherryException::ERROR_IMAGE_DRAW_CACHED_IMAGE_FAIL;
		}
		else
		{
			if (MakeImage(pGraphics, Rect(nLeft, nTop, nRight - nLeft, nBottom - nTop), nAlphaBlendRatio) != Ok)
				throw CCherryException::ERROR_IMAGE_MAKE_IMAGE_FAIL;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryImage::DrawImage(Graphics *pGraphics, CRect rect, UINT nAlphaBlendRatio)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (NULL == m_pBitmap)
			throw CCherryException::ERROR_IMAGE_NOT_LOADED;

		// right가 0 이하로 넘어올 경우 기본 넓이 셋팅
		if (rect.Width() <= 0)
			rect.right = rect.left + m_pBitmap->GetWidth();

		// bottom이 0 이하로 넘어올 경우 기본 높이 셋팅
		if (rect.Height() <= 0)
			rect.bottom = rect.top + m_pBitmap->GetHeight();

		if (m_bUseCachedImage)
		{
			if (UpdateCachedImage(pGraphics, rect, nAlphaBlendRatio) != Ok)
				throw CCherryException::ERROR_IMAGE_UPDATE_CACHED_IMAGE_FAIL;

			if (m_pCachedBitmap == NULL)
				throw CCherryException::ERROR_IMAGE_CACHED_IMAGE_NOT_LOADED;

			if (pGraphics->DrawCachedBitmap(m_pCachedBitmap, rect.left, rect.top) != Ok)
				throw CCherryException::ERROR_IMAGE_DRAW_CACHED_IMAGE_FAIL;
		}
		else
		{
			if (MakeImage(pGraphics, Rect(rect.left, rect.top, rect.Width(), rect.Height()), nAlphaBlendRatio) != Ok)
				throw CCherryException::ERROR_IMAGE_MAKE_IMAGE_FAIL;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}
/*
BOOL CCherryImage::DrawRatioImage(Graphics *pGraphics, int x, int y, UINT nRatio, UINT nAlphaBlendRatio)
{
	if (!m_pBitmap)
		return FALSE;

	UINT nRatioWidth = (m_pBitmap->GetWidth() * nRatio) / 100;
	UINT nRatioHeight = (m_pBitmap->GetHeight() * nRatio) / 100;

	ImageAttributes *pImageAtt = GetCaculatedAlphaBlendAttributes(nAlphaBlendRatio);

	BOOL bRet = TRUE;

	//if (pGraphics->DrawImage(m_pBitmap, x, y, nPerWidth, nPerHeight) != Ok)
	if (pGraphics->DrawImage(m_pBitmap, Rect(x, y, nRatioWidth, nRatioHeight), 0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight(), UnitPixel, pImageAtt) != Ok)
		bRet = FALSE;

	delete pImageAtt;

	return bRet;
}

BOOL CCherryImage::DrawRatioImage(Graphics *pGraphics, CPoint point, UINT nRatio, UINT nAlphaBlendRatio)
{
	return DrawRatioImage(pGraphics, (int)point.x, (int)point.y, nRatio, nAlphaBlendRatio);
}
*/

Status CCherryImage::MakeStretchImage3x3(Graphics *pGraphics, Rect rect, UINT nAlphaBlendRatio)
{
	Status statusRet = Ok;

	try
	{
		if (NULL == m_pBitmap)
			throw GenericError;

		int nLeft = rect.GetLeft();
		int nTop = rect.GetTop();
		int nRight = rect.GetRight();
		int nBottom = rect.GetBottom();

		UINT nOrgWidth = m_pBitmap->GetWidth();
		UINT nOrgHeight = m_pBitmap->GetHeight();

		// 투명 이미지를 적용할 변환된 속성 가져오기
		ImageAttributes imageAttributes;

		// 투명 효과 적용
		CaculatedAlphaBlendAttributes(nAlphaBlendRatio, imageAttributes);

		// 상단 좌측
		if ((statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft, nTop, nOrgWidth / 3, nOrgHeight / 3),
			0, 0, nOrgWidth / 3, nOrgHeight / 3, UnitPixel, &imageAttributes)) != Ok)
			throw statusRet;

		// 상단 중간
		if ((statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft + nOrgWidth / 3, nTop, (nRight - nLeft) - nOrgWidth / 3 * 2, nOrgHeight / 3),
			nOrgWidth / 3, 0, nOrgWidth - nOrgWidth / 3 * 2, nOrgHeight / 3, UnitPixel, &imageAttributes)) != Ok)
			throw statusRet;

		// 상단 우측
		if ((statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nRight - nOrgWidth / 3, nTop, nOrgWidth / 3, nOrgHeight / 3),
			nOrgWidth - nOrgWidth / 3, 0, nOrgWidth / 3, nOrgHeight / 3, UnitPixel, &imageAttributes)) != Ok)
			throw statusRet;

		////////////////////////////////////////////////////////////////////////////////////////////////

		// 중앙 좌측
		int nVCenterHeight = nBottom - nTop - nOrgHeight / 3 * 2;
		if ((statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft, nTop + nOrgHeight / 3, nOrgWidth / 3, nVCenterHeight),
			0, nOrgHeight / 3, nOrgWidth / 3, nOrgHeight - nOrgHeight / 3 * 2, UnitPixel, &imageAttributes)) != Ok)
			throw statusRet;

		// 정중앙
		if ((statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft + nOrgWidth / 3, nTop + nOrgHeight / 3, nRight - nOrgWidth / 3 * 2, nVCenterHeight),
			nOrgWidth / 3, nOrgHeight / 3, nOrgWidth - nOrgWidth / 3 * 2, nOrgHeight - nOrgHeight / 3 * 2, UnitPixel, &imageAttributes)) != Ok)
			throw statusRet;

		// 중앙 우측
		if ((statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nRight - nOrgWidth / 3, nTop + nOrgHeight / 3, nOrgWidth / 3, nVCenterHeight),
			nOrgWidth - nOrgWidth / 3, nOrgHeight / 3, nOrgWidth / 3, nOrgHeight - nOrgHeight / 3 * 2, UnitPixel, &imageAttributes)) != Ok)
			throw statusRet;

		//////////////////////////////////////////////////////////////////////////////////////////////////

		// 하단 좌측
		if ((statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft, nBottom - nOrgHeight / 3, nOrgWidth / 3, nOrgHeight / 3),
			0, nOrgHeight - nOrgHeight / 3, nOrgWidth / 3, nOrgHeight / 3, UnitPixel, &imageAttributes)) != Ok)
			throw statusRet;

		// 하단 중앙
		if ((statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft + nOrgWidth / 3, nBottom - nOrgHeight / 3, (nRight - nLeft) - nOrgWidth / 3 * 2, nOrgHeight / 3),
			nOrgWidth / 3, nOrgHeight - nOrgHeight / 3, nOrgWidth - nOrgWidth / 3 * 2, nOrgHeight / 3, UnitPixel, &imageAttributes)) != Ok)
			throw statusRet;

		// 하단 우측
		statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nRight - nOrgWidth / 3, nBottom - nOrgHeight / 3, nOrgWidth / 3, nOrgHeight / 3),
			nOrgWidth - nOrgWidth / 3, nOrgHeight - nOrgHeight / 3, nOrgWidth / 3, nOrgHeight / 3, UnitPixel, &imageAttributes);
	}
	catch (const Status &e)
	{
		statusRet = e;
	}

	return statusRet;
}

CHERRY_RET CCherryImage::DrawStretchImage3x3(Graphics *pGraphics, int nLeft, int nTop, int nRight, int nBottom, UINT nAlphaBlendRatio)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		if (NULL == m_pBitmap)
			throw CCherryException::ERROR_IMAGE_FILE_NOT_FOUND;

		// 원본 이미지 보다 작거나 같은 경우 원본 이미지의 크기로 출력한다.
		if (nRight - nLeft <= (int)m_pBitmap->GetWidth())
			nRight = nLeft + m_pBitmap->GetWidth();

		if (nBottom - nTop <= (int)m_pBitmap->GetHeight())
			nBottom = nTop + m_pBitmap->GetHeight();

		if (m_bUseCachedImage)
		{
			if (UpdateCachedImage(pGraphics, CRect(nLeft, nTop, nRight, nBottom), nAlphaBlendRatio, TRUE) != Ok)
				throw CCherryException::ERROR_IMAGE_UPDATE_CACHED_IMAGE_FAIL;

			if (m_pCachedBitmap == NULL)
				throw CCherryException::ERROR_IMAGE_CACHED_IMAGE_NOT_LOADED;

			if (pGraphics->DrawCachedBitmap(m_pCachedBitmap, nLeft, nTop) != Ok)
				throw CCherryException::ERROR_IMAGE_DRAW_CACHED_IMAGE_FAIL;
		}
		else
		{
			if (MakeStretchImage3x3(pGraphics, Rect(nLeft, nTop, nRight - nLeft, nBottom - nTop), nAlphaBlendRatio) != Ok)
				throw CCherryException::ERROR_IMAGE_MAKE_IMAGE_FAIL;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		return errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryImage::DrawStretchImage3x3(Graphics *pGraphics, CRect rect, UINT nAlphaBlendRatio)
{
	return DrawStretchImage3x3(pGraphics, rect.left, rect.top, rect.right, rect.bottom, nAlphaBlendRatio);
}

UINT CCherryImage::GetWidth() const
{
	if (NULL == m_pBitmap)
		return 0;

	return m_pBitmap->GetWidth();
}

UINT CCherryImage::GetHeight() const
{
	if (NULL == m_pBitmap)
		return 0;

	return m_pBitmap->GetHeight();
}

BOOL CCherryImage::RemoveImage()
{
	if (!m_pBitmap)
		return FALSE;

	delete m_pBitmap;
	m_pBitmap = NULL;

	return TRUE;
}

BOOL CCherryImage::RemoveCachedImage()
{
	if (NULL == m_pCachedBitmap)
		return FALSE;

	delete m_pCachedBitmap;
	m_pCachedBitmap = NULL;

	m_cachedBitmapRect.SetRectEmpty();

	return TRUE;
}

Status CCherryImage::UpdateCachedImage(Graphics *pGraphics, CRect rect, UINT nAlphaBlendRatio, BOOL bUseDrawStretch3x3)
{
	Status statusRet = Ok;

	// 크기가 변경되었다면
	if (m_cachedBitmapRect.Width() != rect.Width() || m_cachedBitmapRect.Height() != rect.Height())
	{
		m_cachedBitmapRect = rect;

		Bitmap bitmap(rect.Width(), rect.Height(), pGraphics);
		Graphics memGraphics(&bitmap);

		if (bUseDrawStretch3x3)
		{
			if ((statusRet = MakeStretchImage3x3(&memGraphics, Rect(0, 0, rect.Width(), rect.Height()), nAlphaBlendRatio)) != Ok)
				return statusRet;
		}
		else
		{
			if ((statusRet = MakeImage(&memGraphics, Rect(0, 0, rect.Width(), rect.Height()), nAlphaBlendRatio)) != Ok)
				return statusRet;
		}

		RemoveCachedImage();

		m_pCachedBitmap = new CachedBitmap(&bitmap, pGraphics);
	}

	return statusRet;
}

Status CCherryImage::GetBitmapLastStatus() const
{
	return m_pBitmap->GetLastStatus();
}

Status CCherryImage::CaculatedAlphaBlendAttributes(UINT nAlphaBlendRatio, ImageAttributes &imageAttributes)
{
	// 투명 이미지 적용
	float fAlphaBlend = (float)nAlphaBlendRatio / 100;

	ColorMatrix colorMatrix = 
	{
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, fAlphaBlend, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	return imageAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
}

void CCherryImage::UseCachedImage(BOOL bUse)
{
	m_bUseCachedImage = bUse;
}

void CCherryImage::SetDefaultImagePath(LPCTSTR lpszPath)
{
	m_strDefaultImagePath = lpszPath;
}

//CCherryImage &CCherryImage::operator=(Bitmap *pBitmap)
//{	
//	//LoadImage(pBitmap);
//
//	m_pBitmap = pBitmap;
//
//	return *this;
//}

//CCherryImage::operator CCherryImage&()
//{
//	return *this;
//}
