
// clientDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "clientCon.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CclientDlg 대화 상자



CclientDlg::CclientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHAT, h_ChatLog);
	DDX_Control(pDX, IDC_EDIT_IP, h_ip);
	DDX_Control(pDX, IDC_EDIT_PORT, h_port);
	DDX_Control(pDX, IDC_EDIT_NAME, h_name);
	DDX_Control(pDX, IDC_COMBO_RECEIVER, h_comboReceiver);
	DDX_Control(pDX, IDC_LIST_CLIENT, h_ClientList);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, h_login_btn);
	DDX_Control(pDX, IDC_BUTTON_LOGOUT, h_logout_btn);
	DDX_Control(pDX, IDC_BUTTON_ONSEAT, h_InSeat_Btn);
	DDX_Control(pDX, IDC_BUTTON_OUTSEAT, h_OutSeat_Btn);
	DDX_Control(pDX, IDC_BUTTON_DELEGATION, h_Delegation_Btn);
	DDX_Control(pDX, IDC_BUTTON_KICK, h_Kick_Btn);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, h_message);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CclientDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CclientDlg::OnBnClickedButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CclientDlg::OnBnClickedButtonSend)
	ON_CBN_SELCHANGE(IDC_COMBO_RECEIVER, &CclientDlg::OnCbnSelchangeComboReceiver)
	ON_BN_CLICKED(IDC_BUTTON_ONSEAT, &CclientDlg::OnBnClickedButtonOnseat)
	ON_BN_CLICKED(IDC_BUTTON_OUTSEAT, &CclientDlg::OnBnClickedButtonOutseat)
	ON_BN_CLICKED(IDC_BUTTON_DELEGATION, &CclientDlg::OnBnClickedButtonDelegation)
	ON_BN_CLICKED(IDC_BUTTON_KICK, &CclientDlg::OnBnClickedButtonKick)
	ON_BN_CLICKED(IDC_BUTTON_GAMEBOARD, &CclientDlg::OnBnClickedButtonGameboard)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CclientDlg 메시지 처리기

BOOL CclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	h_ip.SetWindowTextW(_T("127.0.0.1"));
	h_port.SetWindowTextW(_T("9190"));
	h_name.SetWindowTextW(_T("anonymous"));
	h_comboReceiver.SetCurSel(0);
	h_Delegation_Btn.EnableWindow(FALSE);
	h_Kick_Btn.EnableWindow(FALSE);
	h_InSeat_Btn.EnableWindow(FALSE);
	h_OutSeat_Btn.EnableWindow(FALSE);

	isGameDlgInit = TRUE;
	m_pGame = new DialogGameBoard;
	m_pGame->Create(IDD_GAME_DIALOG);
	m_pGame->m_pClient = this;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CclientDlg::OnBnClickedButtonLogin()
{
	cTh = AfxBeginThread(StaticThreadFunc, this);

	//cTh->m_bAutoDelete = FALSE;
	m_Thread_handle = cTh->m_hThread;
	h_login_btn.EnableWindow(FALSE);
	h_logout_btn.EnableWindow(TRUE);
	h_InSeat_Btn.EnableWindow(FALSE);
	h_OutSeat_Btn.EnableWindow(TRUE);
}


void CclientDlg::OnBnClickedButtonLogout()
{
	m_pClient->SendData('S', 'C', "");		// 소켓 종료 메시지
	m_pClient->EndConnect();

	DWORD dwExitCode = 0;
	::GetExitCodeThread(cTh, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE){
		AfxMessageBox(_T("Can't close this"));
		return;
	}
	h_logout_btn.EnableWindow(FALSE);
	h_login_btn.EnableWindow(TRUE);
	h_InSeat_Btn.EnableWindow(FALSE);
	h_OutSeat_Btn.EnableWindow(FALSE);
	h_Delegation_Btn.EnableWindow(FALSE);
	h_Kick_Btn.EnableWindow(FALSE);

	h_ClientList.ResetContent();
	h_ChatLog.ResetContent();
}


void CclientDlg::OnBnClickedButtonSend()
{
	CString sTextData;
	GetDlgItemText(IDC_EDIT_MESSAGE, sTextData);

	CT2CA CStringToAscii(sTextData);
	std::string sResultedString(CStringToAscii);

	// LPCSTR ~ string

	if (m_pClient != NULL){
		int nIndex = h_comboReceiver.GetCurSel();	// 선택한 item의 index
		if (nIndex == CB_ERR) return;
		CString str;
		h_comboReceiver.GetLBText(nIndex, str);

		if (str == "전체") {
			m_pClient->set_uds('M', 'B', sResultedString);
		}
		else {
			m_pClient->set_uds('M', 'W', sResultedString);
		}
		string buf = m_pClient->get_uds();

		m_pClient->SendData(buf);
	}

	CWnd* pWnd = GetDlgItem(IDC_EDIT_MESSAGE);
	pWnd->SetWindowText(_T(""));

	h_comboReceiver.SetCurSel(0);
}

