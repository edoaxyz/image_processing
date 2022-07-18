#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>
#include <wx/numdlg.h>
#include <memory>
#include "Image.h"
#include "PGMManager.h"
#include "PPMManager.h"

#endif

class ImageProcessingApp : public wxApp {
public:
    virtual bool OnInit();
};

class ImageProcessingFrame : public wxFrame {
public:
    ImageProcessingFrame();

private:
    void OnOpen(wxCommandEvent &event);

    void OnNew(wxCommandEvent &event);

    void OnSave(wxCommandEvent &event);

    void EnableImageOptions(bool enable = true);

    void PaintImage(wxPaintEvent &evt);

    wxMenu *fileMenu;
    wxMenu *imageMenu;
    wxPanel *imagePanel;
    std::shared_ptr<AbstractImage<unsigned char>> image;
DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(ImageProcessingFrame, wxFrame)
                EVT_PAINT(ImageProcessingFrame::PaintImage)
END_EVENT_TABLE()

enum {
    ID_SET_PIXEL = 1,
    ID_APPLY_KERNEL = 2,
    ID_GET_GRAYSCALE = 3
};

wxIMPLEMENT_APP(ImageProcessingApp);

bool ImageProcessingApp::OnInit() {
    wxInitAllImageHandlers();
    auto frame = new ImageProcessingFrame();
    frame->Show(true);
    return true;
}

ImageProcessingFrame::ImageProcessingFrame()
        : wxFrame(NULL, wxID_ANY, "Image Processing") {
    fileMenu = new wxMenu;
    fileMenu->Append(wxID_OPEN, "Open PGM/PPM file");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_NEW, "New PGM/PPM file");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_SAVE, "Save PGM/PPM file");

    imageMenu = new wxMenu;
    imageMenu->Append(ID_SET_PIXEL, "Set pixel value");
    imageMenu->AppendSeparator();
    imageMenu->Append(ID_APPLY_KERNEL, "Apply kernel");
    imageMenu->AppendSeparator();
    imageMenu->Append(ID_GET_GRAYSCALE, "Get grayscale");

    EnableImageOptions(false);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "File");
    menuBar->Append(imageMenu, "Image");
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Please open a PGM/PPM file or create a new one");

    Bind(wxEVT_MENU, &ImageProcessingFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &ImageProcessingFrame::OnNew, this, wxID_NEW);
    Bind(wxEVT_MENU, &ImageProcessingFrame::OnSave, this, wxID_SAVE);
}

