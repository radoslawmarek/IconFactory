#pragma once

#include <wx/wx.h>
#include <vector>

wxDECLARE_EVENT(EVT_CELL_HOVERED, wxCommandEvent);
wxDECLARE_EVENT(EVT_ICON_CHANGED, wxCommandEvent);

enum class RulerOrientation
{
    Horizontal,
    Vertical
};

struct Ruler
{
    RulerOrientation orientation;
    int position; // Grid cell position (row for horizontal, col for vertical)
    wxColour color;
    bool selected = false;
};

class IconCanva : public wxWindow
{
public:
    IconCanva(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
    ~IconCanva() override {};

    void SetIconSize(int size);

    int GetIconSize() const { return m_iconSize; }
    const std::vector<std::vector<bool>>& GetIconPixels() const { return m_iconPixels; }
    void SetIconPixels(const std::vector<std::vector<bool>>& pixels);

    void SetLineDrawingMode(bool enabled, bool isHorizontal);
    void SetRectangleDrawingMode(bool enabled);

private:
    void OnPaint(wxPaintEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void OnKeyDown(wxKeyEvent& event);

    void DrawGrid(wxGraphicsContext* gc);
    void DrawRulers(wxGraphicsContext* gc);
    void DrawPreviewRuler(wxGraphicsContext* gc);
    void DrawIconPixels(wxGraphicsContext* gc);
    void DrawPreviewLine(wxGraphicsContext* gc);
    void DrawPreviewRectangle(wxGraphicsContext* gc);

    int GetRulerAtPosition(const wxPoint& pos);
    int GetGridPositionFromMouse(const wxPoint& pos, RulerOrientation orientation);
    wxColour GetRandomPastelColor();

    void TogglePixel(int col, int row);
    void NotifyIconChanged();

    int m_iconSize = 32; // Default size
    int m_gridX = 0;
    int m_gridY = 0;
    int m_gridPixelSize = 0;
    double m_cellSize = 0.0;

    std::vector<Ruler> m_rulers;
    bool m_previewRulerActive = false;
    RulerOrientation m_previewOrientation;
    int m_previewPosition = 0;

    int m_draggingRulerIndex = -1;

    std::vector<std::vector<bool>> m_iconPixels; // [row][col]

    // Line drawing state
    bool m_lineDrawingMode = false;
    bool m_isHorizontalLine = true;
    bool m_lineStartSet = false;
    int m_lineStartCol = 0;
    int m_lineStartRow = 0;
    int m_currentCol = 0;
    int m_currentRow = 0;

    // Rectangle drawing state
    bool m_rectangleDrawingMode = false;
    bool m_rectangleStartSet = false;
    int m_rectangleStartCol = 0;
    int m_rectangleStartRow = 0;
};
