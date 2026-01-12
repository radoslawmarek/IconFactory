#pragma once

#include "wx/wx.h"
#include "IconCanva.h"


class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    // Controls
    wxPanel* m_panel;
    IconCanva* m_iconCanva;
    wxRadioBox* rb_iconSize;

    void SetLayout();
    void OnIconSizeChanged(wxCommandEvent& event);
    void OnCellHovered(wxCommandEvent& event);
};