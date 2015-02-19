// CherryUtil.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "CherryUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며 
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//

// CCherryUtilApp

BEGIN_MESSAGE_MAP(CCherryUtilApp, CWinApp)
END_MESSAGE_MAP()


// CCherryUtilApp 생성

CCherryUtilApp::CCherryUtilApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CCherryUtilApp 개체입니다.

CCherryUtilApp theApp;


// CCherryUtilApp 초기화

BOOL CCherryUtilApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		Windows 버전 확인
/// \author		ogoons
/// \date		2013-11-07
/// \param		
/// \return		BOOL
/// \remark		파라미터로 입력된 버전 이상이면 TRUE 하위 버전이면 FALSE
/// \section	
///
///////////////////////////////////////////////////////////////////////////
extern "C" CHERRYUTIL_DECL_API BOOL IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	OSVERSIONINFOEX osVersionInfoEx = { sizeof(OSVERSIONINFOEX), 0, 0, 0, 0, { 0 }, 0, 0 };
	DWORDLONG const dwlConditionMask = VerSetConditionMask(
		VerSetConditionMask(
		VerSetConditionMask(
		0, VER_MAJORVERSION, VER_GREATER_EQUAL),
		VER_MINORVERSION, VER_GREATER_EQUAL),
		VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

	osVersionInfoEx.dwMajorVersion = wMajorVersion;
	osVersionInfoEx.dwMinorVersion = wMinorVersion;
	osVersionInfoEx.wServicePackMajor = wServicePackMajor;

	return VerifyVersionInfo(&osVersionInfoEx, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		DLL 버전 가져오기
/// \author		ogoons
/// \date		2013-09-04
/// \param		
/// \return		
/// \remark		DWORD를 HIWORD, LOWORD로 받을 수 있다.
/// \section	
///
///////////////////////////////////////////////////////////////////////////
extern "C" CHERRYUTIL_DECL_API DWORD GetDllVersion(_In_ LPCTSTR lpszDllName, _Out_ DWORD &dwMajorVersion, _Out_ DWORD &dwMinorVersion, _Out_ DWORD &dwBuildNumber)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwVersion = 0;
	HINSTANCE hDll = LoadLibrary(lpszDllName);

	if (hDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hDll, "DllGetVersion");

		if (pDllGetVersion)
		{
			DLLVERSIONINFO dllVersionInfo;	

			ZeroMemory(&dllVersionInfo, sizeof(DLLVERSIONINFO));
			dllVersionInfo.cbSize = sizeof(DLLVERSIONINFO);

			HRESULT hr = (*pDllGetVersion)(&dllVersionInfo);

			if (SUCCEEDED(hr))
			{
				dwVersion = MAKELONG(dllVersionInfo.dwMajorVersion, dllVersionInfo.dwMinorVersion);

				dwMajorVersion = dllVersionInfo.dwMajorVersion;
				dwMinorVersion = dllVersionInfo.dwMinorVersion;
				dwBuildNumber = dllVersionInfo.dwBuildNumber;
			}
			else
			{
				// 실패하면 모두 0
				dwMajorVersion = dwMinorVersion = dwBuildNumber = 0;
			}
		}

		FreeLibrary(hDll);
	}

	return dwVersion;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		현재 프로세스의 디렉토리 경로를 반환
/// \author		ogoons
/// \date		2015-01-20
/// \param		
/// \return		CString
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CHERRYUTIL_DECL_API CString GetCurrentPath()
{
	CString strPath;
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();

	PathRemoveFileSpec(strPath.GetBuffer(MAX_PATH));
	strPath.ReleaseBuffer();

	return strPath;
}
