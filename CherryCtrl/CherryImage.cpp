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

	return cherryRet;
}

CHERRY_RET CCherryImage::LoadImage(CCherryImage *pImage, int nLeft, int nTop, int nWidth, int nHeight, BOOL bUseCachedImage)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		RemoveImage();
		RemoveCachedImage();

		// 깊은 복사
		m_pBitmap = pImage->GetBitmap()->Clone(nLeft, nTop, nWidth, nHeight, PixelFormatDontCare);

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

	return cherryRet;
}

CHERRY_RET CCherryImage::LoadImage(CCherryImage *pImage, Rect rect, BOOL bUseCachedImage)
{
	return LoadImage(pImage, rect.GetLeft(), rect.GetTop(), rect.Width, rect.Height, bUseCachedImage);
}

CHERRY_RET CCherryImage::LoadImage(CCherryImage *pImage, BOOL bUseCachedImage)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		RemoveImage();
		RemoveCachedImage();

		m_pBitmap = pImage->GetBitmap()->Clone(0, 0, pImage->GetWidth(), pImage->GetHeight(), PixelFormatDontCare);

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
		if (Ok != (statusRet = CaculatedAlphaBlendAttributes(nAlphaBlendRatio, imageAttributes)))
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
			if (Ok != UpdateCachedImage(pGraphics, CRect(nLeft, nTop, nLeft + m_pBitmap->GetWidth(), nTop + m_pBitmap->GetHeight()), nAlphaBlendRatio))
				throw CCherryException::ERROR_IMAGE_UPDATE_CACHED_IMAGE_FAIL;

			if (NULL == m_pCachedBitmap)
				throw CCherryException::ERROR_IMAGE_CACHED_IMAGE_NOT_LOADED;

			if (Ok != pGraphics->DrawCachedBitmap(m_pCachedBitmap, nLeft, nTop))
				throw CCherryException::ERROR_IMAGE_DRAW_CACHED_IMAGE_FAIL;
		}
		else
		{
			if (Ok != MakeImage(pGraphics, Rect(nLeft, nTop, nLeft + m_pBitmap->GetWidth(), nTop + m_pBitmap->GetHeight()), nAlphaBlendRatio))
				throw CCherryException::ERROR_IMAGE_MAKE_IMAGE_FAIL;
		}

	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
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
			if (Ok != UpdateCachedImage(pGraphics, CRect(nLeft, nTop, nRight, nBottom), nAlphaBlendRatio))
				throw CCherryException::ERROR_IMAGE_UPDATE_CACHED_IMAGE_FAIL;

			if (NULL == m_pCachedBitmap)
				throw CCherryException::ERROR_IMAGE_CACHED_IMAGE_NOT_LOADED;

			if (Ok != pGraphics->DrawCachedBitmap(m_pCachedBitmap, nLeft, nTop))
				throw CCherryException::ERROR_IMAGE_DRAW_CACHED_IMAGE_FAIL;
		}
		else
		{
			if (Ok != MakeImage(pGraphics, Rect(nLeft, nTop, nRight - nLeft, nBottom - nTop), nAlphaBlendRatio))
				throw CCherryException::ERROR_IMAGE_MAKE_IMAGE_FAIL;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
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
			if (Ok != UpdateCachedImage(pGraphics, rect, nAlphaBlendRatio))
				throw CCherryException::ERROR_IMAGE_UPDATE_CACHED_IMAGE_FAIL;

			if (NULL == m_pCachedBitmap)
				throw CCherryException::ERROR_IMAGE_CACHED_IMAGE_NOT_LOADED;

			if (Ok != pGraphics->DrawCachedBitmap(m_pCachedBitmap, rect.left, rect.top))
				throw CCherryException::ERROR_IMAGE_DRAW_CACHED_IMAGE_FAIL;
		}
		else
		{
			if (Ok != MakeImage(pGraphics, Rect(rect.left, rect.top, rect.Width(), rect.Height()), nAlphaBlendRatio))
				throw CCherryException::ERROR_IMAGE_MAKE_IMAGE_FAIL;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

Status CCherryImage::Make9PatchImage(Graphics *pGraphics, Rect rect, UINT nAlphaBlendRatio)
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

		UINT nRawWidth = m_pBitmap->GetWidth();
		UINT nRawHeight = m_pBitmap->GetHeight();

		UINT nDivWidth = m_pBitmap->GetWidth() / 3;
		UINT nDivHeight = m_pBitmap->GetHeight() / 3;

		// 투명 이미지를 적용할 변환된 속성 가져오기
		ImageAttributes imageAttributes;

		// 투명 효과 적용
		CaculatedAlphaBlendAttributes(nAlphaBlendRatio, imageAttributes);

		// 상단 좌측
		if (Ok != (statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft, nTop, nDivWidth, nDivHeight),
			0, 0, nDivWidth, nDivHeight, UnitPixel, &imageAttributes)))
			throw statusRet;

		// 상단 중간
		if (Ok != (statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft + nDivWidth, nTop, (nRight - nLeft) - nDivWidth * 2, nDivHeight),
			nDivWidth, 0, nRawWidth - nDivWidth * 2, nDivHeight, UnitPixel, &imageAttributes)))
			throw statusRet;

		// 상단 우측
		if (Ok != (statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nRight - nDivWidth, nTop, nDivWidth, nDivHeight),
			nRawWidth - nDivWidth, 0, nDivWidth, nDivHeight, UnitPixel, &imageAttributes)))
			throw statusRet;

		////////////////////////////////////////////////////////////////////////////////////////////////

		// 중앙 좌측
		int nVCenterHeight = nBottom - nTop - nDivHeight * 2;
		if (Ok != (statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft, nTop + nDivHeight, nDivWidth, nVCenterHeight),
			0, nDivHeight, nDivWidth, nRawHeight - nDivHeight * 2, UnitPixel, &imageAttributes)))
			throw statusRet;

		// 정중앙
		if (Ok != (statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft + nDivWidth, nTop + nDivHeight, nRight - nDivWidth * 2, nVCenterHeight),
			nDivWidth, nDivHeight, nRawWidth - nDivWidth * 2, nRawHeight - nDivHeight * 2, UnitPixel, &imageAttributes)))
			throw statusRet;

		// 중앙 우측
		if (Ok != (statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nRight - nDivWidth, nTop + nDivHeight, nDivWidth, nVCenterHeight),
			nRawWidth - nDivWidth, nDivHeight, nDivWidth, nRawHeight - nDivHeight * 2, UnitPixel, &imageAttributes)))
			throw statusRet;

		//////////////////////////////////////////////////////////////////////////////////////////////////

		// 하단 좌측
		if (Ok != (statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft, nBottom - nDivHeight, nDivWidth, nDivHeight),
			0, nRawHeight - nDivHeight, nDivWidth, nDivHeight, UnitPixel, &imageAttributes)))
			throw statusRet;

		// 하단 중앙
		if (Ok != (statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nLeft + nDivWidth, nBottom - nDivHeight, (nRight - nLeft) - nDivWidth * 2, nDivHeight),
			nDivWidth, nRawHeight - nDivHeight, nRawWidth - nDivWidth * 2, nDivHeight, UnitPixel, &imageAttributes)))
			throw statusRet;

		// 하단 우측
		statusRet = pGraphics->DrawImage(m_pBitmap, Rect(nRight - nDivWidth, nBottom - nDivHeight, nDivWidth, nDivHeight),
			nRawWidth - nDivWidth, nRawHeight - nDivHeight, nDivWidth, nDivHeight, UnitPixel, &imageAttributes);
	}
	catch (const Status &e)
	{
		statusRet = e;
	}

	return statusRet;
}

