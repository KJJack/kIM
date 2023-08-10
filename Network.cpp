#include "Network.h"

bool Network::connected = false;

Network::Network()
{
	clientSocket = INVALID_SOCKET;
	port = 0;
	connected = false;
}

Network::~Network()
{
	WSACleanup();
}

void Network::CheckDLL()
{
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	
	if (wsaerr != 0)
		return;
}

void Network::CheckSocket()
{
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientSocket == INVALID_SOCKET)
		return;
}

bool Network::CheckConnectionSetup(wxString ipaddress)
{
	std::string convert = ipaddress.wxString::ToStdString();
	std::wstring widestr = std::wstring(convert.begin(), convert.end());
	// not reliable conversion and breaks :(
	//const wchar_t* ip2 = ipaddress.wxString::wchar_str();
	const wchar_t* ip = widestr.c_str();
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, ip, &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService,
		sizeof(clientService)) == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		connected = true;
		return true;
	}
}

bool Network::IsConnected()
{
	return connected;
}

int Network::SendToServer(wxString msg)
{
	char buffer[200];
	strncpy(buffer, (const char*)msg.mb_str(wxConvUTF8), 200);

	return send(clientSocket, buffer, 200, 0);
}

char* Network::RecieveMessage(int &byteCount)
{
	byteCount = recv(clientSocket, outMsg, 200, 0);

	return outMsg;
}


void Network::setPort(int inPort)
{
	port = inPort;
}
