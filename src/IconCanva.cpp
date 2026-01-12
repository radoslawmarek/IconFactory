#include "IconCanva.h"
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <random>

wxDEFINE_EVENT(EVT_CELL_HOVERED, wxCommandEvent);
wxDEFINE_EVENT(EVT_ICON_CHANGED, wxCommandEvent);

IconCanva::IconCanva(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxWindow(parent, id, pos, size)
{
    this->wxWindowBase::SetMinSize(this->FromDIP(wxSize(300, 300)));
    this->wxWindowBase::SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &IconCanva::OnPaint, this);
    this->Bind(wxEVT_MOTION, &IconCanva::OnMouseMove, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &IconCanva::OnMouseLeave, this);
    this->Bind(wxEVT_LEFT_DOWN, &IconCanva::OnLeftDown, this);
    this->Bind(wxEVT_KEY_UP, &IconCanva::OnKeyUp, this);
    this->Bind(wxEVT_KEY_DOWN, &IconCanva::OnKeyDown, this);

    SetFocusFromKbd();

    // Initialize pixel array
    m_iconPixels.resize(m_iconSize, std::vector<bool>(m_iconSize, false));
}

void IconCanva::SetIconSize(int size)
{
    m_iconSize = size;
    m_rulers.clear();
    m_iconPixels.clear();
    m_iconPixels.resize(m_iconSize, std::vector<bool>(m_iconSize, false));
    NotifyIconChanged();
    Refresh();
}

void IconCanva::SetIconPixels(const std::vector<std::vector<bool>>& pixels)
{
    m_iconPixels = pixels;
    NotifyIconChanged();
    Refresh();
}