CHERRY_RET CCherryImage::Draw9PatchImage(Graphics *pGraphics, int nLeft, int nTop, int nRight, int nBottom, UINT nAlphaBlendRatio)
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
			if (Ok != UpdateCachedImage(pGraphics, CRect(nLeft, nTop, nRight, nBottom), nAlphaBlendRatio, TRUE))
				throw CCherryException::ERROR_IMAGE_UPDATE_CACHED_IMAGE_FAIL;

			if (NULL == m_pCachedBitmap)
				throw CCherryException::ERROR_IMAGE_CACHED_IMAGE_NOT_LOADED;

			if (Ok != pGraphics->DrawCachedBitmap(m_pCachedBitmap, nLeft, nTop))
				throw CCherryException::ERROR_IMAGE_DRAW_CACHED_IMAGE_FAIL;
		}
		else
		{
			if (Ok != Make9PatchImage(pGraphics, Rect(nLeft, nTop, nRight - nLeft, nBottom - nTop), nAlphaBlendRatio))
				throw CCherryException::ERROR_IMAGE_MAKE_IMAGE_FAIL;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

CHERRY_RET CCherryImage::Draw9PatchImage(Graphics *pGraphics, CRect rect, UINT nAlphaBlendRatio)
{
	return Draw9PatchImage(pGraphics, rect.left, rect.top, rect.right, rect.bottom, nAlphaBlendRatio);
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
			if (Ok != (statusRet = Make9PatchImage(&memGraphics, Rect(0, 0, rect.Width(), rect.Height()), nAlphaBlendRatio)))
				return statusRet;
		}
		else
		{
			if (Ok != (statusRet = MakeImage(&memGraphics, Rect(0, 0, rect.Width(), rect.Height()), nAlphaBlendRatio)))
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

HRGN CCherryImage::GetHRGN(RGN_TYPE rgnType, short nAlpha) const
{
	if (NULL == m_pBitmap)
		return NULL;

	Rect imageRect(0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight());
	Region region;
	region.MakeEmpty();
	region.Union(imageRect);

	BitmapData *pBitmapData = new BitmapData(); 
	m_pBitmap->LockBits(&imageRect, ImageLockModeRead, PixelFormat32bppARGB, pBitmapData);

	UINT *pPixel = (UINT *)pBitmapData->Scan0;

	for (UINT i = 0; i < GetHeight(); i++)
	{
		for (UINT j = 0; j < GetWidth(); j++)
		{
			DWORD dwAlpha = (pPixel[i * pBitmapData->Stride / 4 + j] & 0xff000000) >> 3 * 8;
			switch (rgnType)
			{
			case RGN_TYPE_VISIBLE:		// 보이는 부분만 가져온다. (반투명 포함)	
				if (0 < dwAlpha)
					region.Xor(Rect(j, i, 1, 1));
				break;
			case RGN_TYPE_INVISIBLE: 	// 안보이는 (완전 불투명) 부분만 가져온다. 
				if (0 == dwAlpha)
					region.Xor(Rect(j, i, 1, 1));
				break;
			case RGN_TYPE_VISIBLE_THRESHOLD:	// 보이는 부분 alpha 값 임계치를 설정하여 가져온다.
				if (nAlpha < (short)dwAlpha)
					region.Xor(Rect(j, i, 1, 1));
				break;
			case RGN_TYPE_INVISIBLE_THRESHOLD:  // 안보이는 부분 alpha 값 임계치를 설정하여 가져온다.
				if (nAlpha >(short)dwAlpha)
					region.Xor(Rect(j, i, 1, 1));
				break;
			default:
				break;
			}
		}
	}

	Graphics graphics(m_pBitmap);
	HRGN hRgn = region.GetHRGN(&graphics); // 추후에 DeleteObject 필요

	m_pBitmap->UnlockBits(pBitmapData);
	if (NULL != pBitmapData)
		delete pBitmapData;

	return hRgn;
}

//HRGN CCherryImage::GetStretchRgn3x3(RGN_TYPE rgnType, CRect newRect, short nAlpha)
//{
//	if (NULL == m_pBitmap)
//		return NULL;
//
//	// 3x3 Stretch 이미지
//	Bitmap *pNewBitmap = new Bitmap(newRect.Width(), newRect.Height(), m_pBitmap->GetPixelFormat());
//	Graphics graphics(pNewBitmap);
//	Make9PatchImage(&graphics, Rect(0, 0, newRect.Width(), newRect.Height()));
//
//	// Alpha 영역 추리기
//	Rect imageRect(0, 0, newRect.Width(), newRect.Height());
//	Region region;
//	region.MakeEmpty();
//	region.Union(imageRect);
//
//	BitmapData *pBitmapData = new BitmapData();
//	pNewBitmap->LockBits(&imageRect, ImageLockModeRead, PixelFormat32bppARGB, pBitmapData);
//
//	UINT *pPixel = (UINT *)pBitmapData->Scan0;
//
//	for (UINT i = 0; i < pNewBitmap->GetHeight(); i++)
//	{
//		for (UINT j = 0; j < pNewBitmap->GetWidth(); j++)
//		{
//			DWORD dwAlpha = (pPixel[i * pBitmapData->Stride / 4 + j] & 0xff000000) >> 3 * 8;
//			switch (rgnType)
//			{
//			case RGN_TYPE_VISIBLE:			// 투명한 부분 제외
//				if (0 == dwAlpha)
//					region.Xor(Rect(j, i, 1, 1));
//				break;
//			case RGN_TYPE_INVISIBLE:		// 불투명한 부분 제외
//				if (0 < dwAlpha)
//					region.Xor(Rect(j, i, 1, 1));
//				break;
//			case RGN_TYPE_VISIBLE_THRESHOLD:
//				if (nAlpha > (short)dwAlpha)
//					region.Xor(Rect(j, i, 1, 1)); // 사용자가 설정한 Alpha 값보다 투명하면 제외
//				break;
//			case RGN_TYPE_INVISIBLE_THRESHOLD:
//				if (nAlpha < (short)dwAlpha)
//					region.Xor(Rect(j, i, 1, 1)); // 사용자가 설정한 Alpha 값보다 불투명하면 제외
//				break;
//			default:
//				break;
//			}
//		}
//	}
//
//	HRGN hRgn = region.GetHRGN(&graphics); // 추후에 DeleteObject 필요
//	pNewBitmap->UnlockBits(pBitmapData);
//
//	if (NULL != pBitmapData)
//	{
//		delete pBitmapData;
//		pBitmapData = NULL;
//	}
//
//	if (NULL != pNewBitmap)
//	{
//		delete pNewBitmap;
//		pNewBitmap = NULL;
//	}
//
//	return hRgn;
//}

Bitmap *CCherryImage::ExtractBitmap(CRect extractRect)
{
	if (NULL == m_pBitmap)
		return NULL;

	return m_pBitmap->Clone(extractRect.left, extractRect.top, extractRect.Width(), extractRect.Height(), PixelFormatDontCare);
}

CCherryImage *CCherryImage::ExtractImage(CRect extractRect)
{
	return new CCherryImage(ExtractBitmap(extractRect));
}

Bitmap *CCherryImage::Extract9PatchBitmap(CRect stretchRect, CRect extractRect, UINT nAlphaBlendRatio, _Out_ CHERRY_RET &cherryRet)
{
	Bitmap *pExtractedBitmap = NULL;
	Bitmap *pStretchedBitmap = NULL;

	try
	{
		if (NULL == m_pBitmap)
			throw CCherryException::ERROR_IMAGE_FILE_NOT_FOUND;

		pStretchedBitmap = new Bitmap(stretchRect.Width(), stretchRect.Height(), PixelFormat32bppARGB);
		Graphics stretchedGraphics(pStretchedBitmap);

		// 원본 이미지 보다 작거나 같은 경우 원본 이미지의 크기로 출력한다.
		if (stretchRect.Width() <= (int)m_pBitmap->GetWidth())
			stretchRect.right = stretchRect.left + m_pBitmap->GetWidth();

		if (stretchRect.Height() <= (int)m_pBitmap->GetHeight())
			stretchRect.bottom = stretchRect.top + m_pBitmap->GetHeight();

		if (m_bUseCachedImage)
		{
			if (Ok != UpdateCachedImage(&stretchedGraphics, stretchRect, nAlphaBlendRatio, TRUE))
				throw CCherryException::ERROR_IMAGE_UPDATE_CACHED_IMAGE_FAIL;

			if (NULL == m_pCachedBitmap)
				throw CCherryException::ERROR_IMAGE_CACHED_IMAGE_NOT_LOADED;

			if (Ok != stretchedGraphics.DrawCachedBitmap(m_pCachedBitmap, stretchRect.left, stretchRect.top))
				throw CCherryException::ERROR_IMAGE_DRAW_CACHED_IMAGE_FAIL;
		}
		else
		{
			if (Ok != Make9PatchImage(&stretchedGraphics, Rect(0, 0, stretchRect.Width(), stretchRect.Height()), nAlphaBlendRatio))
				throw CCherryException::ERROR_IMAGE_MAKE_IMAGE_FAIL;
		}

		pExtractedBitmap = pStretchedBitmap->Clone((INT)extractRect.left, (INT)extractRect.top, (INT)extractRect.Width(), (INT)extractRect.Height(), PixelFormatDontCare);
		
		if (Ok != pStretchedBitmap->GetLastStatus())
			throw CCherryException::ERROR_IMAGE_EXTRACT_IMAGE_FAIL;
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	if (NULL != pStretchedBitmap)
	{
		delete pStretchedBitmap;
		pStretchedBitmap = NULL;
	}

	return pExtractedBitmap;
}

CCherryImage *CCherryImage::Extract9PatchImage(CRect stretchRect, CRect extractRect, UINT nAlphaBlendRatio, _Out_ CHERRY_RET &cherryRet)
{
	CCherryImage *pExtract9PatchImage = NULL;

	try
	{
		Bitmap *pExtract9PatchBitmap = Extract9PatchBitmap(stretchRect, extractRect, nAlphaBlendRatio, cherryRet);
		if (CCherryException::ERROR_CHERRY_SUCCESS != cherryRet)
			throw cherryRet;

		pExtract9PatchImage = new CCherryImage(pExtract9PatchBitmap);
				
		if (NULL != pExtract9PatchBitmap)
		{
			delete pExtract9PatchBitmap;
			pExtract9PatchBitmap = NULL;
		}
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return pExtract9PatchImage;
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
