#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Icon Factory")
{
    m_panel = new wxPanel(this);
    m_iconCanva = new IconCanva(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    SetLayout();
    wxFrameBase::CreateStatusBar();
    this->SetClientSize(wxSize(1024, 768));
}

void MainFrame::SetLayout()
{
    auto* main_sizer = new wxBoxSizer(wxHORIZONTAL);

    main_sizer->Add(m_iconCanva, 1, wxEXPAND);

    m_panel->SetSizer(main_sizer);
    main_sizer->SetSizeHints(m_panel);
}
