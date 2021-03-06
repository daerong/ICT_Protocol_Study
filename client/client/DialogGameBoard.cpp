// DialogGameBoard.cpp: 구현 파일
//

#include "stdafx.h"
#include "client.h"
#include "DialogGameBoard.h"
#include "afxdialogex.h"
#include "ClientDlg.h"

#define MATRIX_SIZE 19
#define MATRIX_LINE 20
#define INTERVAL 20
#define MARGIN INTERVAL/2
#define X_START 10
#define X_END X_START + INTERVAL*MATRIX_LINE
#define Y_START 50
#define Y_END Y_START + INTERVAL*MATRIX_LINE

#define STONE_SIZE 8

#define BLACK -1
#define NOTHING 0
#define WHITE 1

#define SPECTATOR 0
#define PLAYER 1

// DialogGameBoard 대화 상자

IMPLEMENT_DYNAMIC(DialogGameBoard, CDialog)

DialogGameBoard::DialogGameBoard(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_GAME_DIALOG, pParent),turn(0),type(SPECTATOR), even_odd(1)
{
	game_board = new GAME_BOARD[MATRIX_SIZE*MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE*MATRIX_SIZE; i++) {
		game_board[i].color = NOTHING;
	}
}

DialogGameBoard::~DialogGameBoard()
{
	delete game_board;
}

void DialogGameBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VS, h_staticVS);
	DDX_Control(pDX, IDC_STATIC_LEFT, h_staticLEFT);
	DDX_Control(pDX, IDC_STATIC_RIGHT, h_staticRIGHT);
}


BEGIN_MESSAGE_MAP(DialogGameBoard, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// DialogGameBoard 메시지 처리기


BOOL DialogGameBoard::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 240, 180));

	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}


void DialogGameBoard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오
	
	CPen pen;
	CPen* oldPen = dc.SelectObject(&pen);
	CBrush brush;
	brush.CreateSolidBrush(RGB(219, 169, 1));     // 오렌지색 채움색을 생성
	CBrush blackfill;
	blackfill.CreateSolidBrush(RGB(0, 0, 0));
	CBrush* oldBrush = dc.SelectObject(&brush);
	dc.Rectangle(X_START, Y_START, X_END, Y_END);
	dc.SelectObject(oldBrush);

	for (int x = 1; x <= MATRIX_SIZE; x++) {
		dc.MoveTo(X_START + x * INTERVAL, Y_START);
		dc.LineTo(X_START + x * INTERVAL, Y_END);
	}

	for (int y = 1; y <= MATRIX_SIZE; y++) {
		dc.MoveTo(X_START, Y_START + y * INTERVAL);
		dc.LineTo(X_END, Y_START + y * INTERVAL);
	}
	
	for (int y = 0; y < MATRIX_SIZE; y++) {
		for (int x = 0; x < MATRIX_SIZE; x++) {
			int locateX = X_START + INTERVAL + INTERVAL * x;
			int locateY = Y_START + INTERVAL + INTERVAL * y;
			switch (game_board[MATRIX_SIZE*y + x].color) {
			case BLACK:
			{
				dc.SelectObject(&blackfill);
				dc.Ellipse(locateX - STONE_SIZE, locateY - STONE_SIZE, locateX + STONE_SIZE, locateY + STONE_SIZE);
				dc.SelectObject(oldBrush);
			}
				break;
			case WHITE:
				dc.Ellipse(locateX - STONE_SIZE, locateY - STONE_SIZE, locateX + STONE_SIZE, locateY + STONE_SIZE);
				break;
			}
		}
	}

	// 아이콘을 그립니다.
}


HBRUSH DialogGameBoard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	UINT nID = pWnd->GetDlgCtrlID();

	switch (nID)
	{
	case IDC_STATIC_VS:
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	case IDC_STATIC_LEFT:
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	case IDC_STATIC_RIGHT:
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void DialogGameBoard::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (type == SPECTATOR) return;
	if (turn % 2 != even_odd) return;
 
	int mouseX = (point.x - X_START - MARGIN) / INTERVAL;		// 내림이므로 15 ~ 34영역 -> 0
	int	mouseY = (point.y - Y_START - MARGIN) / INTERVAL;

	if ((mouseX < 0 || mouseX >= MATRIX_SIZE || mouseY < 0 || mouseY >= MATRIX_SIZE)) return;

	if(game_board[MATRIX_SIZE*mouseY + mouseX].color == NOTHING) { 
		if(even_odd) game_board[MATRIX_SIZE*mouseY + mouseX].color = BLACK;
		else game_board[MATRIX_SIZE*mouseY + mouseX].color = WHITE;
	}

	char buff[20];
	sprintf(buff, "%02d|%02d", mouseX, mouseY);
	string locateStr = buff;

	m_pClient->m_pClient->took_stone(locateStr);

	//Invalidate();

	CDialog::OnLButtonDown(nFlags, point);
}


void DialogGameBoard::playerSet(int color)
{
	// TODO: 여기에 구현 코드 추가.
	even_odd = color;
	type = PLAYER;
}

void DialogGameBoard::spectatorSet()
{
	// TODO: 여기에 구현 코드 추가.
	type = SPECTATOR;
}

void DialogGameBoard::setPlayerName(string blackPlayer, string whitePlayer) 
{
	CString blackP(blackPlayer.c_str());
	CString whiteP(whitePlayer.c_str());
	h_staticLEFT.SetWindowTextW(blackP);
	h_staticRIGHT.SetWindowTextW(whiteP);
}

void DialogGameBoard::boardUpdate(int locateX, int locateY) {
	if (turn % 2) game_board[MATRIX_SIZE*locateY + locateX].color = BLACK;
	else game_board[MATRIX_SIZE*locateY + locateX].color = WHITE;

	turn++;

	Invalidate();
}

void DialogGameBoard::gameEnd() {
	for (int i = 0; i < MATRIX_SIZE*MATRIX_SIZE; i++) {
		game_board[i].color = NOTHING;
	}

	turn = 0;
	even_odd = 1;
	type = SPECTATOR;

	ShowWindow(SW_HIDE);
}