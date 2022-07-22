#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>
#include <wx/numdlg.h>
#include <wx/valnum.h>
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

    void OnSetPixel(wxCommandEvent &event);

    void OnApplyKernel(wxCommandEvent &event);

    void OnGetGrayscale(wxCommandEvent &event);

    void EnableImageOptions(bool enable = true);

    void PaintImage(wxPaintEvent &evt);


    wxMenu *fileMenu;
    wxMenu *imageMenu;
    wxPanel *imagePanel;
    std::shared_ptr<EightBitAbstractImage> image;
DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(ImageProcessingFrame, wxFrame)
                EVT_PAINT(ImageProcessingFrame::PaintImage)
END_EVENT_TABLE()

enum {
    ID_SET_PIXEL = 1,
    ID_APPLY_KERNEL,
    ID_GET_GRAYSCALE,
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
    Bind(wxEVT_MENU, &ImageProcessingFrame::OnSetPixel, this, ID_SET_PIXEL);
    Bind(wxEVT_MENU, &ImageProcessingFrame::OnApplyKernel, this, ID_APPLY_KERNEL);
    Bind(wxEVT_MENU, &ImageProcessingFrame::OnGetGrayscale, this, ID_GET_GRAYSCALE);
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
            image = PXXManager::readPGM(imagePath.ToStdString());
        } else if (imagePath.EndsWith(".pgm") && chosen == 1) {
            image = PXXManager::readPGM(imagePath.ToStdString(), alphaPath.ToStdString());
        } else if (imagePath.EndsWith(".ppm") && chosen == 0) {
            image = PXXManager::readPPM(imagePath.ToStdString());
        } else if (imagePath.EndsWith(".ppm") && chosen == 1) {
            image = PXXManager::readPPM(imagePath.ToStdString(), alphaPath.ToStdString());
        }
    } catch (const PXXManager::PGMReadException &e) {
        wxMessageBox(e.what(), "Error", wxICON_ERROR);
    } catch (const PXXManager::PPMReadException &e) {
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
            image = std::make_unique<GImage>(width, height);
            break;
        case 2:
            image = std::make_unique<GAImage>(width, height);
            break;
        case 3:
            image = std::make_unique<RGBImage>(width, height);
            break;
        case 4:
            image = std::make_unique<RGBAImage>(width, height);
    }
    EnableImageOptions();
}

