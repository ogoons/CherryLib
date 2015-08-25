// CherryUtil.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "CherryUtil.h"

#include "afxinet.h"
#include "wininet.h"
#pragma comment(lib, "wininet.lib")

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
/// \brief		DLL 버전 가져오기
/// \author		ogoons
/// \date		2013-09-04
/// \param		
/// \return		DWORD type의 버전 HIWORD, LOWORD로 return
/// \remark		
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



CHERRYUTIL_DECL_API BOOL SendHttpRequest(_In_ LPCTSTR lpszURL, _In_ BOOL bPost, _In_ LPCTSTR lpszPostData, _Out_ CString &strResponse)
{
	DWORD dwServiceType = AFX_INET_SERVICE_HTTP;
	CString strServer;
	CString strObject;
	INTERNET_PORT port = INTERNET_INVALID_PORT_NUMBER;

	if (FALSE == AfxParseURL(lpszURL, dwServiceType, strServer, strObject, port))
		return FALSE;

	if (TRUE == strServer.IsEmpty())
		return FALSE;

	if (INTERNET_INVALID_PORT_NUMBER == port)
	{
		switch (dwServiceType)
		{
		case AFX_INET_SERVICE_HTTP:
			port = INTERNET_DEFAULT_HTTP_PORT;
			break;
		case AFX_INET_SERVICE_HTTPS:
			port = INTERNET_DEFAULT_HTTPS_PORT;
			break;
		default:
			return FALSE; // Empty port
		}
	}

	// for clarity, error-checking has been removed
	HINTERNET hSession = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (NULL == hSession)
		return FALSE;

	HINTERNET hConnect = InternetConnect(hSession, strServer, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

	if (NULL == hConnect)
	{
		InternetCloseHandle(hSession);

		return FALSE;
	}

	DWORD dwContext = 0;

	if (AFX_INET_SERVICE_HTTPS == dwServiceType)
		dwContext = INTERNET_FLAG_SECURE;

	const TCHAR *pszAccept[] = { _T("*/*"), NULL };

	HINTERNET hRequest = HttpOpenRequest(hConnect, TRUE == bPost ? _T("POST") : _T("GET"), strObject, NULL, NULL, pszAccept, dwContext, 0);

	if (NULL == hRequest)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);

		return FALSE;
	}

	const TCHAR *pszHeaders = _T("Content-Type: application/x-www-form-urlencoded");

	if (FALSE == HttpSendRequest(hRequest, pszHeaders, _tcslen(pszHeaders), TRUE == bPost ? (LPVOID)lpszPostData : NULL, TRUE == bPost ? _tcslen(lpszPostData) : 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);

		return FALSE;
	}

	DWORD dwReadSize = 1;
	DWORD dwBytesRead = 0;

	while (TRUE == InternetQueryDataAvailable(hRequest, &dwReadSize, 0, 0) && 0 < dwReadSize)
	{
		TCHAR *pszData = new TCHAR[dwReadSize + 1];
		ZeroMemory(pszData, dwReadSize + 1);

		InternetReadFile(hRequest, pszData, dwReadSize, &dwBytesRead);

		// 누적 복사
		strResponse += pszData;
		delete[]pszData;
	}

	return TRUE;
}

CHERRYUTIL_DECL_API BOOL DownloadFile(_In_ LPCTSTR lpszURL, _In_ LPCTSTR lpszReceivePath)
{
	HINTERNET hSession = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (NULL == hSession)
		return FALSE;

	HINTERNET hURL = InternetOpenUrl(hSession, lpszURL, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION, 0);

	if (NULL == hURL)
		return FALSE;

	CString strReceivePath(lpszReceivePath);
	CString strFileName(PathFindFileName(lpszURL));
	PathAppend(strReceivePath.GetBuffer(strReceivePath.GetLength() + strFileName.GetLength() + 1), strFileName);
	strReceivePath.ReleaseBuffer();

	HANDLE hFile = CreateFile(strReceivePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (NULL == hFile)
		return FALSE;

	DWORD dwReadSize = 1;
	DWORD dwBytesRead = 0;
	DWORD dwWritten = 0;

	while (TRUE == InternetQueryDataAvailable(hURL, &dwReadSize, 0, 0) && 0 < dwReadSize)
	{
		BYTE *pData = new BYTE[dwReadSize];
		ZeroMemory(pData, dwReadSize);

		InternetReadFile(hURL, pData, dwReadSize, &dwBytesRead);
		WriteFile(hFile, pData, dwBytesRead, &dwWritten, NULL);

		delete []pData;
	}

	CloseHandle(hFile);
	InternetCloseHandle(hURL);

	return TRUE;
}

CHERRYUTIL_DECL_API CString	ConvertANSIoUnicode(LPCSTR lpszANSI)
{
	if (NULL == lpszANSI)
		return NULL;

	int nLen = strlen(lpszANSI) * sizeof(TCHAR);
	TCHAR *pszBuf = new TCHAR[nLen + 1];
	ZeroMemory(pszBuf, nLen + 1);

	MultiByteToWideChar(CP_ACP, 0, lpszANSI, -1, pszBuf, nLen);
	
	CString strRet;

	if (NULL != pszBuf)
	{
		strRet = pszBuf;

		delete []pszBuf;
		pszBuf = NULL;
	}

	return strRet;
}

CHERRYUTIL_DECL_API CStringA ConvertUnicodeToANSI(LPCWSTR lpszUnicode)
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

		delete[]pszBuf;
		pszBuf = NULL;
	}

	return strRet;
}

CHERRYUTIL_DECL_API CString ConvertUTF8ToUnicode(_In_ LPCWSTR lpszUTF8)
{
	if (NULL == lpszUTF8)
		return NULL;

	int nLen = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)lpszUTF8, -1, NULL, 0);

	WCHAR *pszBuf = new WCHAR[nLen + 1];
	ZeroMemory(pszBuf, nLen + 1);

	// UTF8 -> Unicode
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)lpszUTF8, -1, pszBuf, nLen);

	CString strRet;

	if (NULL != pszBuf)
	{
		strRet = pszBuf;
		
		delete []pszBuf;
		pszBuf = NULL;
	}

	return strRet;
}

CHERRYUTIL_DECL_API CStringA ConvertUTF8ToANSI(_In_ LPCSTR lpszUTF8)
{
	// Calculate UTF-8 text length
	int nUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, lpszUTF8, -1, NULL, 0);

	WCHAR *pszUnicodeBuf = new WCHAR[nUnicodeLen];
	ZeroMemory(pszUnicodeBuf, nUnicodeLen);

	// UTF8 -> Unicode
	int nANSILen = MultiByteToWideChar(CP_UTF8, 0, lpszUTF8, -1, pszUnicodeBuf, nUnicodeLen);

	if (NULL != pszUnicodeBuf)
	{
		delete []pszUnicodeBuf;
		pszUnicodeBuf = NULL;
	}

	CHAR *pszANSIBuf = new CHAR[nANSILen + 1];
	ZeroMemory(pszANSIBuf, nANSILen + 1);

	// Unicode -> ANSI
	WideCharToMultiByte(CP_ACP, 0, pszUnicodeBuf, -1, pszANSIBuf, nANSILen, NULL, NULL);

	CStringA strRet;

	if (NULL != pszANSIBuf)
	{
		strRet = pszANSIBuf;

		delete []pszANSIBuf;
		pszANSIBuf = NULL;
	}

	return strRet;
}