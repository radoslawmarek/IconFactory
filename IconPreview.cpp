#include "IconPreview.h"
#include <wx/dcbuffer.h>

IconPreview::IconPreview(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxWindow(parent, id, pos, size)
{
    this->wxWindowBase::SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &IconPreview::OnPaint, this);

    // Initialize with empty pixel array
    m_pixels.resize(m_iconSize, std::vector<bool>(m_iconSize, false));

    // Set minimum size to accommodate largest icon (64x64 at 4x scale) plus padding
    this->wxWindowBase::SetMinSize(this->FromDIP(wxSize(64 * 4 + 24, 64 * 4 + 24)));
}

void IconPreview::SetIconData(int iconSize, const std::vector<std::vector<bool>>& pixels)
{
    m_iconSize = iconSize;
    m_pixels = pixels;

    // Update minimum size based on icon size and scale
    int scaledSize = m_iconSize * m_scale;
    this->wxWindowBase::SetMinSize(this->FromDIP(wxSize(scaledSize + 24, scaledSize + 24)));

    Refresh();
}

void IconPreview::SetScale(int scale)
{
    m_scale = scale;

    // Update minimum size based on icon size and new scale
    int scaledSize = m_iconSize * m_scale;
    this->wxWindowBase::SetMinSize(this->FromDIP(wxSize(scaledSize + 24, scaledSize + 24)));

    Refresh();
}

void IconPreview::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    // Get client size and calculate centering
    wxSize clientSize = GetClientSize();

    // Calculate scaled icon size in pixels
    // m_iconSize and m_scale are logical values, convert the result to pixels
    int scaledIconSizeLogical = m_iconSize * m_scale;
    int scaledIconSizePixels = FromDIP(scaledIconSizeLogical);

    int startX = (clientSize.GetWidth() - scaledIconSizePixels) / 2;
    int startY = (clientSize.GetHeight() - scaledIconSizePixels) / 2;

    // Draw white background for icon area
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawRectangle(startX, startY, scaledIconSizePixels, scaledIconSizePixels);

    // Draw the icon pixels (scaled)
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(*wxTRANSPARENT_PEN);

    // Each pixel is m_scale DIP large
    int pixelSizePixels = FromDIP(m_scale);

    for (int row = 0; row < m_iconSize; row++)
    {
        for (int col = 0; col < m_iconSize; col++)
        {
            if (row < static_cast<int>(m_pixels.size()) &&
                col < static_cast<int>(m_pixels[row].size()) &&
                m_pixels[row][col])
            {
                // Calculate position: each logical pixel is m_scale DIP
                int x = startX + col * pixelSizePixels;
                int y = startY + row * pixelSizePixels;
                dc.DrawRectangle(x, y, pixelSizePixels, pixelSizePixels);
            }
        }
    }
}
