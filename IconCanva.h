#pragma once

#include <wx/wx.h>

wxDECLARE_EVENT(EVT_CELL_HOVERED, wxCommandEvent);

class IconCanva : public wxWindow
{
public:
    IconCanva(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
    ~IconCanva() override {};

    void SetIconSize(int size);

private:
    void OnPaint(wxPaintEvent& event) override;
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    void DrawGrid(wxGraphicsContext* gc);

    int m_iconSize = 32; // Default size
    int m_gridX = 0;
    int m_gridY = 0;
    int m_gridPixelSize = 0;
    double m_cellSize = 0.0;
};
