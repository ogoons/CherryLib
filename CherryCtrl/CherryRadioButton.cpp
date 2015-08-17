#include "stdafx.h"
#include "CherryRadioButton.h"


// CCherryRadioButton

IMPLEMENT_DYNAMIC(CCherryRadioButton, CCherryCheckBox)

CCherryRadioButton::CCherryRadioButton()
{
	m_pHeadRadioButton = NULL;
}

CCherryRadioButton::~CCherryRadioButton()
{
}

BEGIN_MESSAGE_MAP(CCherryRadioButton, CCherryCheckBox)
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CCherryRadioButton 메시지 처리기입니다.
CHERRY_RET CCherryRadioButton::Create(LPCTSTR lpszCaption, LPCTSTR lpszImagePath, DWORD dwCherryStyle, CCherryRadioButton *pHeadRadioButton, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	CHERRY_RET cherryRet = CCherryException::ERROR_CHERRY_SUCCESS;

	try
	{
		dwStyle |= BS_OWNERDRAW;	// 추가

		if (!CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID))
			throw CCherryException::ERROR_RADIOBUTTON_CREATE_FAIL;

		// CherryCheckBox 전용 스타일 bit 플래그
		ModifyCherryStyle(0, dwCherryStyle);

		if (dwCherryStyle & STYLE_BUTTONTYPE)
		{
			GetNormalFont()->SetFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
			GetHoverFont()->SetFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
			GetDownFont()->SetFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
			GetDisableFont()->SetFontStyle(CCherryFont::STYLE_CENTER | CCherryFont::STYLE_VCENTER);
		}
		else
		{
			// 기본 수직 정렬
			GetNormalFont()->SetFontStyle(CCherryFont::STYLE_VCENTER);
			GetHoverFont()->SetFontStyle(CCherryFont::STYLE_VCENTER);
			GetDownFont()->SetFontStyle(CCherryFont::STYLE_VCENTER);
			GetDisableFont()->SetFontStyle(CCherryFont::STYLE_VCENTER);
		}

		// 기본 비활성화 텍스트 속성
		COLORREF disableColor = GetSysColor(COLOR_GRAYTEXT);
		GetDisableFont()->SetFontColor(Color(GetRValue(disableColor), GetGValue(disableColor), GetBValue(disableColor)));

		// 멤버로 그룹장의 핸들을 가지고 있는다.
		m_pHeadRadioButton = pHeadRadioButton;

		if (pHeadRadioButton)
			pHeadRadioButton->AddRadioVector(this);
		else
			AddRadioVector(this);

		cherryRet = SetImage(lpszImagePath);
	}
	catch (const CHERRY_RET &errorRet)
	{
		cherryRet = errorRet;
	}

	return cherryRet;
}

void CCherryRadioButton::SetCheck(STATUS checkStatus)
{
	if (!GetSafeHwnd())
		return;

	// 자기 자신은 체크
	m_checkStatus = checkStatus;

	Invalidate(FALSE);

	//if (checkStatus == STATUS_CHECKED)
		// 부모 윈도우에게 이 컨트롤의 클릭 메시지 발생 시킴
		//GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED));

	if (GetCherryStyle() & STYLE_AUTOCHECKBOX)
	{
		vector<CCherryRadioButton *> *pRadioVector = GetHeadRadioVector();

		for (vector<CCherryRadioButton *>::iterator it = pRadioVector->begin(); it != pRadioVector->end(); ++it)
		{
			// 눌린 자신 외 라디오인 경우
			if (GetSafeHwnd() != ((CCherryRadioButton *)*it)->GetSafeHwnd())
			{
				// 나머지는 전부 체크 해제
				((CCherryRadioButton *)*it)->m_checkStatus = STATUS_UNCHECKED;
				((CCherryRadioButton *)*it)->Invalidate(FALSE);
			}
		}
	}
}

void CCherryRadioButton::AddRadioVector(CCherryRadioButton *pRadioButton)
{
	m_radioVector.push_back(pRadioButton);
}

vector<CCherryRadioButton *> *CCherryRadioButton::GetRadioVector()
{
	return &m_radioVector;
}

vector<CCherryRadioButton *> *CCherryRadioButton::GetHeadRadioVector()
{
	vector<CCherryRadioButton *> *pRadioVector = NULL;

	if (m_pHeadRadioButton) // 그룹원인 경우
		pRadioVector = m_pHeadRadioButton->GetRadioVector();
	else // 그룹장인 경우
		pRadioVector = &m_radioVector;

	return pRadioVector;
}

CCherryRadioButton *CCherryRadioButton::GetHeadRadioButton() const
{
	return m_pHeadRadioButton;
}

void CCherryRadioButton::SetHeadRadioButton(CCherryRadioButton *pRadioButton)
{
	m_pHeadRadioButton = pRadioButton;
}

void CCherryRadioButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetCheck(STATUS_CHECKED);

	CCherryButton::OnLButtonUp(nFlags, point); // warning: CCherryCheckBox는 call하지 않는다.
}

LRESULT CCherryRadioButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	/*
	if (message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED)
	{
		UINT nCtrlID = LOWORD(wParam);
		
	}
	*/

	return CCherryCheckBox::DefWindowProc(message, wParam, lParam);
}

void CCherryRadioButton::OnDestroy()
{
	CCherryCheckBox::OnDestroy();

	vector<CCherryRadioButton *> *pRadioVector = GetHeadRadioVector();

	for (vector<CCherryRadioButton *>::iterator it = pRadioVector->begin(); it != pRadioVector->end(); ++it)
	{
		// 자기 자신을 벡터에서 제거하자
		if (GetSafeHwnd() == ((CCherryRadioButton *)*it)->GetSafeHwnd())
		{
			pRadioVector->erase(it);

			break;
		}
	}

	// destroy 되는 자신이 그룹장인 경우 (그룹장 위임하기)
	if (m_pHeadRadioButton == NULL)
	{
		for (vector<CCherryRadioButton *>::iterator it = pRadioVector->begin(); it != pRadioVector->end(); ++it)
		{
			// 그룹장이 제거된 벡터 중에 남아있는 것 중에 첫 번째 것을 그룹장으로 위임하자.
			if (it == pRadioVector->begin())
			{
				((CCherryRadioButton *)*it)->GetRadioVector()->assign(pRadioVector->begin(), pRadioVector->end()); // 그룹장에게 벡터 복사
				((CCherryRadioButton *)*it)->SetHeadRadioButton(NULL); // NULL 로 그룹장 정식 임명

				continue;
			}

			((CCherryRadioButton *)*it)->SetHeadRadioButton(pRadioVector->at(0)); // 나머지 그룹원들은 그룹장의 포인터를 가지고 있어야겠다.
		}
	}
}