UINT __cdecl CclientDlg::StaticThreadFunc(LPVOID pParam)
{
	CclientDlg *pYourClass = reinterpret_cast<CclientDlg*>(pParam);
	UINT retCode = pYourClass->ThreadFunc();

	return retCode;
}

UINT CclientDlg::ThreadFunc()
{
	CString edit_ip;
	GetDlgItemText(IDC_EDIT_IP, edit_ip);

	CString edit_port;
	GetDlgItemText(IDC_EDIT_PORT, edit_port);
	int iPort = _wtoi(edit_port.GetString());

	CString edit_name;
	GetDlgItemText(IDC_EDIT_NAME, edit_name);

	m_pClient = new clientCon(this);
	m_pClient->m_pGame = m_pGame;

	CT2CA CStringToAscii(edit_ip);

	// LPCSTR ~ string
	std::string sResultedString(CStringToAscii);

	CT2CA CStringToAscii2(edit_name);

	std::string sResultedString2(CStringToAscii2);

	m_pClient->StartConnect(sResultedString, iPort, sResultedString2);
	return 0;
}

void CclientDlg::AddChatMsg(string sValue)
{
	CString strLine(sValue.c_str());		// string ~ char[]

	// add CR/LF to text
	//MessageBox(sValue.c_str());
	h_ChatLog.AddString(strLine);
	h_ChatLog.SetCurSel(-1);
	//DoModal();
	//UpdateData(TRUE);
}

void CclientDlg::AddNoticeMsg(string sValue)
{
	CString strLine(sValue.c_str());		// string ~ char[]
	h_ChatLog.AddString(strLine);
	h_ChatLog.SetCurSel(-1);
}

void CclientDlg::AddClientList(string sValue)
{
	CString strLine(sValue.c_str());
	// add CR/LF to text
	//MessageBox(sValue.c_str());
	h_ClientList.AddString(strLine);
	h_ClientList.SetCurSel(-1);
	//DoModal();
	//UpdateData(TRUE);
}

void CclientDlg::AddRecverCombo(string sValue)
{
	CString strLine(sValue.c_str());
	h_comboReceiver.AddString(strLine);
}

void CclientDlg::OnCbnSelchangeComboReceiver()
{
	int nIndex = h_comboReceiver.GetCurSel();	// 선택한 item의 index
	if (nIndex == CB_ERR) return;
	CString comboCStr;
	// 현재 아이템의 부가 정보를 읽어온다.
	// 여기서는 단순하게 숫자로 넣었지만, 포인터를 넣을 수 있으므로 모든 데이터를 다
	// 첨부할 수 있다.
	// 디비에서 사용자 정보 전체를 읽어와서 이름만 콤보박스에 넣는다면...
	// 나머지 정보를 구조체등에 넣은다음.. 그 포인터를 SetItemData()로 넣어두면
	// 언제든지 쉽게 접근할 수 있게된다.

	// 현재 선택된 문자열을 얻는다. 
	h_comboReceiver.GetLBText(nIndex, comboCStr);

	if (comboCStr == "전체") {
		h_message.SetSel(0, -1, TRUE);
		h_message.Clear();
		return;
	}

	CString comboName;
	int locateLeft, locateRight;
	locateLeft = comboCStr.Find('(', 0);
	locateRight = comboCStr.Find(')', 0) - locateLeft;
	comboName = comboCStr.Left(locateLeft);
	comboCStr = comboCStr.Mid(locateLeft + 1, locateRight - 1);

	CString myName;
	GetDlgItemText(IDC_EDIT_NAME, myName);

	if (comboName == myName) {
		MessageBox(L"Can't choose yourself");
		return;
	}
	else {
		comboCStr = comboCStr + _T("@");
		h_message.SetWindowTextW(comboCStr);
	}
}

void CclientDlg::OnBnClickedButtonOnseat()
{
	h_InSeat_Btn.EnableWindow(FALSE);
	h_OutSeat_Btn.EnableWindow(TRUE);

	if (m_pClient != NULL) {
		m_pClient->SendData('U', 'I',"");
	}

	h_comboReceiver.SetCurSel(0);
}


