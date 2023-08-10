#include "App.h"
#include "MainFrame.h"
#include "Constants.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* mainFrame = new MainFrame("kIM");
    mainFrame->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    mainFrame->SetOwnBackgroundColour(*wxRED);
    mainFrame->Centre();
    mainFrame->Show();
    return true;
}
