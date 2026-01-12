#include "IconCanva.h"
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

IconCanva::IconCanva(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxWindow(parent, id, pos, size)
{
    this->wxWindowBase::SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &IconCanva::OnPaint, this);
}

void IconCanva::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        wxPoint rectOrigin = this->FromDIP(wxPoint(189, 200));
        wxSize rectSize = this->FromDIP(wxSize(100, 80));

        wxGraphicsBrush brush = gc->CreateBrush(wxColour(255, 0, 0));
        gc->SetBrush(*wxRED_BRUSH);
        gc->DrawRectangle(rectOrigin.x, rectOrigin.y, rectSize.GetWidth(), rectSize.GetHeight());

        delete gc;
    }
}
