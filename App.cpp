#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    auto* main_frame = new MainFrame();
    main_frame->Center();
    main_frame->Show(true);
    return true;
}
