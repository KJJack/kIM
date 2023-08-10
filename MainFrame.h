#pragma once
#include <wx/wx.h>
#include "Network.h"
#include <thread>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	void CreateControls();
	void BindEvents();

	// NETWORK
	void EstablishConnection();
	void DisconnectConnection();
	void SendMessageText(wxString msg);
	void RecMessageText();

	void OnConnButtonClicked(wxCommandEvent& evt);
	void OnDiscButtonClicked(wxCommandEvent& evt);
	void OnSendButtonClicked(wxCommandEvent& evt);
	void OnSendKeyDown(wxCommandEvent& evt);

	// Helper functions
	wxString invalidIP();
	wxString invalidPort();
	wxString invalidUser();
	void EnableControls();
	void DisableControls();

	Network network;

	wxTextCtrl* chatText;
	wxTextCtrl* msgText;
	wxTextCtrl* ipText;
	wxTextCtrl* portText;
	wxTextCtrl* userText;

	wxButton* msgButton;
	wxButton* connButton;
	wxButton* discButton;

	wxString userName;

	std::thread worker;
};

