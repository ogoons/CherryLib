// CherryUtil.h : CherryUtil DLL의 기본 헤더 파일입니다.
//

#pragma once

// CHERRYUTIL.H
#ifdef CHERRYUTIL_IMPL
#define CHERRYUTIL_DECL_API __declspec(dllexport)
#else
#define CHERRYUTIL_DECL_API __declspec(dllimport)
#endif

#ifdef CHERRYUTIL_IMPL
#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CCherryUtilApp
// 이 클래스의 구현을 보려면 CherryUtil.cpp를 참조하십시오.
//

class CCherryUtilApp : public CWinApp
{
public:
	CCherryUtilApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
#endif

/////////////////////////////////////////////////////////////////
// C/C++ functions
#ifdef __cplusplus
extern "C" 
{
#endif
	CHERRYUTIL_DECL_API DWORD	GetDllVersion(_In_ LPCTSTR lpszDllName, _Out_ DWORD &dwMajorVersion, _Out_ DWORD &dwMinorVersion, _Out_ DWORD &dwBuildNumber);
#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////
// C++ only functions

// Get special path functions
CHERRYUTIL_DECL_API CString		GetCurrentPath();

// WinInet functions
CHERRYUTIL_DECL_API BOOL		RequestHttp(_In_ LPCTSTR lpszUrl, _In_ BOOL bPost, _In_ LPCTSTR lpszPostData, _Out_ CString &strResponse);
CHERRYUTIL_DECL_API BOOL		DownloadFile(_In_ LPCTSTR lpszUrl, _In_ LPCTSTR lpszDownloadPath);

CHERRYUTIL_DECL_API CString		ConvertAnsiToUnicode(_In_ LPCSTR lpszAnsi);
CHERRYUTIL_DECL_API CStringA	ConvertUnicodeToAnsi(_In_ LPCWSTR lpszUnicode);
CHERRYUTIL_DECL_API CString		ConvertUtf8ToUnicode(_In_ LPCWSTR lpszUtf8);
CHERRYUTIL_DECL_API CStringA	ConvertUtf8ToAnsi(_In_ LPCSTR lpszUtf8);

CHERRYUTIL_DECL_API BOOL		GetFileSize(_In_ LPCTSTR lpszFilePath, _Out_ LONGLONG &llFileSize);
CHERRYUTIL_DECL_API BOOL		ExecuteProcess(_In_ LPCTSTR lpszPath, _In_ LPCTSTR lpszParam, _In_ BOOL bWaitProcess);
CHERRYUTIL_DECL_API int			CompareVersion(LPCTSTR lpszCurrentVersion, LPCTSTR lpszNewVersion);

CHERRYUTIL_DECL_API void		CreateDirectoryAndParent(LPTSTR lpszPath);				// 디렉토리 생성(path 중 상위 디렉토리가 없다면 전부 생성)
CHERRYUTIL_DECL_API BOOL		DeleteDirectoryAndChild(LPCTSTR lpszPath);				// 디렉토리 삭제(하위 디렉토리, 파일 모두 삭제)