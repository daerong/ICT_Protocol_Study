#include "stdafx.h"
#include "clientCon.h"
#include <stdio.h>
#include <string.h>
#include "ClientDlg.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

clientCon::clientCon(CclientDlg *dlg) {
	m_pClient = dlg;
}


clientCon::~clientCon() {
	closesocket(s);
	WSACleanup();
}

void clientCon::StartConnect(string sAddress, int iPort, string sUsername)
{
	struct sockaddr_in server;
	char server_reply[2048];
	int recv_size;
	m_pUser = sUsername;

	clientCon *self = (clientCon *)this;

	// m_pClient->AddNoticeMsg("[System] Initialising Winsock...");

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		m_pClient->AddNoticeMsg("[Error] Failed. Error Code : " + to_string(WSAGetLastError()));
		return;
	}

	// m_pClient->AddNoticeMsg("[System] Initialised.");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		m_pClient->AddNoticeMsg("[Error] Could not create socket : " + to_string(WSAGetLastError()));
	}

	// m_pClient->AddNoticeMsg("[System] Socket created.");

	server.sin_addr.s_addr = inet_addr(sAddress.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);

	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		m_pClient->AddNoticeMsg("[Error] Connect error.");
		return;
	}

	// m_pClient->AddNoticeMsg("[System] Connected.");

	//Send some data
	//message = "GET / HTTP/1.1\r\n\r\n";

	 // send user name
	set_uds('S', 'N', sUsername);

	string buf = get_uds();

	if (send(s, buf.c_str(), buf.size(), 0) < 0)
	{
		m_pClient->AddNoticeMsg("[Error] Can't send message. : " + buf);
		return;
	}

	//Receive a reply from the server
	while ((recv_size = recv(s, server_reply, 2048, 0)) != SOCKET_ERROR)
	{
		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';
		char buf[2046];
		strncpy(buf, server_reply + 2, 2046);
		string str(buf);

		//char buf[2000];
		// std::string->substr(int pos, int n)
		// pos위치부터 n개 문자를 새로운 서브스트링으로 생성리턴

		uds.GMS = server_reply[0];
		uds.service = server_reply[1];
		uds.msg = str;
		

		if (uds.GMS == 'S') {		// system
			if (uds.service == 'A') {		// name set
				CString prevCStr;
				m_pClient->h_comboReceiver.GetLBText(m_pClient->h_comboReceiver.GetCurSel(), prevCStr);

				m_pClient->h_comboReceiver.ResetContent();
				m_pClient->h_comboReceiver.AddString(_T("전체"));

				m_pClient->h_ClientList.ResetContent();

				int locate;	// 0번쨰 위치에서 부터 @위치를 찾음
				string temp;
	
				while ((locate = str.find("|", 0)) != std::string::npos) {
					temp = str.substr(0, locate);
					m_pClient->AddClientList(temp);
					m_pClient->AddRecverCombo(temp);
					str = str.substr(locate + 1);
				}
				temp = str.substr(0);
				m_pClient->AddClientList(temp);
				m_pClient->AddRecverCombo(temp);

				if ((locate = m_pClient->h_comboReceiver.FindString(0, prevCStr)) > 1) {
					m_pClient->h_comboReceiver.SetCurSel(locate);
				}
				else {
					m_pClient->h_comboReceiver.SetCurSel(0);
				}
				
			}
			else if (uds.service == 'S') {
				self->set_uds('S','H',"");
				string heartBeat = self->get_uds();
				self->SendData(heartBeat);
			}
			else if (uds.service == 'D') {
				m_pClient->SetOwnerMode();
			}
			else if (uds.service == 'F') {
				m_pClient->SetPersonMode();
			}
			else if (uds.service == 'K') {
				m_pClient->PopupMessageBox(uds.msg);
			}
		}
		else if (uds.GMS == 'M') {	// message
			if (uds.service == 'R') {
				m_pClient->AddChatMsg(uds.msg);
			}
			else if (uds.service == 'N') {
				m_pClient->AddNoticeMsg(uds.msg);
			}
			else if (uds.service == 'M') {
				m_pClient->PopupMessageBox(uds.msg);
				m_pClient->OnBnClickedButtonLogout();
			}
		}
		else if (uds.GMS == 'G') {
			if (uds.service == 'P') {
				if(strncmp(uds.msg.c_str(),"You are black player.",20))m_pGame->playerSet(0);
				else m_pGame->playerSet(1);

				m_pClient->AddNoticeMsg(uds.msg);
			}
			else if (uds.service == 'S') {
				int locate = str.find("|", 0);
				string blackPlayerName = str.substr(0, locate);
				string whitePlayerName = str.substr(locate + 1);
				m_pGame->setPlayerName(blackPlayerName, whitePlayerName);
				m_pGame->turn = 1;
			}
			else if (uds.service == 'U') {
				int locate = str.find("|", 0);
				string LocateX = str.substr(0, locate);
				string LocateY = str.substr(locate + 1);
				int locateX = stoi(LocateX);
				int locateY = stoi(LocateY);

				m_pGame->boardUpdate(locateX, locateY);
			}
			else if (uds.service == 'E') {
				m_pClient->AddNoticeMsg(uds.msg);
				m_pGame->gameEnd();
			}
		}
		else if (uds.GMS == 'K'){
			if (uds.service == 'D') {
				self->set_uds('R', 'D');
				string echoDelegation = self->get_uds();
				self->SendData(echoDelegation);
			}
		}

		else {
			string sTempMsg = string(server_reply) + "\n";
			m_pClient->AddChatMsg(sTempMsg);
		}

		uds.msg = "";
	}

}

void clientCon::EndConnect() {
	closesocket(s);
}

void clientCon::SendData(string sMessage)
{

	if (send(s, sMessage.c_str(), sMessage.size(), 0) < 0)
	{
		m_pClient->AddNoticeMsg("[Error] Can't send message. : " + sMessage);
		return;
	}

	uds.msg = "";
}

void clientCon::SendData(char GMS, char service, string sMessage)
{
	uds.GMS = GMS;
	uds.service = service;
	uds.msg = sMessage;

	string buf = get_uds();

	if (send(s, buf.c_str(), buf.size(), 0) < 0)
	{
		m_pClient->AddNoticeMsg("[Error] Can't send message. : " + buf);
		return;
	}

	uds.msg = "";
}

void clientCon::set_uds(char GMS, char service, string msg) {
	uds.GMS = GMS;
	uds.service = service;
	uds.msg = msg;
}

void clientCon::set_uds(char GMS, char service) {
	uds.GMS = GMS;
	uds.service = service;
}

string clientCon::get_uds() {
	string str(uds.msg);
	str.insert(0, 1, uds.service);
	str.insert(0, 1, uds.GMS);
	return str;
}

void clientCon::took_stone(string locateStr) {
	set_uds('G', 'L', locateStr);
	string gameLocate = get_uds();
	SendData(gameLocate);
}