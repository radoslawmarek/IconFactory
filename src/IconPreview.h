#pragma once

#include <wx/wx.h>
#include <vector>

class IconPreview : public wxWindow
{
public:
    IconPreview(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
    ~IconPreview() override {};

    void SetIconData(int iconSize, const std::vector<std::vector<bool>>& pixels);
    void SetScale(int scale);

private:
    void OnPaint(wxPaintEvent& event);

    int m_iconSize = 32;
    int m_scale = 2;
    std::vector<std::vector<bool>> m_pixels;
};