void CclientDlg::OnBnClickedButtonOutseat()
{
	h_InSeat_Btn.EnableWindow(TRUE);
	h_OutSeat_Btn.EnableWindow(FALSE);

	if (m_pClient != NULL) {
		m_pClient->SendData('U', 'O', "");
	}

	h_comboReceiver.SetCurSel(0);
}


void CclientDlg::OnBnClickedButtonDelegation()
{
	int nIndex = h_comboReceiver.GetCurSel();	// 선택한 item의 index
	if (nIndex == CB_ERR) return;
	CString str;
	// 현재 아이템의 부가 정보를 읽어온다.
	// 여기서는 단순하게 숫자로 넣었지만, 포인터를 넣을 수 있으므로 모든 데이터를 다
	// 첨부할 수 있다.
	// 디비에서 사용자 정보 전체를 읽어와서 이름만 콤보박스에 넣는다면...
	// 나머지 정보를 구조체등에 넣은다음.. 그 포인터를 SetItemData()로 넣어두면
	// 언제든지 쉽게 접근할 수 있게된다.

	// 현재 선택된 문자열을 얻는다. 
	h_comboReceiver.GetLBText(nIndex, str);

	if (str == "전체") {
		MessageBox(L"Please select user");
	}
	else {
		int locateLeft, locateRight;
		locateLeft = str.Find('(', 0);
		locateRight = str.Find(')', 0) - locateLeft;
		str = str.Mid(locateLeft + 1, locateRight - 1);
		CT2CA CStringToAscii(str);
		std::string sResultedString(CStringToAscii);
		m_pClient->SendData('U', 'D', sResultedString);
	}

	h_message.SetSel(0, -1, TRUE);
	h_message.Clear();
	h_comboReceiver.SetCurSel(0);
}


void CclientDlg::OnBnClickedButtonKick()
{
	int nIndex = h_comboReceiver.GetCurSel();	// 선택한 item의 index
	if (nIndex == CB_ERR) return;
	CString str;
	// 현재 아이템의 부가 정보를 읽어온다.
	// 여기서는 단순하게 숫자로 넣었지만, 포인터를 넣을 수 있으므로 모든 데이터를 다
	// 첨부할 수 있다.
	// 디비에서 사용자 정보 전체를 읽어와서 이름만 콤보박스에 넣는다면...
	// 나머지 정보를 구조체등에 넣은다음.. 그 포인터를 SetItemData()로 넣어두면
	// 언제든지 쉽게 접근할 수 있게된다.

	// 현재 선택된 문자열을 얻는다. 
	h_comboReceiver.GetLBText(nIndex, str);

	if (str == "전체") {
		MessageBox(L"Please select user");
	}
	else {
		int locateLeft, locateRight;
		locateLeft = str.Find('(', 0);
		locateRight = str.Find(')', 0) - locateLeft;
		str = str.Mid(locateLeft + 1, locateRight - 1);
		CT2CA CStringToAscii(str);
		std::string sResultedString(CStringToAscii);
		m_pClient->SendData('U', 'K', sResultedString);
	}

	h_message.SetSel(0, -1, TRUE);
	h_message.Clear();
	h_comboReceiver.SetCurSel(0);
}

void CclientDlg::SetOwnerMode() {
	AddNoticeMsg("you been owner.");
	h_Delegation_Btn.EnableWindow(TRUE);
	h_Kick_Btn.EnableWindow(TRUE);
}

void CclientDlg::SetPersonMode() {
	AddNoticeMsg("you been person.");
	h_Delegation_Btn.EnableWindow(FALSE);
	h_Kick_Btn.EnableWindow(FALSE);
}

void CclientDlg::PopupMessageBox(string msg) {
	CString PopupCStr(msg.c_str());
	AfxMessageBox(PopupCStr);

	m_pClient->EndConnect();

	DWORD dwExitCode = 0;
	::GetExitCodeThread(cTh, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE) {
		AfxMessageBox(_T("Can't close this"));
		return;
	}
	h_logout_btn.EnableWindow(FALSE);
	h_login_btn.EnableWindow(TRUE);
	h_InSeat_Btn.EnableWindow(FALSE);
	h_OutSeat_Btn.EnableWindow(FALSE);
	h_Delegation_Btn.EnableWindow(FALSE);
	h_Kick_Btn.EnableWindow(FALSE);

	h_ClientList.ResetContent();
	h_ChatLog.ResetContent();
}


void CclientDlg::OnBnClickedButtonGameboard()
{
	BOOL stat = m_pGame->IsWindowVisible();

	if (stat) m_pGame->ShowWindow(SW_HIDE);
	else m_pGame->ShowWindow(SW_SHOW);
}


void CclientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete m_pGame;  //메모리 할당 해제
}
