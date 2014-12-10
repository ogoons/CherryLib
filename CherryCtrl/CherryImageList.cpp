#include "stdafx.h"
#include "CherryImageList.h"

CCherryImageList::CCherryImageList()
{
}

CCherryImageList::CCherryImageList(const CCherryImageList &imageList)
{
	SetImageList(imageList);
}

CCherryImageList::~CCherryImageList()
{
}

void CCherryImageList::Add(LPCTSTR lpszImagePath, BOOL bUseCachedImage)
{
	//CCherryImage *pImage = new CCherryImage(lpszImagePath, bUseCachedImage);
	
	// 복사를 위한 임시 객체
	CCherryImage image(lpszImagePath, bUseCachedImage);
	m_imageList.push_back(image);
}

void CCherryImageList::Add(CCherryImage &image)
{
	m_imageList.push_back(image);
}

CCherryImage *CCherryImageList::Extract(INT_PTR nImage)
{
	return &m_imageList[nImage];
}

INT_PTR CCherryImageList::GetCount()
{
	return (INT_PTR)m_imageList.size();
}

BOOL CCherryImageList::IsEmpty()
{
	return m_imageList.empty() ? TRUE : FALSE;
}

void CCherryImageList::Remove(INT_PTR nImage)
{
	m_imageList.erase(m_imageList.begin() + nImage);
}

void CCherryImageList::RemoveAll()
{
	m_imageList.clear();
}

void CCherryImageList::SetImageList(const CCherryImageList &imageList)
{
	// vector 복사
	m_imageList.clear();
	m_imageList.assign(imageList.m_imageList.begin(), imageList.m_imageList.end());	
}

void CCherryImageList::operator=(const CCherryImageList &imageList)
{
	SetImageList(imageList);
}

CCherryImage *CCherryImageList::operator[](INT_PTR nIndex)
{
	return &m_imageList[nIndex];
}