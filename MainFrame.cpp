#include "MainFrame.h"
#include "Network.h"
#include <string>
#include <thread>

using namespace std::literals::chrono_literals;

MainFrame::MainFrame(const wxString& title)
	:wxFrame(nullptr, wxID_ANY, title)
{
	CreateControls();
	CreateStatusBar();
	BindEvents();
}

void MainFrame::CreateControls()
{
	wxPanel* panel = new wxPanel(this);

	// LOGIN PORTION
	ipText = new wxTextCtrl(panel, wxID_ANY, "",
		wxPoint(540, 30), wxSize(100, -1));

	portText = new wxTextCtrl(panel, wxID_ANY, "",
		wxPoint(645, 30), wxSize(50, -1));

	userText = new wxTextCtrl(panel, wxID_ANY, "",
		wxPoint(540, 60), wxSize(155, -1));

	// NETWORK CONNECT / DISCONNECT
	connButton = new wxButton(panel, wxID_ANY, "Connect",
		wxPoint(540, 90), wxSize(70, -1));

	discButton = new wxButton(panel, wxID_ANY, "Disconnect",
		wxPoint(625, 90), wxSize(70, -1));

	// CHAT
	chatText = new wxTextCtrl(panel, wxID_ANY, "",
		wxPoint(30, 30), wxSize(480, 350), wxTE_MULTILINE | wxTE_READONLY);

	msgText = new wxTextCtrl(panel, wxID_ANY, "",
		wxPoint(30, 400), wxSize(380, 60), wxTE_PROCESS_ENTER);

	msgButton = new wxButton(panel, wxID_ANY, "Submit",
		wxPoint(420, 415), wxSize(90, 30));
}

void MainFrame::BindEvents()
{
	connButton->Bind(wxEVT_BUTTON, &MainFrame::OnConnButtonClicked, this);
	discButton->Bind(wxEVT_BUTTON, &MainFrame::OnDiscButtonClicked, this);
	msgButton->Bind(wxEVT_BUTTON, &MainFrame::OnSendButtonClicked, this);
	msgText->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnSendKeyDown, this);
}

// Works but needs to be reimplemented: can still connect to network without valid Username
void MainFrame::EstablishConnection()
{
	wxString ipaddress = ipText->GetValue();
	wxString strport = portText->GetValue();
	int portNum = wxAtoi(strport);
	network.setPort(portNum);

	if (!userText->IsEmpty())
		userName = userText->GetValue();

	network.CheckDLL();
	network.CheckSocket();
	if (network.CheckConnectionSetup(ipaddress) && !userText->IsEmpty())
	{
		wxLogStatus("Connected to %s::%s", ipaddress, strport);
		DisableControls();

		std::thread myThread(&MainFrame::RecMessageText, this);
		myThread.detach();

	}
	else
		wxLogStatus("Connection Failed \t %s %s %s", invalidIP() , invalidPort(), invalidUser());
}

void MainFrame::DisconnectConnection()
{
	if (!network.IsConnected())
	{
		wxLogStatus("No connection was ever established...");
	}
	else
	{
		wxLogStatus("Disconnected Successfully");
		EnableControls();
		network.~Network();
	}
}

void MainFrame::SendMessageText(wxString msg)
{

	int	result = network.SendToServer(msg);

	if (result > 0)
	{
		wxString _Message = wxString::Format("%s : %s\n", userName, msg);
		chatText->AppendText(_Message);
	}
	else
	{
		WSACleanup();
	}

	msgText->Clear();
}

void MainFrame::RecMessageText()
{
	int byteCount = 0;

	while (network.IsConnected())
	{
		std::string test = network.RecieveMessage(byteCount);

		if (byteCount > 0)
		{
			wxString myString(test.c_str(), wxConvUTF8);
			myString = wxString::Format("SERVER: %s\n", myString);
			chatText->AppendText(myString);
		}
	}
}

void MainFrame::OnConnButtonClicked(wxCommandEvent& evt)
{
	EstablishConnection();
	//evt.Skip();
}

void MainFrame::OnDiscButtonClicked(wxCommandEvent& evt)
{
	DisconnectConnection();
}

void MainFrame::OnSendButtonClicked(wxCommandEvent& evt)
{
	if (!msgText->IsEmpty())
		SendMessageText(msgText->GetValue());
}

void MainFrame::OnSendKeyDown(wxCommandEvent& evt)
{
	if (!msgText->IsEmpty())
		SendMessageText(msgText->GetValue());
}

wxString MainFrame::invalidIP()
{
	wxString errIP;

	if (ipText->IsEmpty())
		errIP = "{ MISSING IP }";
	else
		errIP = "{ INVALID IP }";

	return errIP;
}

wxString MainFrame::invalidPort()
{
	wxString errPort;

	if (portText->IsEmpty())
		errPort = "{ MISSING PORT }";
	else
		errPort = "{ INVALID PORT }";

	return errPort;
}

wxString MainFrame::invalidUser()
{
	wxString errUser;

	if (userText->IsEmpty())
		errUser = "{ MISSING USERNAME }";

	return errUser;
}

void MainFrame::EnableControls()
{
	connButton->Enable(true);
	ipText->Enable(true);
	portText->Enable(true);
	userText->Enable(true);
}

void MainFrame::DisableControls()
{
	connButton->Enable(false);
	ipText->Enable(false);
	portText->Enable(false);
	userText->Enable(false);
}
