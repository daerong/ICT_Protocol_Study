#pragma once

#include "string.h"
#include "string"
#include <winsock2.h>

#define SERVICE_BYTE_VOL 2

using namespace std;

typedef struct UDS {// 모든 항목의 Default(0)으로 고정
	char GMS;		// Game(G), Message(M), System(S), User(U), Knock(K), Return(R)
	char service;	// Game(G)- Start(S), Player set(P), Location(L), Update(U), End(E)
					// Message(M)- Wisper(W), Broadcast(B), Read only(R), Notice(N), Messagebox(M)
					// System(S)- Name set(N), Reject(R), Add member(A), Socket check(S), Heart(H), 
					//				Close socket(C), Delegation(D), Fall back(F), Kick out(K)
					// User(U)- In seat(I), Out seat(O), Delegation(D), Kick out(K)
					// Knock(N)- Delegation(D)
					// Return(R)- Delegation(D)
	string msg;
} UDS;

typedef struct S_LINK { 
	string name;
	char grade;			// Owner(O), Person(P) 
	char status;		// In_seat(I), Out_seat(O), 
	char heart;			// Yes(Y), No(N)
	SOCKET socket;
	struct S_LINK *link;
} S_LINK;

typedef struct S_LINK_H {
	S_LINK *head;
} S_LINK_H;

typedef struct G_STAT {
	S_LINK *blackPlayer;
	S_LINK *whitePlayer;
	char stat;
} G_STAT;

typedef struct GAME_BOARD {
	char color;
	int turn;
} GAME_BOARD;

class CserverDlg;

class serverManager
{
public:
	serverManager(CserverDlg *dlg);
	~serverManager();

	void StartListening(int iPort);
	static UINT __cdecl DataThreadFunc(LPVOID pParam);
	UINT SendReceiveData(SOCKET cSocket);
	void ClearServer();
	void set_uds(char GMS, char service, string msg);
	void set_uds(char GMS, char service);
	string get_uds();
	//static void SetStaticVariable(int iC, SOCKET cS);
	void socketCheck();
	void unknownHeart();
	void NoResponse(S_LINK *target);
	string s_link_stringfi(S_LINK *s);

	S_LINK_H *init_head();
	G_STAT *g_stat;
	GAME_BOARD *game_board;
	void insert_sock(S_LINK **prev, SOCKET target);
	void delete_sock(S_LINK **prev, S_LINK *target);
	S_LINK *search_sock(SOCKET s);
	S_LINK *search_sock(string str);
	S_LINK *search_sock(int s);
	S_LINK *search_name(string name);
	int count_user();
	S_LINK *select_user(int num);
	void deleteAll_sock(S_LINK **prev);
	void sendAll_sock(string msg);
	void sendTarget_sock(S_LINK *target, string msg);
	void sendTarget_sock(SOCKET target, string msg);
	void insert_name(S_LINK *target, string name);
	void appendMsg(string str);
	void refresh_client_list();
	string get_all_client_list();
	string make_wisp_msg(S_LINK *sender, S_LINK *recver);
	string make_broad_msg(S_LINK *sender);
	int giveOwner(S_LINK *target);
	void givePerson(S_LINK *target);
	void ownerUpdate();
	void knockForDelegation(S_LINK *sender, S_LINK *recver);
	void changeOwnerStart(S_LINK *owner, S_LINK *target);
	int playerSet();
	void gameStartPlayerNameSet();
	void endCheck(int lastX, int lastY);
	int paul_check(int locateX, int locateY);
	void xEnd_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y, int *count);
	void yEnd_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y, int *count);
	void uEnd_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y, int *count);
	void dEnd_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y, int *count);
	int xBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y);
	int yBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y);
	int uBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y);
	int dBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y);
	void boardReset();

	WSADATA wsa;
	SOCKET s, new_socket;
	S_LINK *prev;
	S_LINK_H *s_link_h;
	//static SOCKET sArray[100];
	struct sockaddr_in server, client;
	int c;
	//static int iCount;
	int iTempCount;
	CserverDlg* m_pDialog;
	HANDLE m_Thread_handle[100];
	CWinThread *cpTh[100];
	UDS uds;

	int turn;
};