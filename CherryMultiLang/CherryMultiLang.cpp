#include "stdafx.h"
#include "CherryMultiLang.h"

#ifdef _DEBUG
#pragma comment(lib, "TinyXML2D.lib")
#else
#pragma comment(lib, "TinyXML2.lib")
#endif

CCherryMultiLang::CCherryMultiLang()
{
}

CCherryMultiLang::~CCherryMultiLang()
{
}

CCherryMultiLang *CCherryMultiLang::GetInstance()
{
	static CCherryMultiLang instance;
	return &instance;
}

BOOL CCherryMultiLang::LoadResource(LPCTSTR lpszXmlFile)
{
	USES_CONVERSION;

	CStdioFile file;
	CString strBuf;
	TCHAR *pszBuf = nullptr;
	if (file.Open(lpszXmlFile, CFile::modeRead | CFile::typeText))
	{
		UINT nLen = (UINT)file.GetLength() + 1;
		pszBuf = new TCHAR[nLen];
		ZeroMemory(pszBuf, nLen);

		file.Read(pszBuf, nLen);
		strBuf = ConvertUtf8ToUnicode(pszBuf);
		delete[] pszBuf;
		file.Close();
	}

	if (strBuf.IsEmpty())
		return FALSE;

	if (XML_SUCCESS != m_xmlDoc.Parse(W2CA(strBuf)))
		return FALSE;

	return TRUE;
}

CString CCherryMultiLang::GetString(LPCTSTR lpszId)
{
	if (m_xmlDoc.Error())
		return _T("");

	XMLNode *pRootNode = m_xmlDoc.RootElement();

	if (!pRootNode)
		return _T("");

	XMLNode *pNode = pRootNode->FirstChild();

	do
	{
		CString strId = CA2W(pNode->ToElement()->Attribute("id"));
		if (0 == _tcscmp(lpszId, strId))
		{
			CString strData = CA2W(pNode->ToElement()->GetText());
			return strData;
		}
	} while (pNode = pNode->NextSibling());

	return _T("");
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		ANSI > Unicode
/// \author		ogoons
/// \date		2016-09-16
/// \param		
/// \return		CString
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CString	CCherryMultiLang::ConvertAnsiToUnicode(LPCSTR lpszAnsi)
{
	if (NULL == lpszAnsi)
		return NULL;

	int nLen = strlen(lpszAnsi) * sizeof(TCHAR);
	TCHAR *pszBuf = new TCHAR[nLen + 1];
	ZeroMemory(pszBuf, nLen + 1);

	MultiByteToWideChar(CP_ACP, 0, lpszAnsi, -1, pszBuf, nLen);

	CString strRet;

	if (NULL != pszBuf)
	{
		strRet = pszBuf;

		delete[] pszBuf;
		pszBuf = NULL;
	}

	return strRet;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		Unicode > ANSI
/// \author		ogoons
/// \date		2016-09-16
/// \param		
/// \return		CStringA
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CStringA CCherryMultiLang::ConvertUnicodeToAnsi(LPCWSTR lpszUnicode)
{
	if (NULL == lpszUnicode)
		return NULL;

	int nLen = wcslen(lpszUnicode) / sizeof(CHAR);
	CHAR *pszBuf = new CHAR[nLen + 1];
	ZeroMemory(pszBuf, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, lpszUnicode, -1, pszBuf, nLen, NULL, NULL);

	CStringA strRet;

	if (NULL != pszBuf)
	{
		strRet = pszBuf;

		delete[] pszBuf;
		pszBuf = NULL;
	}

	return strRet;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		UTF-8 > Unicode
/// \author		ogoons
/// \date		2016-09-16
/// \param		
/// \return		CString
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CString CCherryMultiLang::ConvertUtf8ToUnicode(_In_ const char *pszUtf8)
{
	if (NULL == pszUtf8)
		return NULL;

	int nLen = MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, NULL, 0);

	WCHAR *pszBuf = new WCHAR[nLen + 1];
	ZeroMemory(pszBuf, nLen + 1);

	// UTF8 -> Unicode
	MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, pszBuf, nLen);

	CString strRet;

	if (NULL != pszBuf)
	{
		strRet = pszBuf;

		delete[] pszBuf;
		pszBuf = NULL;
	}

	return strRet;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		UTF-8 > Unicode
/// \author		ogoons
/// \date		2016-09-16
/// \param		
/// \return		CString
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CString CCherryMultiLang::ConvertUtf8ToUnicode(_In_ LPCWSTR lpszUtf8)
{
	if (NULL == lpszUtf8)
		return NULL;

	int nLen = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)lpszUtf8, -1, NULL, 0);

	WCHAR *pszBuf = new WCHAR[nLen + 1];
	ZeroMemory(pszBuf, nLen + 1);

	// UTF8 -> Unicode
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)lpszUtf8, -1, pszBuf, nLen);

	CString strRet;

	if (NULL != pszBuf)
	{
		strRet = pszBuf;

		delete[] pszBuf;
		pszBuf = NULL;
	}

	return strRet;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		UTF-8 > ANSI
/// \author		ogoons
/// \date		2016-09-16
/// \param		
/// \return		CStringA
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CStringA CCherryMultiLang::ConvertUtf8ToAnsi(_In_ LPCSTR lpszUtf8)
{
	// Calculate UTF-8 text length
	int nUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, lpszUtf8, -1, NULL, 0);

	WCHAR *pszUnicodeBuf = new WCHAR[nUnicodeLen];
	ZeroMemory(pszUnicodeBuf, nUnicodeLen);

	// UTF8 -> Unicode
	int nAnsiLen = MultiByteToWideChar(CP_UTF8, 0, lpszUtf8, -1, pszUnicodeBuf, nUnicodeLen);

	if (NULL != pszUnicodeBuf)
	{
		delete[] pszUnicodeBuf;
		pszUnicodeBuf = NULL;
	}

	CHAR *pszAnsiBuf = new CHAR[nAnsiLen + 1];
	ZeroMemory(pszAnsiBuf, nAnsiLen + 1);

	// Unicode -> ANSI
	WideCharToMultiByte(CP_ACP, 0, pszUnicodeBuf, -1, pszAnsiBuf, nAnsiLen, NULL, NULL);

	CStringA strRet;

	if (NULL != pszAnsiBuf)
	{
		strRet = pszAnsiBuf;

		delete[] pszAnsiBuf;
		pszAnsiBuf = NULL;
	}

	return strRet;
}