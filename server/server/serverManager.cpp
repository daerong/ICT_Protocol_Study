#include "stdafx.h"
#include "serverManager.h"
#include "server.h"
#include "serverDlg.h"
#include <Windows.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

#define PLAYING 1
#define NON_PLAYING 0

#define MATRIX_SIZE 19

#define BLACK -1
#define NOTHING 0
#define WHITE 1

//static SOCKET sArray[100];
static int iCount;

serverManager::serverManager(CserverDlg *dlg) : prev(NULL), m_pDialog(dlg), turn(1)
{
	g_stat = new G_STAT;
	g_stat->stat = NON_PLAYING;
	g_stat->whitePlayer = NULL;
	g_stat->blackPlayer = NULL;

	s_link_h = init_head();

	game_board = new GAME_BOARD[MATRIX_SIZE*MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE*MATRIX_SIZE; i++) {
		game_board[i].color = NOTHING;
	}
}


serverManager::~serverManager(){
	closesocket(s);
	WSACleanup();
	deleteAll_sock(&prev);
	delete s_link_h;
	delete prev;
	delete g_stat;
	delete game_board;
}

void serverManager::ClearServer()
{
	closesocket(s);
	WSACleanup();

	/*
	for(int i=1;i<=iCount;++i)
	{
		DWORD dwCode;
		GetExitCodeThread(cpTh[i]->m_hThread, &dwCode);
		delete cpTh[i];
		//CloseHandle(m_Thread_handle[i]);
	}*/
}

void serverManager::StartListening(int iPort)
{
	iCount = 0;
	char str[256];
	memset(str, 0, sizeof(char) * 256);

	m_pDialog->ShowServerLog("Initialising Winsock...");

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		sprintf(str ,"Failed. Error Code : %d", WSAGetLastError());
		m_pDialog->ShowServerLog(str);
		return;
	}

	m_pDialog->ShowServerLog("Initialised.");

	// 소켓 생성
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		sprintf(str, "Could not create socket : %d", WSAGetLastError());
		m_pDialog->ShowServerLog(str);
	}

	m_pDialog->ShowServerLog("Socket created.");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(iPort);

	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		sprintf(str, "Bind failed with error code : %d", WSAGetLastError());
		m_pDialog->ShowServerLog(str);

		exit(EXIT_FAILURE);
	}

	m_pDialog->ShowServerLog("Bind done");

	listen(s, 100);

	m_pDialog->ShowServerLog("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		m_pDialog->ShowServerLog("Connection accepted");

		socklen_t len;
		struct sockaddr_storage addr;
		char ipstr[INET6_ADDRSTRLEN];
		int port;

		len = sizeof addr;
		getpeername(new_socket, (struct sockaddr*)&addr, &len);

		if (addr.ss_family == AF_INET) {
			struct sockaddr_in *s = (struct sockaddr_in *)&addr;
			port = ntohs(s->sin_port);
			inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
		}

		m_pDialog->ShowServerLog("Connected Peer IP address: " + string(ipstr) + "\n");

		CWinThread *cTh = AfxBeginThread(DataThreadFunc, (LPVOID)new_socket);
		++iCount;
		//m_Thread_handle[++iCount] = cTh->m_hThread;
		//cpTh[iCount] = cTh;
		insert_sock(&prev, new_socket);
		//sprintf(str, "Hello Client(%d) , I have received your connection.", new_socket);
		//send(new_socket, str, strlen(str) , 0);

		//SetStaticVariable(iTempCount, new_socket);

		/*ownerUpdate();*/
	}

	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
		return;
	}
}

