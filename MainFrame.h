#pragma once

#include "wx/wx.h"
#include "IconCanva.h"
#include "IconPreview.h"


class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    // Controls
    wxPanel* m_panel;
    IconCanva* m_iconCanva;
    IconPreview* m_iconPreview;
    wxRadioBox* rb_iconSize;
    wxSlider* m_scaleSlider;

    void SetLayout();
    void OnIconSizeChanged(wxCommandEvent& event);
    void OnCellHovered(wxCommandEvent& event);
    void OnIconChanged(wxCommandEvent& event);
    void OnScaleChanged(wxCommandEvent& event);
};