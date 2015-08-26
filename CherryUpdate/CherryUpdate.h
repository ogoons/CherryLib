#pragma once

//#define USE_MFC_CLASS
//#define INCLUDE_FILE_SIZE_INFO

#include "afxinet.h"
#include "wininet.h"
#pragma comment(lib, "wininet.lib")

#include "sensapi.h"
#pragma comment(lib, "sensapi.lib")

#include <vector>
using namespace std;

#include "tinyxml2.h"
#ifdef _DEBUG
#pragma comment(lib, "TinyXML2D.lib")
#else
#pragma comment(lib, "TinyXML2.lib")
#endif
using namespace tinyxml2;

#include "CherryException.h"

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryUpdate
/// \brief		라이브 업데이트 매니저 클래스
/// \author		ogoons
/// \date		2014-05-19
///
/// \remark		Win32 / MFC 버전 두 가지로 구분되며 
///             상단 USE_MFC_CLASS define을 사용하면 MFC 클래스를 사용한
///             라이브 업데이트 dll 빌드가 가능하다.
///             속도의 이점이 크므로 Win32 버전 빌드를 추천한다.
///
/// \warning	- 한글 이름으로 된 파일을 다운로드 할 수 없다.
///				- 관리자 권한 필수	
///				- TinyXML2 라이브러리 필요
/// \section	
///
///////////////////////////////////////////////////////////////////////////