void ImageProcessingFrame::OnSave(wxCommandEvent &event) {
    if (auto i = std::dynamic_pointer_cast<GImage>(image); i) {
        wxFileDialog
                imageFileDialog(this, "Save PGM file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (imageFileDialog.ShowModal() == wxID_CANCEL) return;
        PXXManager::writePGM(imageFileDialog.GetPath().ToStdString(), *i);
    } else if (auto i = std::dynamic_pointer_cast<GAImage>(image); i) {
        wxFileDialog
                imageFileDialog(this, "Save PGM file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (imageFileDialog.ShowModal() == wxID_CANCEL) return;
        wxFileDialog
                alphaFileDialog(this, "Save PGM alpha file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (alphaFileDialog.ShowModal() == wxID_CANCEL) return;
        PXXManager::writePGM(imageFileDialog.GetPath().ToStdString(), alphaFileDialog.GetPath().ToStdString(), *i);
    } else if (auto i = std::dynamic_pointer_cast<RGBImage>(image); i) {
        wxFileDialog
                imageFileDialog(this, "Save PPM file", "", "",
                                "PPM files (*.ppm)|*.ppm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (imageFileDialog.ShowModal() == wxID_CANCEL) return;
        PXXManager::writePPM(imageFileDialog.GetPath().ToStdString(), *i);
    } else if (auto i = std::dynamic_pointer_cast<RGBAImage>(image); i) {
        wxFileDialog
                imageFileDialog(this, "Save PPM file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (imageFileDialog.ShowModal() == wxID_CANCEL) return;
        wxFileDialog
                alphaFileDialog(this, "Save PGM alpha file", "", "",
                                "PGM files (*.pgm)|*.pgm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (alphaFileDialog.ShowModal() == wxID_CANCEL) return;
        PXXManager::writePPM(imageFileDialog.GetPath().ToStdString(), alphaFileDialog.GetPath().ToStdString(), *i);
    }
}

void ImageProcessingFrame::EnableImageOptions(bool enable) {
    fileMenu->Enable(wxID_SAVE, enable);
    imageMenu->Enable(ID_SET_PIXEL, enable);
    imageMenu->Enable(ID_APPLY_KERNEL, enable);
    imageMenu->Enable(ID_GET_GRAYSCALE, enable);
    if (image) SetStatusText("Image loaded. Width: " + std::to_string(image->getWidth()) + "px, Height: " +
                             std::to_string(image->getHeight()) + "px");
}

void ImageProcessingFrame::PaintImage(wxPaintEvent &evt) {
    wxPaintDC dc(this);
    if (image) {
        wxImage bitmap(image->getWidth(), image->getHeight());
        if (auto i = std::dynamic_pointer_cast<GImage>(image); i) {
            for (int y = 0; y < image->getHeight(); y++)
                for (int x = 0; x < image->getWidth(); x++) {
                    unsigned char val = i->get(x, y, 0);
                    bitmap.SetRGB(x, y, val, val, val);
                }
        } else if (auto i = std::dynamic_pointer_cast<GAImage>(image); i) {
            bitmap.InitAlpha();
            for (int y = 0; y < image->getHeight(); y++)
                for (int x = 0; x < image->getWidth(); x++) {
                    unsigned char val = i->get(x, y, 0);
                    bitmap.SetRGB(x, y, val, val, val);
                    bitmap.SetAlpha(x, y, i->get(x, y, 1));
                }
        } else if (auto i = std::dynamic_pointer_cast<RGBImage>(image); i) {
            for (int y = 0; y < image->getHeight(); y++)
                for (int x = 0; x < image->getWidth(); x++) {
                    unsigned char r = i->get(x, y, 0);
                    unsigned char g = i->get(x, y, 1);
                    unsigned char b = i->get(x, y, 2);
                    bitmap.SetRGB(x, y, r, g, b);
                }
        } else if (auto i = std::dynamic_pointer_cast<RGBAImage>(image); i) {
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

void ImageProcessingFrame::OnSetPixel(wxCommandEvent &event) {
    int channels;
    if (auto i = std::dynamic_pointer_cast<GImage>(image); i) channels = 1;
    else if (auto i = std::dynamic_pointer_cast<GAImage>(image); i) channels = 2;
    else if (auto i = std::dynamic_pointer_cast<RGBImage>(image); i) channels = 3;
    else if (auto i = std::dynamic_pointer_cast<RGBAImage>(image); i) channels = 4;
    int x = wxGetNumberFromUser("Select X coordinate", "X", "Edit Image", 1, 1, 100000, this);
    if (x == -1) return;
    int y = wxGetNumberFromUser("Select Y coordinate", "Y", "Edit Image", 1, 1, 100000, this);
    if (y == -1) return;
    int c = wxGetNumberFromUser("Select channel", "Channels", "Edit Image", 1, 1, channels, this);
    if (c == -1) return;
    image->set(x, y, c, wxGetNumberFromUser("Set value", "value", "Edit Image", 0, 0, 256, this));
}

void ImageProcessingFrame::OnApplyKernel(wxCommandEvent &event) {
    const wxString choices[] = {"Identity Kernel", "Edge Kernel 1", "Edge Kernel 2", "Edge Kernel 3", "Sharpen Kernel",
                                "Gaussian Blur Kernel"};
    int chosen = wxGetSingleChoiceIndex("Select kernel", "Apply kernel", 6, choices, 0);
    switch (chosen) {
        case 0:
            image->applyKernel(IdentityKernel());
            break;
        case 1:
            image->applyKernel(Edge0Kernel());
            break;
        case 2:
            image->applyKernel(Edge1Kernel());
            break;
        case 3:
            image->applyKernel(Edge2Kernel());
            break;
        case 4:
            image->applyKernel(SharpenKernel());
            break;
        case 5:
            float stdDev;
            int dimension;
            try {
                stdDev = stof(wxGetTextFromUser("Insert kernel standard deviation", "Define Gauss Blur Kernel",
                                                "1.00").ToStdString());
            } catch (...) {
                wxMessageBox("Cannot parse standard deviation", "Error", wxICON_ERROR);
                return;
            }
            dimension = wxGetNumberFromUser("Insert kernel dimension", "Dimension", "Define Gauss Blur Kernel", 3, 1,
                                            999999);
            if (dimension == -1) return;
            try {
                GaussBlurKernel kernel(stdDev, dimension);
                image->applyKernel(kernel);
            } catch (const WrongSizeKernelException &e) {
                wxMessageBox(e.what(), "Error while creating kernel", wxICON_ERROR);
            }
    }

}

void ImageProcessingFrame::OnGetGrayscale(wxCommandEvent &event) {
    if (auto i = std::dynamic_pointer_cast<GAImage>(image); i) {
        image = i->getGrayScale();
    } else if (auto i = std::dynamic_pointer_cast<RGBImage>(image); i) {
        image = i->getGrayScale();
    } else if (auto i = std::dynamic_pointer_cast<RGBAImage>(image); i) {
        image = i->getGrayScale();
    }
    wxWindow::Refresh();
}