void IconCanva::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        DrawRulers(gc);
        DrawPreviewRuler(gc);
        DrawIconPixels(gc);
        DrawPreviewLine(gc);
        DrawPreviewRectangle(gc);
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

    // Handle ruler dragging
    if (m_draggingRulerIndex >= 0 && event.Dragging() && event.LeftIsDown())
    {
        int newPos = GetGridPositionFromMouse(mousePos, m_rulers[m_draggingRulerIndex].orientation);
        if (newPos >= 0)
        {
            m_rulers[m_draggingRulerIndex].position = newPos;
            Refresh();
        }
    }
    // Handle preview ruler movement
    else if (m_previewRulerActive)
    {
        int newPos = GetGridPositionFromMouse(mousePos, m_previewOrientation);
        if (newPos >= 0)
        {
            m_previewPosition = newPos;
            Refresh();
        }
    }

    // Check if mouse is within grid bounds
    if (mousePos.x >= m_gridX && mousePos.x < m_gridX + m_gridPixelSize &&
        mousePos.y >= m_gridY && mousePos.y < m_gridY + m_gridPixelSize)
    {
        // Calculate cell position
        const int col = static_cast<int>((mousePos.x - m_gridX) / m_cellSize);
        const int row = static_cast<int>((mousePos.y - m_gridY) / m_cellSize);

        // Update current position for line drawing preview
        if (m_lineDrawingMode && m_lineStartSet)
        {
            m_currentCol = col;
            m_currentRow = row;
            Refresh();
        }

        // Update current position for rectangle drawing preview
        if (m_rectangleDrawingMode && m_rectangleStartSet)
        {
            m_currentCol = col;
            m_currentRow = row;
            Refresh();
        }

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

void IconCanva::OnLeftDown(wxMouseEvent& event)
{
    SetFocus();

    wxPoint mousePos = event.GetPosition();

    // If in preview mode, place the ruler
    if (m_previewRulerActive)
    {
        Ruler ruler;
        ruler.orientation = m_previewOrientation;
        ruler.position = m_previewPosition;
        ruler.color = wxColour(173, 216, 230); // Light blue pastel that complements gray grid
        ruler.selected = false;

        m_rulers.push_back(ruler);
        m_previewRulerActive = false;
        Refresh();
        return;
    }

    // Check if clicking on a grid cell
    if (mousePos.x >= m_gridX && mousePos.x < m_gridX + m_gridPixelSize &&
        mousePos.y >= m_gridY && mousePos.y < m_gridY + m_gridPixelSize)
    {
        int col = static_cast<int>((mousePos.x - m_gridX) / m_cellSize);
        int row = static_cast<int>((mousePos.y - m_gridY) / m_cellSize);

        // Handle rectangle drawing mode
        if (m_rectangleDrawingMode)
        {
            if (!m_rectangleStartSet)
            {
                // First click - set first corner
                m_rectangleStartCol = col;
                m_rectangleStartRow = row;
                m_currentCol = col;
                m_currentRow = row;
                m_rectangleStartSet = true;
                Refresh();
            }
            else
            {
                // Second click - draw the rectangle outline
                int minCol = std::min(m_rectangleStartCol, col);
                int maxCol = std::max(m_rectangleStartCol, col);
                int minRow = std::min(m_rectangleStartRow, row);
                int maxRow = std::max(m_rectangleStartRow, row);

                // Draw top and bottom edges
                for (int c = minCol; c <= maxCol; c++)
                {
                    m_iconPixels[minRow][c] = true;
                    m_iconPixels[maxRow][c] = true;
                }

                // Draw left and right edges
                for (int r = minRow; r <= maxRow; r++)
                {
                    m_iconPixels[r][minCol] = true;
                    m_iconPixels[r][maxCol] = true;
                }

                // Exit rectangle drawing mode
                m_rectangleDrawingMode = false;
                m_rectangleStartSet = false;
                NotifyIconChanged();
                Refresh();
            }
            return;
        }

        // Handle line drawing mode
        if (m_lineDrawingMode)
        {
            if (!m_lineStartSet)
            {
                // First click - set start point
                m_lineStartCol = col;
                m_lineStartRow = row;
                m_currentCol = col;
                m_currentRow = row;
                m_lineStartSet = true;
                Refresh();
            }
            else
            {
                // Second click - draw the line
                if (m_isHorizontalLine)
                {
                    // Draw horizontal line
                    int startCol = std::min(m_lineStartCol, col);
                    int endCol = std::max(m_lineStartCol, col);
                    for (int c = startCol; c <= endCol; c++)
                    {
                        m_iconPixels[m_lineStartRow][c] = true;
                    }
                }
                else
                {
                    // Draw vertical line
                    int startRow = std::min(m_lineStartRow, row);
                    int endRow = std::max(m_lineStartRow, row);
                    for (int r = startRow; r <= endRow; r++)
                    {
                        m_iconPixels[r][m_lineStartCol] = true;
                    }
                }

                // Exit line drawing mode
                m_lineDrawingMode = false;
                m_lineStartSet = false;
                NotifyIconChanged();
                Refresh();
            }
            return;
        }

        // Check if clicking on ruler
        int rulerIndex = GetRulerAtPosition(mousePos);

        if (rulerIndex >= 0)
        {
            // Deselect all rulers first
            for (auto& ruler : m_rulers)
            {
                ruler.selected = false;
            }

            m_rulers[rulerIndex].selected = true;
            m_draggingRulerIndex = rulerIndex;
            Refresh();
        }
        else
        {
            // Deselect all rulers
            for (auto& ruler : m_rulers)
            {
                ruler.selected = false;
            }
            m_draggingRulerIndex = -1;

            // Toggle pixel
            TogglePixel(col, row);
        }
    }
    else
    {
        // Deselect all rulers if clicking outside grid
        for (auto& ruler : m_rulers)
        {
            ruler.selected = false;
        }
        m_draggingRulerIndex = -1;
        Refresh();
    }

    event.Skip();
}

void IconCanva::OnKeyUp(wxKeyEvent& event)
{
    int keyCode = event.GetKeyCode();

    if (keyCode == 'H')
    {
        m_previewRulerActive = true;
        m_previewOrientation = RulerOrientation::Horizontal;
        m_previewPosition = m_iconSize / 2;
        Refresh();
    }
    else if (keyCode == 'V')
    {
        m_previewRulerActive = true;
        m_previewOrientation = RulerOrientation::Vertical;
        m_previewPosition = m_iconSize / 2;
        Refresh();
    }

    event.Skip();
}

void IconCanva::OnKeyDown(wxKeyEvent& event)
{
    int keyCode = event.GetKeyCode();

    if (keyCode == WXK_DELETE || keyCode == WXK_BACK)
    {
        // Delete selected ruler
        for (auto it = m_rulers.begin(); it != m_rulers.end(); )
        {
            if (it->selected)
            {
                it = m_rulers.erase(it);
                Refresh();
            }
            else
            {
                ++it;
            }
        }
    }

    event.Skip();
}

void IconCanva::DrawRulers(wxGraphicsContext* gc)
{
    wxColour rulerColor(173, 216, 230); // Light blue pastel

    for (const auto& ruler : m_rulers)
    {
        wxColour drawColor = rulerColor;
        if (ruler.selected)
        {
            // Make selected rulers slightly darker
            drawColor = wxColour(
                static_cast<unsigned char>(rulerColor.Red() * 0.7),
                static_cast<unsigned char>(rulerColor.Green() * 0.7),
                static_cast<unsigned char>(rulerColor.Blue() * 0.7)
            );
        }

        gc->SetBrush(wxBrush(drawColor));
        gc->SetPen(wxPen(drawColor.ChangeLightness(80), ruler.selected ? 2 : 1));

        if (ruler.orientation == RulerOrientation::Horizontal)
        {
            double y = m_gridY + ruler.position * m_cellSize;
            double rectHeight = m_cellSize;
            gc->DrawRectangle(m_gridX, y, m_gridPixelSize, rectHeight);
        }
        else // Vertical
        {
            double x = m_gridX + ruler.position * m_cellSize;
            double rectWidth = m_cellSize;
            gc->DrawRectangle(x, m_gridY, rectWidth, m_gridPixelSize);
        }
    }
}

void IconCanva::DrawPreviewRuler(wxGraphicsContext* gc)
{
    if (!m_previewRulerActive) return;

    wxColour previewColor(200, 200, 200, 128); // Semi-transparent gray
    gc->SetBrush(wxBrush(previewColor));
    gc->SetPen(wxPen(previewColor, 1, wxPENSTYLE_DOT));

    if (m_previewOrientation == RulerOrientation::Horizontal)
    {
        double y = m_gridY + m_previewPosition * m_cellSize;
        double rectHeight = m_cellSize;
        gc->DrawRectangle(m_gridX, y, m_gridPixelSize, rectHeight);
    }
    else // Vertical
    {
        double x = m_gridX + m_previewPosition * m_cellSize;
        double rectWidth = m_cellSize;
        gc->DrawRectangle(x, m_gridY, rectWidth, m_gridPixelSize);
    }
}

int IconCanva::GetRulerAtPosition(const wxPoint& pos)
{
    // Check rulers in reverse order (top to bottom in rendering)
    for (int i = m_rulers.size() - 1; i >= 0; i--)
    {
        const auto& ruler = m_rulers[i];

        if (ruler.orientation == RulerOrientation::Horizontal)
        {
            double y = m_gridY + ruler.position * m_cellSize;
            double rectHeight = m_cellSize;

            if (pos.x >= m_gridX && pos.x < m_gridX + m_gridPixelSize &&
                pos.y >= y && pos.y < y + rectHeight)
            {
                return i;
            }
        }
        else // Vertical
        {
            double x = m_gridX + ruler.position * m_cellSize;
            double rectWidth = m_cellSize;

            if (pos.y >= m_gridY && pos.y < m_gridY + m_gridPixelSize &&
                pos.x >= x && pos.x < x + rectWidth)
            {
                return i;
            }
        }
    }

    return -1;
}

int IconCanva::GetGridPositionFromMouse(const wxPoint& pos, RulerOrientation orientation)
{
    if (orientation == RulerOrientation::Horizontal)
    {
        if (pos.y >= m_gridY && pos.y < m_gridY + m_gridPixelSize)
        {
            int row = static_cast<int>((pos.y - m_gridY) / m_cellSize);
            return std::clamp(row, 0, m_iconSize - 1);
        }
    }
    else // Vertical
    {
        if (pos.x >= m_gridX && pos.x < m_gridX + m_gridPixelSize)
        {
            int col = static_cast<int>((pos.x - m_gridX) / m_cellSize);
            return std::clamp(col, 0, m_iconSize - 1);
        }
    }

    return -1;
}

wxColour IconCanva::GetRandomPastelColor()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 5);

    // Array of pastel colors
    const wxColour pastelColors[] = {
        wxColour(255, 179, 186), // Pastel Pink
        wxColour(255, 223, 186), // Pastel Peach
        wxColour(255, 255, 186), // Pastel Yellow
        wxColour(186, 255, 201), // Pastel Green
        wxColour(186, 225, 255), // Pastel Blue
        wxColour(220, 198, 255)  // Pastel Purple
    };

    return pastelColors[dis(gen)];
}

