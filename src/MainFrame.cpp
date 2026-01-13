#include "MainFrame.h"
#include "Resources.h"
#include <wx/mstream.h>
#include <wx/file.h>

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Icon Factory")
{
    m_panel = new wxPanel(this);
    m_iconCanva = new IconCanva(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_iconPreview = new IconPreview(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    rb_iconSize = new wxRadioBox(m_panel, wxID_ANY, "Icon Size", wxDefaultPosition, wxDefaultSize, {"Small (16x16)", "Medium (32x32)", "Large (64x64)"}, 1, wxRA_SPECIFY_COLS);
    m_scaleSlider = new wxSlider(m_panel, wxID_ANY, 2, 1, 4, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

    // Load embedded PNG icons
    wxMemoryInputStream streamHR(fold_horizontal_png, fold_horizontal_png_size);
    wxMemoryInputStream streamVR(fold_vertical_png, fold_vertical_png_size);
    wxMemoryInputStream streamHL(minus_horizontal_png, minus_horizontal_png_size);
    wxMemoryInputStream streamVL(minus_vertical_png, minus_vertical_png_size);
    wxMemoryInputStream streamRect(rectangle_png, rectangle_png_size);
    wxMemoryInputStream streamOpen(open_png, open_png_size);
    wxMemoryInputStream streamSave(save_png, save_png_size);
    wxMemoryInputStream streamRotateClockwise(rotate_cw_png, rotate_cw_png_size);
    wxMemoryInputStream streamSelectRectangle(select_rectangle_png, select_rectangle_png_size);
    wxMemoryInputStream streamMove(move_png, move_png_size);
    wxMemoryInputStream streamFill(fill_png, fill_png_size);

    wxImage imgHR(streamHR, wxBITMAP_TYPE_PNG);
    wxImage imgVR(streamVR, wxBITMAP_TYPE_PNG);
    wxImage imgHL(streamHL, wxBITMAP_TYPE_PNG);
    wxImage imgVL(streamVL, wxBITMAP_TYPE_PNG);
    wxImage imgRect(streamRect, wxBITMAP_TYPE_PNG);
    wxImage imgOpen(streamOpen, wxBITMAP_TYPE_PNG);
    wxImage imgSave(streamSave, wxBITMAP_TYPE_PNG);
    wxImage imgRotateClockwise(streamRotateClockwise, wxBITMAP_TYPE_PNG);
    wxImage imgSelectRectangle(streamSelectRectangle, wxBITMAP_TYPE_PNG);
    wxImage imgMove(streamMove, wxBITMAP_TYPE_PNG);
    wxImage imgFill(streamFill, wxBITMAP_TYPE_PNG);

    wxBitmap bmpHR(imgHR);
    wxBitmap bmpVR(imgVR);
    wxBitmap bmpHL(imgHL);
    wxBitmap bmpVL(imgVL);
    wxBitmap bmpRect(imgRect);
    wxBitmap bmpOpen(imgOpen);
    wxBitmap bmpSave(imgSave);
    wxBitmap bmpRotateClockwise(imgRotateClockwise);
    wxBitmap bmpSelectRectangle(imgSelectRectangle);
    wxBitmap bmpMove(imgMove);
    wxBitmap bmpFill(imgFill);

    m_btnHorizontalRuler = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnVerticalRuler = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnHorizontalLine = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnVerticalLine = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnRectangle = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnOpen = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnSave = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnRotateClockwise = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnSelect = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnMove = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    m_btnFill = new wxButton(m_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

    m_btnHorizontalRuler->SetBitmap(bmpHR);
    m_btnVerticalRuler->SetBitmap(bmpVR);
    m_btnHorizontalLine->SetBitmap(bmpHL);
    m_btnVerticalLine->SetBitmap(bmpVL);
    m_btnRectangle->SetBitmap(bmpRect);
    m_btnOpen->SetBitmap(bmpOpen);
    m_btnSave->SetBitmap(bmpSave);
    m_btnRotateClockwise->SetBitmap(bmpRotateClockwise);
    m_btnSelect->SetBitmap(bmpSelectRectangle);
    m_btnMove->SetBitmap(bmpMove);
    m_btnFill->SetBitmap(bmpFill);

    // Set tooltips
    m_btnHorizontalRuler->SetToolTip("Add Horizontal Ruler (H)");
    m_btnVerticalRuler->SetToolTip("Add Vertical Ruler (V)");
    m_btnHorizontalLine->SetToolTip("Draw Horizontal Line");
    m_btnVerticalLine->SetToolTip("Draw Vertical Line");
    m_btnRectangle->SetToolTip("Draw Rectangle");
    m_btnOpen->SetToolTip("Open");
    m_btnSave->SetToolTip("Save");
    m_btnRotateClockwise->SetToolTip("Rotate Clockwise");
    m_btnSelect->SetToolTip("Select Rectangle");
    m_btnMove->SetToolTip("Move");
    m_btnFill->SetToolTip("Fill");

    // Set the default selection to Medium (32x32)
    rb_iconSize->SetSelection(1);
    m_iconCanva->SetIconSize(32);
    m_iconPreview->SetIconData(32, m_iconCanva->GetIconPixels());
    m_iconPreview->SetScale(2); // Set initial scale to 2x

    // Bind event handlers
    rb_iconSize->Bind(wxEVT_RADIOBOX, &MainFrame::OnIconSizeChanged, this);
    m_panel->Bind(EVT_CELL_HOVERED, &MainFrame::OnCellHovered, this);
    m_panel->Bind(EVT_ICON_CHANGED, &MainFrame::OnIconChanged, this);
    m_scaleSlider->Bind(wxEVT_SLIDER, &MainFrame::OnScaleChanged, this);
    m_btnHorizontalRuler->Bind(wxEVT_BUTTON, &MainFrame::OnHorizontalRulerClick, this);
    m_btnVerticalRuler->Bind(wxEVT_BUTTON, &MainFrame::OnVerticalRulerClick, this);
    m_btnHorizontalLine->Bind(wxEVT_BUTTON, &MainFrame::OnHorizontalLineClick, this);
    m_btnVerticalLine->Bind(wxEVT_BUTTON, &MainFrame::OnVerticalLineClick, this);
    m_btnRectangle->Bind(wxEVT_BUTTON, &MainFrame::OnRectangleClick, this);
    m_btnOpen->Bind(wxEVT_BUTTON, &MainFrame::OnOpenClick, this);
    m_btnSave->Bind(wxEVT_BUTTON, &MainFrame::OnSaveClick, this);
    m_btnRotateClockwise->Bind(wxEVT_BUTTON, &MainFrame::OnRotateClockwiseClick, this);
    m_btnSelect->Bind(wxEVT_BUTTON, &MainFrame::OnSelectRectangleClick, this);
    m_btnMove->Bind(wxEVT_BUTTON, &MainFrame::OnMoveClick, this);
    m_btnFill->Bind(wxEVT_BUTTON, &MainFrame::OnFillClick, this);

    SetLayout();

    // Create a sizer for the Frame itself to manage the panel
    auto* frame_sizer = new wxBoxSizer(wxVERTICAL);
    frame_sizer->Add(m_panel, 1, wxEXPAND);
    this->SetSizer(frame_sizer);

    // This tells the window to respect the minimum size of the sizer
    frame_sizer->SetSizeHints(this);

    wxFrameBase::CreateStatusBar();
    this->SetClientSize(this->FromDIP(wxSize(1024, 768)));
}

void MainFrame::SetLayout()
{
    auto* main_sizer = new wxBoxSizer(wxHORIZONTAL);
    auto* toolbox_sizer = new wxBoxSizer(wxVERTICAL);

    // Icon size selection
    toolbox_sizer->Add(rb_iconSize, 0, wxEXPAND);

    // File operations toolbar
    auto* file_toolbar_sizer = new wxBoxSizer(wxHORIZONTAL);
    file_toolbar_sizer->Add(m_btnOpen, 0, wxALL, this->FromDIP(2));
    file_toolbar_sizer->Add(m_btnSave, 0, wxALL, this->FromDIP(2));
    toolbox_sizer->Add(file_toolbar_sizer, 0, wxEXPAND | wxTOP, this->FromDIP(8));

    // Drawing tools toolbar
    auto* toolbar_sizer = new wxBoxSizer(wxHORIZONTAL);
    toolbar_sizer->Add(m_btnHorizontalRuler, 0, wxALL, this->FromDIP(2));
    toolbar_sizer->Add(m_btnVerticalRuler, 0, wxALL, this->FromDIP(2));
    toolbar_sizer->AddSpacer(this->FromDIP(8));
    toolbar_sizer->Add(m_btnHorizontalLine, 0, wxALL, this->FromDIP(2));
    toolbar_sizer->Add(m_btnVerticalLine, 0, wxALL, this->FromDIP(2));
    toolbar_sizer->AddSpacer(this->FromDIP(8));
    toolbar_sizer->Add(m_btnRectangle, 0, wxALL, this->FromDIP(2));
    toolbox_sizer->Add(toolbar_sizer, 0, wxEXPAND | wxTOP, this->FromDIP(8));
    toolbar_sizer->AddStretchSpacer(this->FromDIP(8));

    auto* additional_operations_sizer = new wxBoxSizer(wxHORIZONTAL);
    additional_operations_sizer->Add(m_btnRotateClockwise, 0, wxALL, this->FromDIP(2));
    additional_operations_sizer->Add(m_btnSelect, 0, wxALL, this->FromDIP(2));
    additional_operations_sizer->Add(m_btnMove, 0, wxALL, this->FromDIP(2));
    additional_operations_sizer->Add(m_btnFill, 0, wxALL, this->FromDIP(2));
    toolbox_sizer->Add(additional_operations_sizer, 0, wxEXPAND | wxTOP, this->FromDIP(8));

    // Stretch spacer to push preview to bottom
    toolbox_sizer->AddStretchSpacer(1);

    // Preview and scale slider at bottom
    toolbox_sizer->Add(m_iconPreview, 0, wxALIGN_CENTER | wxTOP, this->FromDIP(12));
    toolbox_sizer->Add(m_scaleSlider, 0, wxEXPAND | wxTOP, this->FromDIP(12));

    main_sizer->Add(m_iconCanva, 1, wxEXPAND);
    main_sizer->Add(toolbox_sizer, 0, wxEXPAND | wxALL, 12);

    m_panel->SetSizer(main_sizer);
    main_sizer->SetSizeHints(m_panel);
}

void MainFrame::OnIconSizeChanged(wxCommandEvent& event)
{
    int selection = rb_iconSize->GetSelection();
    int size;

    switch (selection)
    {
        case 0: size = 16; break;
        case 1: size = 32; break;
        case 2: size = 64; break;
        default: size = 32; break;
    }

    m_iconCanva->SetIconSize(size);
    m_iconPreview->SetIconData(size, m_iconCanva->GetIconPixels());
}

void MainFrame::OnCellHovered(wxCommandEvent& event)
{
    if (wxString cellPos = event.GetString(); cellPos.IsEmpty())
    {
        SetStatusText("");
    }
    else
    {
        SetStatusText("Cell: " + cellPos);
    }
}

void MainFrame::OnIconChanged(wxCommandEvent& event)
{
    // Update the preview with the current icon data
    m_iconPreview->SetIconData(m_iconCanva->GetIconSize(), m_iconCanva->GetIconPixels());
}

void MainFrame::OnScaleChanged(wxCommandEvent& event)
{
    int scale = m_scaleSlider->GetValue();
    m_iconPreview->SetScale(scale);
}

void MainFrame::OnHorizontalRulerClick(wxCommandEvent& event)
{
    // Simulate pressing 'H' key on the IconCanva
    wxKeyEvent keyEvent(wxEVT_KEY_UP);
    keyEvent.m_keyCode = 'H';
    m_iconCanva->GetEventHandler()->ProcessEvent(keyEvent);
    m_iconCanva->SetFocus();
}

void MainFrame::OnVerticalRulerClick(wxCommandEvent& event)
{
    // Simulate pressing 'V' key on the IconCanva
    wxKeyEvent keyEvent(wxEVT_KEY_UP);
    keyEvent.m_keyCode = 'V';
    m_iconCanva->GetEventHandler()->ProcessEvent(keyEvent);
    m_iconCanva->SetFocus();
}

void MainFrame::OnHorizontalLineClick(wxCommandEvent& event)
{
    m_iconCanva->SetLineDrawingMode(true, true); // horizontal mode
    m_iconCanva->SetFocus();
}

void MainFrame::OnVerticalLineClick(wxCommandEvent& event)
{
    m_iconCanva->SetLineDrawingMode(true, false); // vertical mode
    m_iconCanva->SetFocus();
}

void MainFrame::OnRectangleClick(wxCommandEvent& event)
{
    m_iconCanva->SetRectangleDrawingMode(true);
    m_iconCanva->SetFocus();
}

void MainFrame::OnSelectRectangleClick(wxCommandEvent& event)
{
    m_iconCanva->SetSelectionMode(true);
    m_iconCanva->SetFocus();
}

void MainFrame::OnMoveClick(wxCommandEvent& event)
{
    m_iconCanva->SetMoveMode(true);
    m_iconCanva->SetFocus();
}

void MainFrame::OnFillClick(wxCommandEvent& event)
{
    m_iconCanva->SetFillMode(true);
    m_iconCanva->SetFocus();
}

void MainFrame::OnRotateClockwiseClick(wxCommandEvent& event)
{
    m_iconCanva->RotateClockwise();
    m_iconCanva->SetFocus();
}

void MainFrame::OnOpenClick(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open Icon", "", "",
                                "C++ Header files (*.h)|*.h|C++ Source files (*.cpp)|*.cpp",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString path = openFileDialog.GetPath();

    // Read file content
    wxFile file(path, wxFile::read);
    if (!file.IsOpened())
    {
        wxMessageBox("Failed to open file", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString content;
    file.ReadAll(&content);
    file.Close();

    // Parse the file to extract icon data
    // Look for array declaration pattern: "static const unsigned char xxx_data[] = {"
    int arrayStart = content.Find("_data[] = {");
    if (arrayStart == wxNOT_FOUND)
    {
        wxMessageBox("Invalid file format: Could not find icon data array", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Extract icon size from comment "// Icon size: WxH"
    int sizeCommentPos = content.Find("// Icon size: ");
    if (sizeCommentPos == wxNOT_FOUND)
    {
        wxMessageBox("Invalid file format: Could not find icon size", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Parse size
    wxString sizeStr = content.Mid(sizeCommentPos + 14); // Skip "// Icon size: "
    sizeStr = sizeStr.BeforeFirst('\n');
    long width, height;
    if (!sizeStr.BeforeFirst('x').ToLong(&width) || !sizeStr.AfterFirst('x').ToLong(&height))
    {
        wxMessageBox("Invalid file format: Could not parse icon size", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (width != height || (width != 16 && width != 32 && width != 64))
    {
        wxMessageBox("Unsupported icon size. Only 16x16, 32x32, and 64x64 are supported.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    int iconSize = width;

    // Extract array content between { and }
    int braceStart = content.Find('{', arrayStart);
    int braceEnd = content.Find('}', braceStart);
    if (braceStart == wxNOT_FOUND || braceEnd == wxNOT_FOUND)
    {
        wxMessageBox("Invalid file format: Could not find array data", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString arrayData = content.Mid(braceStart + 1, braceEnd - braceStart - 1);

    // Parse hex values
    wxArrayString hexValues;
    wxString current;
    for (size_t i = 0; i < arrayData.length(); i++)
    {
        wxChar ch = arrayData[i];
        if (ch == '0' && i + 1 < arrayData.length() && (arrayData[i + 1] == 'x' || arrayData[i + 1] == 'X'))
        {
            current = "0x";
            i++; // Skip 'x'
        }
        else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
        {
            current += ch;
        }
        else if (!current.IsEmpty())
        {
            hexValues.Add(current);
            current.clear();
        }
    }
    if (!current.IsEmpty())
        hexValues.Add(current);

    // Calculate expected bytes
    int bytesPerRow = (iconSize + 7) / 8;
    int expectedBytes = iconSize * bytesPerRow;

    if (hexValues.Count() != expectedBytes)
    {
        wxMessageBox(wxString::Format("Invalid data: Expected %d bytes, found %d", expectedBytes, hexValues.Count()),
                     "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Set icon size
    m_iconCanva->SetIconSize(iconSize);

    // Update radio box selection
    int selection = (iconSize == 16) ? 0 : (iconSize == 32) ? 1 : 2;
    rb_iconSize->SetSelection(selection);

    // Unpack bits and set pixels
    std::vector<std::vector<bool>> pixels(iconSize, std::vector<bool>(iconSize, false));
    int byteIdx = 0;

    for (int row = 0; row < iconSize; row++)
    {
        for (int byteInRow = 0; byteInRow < bytesPerRow; byteInRow++)
        {
            unsigned long byteValue;
            if (!hexValues[byteIdx].ToULong(&byteValue, 16))
            {
                wxMessageBox("Invalid hex value in data", "Error", wxOK | wxICON_ERROR);
                return;
            }

            for (int bit = 0; bit < 8; bit++)
            {
                int col = byteInRow * 8 + bit;
                if (col < iconSize)
                {
                    pixels[row][col] = (byteValue & (1 << (7 - bit))) != 0;
                }
            }

            byteIdx++;
        }
    }

    // Apply pixels to canvas
    m_iconCanva->SetIconPixels(pixels);
    m_iconPreview->SetIconData(iconSize, m_iconCanva->GetIconPixels());

    wxMessageBox("Icon loaded successfully", "Success", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSaveClick(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, "Save Icon", "", "",
                                "C++ Header files (*.h)|*.h|C++ Source files (*.cpp)|*.cpp",
                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString path = saveFileDialog.GetPath();
    wxString filename = saveFileDialog.GetFilename();

    // Extract variable name from filename (remove extension)
    wxString varName = filename.BeforeLast('.');
    // Replace invalid characters with underscores
    varName.Replace("-", "_");
    varName.Replace(" ", "_");
    varName.Replace(".", "_");

    // Get icon data
    int iconSize = m_iconCanva->GetIconSize();
    const auto& iconPixels = m_iconCanva->GetIconPixels();

    // Build C/C++ compatible array content with bit packing
    // Pack 8 pixels per byte for memory efficiency
    int bytesPerRow = (iconSize + 7) / 8; // Round up to nearest byte
    int totalBytes = iconSize * bytesPerRow;

    wxString content = "// Generated icon data\n";
    content += "// Icon size: " + wxString::Format("%dx%d", iconSize, iconSize) + "\n";
    content += "// Data format: Each byte contains 8 pixels (1 bit per pixel, MSB first)\n";
    content += "// Total bytes: " + wxString::Format("%d", totalBytes) + "\n\n";
    content += "static const unsigned char " + varName + "_data[] = {\n";

    for (int row = 0; row < iconSize; row++)
    {
        content += "    ";
        for (int byteIdx = 0; byteIdx < bytesPerRow; byteIdx++)
        {
            unsigned char byte = 0;
            for (int bit = 0; bit < 8; bit++)
            {
                int col = byteIdx * 8 + bit;
                if (col < iconSize && iconPixels[row][col])
                {
                    byte |= (1 << (7 - bit)); // MSB first
                }
            }
            content += wxString::Format("0x%02x", byte);

            if (row < iconSize - 1 || byteIdx < bytesPerRow - 1)
                content += ", ";
        }
        content += "\n";
    }
    content += "};\n";

    // Write to file
    wxFile file(path, wxFile::write);
    if (!file.IsOpened())
    {
        wxMessageBox("Failed to save file", "Error", wxOK | wxICON_ERROR);
        return;
    }

    file.Write(content);
    file.Close();

    wxMessageBox("Icon saved successfully", "Success", wxOK | wxICON_INFORMATION);
}
