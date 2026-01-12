#include "IconCanva.h"
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

wxDEFINE_EVENT(EVT_CELL_HOVERED, wxCommandEvent);

IconCanva::IconCanva(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxWindow(parent, id, pos, size)
{
    this->wxWindowBase::SetMinSize(this->FromDIP(wxSize(300, 300)));
    this->wxWindowBase::SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &IconCanva::OnPaint, this);
    this->Bind(wxEVT_MOTION, &IconCanva::OnMouseMove, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &IconCanva::OnMouseLeave, this);
}

void IconCanva::SetIconSize(int size)
{
    m_iconSize = size;
    Refresh();
}

void IconCanva::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        DrawGrid(gc);
        delete gc;
    }
}

void IconCanva::DrawGrid(wxGraphicsContext* gc)
{
    const wxSize clientSize = GetClientSize();
    const int padding = FromDIP(12);

    // Calculate available space for the grid
    const int availableWidth = clientSize.GetWidth() - 2 * padding;
    const int availableHeight = clientSize.GetHeight() - 2 * padding;
    const int gridSize = std::min(availableWidth, availableHeight);

    // Calculate cell size
    m_cellSize = static_cast<double>(gridSize) / m_iconSize;

    // Calculate grid origin to center it
    m_gridPixelSize = static_cast<int>(m_cellSize * m_iconSize);
    m_gridX = (clientSize.GetWidth() - m_gridPixelSize) / 2;
    m_gridY = (clientSize.GetHeight() - m_gridPixelSize) / 2;

    // Draw grid lines
    const wxColour lightGray(200, 200, 200);
    const wxColour darkGray(100, 100, 100);

    const int centerRow = m_iconSize / 2;
    const int centerCol = m_iconSize / 2;

    // Draw vertical lines
    for (int i = 0; i <= m_iconSize; i++)
    {
        const double x = m_gridX + i * m_cellSize;

        // Outline or center line - darker
        if (i == 0 || i == m_iconSize || i == centerCol)
        {
            gc->SetPen(wxPen(darkGray, 1));
        }
        else
        {
            gc->SetPen(wxPen(lightGray, 1));
        }

        gc->StrokeLine(x, m_gridY, x, m_gridY + m_gridPixelSize);
    }

    // Draw horizontal lines
    for (int i = 0; i <= m_iconSize; i++)
    {
        const double y = m_gridY + i * m_cellSize;

        // Outline or center line - darker
        if (i == 0 || i == m_iconSize || i == centerRow)
        {
            gc->SetPen(wxPen(darkGray, 1));
        }
        else
        {
            gc->SetPen(wxPen(lightGray, 1));
        }

        gc->StrokeLine(m_gridX, y, m_gridX + m_gridPixelSize, y);
    }
}

void IconCanva::OnMouseMove(wxMouseEvent& event)
{
    wxPoint mousePos = event.GetPosition();

    // Check if mouse is within grid bounds
    if (mousePos.x >= m_gridX && mousePos.x < m_gridX + m_gridPixelSize &&
        mousePos.y >= m_gridY && mousePos.y < m_gridY + m_gridPixelSize)
    {
        // Calculate cell position
        const int col = static_cast<int>((mousePos.x - m_gridX) / m_cellSize);
        const int row = static_cast<int>((mousePos.y - m_gridY) / m_cellSize);

        // Send event to parent with cell position
        wxCommandEvent evt(EVT_CELL_HOVERED, GetId());
        evt.SetString(wxString::Format("(%d, %d)", col, row));
        wxPostEvent(GetParent(), evt);
    }
    else
    {
        // Mouse outside grid
        wxCommandEvent evt(EVT_CELL_HOVERED, GetId());
        evt.SetString("");
        wxPostEvent(GetParent(), evt);
    }

    event.Skip();
}

void IconCanva::OnMouseLeave(wxMouseEvent& event)
{
    // Clear status when mouse leaves control
    wxCommandEvent evt(EVT_CELL_HOVERED, GetId());
    evt.SetString("");
    wxPostEvent(GetParent(), evt);

    event.Skip();
}
