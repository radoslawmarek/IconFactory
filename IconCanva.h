#pragma once

#include <wx/wx.h>

class IconCanva : public wxWindow
{
public:
    IconCanva(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
    ~IconCanva() override {};
private:
    void OnPaint(wxPaintEvent& event);
};
