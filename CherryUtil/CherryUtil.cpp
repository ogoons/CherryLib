// CherryUtil.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "CherryUtil.h"

#include "afxinet.h"
#include "wininet.h"
#pragma comment(lib, "wininet.lib")

#include "winver.h"
#pragma comment(lib, "version.lib")

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
/// \brief		현재 프로세스의 실행 파일 위치를 반환
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

///////////////////////////////////////////////////////////////////////////
///
/// \brief		HTTP 요청
/// \author		ogoons
/// \date		2016-09-16
/// \param		
/// \return		BOOL
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CHERRYUTIL_DECL_API BOOL RequestHttp(_In_ LPCTSTR lpszUrl, _In_ BOOL bPost, _In_ LPCTSTR lpszPostData, _Out_ CString &strResponse)
{
	DWORD dwServiceType = AFX_INET_SERVICE_HTTP;
	CString strServer;
	CString strObject;
	INTERNET_PORT port = INTERNET_INVALID_PORT_NUMBER;

	if (FALSE == AfxParseURL(lpszUrl, dwServiceType, strServer, strObject, port))
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
		delete[] pszData;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		파일 다운로드
/// \author		ogoons
/// \date		2016-09-16
/// \param		
/// \return		BOOL
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CHERRYUTIL_DECL_API BOOL DownloadFile(_In_ LPCTSTR lpszUrl, _In_ LPCTSTR lpszDownloadPath)
{
	HINTERNET hSession = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (NULL == hSession)
		return FALSE;

	HINTERNET hUrl = InternetOpenUrl(hSession, lpszUrl, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION, 0);

	if (NULL == hUrl)
		return FALSE;

	CString strDownloadPath(lpszDownloadPath);
	CString strFileName(PathFindFileName(lpszUrl));
	PathAppend(strDownloadPath.GetBuffer(strDownloadPath.GetLength() + strFileName.GetLength() + 1), strFileName);
	strDownloadPath.ReleaseBuffer();

	HANDLE hFile = CreateFile(strDownloadPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (NULL == hFile)
		return FALSE;

	DWORD dwReadSize = 1;
	DWORD dwBytesRead = 0;
	DWORD dwWritten = 0;

	while (TRUE == InternetQueryDataAvailable(hUrl, &dwReadSize, 0, 0) && 0 < dwReadSize)
	{
		BYTE *pData = new BYTE[dwReadSize];
		ZeroMemory(pData, dwReadSize);

		InternetReadFile(hUrl, pData, dwReadSize, &dwBytesRead);
		WriteFile(hFile, pData, dwBytesRead, &dwWritten, NULL);

		delete[] pData;
	}

	CloseHandle(hFile);
	InternetCloseHandle(hUrl);

	return TRUE;
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
CHERRYUTIL_DECL_API CString	ConvertAnsiToUnicode(LPCSTR lpszAnsi)
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
CHERRYUTIL_DECL_API CStringA ConvertUnicodeToAnsi(LPCWSTR lpszUnicode)
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
CHERRYUTIL_DECL_API CString ConvertUtf8ToUnicode(_In_ LPCWSTR lpszUtf8)
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
CHERRYUTIL_DECL_API CStringA ConvertUtf8ToAnsi(_In_ LPCSTR lpszUtf8)
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

///////////////////////////////////////////////////////////////////////////
///
/// \brief		파일 사이즈 반환
/// \author		ogoons
/// \date		2015-08-26
/// \param		
/// \return		BOOL
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CHERRYUTIL_DECL_API BOOL GetFileSize(_In_ LPCTSTR lpszFilePath, _Out_ LONGLONG &llFileSize)
{
	BOOL bRet = TRUE;

	HANDLE hFile = CreateFile(lpszFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	LARGE_INTEGER largeInt = { 0, };

	if (TRUE == GetFileSizeEx(hFile, &largeInt))
	{
		llFileSize = largeInt.QuadPart;
	}
	else
	{
		llFileSize = 0;
		bRet = FALSE;
	}

	CloseHandle(hFile);

	return bRet;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		파일 버전 반환
/// \author		ogoons
/// \date		2015-08-26
/// \param		
/// \return		BOOL
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CHERRYUTIL_DECL_API BOOL GetFileVersion(_In_ LPCTSTR lpszFilePath, _Out_ CString &strVersion)
{
	BOOL bRet = TRUE;

	DWORD dwHandle;
	DWORD dwVersionInfoSize = GetFileVersionInfoSize(lpszFilePath, &dwHandle);

	if (0 == dwVersionInfoSize)
		return FALSE;

	HANDLE hBuf = GlobalAlloc(GMEM_MOVEABLE, dwVersionInfoSize);
	LPVOID lpBuf = GlobalLock(hBuf);

	try
	{
		if (FALSE == GetFileVersionInfo(lpszFilePath, dwHandle, dwVersionInfoSize, lpBuf))
			throw FALSE;

		VS_FIXEDFILEINFO *pBuf = NULL;
		UINT nDataSize = 0;

		if (FALSE == VerQueryValue(lpBuf, _T("\\"), (void **)&pBuf, &nDataSize))
			throw FALSE;

		if (NULL == pBuf)
			throw FALSE;

		strVersion.Format(_T("%d.%d.%d.%d"), (pBuf->dwFileVersionMS / 65536),
			(pBuf->dwFileVersionMS % 65536), (pBuf->dwFileVersionLS / 65536),
			(pBuf->dwFileVersionLS % 65536));

	}
	catch (BOOL &bErrorRet)
	{
		bRet = bErrorRet;
	}

	GlobalUnlock(lpBuf);
	GlobalFree(hBuf);

	return bRet;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		프로세스 실행
/// \author		ogoons
/// \date		2015-08-26
/// \param		
/// \return		BOOL
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CHERRYUTIL_DECL_API BOOL ExecuteProcess(_In_ LPCTSTR lpszPath, _In_ LPCTSTR lpszParam, _In_ BOOL bWaitProcess)
{
	SHELLEXECUTEINFO shellExecInfo;
	ZeroMemory(&shellExecInfo, sizeof(shellExecInfo));

	shellExecInfo.cbSize = sizeof(shellExecInfo);
	shellExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shellExecInfo.hwnd = NULL;
	shellExecInfo.lpVerb = _T("open");
	shellExecInfo.lpParameters = lpszParam;
	shellExecInfo.nShow = SW_SHOWNORMAL;
	shellExecInfo.lpFile = lpszPath;

	// 패키지 실행
	if (FALSE == ShellExecuteEx(&shellExecInfo))
		return FALSE;

	if (TRUE == bWaitProcess)
	{
		DWORD dwRet = WaitForSingleObject(shellExecInfo.hProcess, INFINITE);

		switch (dwRet)
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_FAILED:
			return FALSE;
		default:
			break;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		버전 비교 함수
/// \author		ogoons
/// \date		2014-04-27
///
/// \param		lpszCurrentVersion : 현재 버전
///				lpszNewVersion : 새 버전
///
/// \return		-1 : 현 버전이 상위 버전.
///				 0 : 두 버전은 동일 버전.
///				 1 : 새 버전이 상위 버전.
///
/// \remark		ex. int nRet = CompareVersion(_T("1.0.0.1"), _T("1.0.0.2"));
///					printf(_T("Result: %d"), nRet);
///					.
///					.
///					.
///					Result: 1
///				
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CHERRYUTIL_DECL_API int CompareVersion(LPCTSTR lpszCurrentVersion, LPCTSTR lpszNewVersion)
{
	int nRet = 0; // is equals

	CString strCurVerBuf(lpszCurrentVersion);
	CString strNewVerBuf(lpszNewVersion);
	int nCurVerPos = 0;
	int nNewVerPos = 0;

	do
	{
		UINT nCurVerNum = (UINT)_ttoi(strCurVerBuf.Tokenize(_T("."), nCurVerPos));
		UINT nNewVerNum = (UINT)_ttoi(strNewVerBuf.Tokenize(_T("."), nNewVerPos));
		
		if (nNewVerNum > nCurVerNum)
		{
			nRet = 1;

			break;
		}
		else if (nNewVerNum < nCurVerNum)
		{
			nRet = -1;

			break;
		}
	} 
	while (-1 < nCurVerPos || -1 < nNewVerPos);

	return nRet;
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		디렉토리 생성 (상위 디렉토리 동시 생성)
/// \author		ogoons
/// \date		2014-02-10
/// \param		
/// \return		
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CHERRYUTIL_DECL_API void CreateDirectoryAndParent(LPTSTR lpszPath)
{
	TCHAR szDirName[MAX_PATH] = { 0, };	// 생성할 디렉초리 이름
	TCHAR *pszPath = lpszPath;			// 인자로 받은 디렉토리
	TCHAR *pszDirName = szDirName;

	while (NULL != *pszPath)
	{
		if ((_T('\\') == *pszPath) || (_T('/') == *pszPath))   // 루트디렉토리 혹은 Sub디렉토리
		{
			if (_T(':') != *(pszPath - 1))
			{
				CreateDirectory(szDirName, NULL);
			}
		}

		*pszDirName++ = *pszPath++;
		*pszDirName = _T('\0');
	}

	CreateDirectory(szDirName, NULL);
}

///////////////////////////////////////////////////////////////////////////
///
/// \brief		폴더 삭제 (하위 폴더 및 파일 삭제)
/// \author		ogoons
/// \date		2014-02-10
/// \param		
/// \return		
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////
CHERRYUTIL_DECL_API BOOL DeleteDirectoryAndChild(LPCTSTR lpszPath)
{
	if (NULL == lpszPath)
		return FALSE;

	CString strNextDirPath;
	CString strRootUrl;
	CFileFind fileFind;

	BOOL bRet = fileFind.FindFile(lpszPath);

	if (FALSE == bRet)
		return bRet;

	while (bRet)
	{
		bRet = fileFind.FindNextFile();

		if (TRUE == fileFind.IsDots())
			continue;

		if (TRUE == fileFind.IsDirectory())
		{
			strNextDirPath = fileFind.GetFilePath() + _T("\\*.*");
			DeleteDirectoryAndChild(strNextDirPath);	// 재귀호출
		}
		else
		{
			CString strFilePath(fileFind.GetFilePath());

			// Remove readonly attributes
			DWORD dwAttr = GetFileAttributes(strFilePath);
			if (FILE_ATTRIBUTE_READONLY & dwAttr)
				SetFileAttributes(strFilePath, dwAttr & ~FILE_ATTRIBUTE_READONLY);

			DeleteFile(strFilePath);
		}
	}

	strRootUrl = fileFind.GetRoot();
	fileFind.Close();

	return RemoveDirectory(strRootUrl);
}