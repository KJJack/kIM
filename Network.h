#pragma once
//#define _AFX_SECURE_NO_WARNINGS
#include <wx/wx.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>


class Network
{
public:
	Network();
	~Network();

	void CheckDLL();
	void CheckSocket();
	bool CheckConnectionSetup(wxString ipaddress);
	bool IsConnected();
	int SendToServer(wxString msg);
	char* RecieveMessage(int &byteCount);

	void setPort(int inPort);

private:
	SOCKET clientSocket;
	int port;
	WSADATA wsaData;
	static bool connected;
	char outMsg[200];
};