void IconCanva::DrawIconPixels(wxGraphicsContext* gc)
{
    gc->SetBrush(*wxBLACK_BRUSH);
    gc->SetPen(*wxTRANSPARENT_PEN);

    for (int row = 0; row < m_iconSize; row++)
    {
        for (int col = 0; col < m_iconSize; col++)
        {
            if (m_iconPixels[row][col])
            {
                double x = m_gridX + col * m_cellSize;
                double y = m_gridY + row * m_cellSize;
                gc->DrawRectangle(x, y, m_cellSize, m_cellSize);
            }
        }
    }
}

void IconCanva::TogglePixel(int col, int row)
{
    if (row >= 0 && row < m_iconSize && col >= 0 && col < m_iconSize)
    {
        m_iconPixels[row][col] = !m_iconPixels[row][col];
        NotifyIconChanged();
        Refresh();
    }
}

void IconCanva::NotifyIconChanged()
{
    wxCommandEvent evt(EVT_ICON_CHANGED, GetId());
    wxPostEvent(GetParent(), evt);
}

void IconCanva::SetLineDrawingMode(bool enabled, bool isHorizontal)
{
    m_lineDrawingMode = enabled;
    m_isHorizontalLine = isHorizontal;
    m_lineStartSet = false;
    m_lineStartCol = 0;
    m_lineStartRow = 0;
    m_currentCol = 0;
    m_currentRow = 0;
}

