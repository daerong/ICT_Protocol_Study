
// clientDlg.h: 헤더 파일
//

#pragma once
#include "clientCon.h"
#include "DialogGameBoard.h"
#include <Windows.h>
#include "resource.h"

static BOOL isGameDlgInit = FALSE; //InitDialog 실행여부 파악용 전역변수

// CclientDlg 대화 상자
class CclientDlg : public CDialogEx
{
// 생성입니다.
public:
	CclientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnBnClickedButtonSend();

	static UINT __cdecl StaticThreadFunc(LPVOID pParam);
	UINT ThreadFunc();
	void AddChatMsg(string sValue);
	void AddNoticeMsg(string sValue);
	void AddClientList(string sValue);
	void AddRecverCombo(string sValue);
	void SetOwnerMode();
	void SetPersonMode();
	void PopupMessageBox(string msg);

	clientCon *m_pClient;
	DialogGameBoard *m_pGame;

private:
	HANDLE m_Thread_handle;
	CWinThread *cTh;
public:
	CListBox h_ChatLog;
	CEdit h_ip;
	CEdit h_port;
	CEdit h_name;
	CComboBox h_comboReceiver;
	CListBox h_ClientList;
	afx_msg void OnCbnSelchangeComboReceiver();
	CButton h_login_btn;
	CButton h_logout_btn;
	afx_msg void OnBnClickedButtonOnseat();
	afx_msg void OnBnClickedButtonOutseat();
	afx_msg void OnBnClickedButtonDelegation();
	afx_msg void OnBnClickedButtonKick();
	CButton h_InSeat_Btn;
	CButton h_OutSeat_Btn;
	CButton h_Delegation_Btn;
	CButton h_Kick_Btn;
	CEdit h_message;
	afx_msg void OnBnClickedButtonGameboard();
	afx_msg void OnDestroy();
};
