#pragma once

#include "string.h"
#include "string"

using namespace std;

// DialogGameBoard 대화 상자

class CclientDlg;

typedef struct GAME_BOARD {
	char color;
	int turn;
} GAME_BOARD;

class DialogGameBoard : public CDialog
{
	DECLARE_DYNAMIC(DialogGameBoard)

public:
	DialogGameBoard(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DialogGameBoard();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int turn;
	int even_odd;
	int type;
	GAME_BOARD *game_board;
	CclientDlg *m_pClient;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic h_staticVS;
	CStatic h_staticLEFT;
	CStatic h_staticRIGHT;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void playerSet(int even_odd);
	void spectatorSet();
	void setPlayerName(string blackPlayer, string whitePlayer);
	void boardUpdate(int locateX, int locateY);
	void gameEnd();
};
