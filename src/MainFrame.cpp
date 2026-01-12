#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Icon Factory")
{
    m_panel = new wxPanel(this);
    m_iconCanva = new IconCanva(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_iconPreview = new IconPreview(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    rb_iconSize = new wxRadioBox(m_panel, wxID_ANY, "Icon Size", wxDefaultPosition, wxDefaultSize, {"Small (16x16)", "Medium (32x32)", "Large (64x64)"}, 1, wxRA_SPECIFY_COLS);
    m_scaleSlider = new wxSlider(m_panel, wxID_ANY, 2, 1, 4, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

    // Set the default selection to Medium (32x32)
    rb_iconSize->SetSelection(1);
    m_iconCanva->SetIconSize(32);
    m_iconPreview->SetIconData(32, m_iconCanva->GetIconPixels());
    m_iconPreview->SetScale(2); // Set initial scale to 2x

    // Bind event handlers
    rb_iconSize->Bind(wxEVT_RADIOBOX, &MainFrame::OnIconSizeChanged, this);
    m_panel->Bind(EVT_CELL_HOVERED, &MainFrame::OnCellHovered, this);
    m_panel->Bind(EVT_ICON_CHANGED, &MainFrame::OnIconChanged, this);
    m_scaleSlider->Bind(wxEVT_SLIDER, &MainFrame::OnScaleChanged, this);

    SetLayout();

    // Create a sizer for the Frame itself to manage the panel
    auto* frame_sizer = new wxBoxSizer(wxVERTICAL);
    frame_sizer->Add(m_panel, 1, wxEXPAND);
    this->SetSizer(frame_sizer);

    // This tells the window to respect the minimum size of the sizer
    frame_sizer->SetSizeHints(this);

    wxFrameBase::CreateStatusBar();
    this->SetClientSize(this->FromDIP(wxSize(1024, 768)));
}

void MainFrame::SetLayout()
{
    auto* main_sizer = new wxBoxSizer(wxHORIZONTAL);
    auto* toolbox_sizer = new wxBoxSizer(wxVERTICAL);

    toolbox_sizer->Add(rb_iconSize, 0, wxEXPAND);
    toolbox_sizer->AddStretchSpacer(1);
    toolbox_sizer->Add(m_iconPreview, 0, wxALIGN_CENTER | wxTOP, this->FromDIP(12));
    toolbox_sizer->Add(m_scaleSlider, 0, wxEXPAND | wxTOP, this->FromDIP(12));

    main_sizer->Add(m_iconCanva, 1, wxEXPAND);
    main_sizer->Add(toolbox_sizer, 0, wxEXPAND | wxALL, 12);

    m_panel->SetSizer(main_sizer);
    main_sizer->SetSizeHints(m_panel);
}

void MainFrame::OnIconSizeChanged(wxCommandEvent& event)
{
    int selection = rb_iconSize->GetSelection();
    int size;

    switch (selection)
    {
        case 0: size = 16; break;
        case 1: size = 32; break;
        case 2: size = 64; break;
        default: size = 32; break;
    }

    m_iconCanva->SetIconSize(size);
    m_iconPreview->SetIconData(size, m_iconCanva->GetIconPixels());
}

void MainFrame::OnCellHovered(wxCommandEvent& event)
{
    wxString cellPos = event.GetString();
    if (cellPos.IsEmpty())
    {
        SetStatusText("");
    }
    else
    {
        SetStatusText("Cell: " + cellPos);
    }
}

void MainFrame::OnIconChanged(wxCommandEvent& event)
{
    // Update the preview with the current icon data
    m_iconPreview->SetIconData(m_iconCanva->GetIconSize(), m_iconCanva->GetIconPixels());
}

void MainFrame::OnScaleChanged(wxCommandEvent& event)
{
    int scale = m_scaleSlider->GetValue();
    m_iconPreview->SetScale(scale);
}
