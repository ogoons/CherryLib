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

/*
판단 가능한 Windows 버전

- Windows XP
- Windows XP SP1
- Windows XP SP2
- Windows XP SP3
- Windows Vista SP1
- Windows Vista SP2
- Windows 7
- Windows 7 SP1
- Windows 8
- Windows 8.1
*/

extern "C" CHERRYUTIL_DECL_API BOOL		IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor);
extern "C" CHERRYUTIL_DECL_API DWORD	GetDllVersion(_In_ LPCTSTR lpszDllName, _Out_ DWORD &dwMajorVersion, _Out_ DWORD &dwMinorVersion, _Out_ DWORD &dwBuildNumber);