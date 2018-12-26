#pragma once

#include "string.h"
#include "string"
#include <winsock2.h>

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

class CclientDlg;
class DialogGameBoard;

class clientCon
{
public:
	clientCon(CclientDlg *dlg);
	~clientCon();

	void StartConnect(string sAddress, int iPort, string sUsername);
	void EndConnect();
	void SendData(string sMessage);
	void SendData(char GMS, char service, string sMessage);
	void set_uds(char GMS, char service, string msg);
	void set_uds(char GMS, char service);
	string get_uds();
	void took_stone(string locateStr);

	WSADATA wsa;
	SOCKET s;
	CclientDlg *m_pClient;
	DialogGameBoard *m_pGame;
	string m_pUser;
	UDS uds;		// 전송에 사용될 구조체
};

