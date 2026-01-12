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

    void SetLayout();
};