
// serverDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "server.h"
#include "serverDlg.h"
#include "afxdialogex.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WATCHDOG_TIMER 1
#define UPDATE_DELAY 2
#define DELAY_START_TIMER 3
#define PLAYER_SET_TIMER 4
#define GAME_START_DELAY 5

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


// CserverDlg 대화 상자



CserverDlg::CserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIENT, h_clientList);
	DDX_Control(pDX, IDC_LIST_LOG, h_logList);
	DDX_Control(pDX, IDC_LIST_CHAT, h_chatList);
	DDX_Control(pDX, IDC_EDIT_PORT, h_port);
}

BEGIN_MESSAGE_MAP(CserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CserverDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CserverDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CserverDlg 메시지 처리기

BOOL CserverDlg::OnInitDialog()
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

	h_port.SetWindowTextW(_T("9190"));				// Edit control 세팅

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CserverDlg::OnPaint()
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
HCURSOR CserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CserverDlg::OnBnClickedButtonStart()
{
	cTh = AfxBeginThread(StaticThreadFunc, this);
	m_Thread_handle = cTh->m_hThread;
	SetTimer(WATCHDOG_TIMER, 10000, NULL);
	SetTimer(DELAY_START_TIMER, 5000, NULL);
}


void CserverDlg::OnBnClickedButtonStop()
{
	CloseHandle(m_Thread_handle);

	//AfxEndThread(0 ,true);
	//DWORD dwCode;  
	//GetExitCodeThread(cTh->m_hThread, &dwCode);  
	//delete cTh; 
	//Sleep(1);
	//m_pServer->ClearServer();
	KillTimer(WATCHDOG_TIMER);

	delete m_pServer;

	exit(0);
}

void CserverDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {
	case WATCHDOG_TIMER:
		{		
			string watchdogStr = "test";
			CString watchdogCStr(watchdogStr.c_str());
			m_pServer->socketCheck(); 
		}
		break;

	case UPDATE_DELAY:
		{
			KillTimer(UPDATE_DELAY);

			string clientArrMsg = m_pServer->get_all_client_list();
			m_pServer->set_uds('S', 'A', clientArrMsg);		// 모든 클라이언트로 추가된 client 추가 요청
			string buf = m_pServer->get_uds();
			m_pServer->sendAll_sock(buf);
		}
		break;
	case DELAY_START_TIMER:
		{
			KillTimer(DELAY_START_TIMER);	
			SetTimer(PLAYER_SET_TIMER, 10000, NULL); 
		}
		break;
	case PLAYER_SET_TIMER:
		ShowServerLog("Player search...");
		if (m_pServer->playerSet()) {
			SetTimer(GAME_START_DELAY, 1000, NULL);
		}
		break;
	case GAME_START_DELAY:
		{
			m_pServer->gameStartPlayerNameSet();
			KillTimer(GAME_START_DELAY);
		}
		break;
	}
}

UINT __cdecl CserverDlg::StaticThreadFunc(LPVOID pParam)
{
	CserverDlg *pYourClass = reinterpret_cast<CserverDlg *>(pParam);
	UINT retCode = pYourClass->ThreadFunc();

	return retCode;
}

UINT CserverDlg::ThreadFunc()
{
	// Do your thing, this thread now has access to all the classes member variables
	CString edit_port;
	GetDlgItemText(IDC_EDIT_PORT, edit_port);
	int iPort = _wtoi(edit_port.GetString());

	if (iPort == 0)
	{
		return -1;
	}
	m_pServer = new serverManager(this);
	m_pServer->StartListening(iPort);
	return 0;
}

void CserverDlg::ShowServerLog(string sValue)
{
	CString strLine(sValue.c_str());
	// add CR/LF to text
	//MessageBox(sValue.c_str());
	h_logList.AddString(strLine);
	h_logList.SetCurSel(-1);
	//DoModal();
	//UpdateData(TRUE);
}

void CserverDlg::ShowChatLog(string sValue)
{
	CString strLine(sValue.c_str());
	// add CR/LF to text
	//MessageBox(sValue.c_str());
	h_chatList.AddString(strLine);
	h_chatList.SetCurSel(-1);
	//DoModal();
	//UpdateData(TRUE);
}

void CserverDlg::AddClientList(string sValue)
{
	CString strLine(sValue.c_str());
	// add CR/LF to text
	//MessageBox(sValue.c_str());
	h_clientList.AddString(strLine);
	h_clientList.SetCurSel(-1);
	//DoModal();
	//UpdateData(TRUE);
}

void CserverDlg::DelayUpdatePlz() {
	SetTimer(UPDATE_DELAY, 1000, NULL);
}