#ifdef USE_MFC_CLASS
class AFX_EXT_CLASS CCherryUpdate : public CInternetSession
#else
class AFX_EXT_CLASS CCherryUpdate
#endif
{
// Constructors
public:
	CCherryUpdate();
	BOOL Open(
		LPCTSTR lpszAgent,
		LPCTSTR lpszClientVersion,
		LPCTSTR lpszRootUrl,
		LPCTSTR lpszUpdateProfileXml,
		LPCTSTR lpszPatchTargetPath,
		BOOL bUseUtf8 = TRUE);							// 업데이트에 필요한 정보를 모두 메모리에 로드하고 업데이트 준비 시작

// Attributes
protected:
#ifndef USE_MFC_CLASS
	HINTERNET m_hSession;
#endif

#ifdef INCLUDE_FILE_SIZE_INFO
	// 파일 리스트에 담을 구조체
	typedef struct _FILE_ATTR
	{
		CString		strFile;
		LONGLONG	llSize;
	} FILE_ATTR, *PFILE_ATTR;
#endif

	// 업데이트 프로파일 정보 xml에서 가져온 데이터를 담을 구조체
	typedef struct _UPDATE_PROFILE
	{
		//vector<CString> packageFileVector;

#ifdef INCLUDE_FILE_SIZE_INFO
		vector<FILE_ATTR> fileUrlVector;
#else
		vector<CString> fileUrlVector;
#endif
		CString			strNewVerion;
		CString			strRootUrl;
		BOOL			bPackageUpdate;
		CString			strRunPackageFilePath;
		ULONGLONG		llFileTotalSize;

		void Initialize()
		{
			fileUrlVector.clear();
			strNewVerion.Empty();
			strRootUrl.Empty();
			bPackageUpdate = TRUE;
			strRunPackageFilePath.Empty();
			llFileTotalSize = 0;
		}
	} UPDATE_PROFILE, *PUPDATE_PROFILE;

	UPDATE_PROFILE m_updateProfile;

	CString		m_strAgent;
	CString		m_strClientVersion;
	CString		m_strPatchTargetPath;

	CWinThread	*m_pUpdateProcessThread;
	BOOL		m_bStarted;
	BOOL		m_bRunPackage;
	ULONGLONG	m_llReceivedSize; // 현재까지 다운로드 받은 사이즈 기록

	// Callback
	void (*m_pDownloadStatusCallback)(ULONGLONG, ULONGLONG, void *);
	void (*m_pDownloadCompleteCallback)(BOOL, BOOL, void *);
	void (*m_pPatchCompleteCallback)(BOOL, BOOL, void *);

	void *m_pDownloadStatusUserData;
	void *m_pDownloadCompleteUserData;
	void *m_pPatchCompleteUserData;

// Operations
public:
	BOOL		IsOpen() const;																									// Internet session이 정상 open되어서 업데이트가 가능한 상태인지
	BOOL		IsStarted() const;																								// 업데이트 시작 여부
	BOOL		CheckNewVersion(LPCTSTR lpszCurrentVersion);																	// 새 버전 존재 여부
	CHERRY_RET	OpenUpdateProfile(LPCTSTR lpszRootUrl, LPCTSTR lpszUpdateProfileXml, BOOL bUseUtf8 = TRUE);						// 업데이트 정보 파일 열기(다운로드, 파싱, 메모리 로드)

	BOOL		StartUpdate();																									// 업데이트 시작
	BOOL		StopUpdate();																									// 업데이트 중단

	BOOL		DownloadFile(_In_ LPCTSTR lpszUrl, _In_ LPCTSTR lpszDownloadPath, _Out_ CString &strReceivedFullPath);			// 파일 다운로드
	BOOL		SendHttpRequest(_In_ LPCTSTR lpszUrl, _In_ BOOL bPost, _In_ LPCTSTR lpszPostData, _Out_ CString &strResponse);	// 서버에 웹페이지 요청 보내기

	ULONGLONG	GetTotalReceiveFileSize();																						// 받을 파일의 총 사이즈 가져오기 (Open이 성공했을 때 가능)
	void		EnableRunPackage(BOOL bRun = TRUE);																				// 패키지 업데이트 시 패키지 파일의 실행/비실행 여부

	// Set callback
	void		SetDownloadStatusCallback(LPVOID lpCallbackFunc, void *pUserData);												// 다운로드 진행 상태 변화 시 호출
	void		SetDownloadCompleteCallback(LPVOID lpCallbackFunc, void *pUserData);											// 다운로드 완료 시 호출
	void		SetPatchCompleteCallback(LPVOID lpCallbackFunc, void *pUserData);												// 다운로드 된 파일을 해당 디렉토리로 복사 완료 시 호출

	// Utilities
	static BOOL		IsInternetConnected();																						// 인터넷 연결 여부
	static BOOL		GetFileVersion(_In_ LPCTSTR lpszFilePath, _Out_ CString &strVersion);										// 파일의 버전 가져오기		
	static BOOL		GetFileSize(_In_ LPCTSTR lpszFilePath, _Out_ LONGLONG &llFileSize);											// 파일의 사이즈 가져오기
	static BOOL		ExecuteProcess(_In_ LPCTSTR lpszPath, _In_ LPCTSTR lpszParam = NULL, _In_ BOOL bWaitProcess = FALSE);		// 프로세스 실행															// 파일 실행
	static int		CompareVersion(LPCTSTR lpszCurrentVersion, LPCTSTR lpszNewVersion);											// 버전 비교
	static void		CreateDirectoryAndParent(LPTSTR lpszPath);																	// 디렉토리 생성(path 중 상위 디렉토리가 없다면 전부 생성)
	static BOOL		DeleteDirectoryAndChild(LPCTSTR lpszPath);																	// 디렉토리 삭제(하위 디렉토리, 파일 모두 삭제)
	static CString	ConvertUtf8ToUnicode(_In_ LPCWSTR lpszUtf8);																// UTF-8 -> Unicode(UTF-8 LE)
	void			UrlEncode(char *output, char *input);																		// 현재 사용 안 함

#ifdef USE_MFC_CLASS
	virtual void	Close();
#else
	BOOL			Close();																									// 세션 닫기
#endif

protected:
	void			Initialize();																								// 초기화
	CString			GetIncomingPath();																							// 다운로드된 파일이 임시 저장될 디렉토리 경로 가져오기

	void			ParseFileNode(BOOL bPackageNode, XMLNode *pFileListNode, LPCTSTR lpszRootUrl, BOOL bUseUtf8);				// 파일 리스트 노드를 파싱하여 가져온다. (내부에서만 사용)

	// Thread
	static UINT		UpdateProcessThread(LPVOID lpParam);																		// 파일 다운로드에서 복사까지의 모든 업데이트 진행 시퀀스 (Worker thread)

// Implementation
public:
	virtual ~CCherryUpdate();
};