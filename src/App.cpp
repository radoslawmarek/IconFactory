#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    // Initialize all image handlers (required for PNG support)
    wxInitAllImageHandlers();

    auto* main_frame = new MainFrame();
    main_frame->Center();
    main_frame->Show(true);
    return true;
}
