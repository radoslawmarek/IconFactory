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
    wxButton* m_btnHorizontalRuler;
    wxButton* m_btnVerticalRuler;
    wxButton* m_btnHorizontalLine;
    wxButton* m_btnVerticalLine;

    void SetLayout();
    void OnIconSizeChanged(wxCommandEvent& event);
    void OnCellHovered(wxCommandEvent& event);
    void OnIconChanged(wxCommandEvent& event);
    void OnScaleChanged(wxCommandEvent& event);
    void OnHorizontalRulerClick(wxCommandEvent& event);
    void OnVerticalRulerClick(wxCommandEvent& event);
    void OnHorizontalLineClick(wxCommandEvent& event);
    void OnVerticalLineClick(wxCommandEvent& event);
};