UINT __cdecl serverManager::DataThreadFunc(LPVOID pParam)
{
	SOCKET pYourSocket = reinterpret_cast<SOCKET>(pParam);
	//UINT retCode = pYourClass->ThreadFunc();
	//SendReceiveData(pYourClass);

	CserverDlg *lpDlg = (CserverDlg *)AfxGetApp()->GetMainWnd();		// Main Dlg 접근방법
	serverManager *self = lpDlg->m_pServer;

	char server_reply[2048];
	int recv_size;

	while ((recv_size = recv(pYourSocket, server_reply, 2048, 0)) != SOCKET_ERROR)
	{
		server_reply[recv_size] = '\0';
		char buf[2046];
		strncpy(buf, server_reply + 2, 2046);
		string str(buf);

		// std::string->substr(int pos, int n)
		// pos위치부터 n개 문자를 새로운 서브스트링으로 생성리턴

		self->uds.GMS = server_reply[0];
		self->uds.service = server_reply[1];
		self->uds.msg = str;

		if (self->uds.GMS == 'S') {		// system
			if (self->uds.service == 'N') {		// name set
				S_LINK *target = self->search_sock(pYourSocket);
				if (self->search_name(str) == NULL) {
					self->insert_name(target, str);

					string insert_msg = "[Notice]" + target->name + " has join.";
					lpDlg->ShowServerLog(target->name + "(" + to_string(target->socket) + ") has join.");
					self->set_uds('M', 'N', insert_msg);
					string buf = self->get_uds();
					self->sendAll_sock(buf);

					self->refresh_client_list();

					lpDlg->DelayUpdatePlz();
				}
				else {
					string rejectMsg = "Your name already exist.";

					self->set_uds('S', 'K', rejectMsg);
					string buf = self->get_uds();
					self->sendTarget_sock(pYourSocket, buf);

					lpDlg->ShowServerLog(target->name + "(" + to_string(target->socket) + ") was kicked off because same name already exist.");
					self->delete_sock(&self->prev, target);
				}
			}
			else if (self->uds.service == 'C') {
				S_LINK *target = self->search_sock(pYourSocket);
				string delete_msg = "[Notice]" + target->name + " has exit.";
				lpDlg->ShowServerLog(target->name + "(" + to_string(target->socket) + ") has exit.");
				self->delete_sock(&self->prev, target);
				self->set_uds('M', 'N', delete_msg);		// 클라이언트 종료 메시지 전송
				string buf = self->get_uds();
				self->sendAll_sock(buf);

				self->refresh_client_list();

				lpDlg->DelayUpdatePlz();
			}
			else if (self->uds.service == 'H') {
				S_LINK *target = self->search_sock(pYourSocket);
				target->heart = 'Y';
			}
		}
		else if (self->uds.GMS == 'U') {
			if (self->uds.service == 'I') {
				S_LINK *target = self->search_sock(pYourSocket);
				target->status = 'I';
				lpDlg->ShowServerLog(target->name + "(" + to_string(target->socket) + ") changed status[In seat].");

				string clientArrMsg = self->get_all_client_list();
				self->set_uds('S', 'A', clientArrMsg);		// 모든 클라이언트로 추가된 client 추가 요청
				string buf = self->get_uds();
				self->sendAll_sock(buf);
			}
			else if (self->uds.service == 'O') {
				S_LINK *target = self->search_sock(pYourSocket);
				target->status = 'O';
				lpDlg->ShowServerLog(target->name + "(" + to_string(target->socket) + ") changed status[Out seat].");

				string clientArrMsg = self->get_all_client_list();
				self->set_uds('S', 'A', clientArrMsg);		// 모든 클라이언트로 추가된 client 추가 요청
				string buf = self->get_uds();
				self->sendAll_sock(buf);
			}
			else if (self->uds.service == 'D') {
				S_LINK *recver = self->search_sock(self->uds.msg);
				S_LINK *sender = self->search_sock(pYourSocket);
				self->knockForDelegation(sender, recver);
			}
			else if (self->uds.service == 'K') {
				S_LINK *target = self->search_sock(self->uds.msg);
				string kickoutMsg = "You were kicked off by the room owner.";

				self->set_uds('S', 'K', kickoutMsg);
				string buf = self->get_uds();
				self->sendTarget_sock(target, buf);

				string delete_msg = "[Notice]" + target->name + " was kicked off by the room owner.";
				lpDlg->ShowServerLog(target->name + "(" + to_string(target->socket) + ") was kicked off by the room owner.");
				self->delete_sock(&self->prev, target);
				self->set_uds('M', 'N', delete_msg);		// 클라이언트 종료 메시지 전송
				buf = self->get_uds();
				self->sendAll_sock(buf);	

				self->refresh_client_list();

				lpDlg->DelayUpdatePlz();
			}
		}
		else if (self->uds.GMS == 'M') {		
			if (self->uds.service == 'W') {			// Wisper 요정이 오면
				int locate;	// 0번쨰 위치에서 부터 @위치를 찾음
				if((locate = str.find("@", 0)) == std::string::npos) {
					lpDlg->ShowServerLog("Can't find '@' in recv message.");
				}

				int target = stoi(str.substr(0, locate));
				string wisp_msg = str.substr(locate + 1);
				
				S_LINK *sender = self->search_sock(pYourSocket);
				S_LINK *recver = self->search_sock(target);

				if (recver != NULL) {
					string wisp_head = self->make_wisp_msg(sender, recver);
					wisp_msg = wisp_head + wisp_msg;

					lpDlg->ShowChatLog(wisp_msg);

					self->set_uds('M', 'R', wisp_msg);
					self->sendTarget_sock(sender, wisp_msg);
					self->sendTarget_sock(recver, wisp_msg);
				}
				else {
					wisp_msg = "There's no recver.";
					self->set_uds('M', 'R', wisp_msg);
					self->sendTarget_sock(sender, wisp_msg);
				}	
			}
			else if (self->uds.service == 'B') {		// Broadcast 요정이 오면
				S_LINK *sender = self->search_sock(pYourSocket);
				string broad_msg = self->make_broad_msg(sender) + self->uds.msg;

				lpDlg->ShowChatLog(broad_msg);

				self->set_uds('M', 'R', broad_msg);			// Read Only 메시지 전송
				string buf = self->get_uds();
				self->sendAll_sock(buf);
			}
		}
		else if (self->uds.GMS == 'G') {
			if (self->uds.service == 'L') {
				int locate = str.find("|", 0);
				string LocateX = str.substr(0, locate);
				string LocateY = str.substr(locate + 1);
				int locateX = stoi(LocateX);
				int locateY = stoi(LocateY);

				if (self->turn % 2) self->game_board[locateY*MATRIX_SIZE + locateX].color = BLACK;
				else self->game_board[locateY*MATRIX_SIZE + locateX].color = WHITE;

				self->endCheck(locateX, locateY);

				if (self->turn == 0) {
					self->g_stat->stat = NON_PLAYING;

					self->set_uds('G', 'E');	
					string buf = self->get_uds();
					self->sendAll_sock(buf);

					self->boardReset();
				}
				else {
					self->turn++;

					self->set_uds('G', 'U');
					string buf = self->get_uds();
					self->sendAll_sock(buf);
				}
			}
		}
		else if (self->uds.GMS == 'R') {
			int target = stoi(str);
			S_LINK *ownerSock = self->search_sock(target);
			S_LINK *targetSock = self->search_sock(pYourSocket);

			self->changeOwnerStart(ownerSock, targetSock);
		}
		else {
			lpDlg->ShowChatLog(string(server_reply));

			string default_msg(server_reply);

			self->sendAll_sock(default_msg);
		}
	}
	return 0;
}