void IconCanva::DrawPreviewLine(wxGraphicsContext* gc)
{
    if (!m_lineDrawingMode || !m_lineStartSet) return;

    wxColour previewColor(128, 128, 128, 128); // Semi-transparent gray
    gc->SetBrush(wxBrush(previewColor));
    gc->SetPen(*wxTRANSPARENT_PEN);

    if (m_isHorizontalLine)
    {
        // Draw horizontal line preview
        int startCol = std::min(m_lineStartCol, m_currentCol);
        int endCol = std::max(m_lineStartCol, m_currentCol);
        for (int c = startCol; c <= endCol; c++)
        {
            double x = m_gridX + c * m_cellSize;
            double y = m_gridY + m_lineStartRow * m_cellSize;
            gc->DrawRectangle(x, y, m_cellSize, m_cellSize);
        }
    }
    else
    {
        // Draw vertical line preview
        int startRow = std::min(m_lineStartRow, m_currentRow);
        int endRow = std::max(m_lineStartRow, m_currentRow);
        for (int r = startRow; r <= endRow; r++)
        {
            double x = m_gridX + m_lineStartCol * m_cellSize;
            double y = m_gridY + r * m_cellSize;
            gc->DrawRectangle(x, y, m_cellSize, m_cellSize);
        }
    }
}

void IconCanva::SetRectangleDrawingMode(bool enabled)
{
    m_rectangleDrawingMode = enabled;
    m_rectangleStartSet = false;
    m_rectangleStartCol = 0;
    m_rectangleStartRow = 0;
    m_currentCol = 0;
    m_currentRow = 0;
}

void IconCanva::DrawPreviewRectangle(wxGraphicsContext* gc)
{
    if (!m_rectangleDrawingMode || !m_rectangleStartSet) return;

    wxColour previewColor(128, 128, 128, 128); // Semi-transparent gray
    gc->SetBrush(wxBrush(previewColor));
    gc->SetPen(*wxTRANSPARENT_PEN);

    int minCol = std::min(m_rectangleStartCol, m_currentCol);
    int maxCol = std::max(m_rectangleStartCol, m_currentCol);
    int minRow = std::min(m_rectangleStartRow, m_currentRow);
    int maxRow = std::max(m_rectangleStartRow, m_currentRow);

    // Draw top and bottom edges
    for (int c = minCol; c <= maxCol; c++)
    {
        double x = m_gridX + c * m_cellSize;
        double y = m_gridY + minRow * m_cellSize;
        gc->DrawRectangle(x, y, m_cellSize, m_cellSize);

        y = m_gridY + maxRow * m_cellSize;
        gc->DrawRectangle(x, y, m_cellSize, m_cellSize);
    }

    // Draw left and right edges (excluding corners already drawn)
    for (int r = minRow + 1; r < maxRow; r++)
    {
        double x = m_gridX + minCol * m_cellSize;
        double y = m_gridY + r * m_cellSize;
        gc->DrawRectangle(x, y, m_cellSize, m_cellSize);

        x = m_gridX + maxCol * m_cellSize;
        gc->DrawRectangle(x, y, m_cellSize, m_cellSize);
    }
}
