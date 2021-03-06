
// serverDlg.h: 헤더 파일
//

#pragma once
#include "serverManager.h"
#include <Windows.h>

// CserverDlg 대화 상자
class CserverDlg : public CDialogEx
{
// 생성입니다.
public:
	CserverDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
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
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT nIDEvent);

	static UINT __cdecl StaticThreadFunc(LPVOID pParam);
	UINT ThreadFunc();
	void ShowServerLog(string sValue);
	void ShowChatLog(string sValue);
	void AddClientList(string sValue);
	void DelayUpdatePlz();

	CListBox h_clientList;
	CListBox h_logList;
	CListBox h_chatList;
	CEdit h_port;
	serverManager *m_pServer;

private:
	HANDLE m_Thread_handle;
	CWinThread *cTh;	
};