UINT serverManager::SendReceiveData(SOCKET cSocket)
{

	return 0;
}

void serverManager::set_uds(char GMS, char service, string msg) {
	uds.GMS = GMS;
	uds.service = service;
	uds.msg = msg;
}

void serverManager::set_uds(char GMS, char service) {
	uds.GMS = GMS;
	uds.service = service;
}

string serverManager::get_uds() {
	string str(uds.msg);
	str.insert(0, 1, uds.service);
	str.insert(0, 1, uds.GMS);
	return str;
}

//void serverManager::SetStaticVariable(int iC, SOCKET cS)
//{
//	iCount = iC;
//	sArray[iCount] = cS;
//}

void serverManager::socketCheck() {
	set_uds('S', 'S', "");

	unknownHeart();
	refresh_client_list();

	string buf = get_uds();
	sendAll_sock(buf);

	string clientArrMsg = get_all_client_list();
	m_pDialog->ShowServerLog("Checking socket...");
	set_uds('S', 'A', clientArrMsg);		// 모든 클라이언트로 추가된 client 추가 요청
	buf = get_uds();
	sendAll_sock(buf);
}

void serverManager::unknownHeart() {
	S_LINK *search_p = s_link_h->head;
	S_LINK *next = NULL;
	while (search_p != NULL) {
		if (search_p->heart == 'N') {
			next = search_p->link;
			m_pDialog->ShowServerLog("Shut down " + search_p->name + "(" + to_string(search_p->socket) + ")'s connection because no response.");
			delete_sock(&prev, search_p);
			search_p = next;
		}
		else {
			search_p->heart = 'N';
			search_p = search_p->link;
		}
	}
}

void serverManager::NoResponse(S_LINK *target) {
	if (target->heart == 'N') {
		m_pDialog->ShowServerLog("Shut down " + target->name + "(" + to_string(target->socket) + ") because no response.");
		delete_sock(&prev, target);
	}
	else {
		target->heart = 'N';
	}
}

S_LINK_H *serverManager::init_head() {
	S_LINK_H *init = new S_LINK_H;
	init->head = NULL;
	return init;
}

void serverManager::insert_sock(S_LINK **prev, SOCKET target) {
	S_LINK *new_link = new S_LINK;
	new_link->socket = target;
	new_link->link = NULL;
	new_link->status = 'I';
	new_link->heart = 'Y';
	if (s_link_h->head == NULL) {
		new_link->grade = 'O';
		s_link_h->head = new_link;
		giveOwner(s_link_h->head);
	}
	else {
		new_link->grade = 'P';
		(*prev)->link = new_link;
	}
	*prev = new_link;
}

void serverManager::delete_sock(S_LINK **prev, S_LINK *target) {

	if (target == NULL) return;
	if (target == s_link_h->head) {
		s_link_h->head = target->link;
		if (s_link_h->head != NULL) {
			s_link_h->head->grade = 'O';
			giveOwner(s_link_h->head);
		}
		return;
	}
	S_LINK *search_p = s_link_h->head;

	while (search_p->link != target) {
		search_p = search_p->link;
	}
	search_p->link = target->link;
	if (target->link == NULL) {
		*prev = search_p;
	}
	delete target;
}

S_LINK *serverManager::search_sock(SOCKET s) {
	S_LINK *search_p = s_link_h->head;
	while (search_p != NULL) {
		if (search_p->socket == s) {
			return search_p;
		}
		search_p = search_p->link;
	}
	return NULL;
}

S_LINK *serverManager::search_sock(string str) {
	int s = atoi(str.c_str());

	S_LINK *search_p = s_link_h->head;
	while (search_p != NULL) {
		if ((int)(search_p->socket) == s) {
			return search_p;
		}
		search_p = search_p->link;
	}
	return NULL;
}

S_LINK *serverManager::search_sock(int s) {
	S_LINK *search_p = s_link_h->head;
	while (search_p != NULL) {
		if ((int)(search_p->socket) == s) {
			return search_p;
		}
		search_p = search_p->link;
	}
	return NULL;
}

int serverManager::count_user() {
	S_LINK *search_p = s_link_h->head;
	
	int count = 0;

	while (search_p != NULL) {
		if (search_p->status == 'I') {
			count++;
		}
		search_p = search_p->link;
	}
	return count;
}

S_LINK *serverManager::select_user(int num) {
	S_LINK *search_p = s_link_h->head;
	int count = 0;

	while (search_p != NULL) {
		if (search_p->status == 'I') {
			if (count == num) return search_p;
			count++;
		}
		search_p = search_p->link;
	}
	return NULL;
}

S_LINK *serverManager::search_name(string name) {
	S_LINK *search_p = s_link_h->head;
	while (search_p != NULL) {
		if (search_p->name == name) {
			return search_p;
		}
		search_p = search_p->link;
	}
	return NULL;
}

void serverManager::deleteAll_sock(S_LINK **prev) {
	S_LINK *search_p = s_link_h->head;
	S_LINK *cursor = NULL;
	while (search_p != NULL) {
		cursor = search_p;
		search_p = search_p->link;
		delete cursor;
	}
	prev = NULL;
}

