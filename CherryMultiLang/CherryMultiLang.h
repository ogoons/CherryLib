#pragma once

#include "tinyxml2.h"

#define CHEERY_MULTI_LANG_INIT(x)		CCherryMultiLang::GetInstance()->LoadResource(x)
#define CMLSTR(x)						CCherryMultiLang::GetInstance()->GetString(x)

using namespace tinyxml2;

class AFX_EXT_CLASS CCherryMultiLang
{
public:
	CCherryMultiLang();
	~CCherryMultiLang();

private:
	tinyxml2::XMLDocument m_xmlDoc;

public:
	static CCherryMultiLang		*GetInstance();

	BOOL						LoadResource(LPCTSTR lpszXmlFile);
	CString						GetString(LPCTSTR lpszId);

	static CString		ConvertAnsiToUnicode(LPCSTR lpszAnsi);
	static CStringA		ConvertUnicodeToAnsi(LPCWSTR lpszUnicode);
	static CString		ConvertUtf8ToUnicode(const char *pszUtf8);
	static CString		ConvertUtf8ToUnicode(LPCWSTR lpszUtf8);
	static CStringA		ConvertUtf8ToAnsi(LPCSTR lpszUtf8);
};