void ImageProcessingFrame::OnOpen(wxCommandEvent &event) {
    const wxString choices[] = {wxT("without alpha layer"), wxT("with alpha layer")};
    int chosen = wxGetSingleChoiceIndex("Open image", "Open image...", 2, choices, 0);

    wxString imagePath, alphaPath;
    wxFileDialog
            imageDialog(this, "Open PGM/PPM", "", "",
                        "PGM/PPM files (*.pgm/*.ppm)|*.pgm;*.ppm", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (imageDialog.ShowModal() != wxID_OK) return;
    imagePath = imageDialog.GetPath();
    if (chosen == 1) {
        wxFileDialog
                alphaDialog(this, "Select alpha layer", "", "",
                            "PGM files (*.pgm)|*.pgm", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        if (alphaDialog.ShowModal() != wxID_OK) return;
        alphaPath = alphaDialog.GetPath();
    }
    try {
        if (imagePath.EndsWith(".pgm") && chosen == 0) {
            image = PGMManager::readPGM(imagePath.ToStdString());
        } else if (imagePath.EndsWith(".pgm") && chosen == 1) {
            image = PGMManager::readPGM(imagePath.ToStdString(), alphaPath.ToStdString());
        } else if (imagePath.EndsWith(".ppm") && chosen == 0) {
            image = PPMManager::readPPM(imagePath.ToStdString());
        } else if (imagePath.EndsWith(".ppm") && chosen == 1) {
            image = PPMManager::readPPM(imagePath.ToStdString(), alphaPath.ToStdString());
        }
    } catch (PGMReadException &e) {
        wxMessageBox(e.what(), "Error", wxICON_ERROR);
    } catch (PPMReadException &e) {
        wxMessageBox(e.what(), "Error", wxICON_ERROR);
    }
    EnableImageOptions(true);
}

void ImageProcessingFrame::OnNew(wxCommandEvent &event) {
    int width = wxGetNumberFromUser("Insert width", "Width", "New Image", 1, 1, 100000, this);
    if (width == -1) return;
    int height = wxGetNumberFromUser("Insert height", "Height", "New Image", 1, 1, 100000, this);
    if (height == -1) return;
    int channels = wxGetNumberFromUser("Insert channels number", "Channels", "New Image", 1, 1, 4, this);
    if (channels == -1) return;
    switch (channels) {
        case 1:
            image = std::make_unique<Image<1>>(width, height);
            break;
        case 2:
            image = std::make_unique<Image<2>>(width, height);
            break;
        case 3:
            image = std::make_unique<Image<3>>(width, height);
            break;
        case 4:
            image = std::make_unique<Image<4>>(width, height);
    }
    EnableImageOptions();
}

void ImageProcessingFrame::OnSave(wxCommandEvent &event) {
    if (auto i = std::dynamic_pointer_cast<Image<1>>(image); i) {
        wxFileDialog
                imageFileDialog(this, "Save PGM file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (imageFileDialog.ShowModal() == wxID_CANCEL) return;
        PGMManager::writePGM(imageFileDialog.GetPath().ToStdString(), *i);
    } else if (auto i = std::dynamic_pointer_cast<Image<2>>(image); i) {
        wxFileDialog
                imageFileDialog(this, "Save PGM file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (imageFileDialog.ShowModal() == wxID_CANCEL) return;
        wxFileDialog
                alphaFileDialog(this, "Save PGM alpha file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (alphaFileDialog.ShowModal() == wxID_CANCEL) return;
        PGMManager::writePGM(imageFileDialog.GetPath().ToStdString(), alphaFileDialog.GetPath().ToStdString(), *i);
    } else if (auto i = std::dynamic_pointer_cast<Image<3>>(image); i) {
        wxFileDialog
                imageFileDialog(this, "Save PPM file", "", "",
                                "PPM files (*.ppm)|*.ppm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (imageFileDialog.ShowModal() == wxID_CANCEL) return;
        PPMManager::writePPM(imageFileDialog.GetPath().ToStdString(), *i);
    } else if (auto i = std::dynamic_pointer_cast<Image<4>>(image); i) {
        wxFileDialog
                imageFileDialog(this, "Save PPM file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (imageFileDialog.ShowModal() == wxID_CANCEL) return;
        wxFileDialog
                alphaFileDialog(this, "Save PGM alpha file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (alphaFileDialog.ShowModal() == wxID_CANCEL) return;
        PPMManager::writePPM(imageFileDialog.GetPath().ToStdString(), alphaFileDialog.GetPath().ToStdString(), *i);
    }
}

void ImageProcessingFrame::EnableImageOptions(bool enable) {
    fileMenu->Enable(wxID_SAVE, enable);
    imageMenu->Enable(ID_SET_PIXEL, enable);
    imageMenu->Enable(ID_APPLY_KERNEL, enable);
    imageMenu->Enable(ID_GET_GRAYSCALE, enable);
}

void ImageProcessingFrame::PaintImage(wxPaintEvent &evt) {
    wxPaintDC dc(this);
    if (image) {
        wxImage bitmap(image->getWidth(), image->getHeight());
        if (auto i = std::dynamic_pointer_cast<Image<1>>(image); i) {
            for (int y = 0; y < image->getHeight(); y++)
                for (int x = 0; x < image->getWidth(); x++) {
                    unsigned char val = i->get(x, y, 0);
                    bitmap.SetRGB(x, y, val, val, val);
                }
        } else if (auto i = std::dynamic_pointer_cast<Image<2>>(image); i) {
            bitmap.InitAlpha();
            for (int y = 0; y < image->getHeight(); y++)
                for (int x = 0; x < image->getWidth(); x++) {
                    unsigned char val = i->get(x, y, 0);
                    bitmap.SetRGB(x, y, val, val, val);
                    bitmap.SetAlpha(x, y, i->get(x, y, 1));
                }
        } else if (auto i = std::dynamic_pointer_cast<Image<3>>(image); i) {
            for (int y = 0; y < image->getHeight(); y++)
                for (int x = 0; x < image->getWidth(); x++) {
                    unsigned char r = i->get(x, y, 0);
                    unsigned char g = i->get(x, y, 1);
                    unsigned char b = i->get(x, y, 2);
                    bitmap.SetRGB(x, y, r, g, b);
                }
        } else if (auto i = std::dynamic_pointer_cast<Image<4>>(image); i) {
            bitmap.InitAlpha();
            for (int y = 0; y < image->getHeight(); y++)
                for (int x = 0; x < image->getWidth(); x++) {
                    unsigned char r = i->get(x, y, 0);
                    unsigned char g = i->get(x, y, 1);
                    unsigned char b = i->get(x, y, 2);
                    bitmap.SetRGB(x, y, r, g, b);
                    bitmap.SetAlpha(x, y, i->get(x, y, 3));
                }
        }
        dc.DrawBitmap(bitmap, 0, 0, false);
    }
}