void serverManager::sendAll_sock(string msg) {
	S_LINK *search_p = s_link_h->head;
	while (search_p != NULL) {
		if (send(search_p->socket, msg.c_str(), msg.size(), 0) < 0) {
			m_pDialog->ShowServerLog("Can't send message to " + search_p->name + "(" + to_string(search_p->socket) + ") : " + msg);
			//NoResponse(search_p);
		}
		search_p = search_p->link;
	}
}

void serverManager::sendTarget_sock(S_LINK *target, string msg) {
	if (target == NULL) {
		m_pDialog->ShowServerLog("[Error occur] sendTarget_sock : target is NULL.");
		return;
	}
	if (send(target->socket, msg.c_str(), msg.size(), 0) < 0) {
		m_pDialog->ShowServerLog("Can't send message to " + target->name + "(" + to_string(target->socket) + ") : " + msg);
		//NoResponse(target);
	}
}

void serverManager::sendTarget_sock(SOCKET target, string msg) {
	if (send(target, msg.c_str(), msg.size(), 0) < 0) {
		m_pDialog->ShowServerLog("Can't send message new user with duplicate name.");
	}
}

string serverManager::s_link_stringfi(S_LINK *in_temp) {
	string str = in_temp->name + "(" + to_string((int)in_temp->socket) + ")";
	return str;
}

void serverManager::insert_name(S_LINK *target, string name) {
	target->name = name;
}

void serverManager::appendMsg(string str) {
	uds.msg += str;
}

void serverManager::refresh_client_list() {
	m_pDialog->h_clientList.ResetContent();
	S_LINK *search_p = s_link_h->head;
	string insert_client_msg;
	while (search_p != NULL) {
		insert_client_msg = s_link_stringfi(search_p);
		m_pDialog->AddClientList(insert_client_msg);
		search_p = search_p->link;
	}
}

string serverManager::get_all_client_list() {
	S_LINK *search_p = s_link_h->head;
	string insert_client_msg;
	if (search_p == NULL) return "";
	while (search_p != NULL) {
		string option = "";
		if (search_p->grade == 'O') { option += "[방장]"; }
		if(search_p->status == 'O') { option += "[자리비움]"; }
		insert_client_msg += s_link_stringfi(search_p) + option + "|";
		search_p = search_p->link;
	}
	insert_client_msg = insert_client_msg.substr(0, insert_client_msg.length() - 1);

	return insert_client_msg;
}

string serverManager::make_wisp_msg(S_LINK *sender, S_LINK *recver) {
	string result_str = sender->name + "->" + recver->name + ":";
	return result_str;
}

string serverManager::make_broad_msg(S_LINK *sender) {
	string result_str = sender->name + ":";
	return result_str;
}

int serverManager::giveOwner(S_LINK *target) {
	if (target == NULL) {
		m_pDialog->ShowServerLog("[Error occur] giveOwner : target is NULL.");
		return 0;
	}

	set_uds('S', 'D', "");
	string delegationStr = get_uds();

	if (send(target->socket, delegationStr.c_str(), delegationStr.size(), 0) < 0) {
		m_pDialog->ShowServerLog("Failed to Delegation.");
		return 0;
	}

	target->grade = 'O';

	return 1;
}

void serverManager::givePerson(S_LINK *target) {
	if (target == NULL) {
		m_pDialog->ShowServerLog("[Error occur] givePerson : target is NULL.");
		return;
	}

	set_uds('S', 'F', "");
	string FallbackStr = get_uds();
	if (send(target->socket, FallbackStr.c_str(), FallbackStr.size(), 0) < 0) {
		m_pDialog->ShowServerLog("Can't find user. : " + target->name + "(" + to_string(target->socket) + ")");
	}

	target->grade = 'P';
}

void serverManager::ownerUpdate() {
	S_LINK *search_p = s_link_h->head;
	while (search_p != NULL) {
		if (search_p->grade == 'O') {
			giveOwner(search_p);
			break;
		}
		search_p = search_p->link;
	}
	if (search_p == NULL && s_link_h->head != NULL) {
		s_link_h->head->grade = 'O';
		giveOwner(s_link_h->head);
	}
}

void serverManager::knockForDelegation(S_LINK *sender, S_LINK *recver) {
	if (recver == NULL) {
		m_pDialog->ShowServerLog("[Error occur] knockForDelegation : recver is NULL.");
		return;
	}

	set_uds('K', 'D', to_string(sender->socket));
	string buf = get_uds();

	if (send(recver->socket, buf.c_str(), buf.size(), 0) < 0) {
		string failMsg = "There's no user named " + recver->name + "(" + to_string(recver->socket) + ")";
		set_uds('M', 'N', failMsg);
		buf = get_uds();
		sendTarget_sock(sender, buf);
	}
}

void serverManager::changeOwnerStart(S_LINK *owner, S_LINK *target) {
	if (giveOwner(target)) {
		givePerson(owner);
		m_pDialog->DelayUpdatePlz();
		return;
	}

	uds.msg = "Failed to Delegate.";
	set_uds('M', 'N');
	string buf = get_uds();
	sendTarget_sock(owner, buf);
}

int serverManager::playerSet() {
	if (g_stat->stat == PLAYING) return 0;		// 게임 진행중
	if (s_link_h == NULL) {
		m_pDialog->ShowServerLog("[Error occur] playerSet : s_link_h is NULL.");
		return 0;				// 연결리스트 헤더가 없을 때
	}

	int userVol = count_user();
	if (userVol < 2) {
		m_pDialog->ShowServerLog("[Error occur] playerSet : Need 2 Player.");
		return 0;				// 플레이어 1명 이하
	}

	int RandNo = rand() % userVol;			// 0 ~ user 수-1
	g_stat->blackPlayer = select_user(RandNo);

	set_uds('G', 'P', "You are black player.");
	string buf = get_uds();
	sendTarget_sock(g_stat->blackPlayer, buf);

	if (RandNo == userVol - 1) {
		g_stat->whitePlayer = select_user(0);
	}
	else g_stat->whitePlayer = select_user(RandNo + 1);

	set_uds('G', 'P', "You are white player.");
	buf = get_uds();
	sendTarget_sock(g_stat->whitePlayer, buf);

	return 1;
}

void serverManager::gameStartPlayerNameSet(){
	S_LINK *blackP = g_stat->blackPlayer;
	S_LINK *whiteP = g_stat->whitePlayer;

	string insert_player_msg = blackP->name + "|" + whiteP->name;
	set_uds('G', 'S', insert_player_msg);
	string buf = get_uds();

	S_LINK *search_p = s_link_h->head;
	while (search_p != NULL) {
		send(search_p->socket, buf.c_str(), buf.size(), 0);
		search_p = search_p->link;
	}
	
	turn = 1;
	g_stat->stat = PLAYING;
}

void serverManager::endCheck(int lastX, int lastY)
{
	string gameResultStr;

	int color = 0;

	if (turn % 2) color = BLACK;
	else color = WHITE;

	if (color == BLACK) {
		for (int y = 0; y < MATRIX_SIZE; y++) {
			for (int x = 0; x < MATRIX_SIZE; x++) {
				switch (paul_check(x, y)) {
				case 0:
					break;
				case 3:
					gameResultStr = "[Game Result] Black paul : 3x3 stone.";
					m_pDialog->ShowServerLog(gameResultStr);
					uds.msg = gameResultStr;
					turn = 0;
					return;
				case 4:
					gameResultStr = "[Game Result] Black paul : 4x4 stone.";
					m_pDialog->ShowServerLog(gameResultStr);
					uds.msg = gameResultStr;
					turn = 0;
					return;
				case 6:
					gameResultStr = "[Game Result] Black paul : 6 stone.";
					m_pDialog->ShowServerLog(gameResultStr);
					uds.msg = gameResultStr;
					turn = 0;
					return;
				default:
					m_pDialog->ShowServerLog("[Error] endCheck : Undefined value.");
				}
			}
		}
	}

	int PX = 0, PY = 0, PU = 0, PD = 0, MX = 0, MY = 0, MU = 0, MD = 0;
	xEnd_check_2011144024(lastX, lastY, color, 1, 0, &PX);
	xEnd_check_2011144024(lastX, lastY, color, -1, 0, &MX);
	yEnd_check_2011144024(lastX, lastY, color, 0, 1, &PY);
	yEnd_check_2011144024(lastX, lastY, color, 0, -1, &MY);
	uEnd_check_2011144024(lastX, lastY, color, 1, -1, &PU);
	uEnd_check_2011144024(lastX, lastY, color, -1, 1, &MU);
	dEnd_check_2011144024(lastX, lastY, color, 1, 1, &PD);
	dEnd_check_2011144024(lastX, lastY, color, -1, -1, &MD);

	if (PX + MX >= 4 || PY + MY >= 4 || PU + MU >= 4 || PD + MD >= 4) {
		if(color == BLACK) gameResultStr = "[Game Result] Black win.";
		else gameResultStr = "[Game Result] White win.";
		m_pDialog->ShowServerLog(gameResultStr);
		uds.msg = gameResultStr;
		turn = 0;
	}
}

int serverManager::paul_check(int locateX, int locateY) {

	if (game_board[MATRIX_SIZE*locateY + locateX].color != BLACK) return 0;

	int threethree_count_using_foul = 0;
	int fourfour_count_using_foul = 0;
	int over_six_check_using_foul = 0;

	int PX = 0, PY = 0, PU = 0, PD = 0, MX = 0, MY = 0, MU = 0, MD = 0;
	xEnd_check_2011144024(locateX, locateY, BLACK, 1, 0, &PX);
	xEnd_check_2011144024(locateX, locateY, BLACK, -1, 0, &MX);
	yEnd_check_2011144024(locateX, locateY, BLACK, 0, 1, &PY);
	yEnd_check_2011144024(locateX, locateY, BLACK, 0, -1, &MY);
	uEnd_check_2011144024(locateX, locateY, BLACK, 1, -1, &PU);
	uEnd_check_2011144024(locateX, locateY, BLACK, -1, 1, &MU);
	dEnd_check_2011144024(locateX, locateY, BLACK, 1, 1, &PD);
	dEnd_check_2011144024(locateX, locateY, BLACK, -1, -1, &MD);

	if (PX + MX >= 5) over_six_check_using_foul += 1;
	if (PY + MY >= 5) over_six_check_using_foul += 1;
	if (PU + MU >= 5) over_six_check_using_foul += 1;
	if (PD + MD >= 5) over_six_check_using_foul += 1;

	if (locateX == 0) {
		if (PY == 2 && !yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1)) threethree_count_using_foul += 1;
		else if (MY == 2 && !yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1)) threethree_count_using_foul += 1;
	}
	else if (locateX == MATRIX_SIZE - 1) {
		if (PY == 2 && !yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1)) threethree_count_using_foul += 1;
		else if (MY == 2 && !yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1)) threethree_count_using_foul += 1;
	}
	else if (locateY == 0) {
		if (PX == 2 && !xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0)) threethree_count_using_foul += 1;
		else if (MX == 2 && !xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0)) threethree_count_using_foul += 1;
	}
	else if (locateY == MATRIX_SIZE - 1) {
		if (PX == 2 && !xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0)) threethree_count_using_foul += 1;
		else if (MX == 2 && !xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0)) threethree_count_using_foul += 1;
	}
	else {
		int E_PX = 0, E_PY = 0, E_PU = 0, E_PD = 0, E_MX = 0, E_MY = 0, E_MU = 0, E_MD = 0;			// 한 칸 띄고
		if (PX == 0) xEnd_check_2011144024(locateX + 1, locateY, BLACK, 1, 0, &E_PX);
		if (MX == 0) xEnd_check_2011144024(locateX - 1, locateY, BLACK, -1, 0, &E_MX);
		if (PY == 0) yEnd_check_2011144024(locateX, locateY + 1, BLACK, 0, 1, &E_PY);
		if (MY == 0) yEnd_check_2011144024(locateX, locateY - 1, BLACK, 0, -1, &E_MY);
		if (PU == 0) uEnd_check_2011144024(locateX + 1, locateY - 1, BLACK, 1, -1, &E_PU);
		if (MU == 0) uEnd_check_2011144024(locateX - 1, locateY + 1, BLACK, -1, 1, &E_MU);
		if (PD == 0) dEnd_check_2011144024(locateX + 1, locateY + 1, BLACK, 1, 1, &E_PD);
		if (MD == 0) dEnd_check_2011144024(locateX - 1, locateY - 1, BLACK, -1, -1, &E_MD);

		// 3x3 check
		if (PX == 2 && MX == 0 && !(xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0) || xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (PX == 0 && MX == 2 && !(xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0) || xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (PX == 1 && MX == 1 && !(xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0) || xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (E_PX == 2 && MX == 0 && !(xBlock_check_2011144024(locateX + 1, locateY, BLACK, 1, 0) || xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (E_PX == 1 && MX == 1 && !(xBlock_check_2011144024(locateX + 1, locateY, BLACK, 1, 0) || xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (PX == 0 && E_MX == 2 && !(xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0) || xBlock_check_2011144024(locateX - 1, locateY, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (PX == 1 && E_MX == 1 && !(xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0) || xBlock_check_2011144024(locateX - 1, locateY, BLACK, -1, 0))) threethree_count_using_foul += 1;

		if (PY == 2 && MY == 0 && !(yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1) || yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (PY == 0 && MY == 2 && !(yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1) || yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (PY == 1 && MY == 1 && !(yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1) || yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (E_PY == 2 && MY == 0 && !(yBlock_check_2011144024(locateX, locateY + 1, BLACK, 0, 1) || yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (E_PY == 1 && MY == 1 && !(yBlock_check_2011144024(locateX, locateY + 1, BLACK, 0, 1) || yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (PY == 0 && E_MY == 2 && !(yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1) || yBlock_check_2011144024(locateX, locateY - 1, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (PY == 1 && E_MY == 1 && !(yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1) || yBlock_check_2011144024(locateX, locateY - 1, BLACK, 0, -1))) threethree_count_using_foul += 1;

		if (PU == 2 && MU == 0 && !(uBlock_check_2011144024(locateX, locateY, BLACK, 1, -1) || uBlock_check_2011144024(locateX, locateY, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (PU == 0 && MU == 2 && !(uBlock_check_2011144024(locateX, locateY, BLACK, 1, -1) || uBlock_check_2011144024(locateX, locateY, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (PU == 1 && MU == 1 && !(uBlock_check_2011144024(locateX, locateY, BLACK, 1, -1) || uBlock_check_2011144024(locateX, locateY, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (E_PU == 2 && MU == 0 && !(uBlock_check_2011144024(locateX + 1, locateY - 1, BLACK, 1, -1) || uBlock_check_2011144024(locateX, locateY, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (E_PU == 1 && MU == 1 && !(uBlock_check_2011144024(locateX + 1, locateY - 1, BLACK, 1, -1) || uBlock_check_2011144024(locateX, locateY, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (PU == 0 && E_MU == 2 && !(uBlock_check_2011144024(locateX, locateY, BLACK, 1, -1) || uBlock_check_2011144024(locateX - 1, locateY + 1, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (PU == 1 && E_MU == 1 && !(uBlock_check_2011144024(locateX, locateY, BLACK, 1, -1) || uBlock_check_2011144024(locateX - 1, locateY + 1, BLACK, -1, 1))) threethree_count_using_foul += 1;

		if (PD == 2 && MD == 0 && !(dBlock_check_2011144024(locateX, locateY, BLACK, 1, 1) || dBlock_check_2011144024(locateX, locateY, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (PD == 0 && MD == 2 && !(dBlock_check_2011144024(locateX, locateY, BLACK, 1, 1) || dBlock_check_2011144024(locateX, locateY, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (PD == 1 && MD == 1 && !(dBlock_check_2011144024(locateX, locateY, BLACK, 1, 1) || dBlock_check_2011144024(locateX, locateY, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (E_PD == 2 && MD == 0 && !(dBlock_check_2011144024(locateX + 1, locateY + 1, BLACK, 1, 1) || dBlock_check_2011144024(locateX, locateY, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (E_PD == 1 && MD == 1 && !(dBlock_check_2011144024(locateX + 1, locateY + 1, BLACK, 1, 1) || dBlock_check_2011144024(locateX, locateY, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (PD == 0 && E_MD == 2 && !(dBlock_check_2011144024(locateX, locateY, BLACK, 1, 1) || dBlock_check_2011144024(locateX - 1, locateY - 1, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (PD == 1 && E_MD == 1 && !(dBlock_check_2011144024(locateX, locateY, BLACK, 1, 1) || dBlock_check_2011144024(locateX - 1, locateY - 1, BLACK, -1, -1))) threethree_count_using_foul += 1;

		// 4x4 check
		if (PX == 3) fourfour_count_using_foul += 1;
		else if (MX == 3) fourfour_count_using_foul += 1;
		else if (PX == 2 && MX == 1 && (!xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0) || !xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (PX == 1 && MX == 2 && (!xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0) || !xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (E_PX == 3) fourfour_count_using_foul += 1;
		else if (E_MX == 3) fourfour_count_using_foul += 1;
		else if (E_PX == 2 && MX == 1 && (!xBlock_check_2011144024(locateX + 1, locateY, BLACK, 1, 0) || !xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (E_PX == 1 && MX == 2 && (!xBlock_check_2011144024(locateX + 1, locateY, BLACK, 1, 0) || !xBlock_check_2011144024(locateX, locateY, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (PX == 2 && E_MX == 1 && (!xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0) || !xBlock_check_2011144024(locateX - 1, locateY, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (PX == 1 && E_MX == 2 && (!xBlock_check_2011144024(locateX, locateY, BLACK, 1, 0) || !xBlock_check_2011144024(locateX - 1, locateY, BLACK, -1, 0))) fourfour_count_using_foul += 1;

		if (PY == 3) fourfour_count_using_foul += 1;
		else if (MY == 3) fourfour_count_using_foul += 1;
		else if (PY == 2 && MY == 1 && (!yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1) || !yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (PY == 1 && MY == 2 && (!yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1) || !yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (E_PY == 3) fourfour_count_using_foul += 1;
		else if (E_MY == 3) fourfour_count_using_foul += 1;
		else if (E_PY == 2 && MY == 1 && (!yBlock_check_2011144024(locateX, locateY + 1, BLACK, 0, 1) || !yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (E_PY == 1 && MY == 2 && (!yBlock_check_2011144024(locateX, locateY + 1, BLACK, 0, 1) || !yBlock_check_2011144024(locateX, locateY, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (PY == 2 && E_MY == 1 && (!yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1) || !yBlock_check_2011144024(locateX, locateY - 1, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (PY == 1 && E_MY == 2 && (!yBlock_check_2011144024(locateX, locateY, BLACK, 0, 1) || !yBlock_check_2011144024(locateX, locateY - 1, BLACK, 0, -1))) fourfour_count_using_foul += 1;

		if (PU == 3) fourfour_count_using_foul += 1;
		else if (MU == 3) fourfour_count_using_foul += 1;
		else if (PU == 2 && MU == 1 && (!uBlock_check_2011144024(locateX, locateY, BLACK, 1, -1) || !uBlock_check_2011144024(locateX, locateY, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (PU == 1 && MU == 2 && (!uBlock_check_2011144024(locateX, locateY, BLACK, 1, -1) || !uBlock_check_2011144024(locateX, locateY, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (E_PU == 3) fourfour_count_using_foul += 1;
		else if (E_MU == 3) fourfour_count_using_foul += 1;
		else if (E_PU == 2 && MU == 1 && (!uBlock_check_2011144024(locateX + 1, locateY - 1, BLACK, 1, -1) || !uBlock_check_2011144024(locateX, locateY, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (E_PU == 1 && MU == 2 && (!uBlock_check_2011144024(locateX + 1, locateY - 1, BLACK, 1, -1) || !uBlock_check_2011144024(locateX, locateY, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (PU == 2 && E_MU == 1 && (!uBlock_check_2011144024(locateX, locateY, BLACK, 1, -1) || !uBlock_check_2011144024(locateX - 1, locateY + 1, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (PU == 1 && E_MU == 2 && (!uBlock_check_2011144024(locateX, locateY, BLACK, 1, -1) || !uBlock_check_2011144024(locateX - 1, locateY + 1, BLACK, -1, 1))) fourfour_count_using_foul += 1;

		if (PD == 3) fourfour_count_using_foul += 1;
		else if (MD == 3) fourfour_count_using_foul += 1;
		else if (PD == 2 && MD == 1 && (!dBlock_check_2011144024(locateX, locateY, BLACK, 1, 1) || !dBlock_check_2011144024(locateX, locateY, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (PD == 1 && MD == 2 && (!dBlock_check_2011144024(locateX, locateY, BLACK, 1, 1) || !dBlock_check_2011144024(locateX, locateY, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (E_PD == 3) fourfour_count_using_foul += 1;
		else if (E_MD == 3) fourfour_count_using_foul += 1;
		else if (E_PD == 2 && MD == 1 && (!dBlock_check_2011144024(locateX + 1, locateY + 1, BLACK, 1, 1) || !dBlock_check_2011144024(locateX, locateY, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (E_PD == 1 && MD == 2 && (!dBlock_check_2011144024(locateX + 1, locateY + 1, BLACK, 1, 1) || !dBlock_check_2011144024(locateX, locateY, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (PD == 2 && E_MD == 1 && (!dBlock_check_2011144024(locateX, locateY, BLACK, 1, 1) || !dBlock_check_2011144024(locateX - 1, locateY - 1, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (PD == 1 && E_MD == 2 && (!dBlock_check_2011144024(locateX, locateY, BLACK, 1, 1) || !dBlock_check_2011144024(locateX - 1, locateY - 1, BLACK, -1, -1))) fourfour_count_using_foul += 1;
	}

	if (over_six_check_using_foul > 0) { return 6; }
	else if (fourfour_count_using_foul > 1) { return 4; }
	else if (threethree_count_using_foul > 1) { return 3; }

	return 0;
}

void serverManager::xEnd_check_2011144024(int lastX, int lastY, int type, int move_x, int move_y, int *count) {		// 돌 없으면 1리턴
	int lastX_add = lastX + move_x;
	int lastY_add = lastY + move_y;
	if (lastX_add < 0) {
		return;
	}
	else if (lastX_add > MATRIX_SIZE - 1) {
		return;
	}
	else {
		if (game_board[MATRIX_SIZE * lastY_add + lastX_add].color == type) {
			*count += 1;
			xEnd_check_2011144024(lastX_add, lastY_add, type, move_x, move_y, count);
		}
		else if (game_board[MATRIX_SIZE * lastY_add + lastX_add].color == -1 * type) {
			return;		// 끝이 막혀있음
		}
		else return;
	}
}

void serverManager::yEnd_check_2011144024(int lastX, int lastY, int type, int move_x, int move_y, int *count) {		// 돌 없으면 1리턴
	int lastX_add = lastX + move_x;
	int lastY_add = lastY + move_y;
	if (lastY_add < 0) {
		return;
	}
	else if (lastY_add > MATRIX_SIZE - 1) {
		return;
	}
	else {
		if (game_board[MATRIX_SIZE * lastY_add + lastX_add].color == type) {
			*count += 1;
			yEnd_check_2011144024(lastX_add, lastY_add, type, move_x, move_y, count);
		}
		else if (game_board[MATRIX_SIZE * lastY_add + lastX_add].color == -1 * type) {
			return;		// 끝이 막혀있음
		}
		else return;
	}
}

void serverManager::uEnd_check_2011144024(int lastX, int lastY, int type, int move_x, int move_y, int *count) {		// 돌 없으면 1리턴
	int lastX_add = lastX + move_x;
	int lastY_add = lastY + move_y;
	if (lastX_add > MATRIX_SIZE - 1 || lastY_add < 0) {
		return;
	}
	else if (lastX_add < 0 || lastY_add > MATRIX_SIZE - 1) {
		return;
	}
	else {
		if (game_board[MATRIX_SIZE * lastY_add + lastX_add].color == type) {
			*count += 1;
			uEnd_check_2011144024(lastX_add, lastY_add, type, move_x, move_y, count);
		}
		else if (game_board[MATRIX_SIZE * lastY_add + lastX_add].color == -1 * type) {
			return;		// 끝이 막혀있음
		}
		else return;
	}
}

void serverManager::dEnd_check_2011144024(int lastX, int lastY, int type, int move_x, int move_y, int *count) {		// 돌 없으면 1리턴
	int lastX_add = lastX + move_x;
	int lastY_add = lastY + move_y;
	if (lastX_add < 0 || lastY_add < 0) {
		return;
	}
	else if (lastX_add > MATRIX_SIZE - 1 || lastY_add > MATRIX_SIZE - 1) {
		return;
	}
	else {
		if (game_board[MATRIX_SIZE * lastY_add + lastX_add].color == type) {
			*count += 1;
			dEnd_check_2011144024(lastX_add, lastY_add, type, move_x, move_y, count);
		}
		else if (game_board[MATRIX_SIZE * lastY_add + lastX_add].color == -1 * type) {
			return;		// 끝이 막혀있음
		}
		else return;
	}
}

int serverManager::xBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_x_add < 0) {
		return 0;
	}
	else if (inner_x_add > MATRIX_SIZE - 1) {
		return 0;
	}
	else {
		if (game_board[MATRIX_SIZE * inner_y_add + inner_x_add].color == type) {
			return xBlock_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y);
		}
		else if (game_board[MATRIX_SIZE * inner_y_add + inner_x_add].color == -1 * type) {
			return 1;		// 끝이 막혀있음
		}
		else return 0;
	}
}

int serverManager::yBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_y_add < 0) {
		return 0;
	}
	else if (inner_y_add > MATRIX_SIZE - 1) {
		return 0;
	}
	else {
		if (game_board[MATRIX_SIZE * inner_y_add + inner_x_add].color == type) {
			return yBlock_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y);
		}
		else if (game_board[MATRIX_SIZE * inner_y_add + inner_x_add].color == -1 * type) {
			return 1;
		}
		else return 0;
	}
}

int serverManager::uBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_x_add > MATRIX_SIZE - 1 || inner_y_add < 0) {
		return 0;
	}
	else if (inner_x_add < 0 || inner_y_add > MATRIX_SIZE - 1) {
		return 0;
	}
	else {
		if (game_board[MATRIX_SIZE * inner_y_add + inner_x_add].color == type) {
			return uBlock_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y);
		}
		else if (game_board[MATRIX_SIZE * inner_y_add + inner_x_add].color == -1 * type) {
			return 1;
		}
		else return 0;
	}
}

int serverManager::dBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_x_add < 0 || inner_y_add < 0) {
		return 0;
	}
	else if (inner_x_add > MATRIX_SIZE - 1 || inner_y_add > MATRIX_SIZE - 1) {
		return 0;
	}
	else {
		if (game_board[MATRIX_SIZE * inner_y_add + inner_x_add].color == type) {
			return dBlock_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y);
		}
		else if (game_board[MATRIX_SIZE * inner_y_add + inner_x_add].color == -1 * type) {
			return 1;
		}
		else return 0;
	}
}

void serverManager::boardReset() {
	for (int i = 0; i < MATRIX_SIZE*MATRIX_SIZE; i++) {
		game_board[i].color = NOTHING;
	}
}