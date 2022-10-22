#include "MainWindow.hpp"

#include <Core/Image/ColorModel/ColorModelConverter.hpp>
#include <Core/Image/Image.hpp>
#include <Core/Log.hpp>
#include <Window/Dialogs/NewImageDialog.hpp>

#include <QFileDialog>
#include <QPixmap>
#include <QtWidgets>

#include <string>

std::string NEW_IMAGE_DEFAULT_NAME = "Untitled";
ColorModel DEFAULT_COLOR_MODEL = ColorModel::RGB;

MainWindow::MainWindow(const WindowSettings& settings)
    : m_BaseTitle(settings.Title)
    , m_SelectedColorModel(DEFAULT_COLOR_MODEL)
{
    InitWindow(settings);

    InitMenuBar();
    InitImageView();
    InitImageFileFilters();
}

void MainWindow::InitWindow(const WindowSettings& settings)
{
    setWindowTitle(m_BaseTitle.c_str());

    resize(settings.Width, settings.Height);
    if (settings.IsMaximized) {
        setWindowState(Qt::WindowMaximized);
    }
}

void MainWindow::InitMenuBar()
{
    auto* menu = menuBar();

    auto* fileMenu = menu->addMenu("File");
    {
        auto* newAction = fileMenu->addAction("New");
        newAction->setShortcut(QKeySequence::New);
        connect(newAction, &QAction::triggered, this, &MainWindow::OnFileNewAction);

        auto* openAction = fileMenu->addAction("Open");
        openAction->setShortcut(QKeySequence::Open);
        connect(openAction, &QAction::triggered, this, &MainWindow::OnFileOpenAction);

        m_SaveAction = fileMenu->addAction("Save");
        m_SaveAction->setShortcut(QKeySequence::Save);
        m_SaveAction->setEnabled(false);
        connect(m_SaveAction, &QAction::triggered, this, &MainWindow::OnFileSaveAction);

        m_SaveAsAction = fileMenu->addAction("Save As");
        m_SaveAsAction->setShortcut(QKeySequence::SaveAs);
        m_SaveAsAction->setEnabled(false);
        connect(m_SaveAsAction, &QAction::triggered, this, &MainWindow::OnFileSaveAsAction);
    }

    auto* imageMenu = menu->addMenu("Image");
    {
        auto* colorModelMenu = imageMenu->addMenu("Color Model");
        {
            auto* colorModelActionGroup = new QActionGroup(this);
            colorModelActionGroup->setExclusive(true);

            for (const auto& [colorModelEnum, colorModelStr] : ENUM_TO_STRING_COLOR_MODEL_MAPPING) {
                auto* modelAction = colorModelMenu->addAction(colorModelStr.c_str());
                modelAction->setCheckable(true);
                colorModelActionGroup->addAction(modelAction);

                if (colorModelEnum == DEFAULT_COLOR_MODEL) {
                    modelAction->setChecked(true);
                    m_DefaultColorModelAction = modelAction;
                }

                connect(modelAction, &QAction::toggled, this, [this, model = colorModelEnum](bool checked) {
                    if (checked) {
                        OnImageColorModelActionSelected(model);
                    }
                });
            }
        }
    }
}

void MainWindow::InitImageView()
{
    m_ImageView = std::make_unique<ImageViewWithInfo>(this);
    setCentralWidget(m_ImageView.get());

    UpdateColorModelText(m_SelectedColorModel);
}

void MainWindow::InitImageFileFilters()
{
    m_ImageFileFilters.clear();
    bool isFirstValue = true;

    for (const auto& [filter, _] : FILTER_TO_IMAGE_FORMAT_INFO_MAPPING) {
        if (!isFirstValue) {
            m_ImageFileFilters += ";;";
        }
        isFirstValue = false;

        m_ImageFileFilters += filter.c_str();
    }
}

void MainWindow::OnFileNewAction()
{
    NewImageDialog dialog;
    if (!dialog.Exec()) {
        return;
    }

    int width = dialog.GetWidth();
    int height = dialog.GetHeight();
    Log::Debug("OnFileNewAction: New image width: {}, height: {}", width, height);

    m_SelectedColorModel = DEFAULT_COLOR_MODEL;
    m_DefaultColorModelAction->setChecked(true);

    SetImage(std::make_unique<Image>(width, height));
    SetImagePath(std::string());
}

void MainWindow::OnFileOpenAction()
{
    QFileDialog dialog(this);
    dialog.setViewMode(QFileDialog::List);
    if (!dialog.exec()) {
        return;
    }

    auto filename = dialog.selectedFiles()[0];

    if (filename.isEmpty()) {
        return;
    }

    Log::Debug("OnFileOpenAction: Trying to read image from file: {}", filename.toStdString().c_str());

    LoadedImageData imageData;
    try {
        imageData = Image::FromFile(filename.toStdString());
    } catch (const std::exception& exception) {
        Log::Error("OnFileOpenAction: An error occured while reading image from file: {}", exception.what());
        return;
    }

    Log::Debug("OnFileOpenAction: successufully loaded image");
    std::unique_ptr<Image> loadedImage = std::move(imageData.LoadedImage);

    if (m_SelectedColorModel != ColorModel::RGB && imageData.Format != ImageFormat::Ppm) {
        Log::Debug("OnFileOpenAction: need to convert image to current color space");
        loadedImage = ConvertImageToNewModel(*loadedImage, ColorModel::RGB, m_SelectedColorModel);
    }

    SetImage(std::move(loadedImage));
    SetImagePath(filename.toStdString());
    m_LastSelectedSaveFormat = imageData.Format;
}

void MainWindow::OnFileSaveAction()
{
    if (!m_Image) {
        Log::Warn("OnFileSaveAction: no image to save");
        return;
    }

    if (m_ImagePath.empty()) {
        OnFileSaveAsAction();
    } else {
        TrySaveImage(*m_Image, m_ImagePath, m_LastSelectedSaveFormat);
    }
}

void MainWindow::OnFileSaveAsAction()
{
    if (!m_Image) {
        Log::Warn("OnFileSaveAsAction: no image to save");
        return;
    }

    QString selectedFilter;
    auto filename = QFileDialog::getSaveFileName(this, "Save As", QString(), m_ImageFileFilters, &selectedFilter);
    if (filename.isEmpty()) {
        return;
    }

    auto formatInfo = FILTER_TO_IMAGE_FORMAT_INFO_MAPPING.find(selectedFilter.toStdString());
    if (formatInfo == FILTER_TO_IMAGE_FORMAT_INFO_MAPPING.end()) {
        Log::Error("OnFileSaveAsAction: can't map filter to image format info");
        return;
    }

    const char* selectedFileExtension = formatInfo->second.FileExtension.c_str();
    ImageFormat selectedImageFormat = formatInfo->second.Format;

    if (!filename.endsWith(selectedFileExtension)) {
        filename.append(selectedFileExtension);
    }

    TrySaveImage(*m_Image, filename.toStdString(), selectedImageFormat);
    SetImagePath(filename.toStdString());
    m_LastSelectedSaveFormat = selectedImageFormat;
}

void MainWindow::OnImageColorModelActionSelected(ColorModel selectedColorModel)
{
    UpdateColorModelText(selectedColorModel);

    if (selectedColorModel == m_SelectedColorModel) {
        return;
    }

    ColorModel prevModel = m_SelectedColorModel;
    m_SelectedColorModel = selectedColorModel;

    if (m_Image) {
        SetImage(ConvertImageToNewModel(*m_Image, prevModel, m_SelectedColorModel));
    }
}

void MainWindow::UpdateColorModelText(ColorModel colorModel)
{
    auto it = ENUM_TO_STRING_COLOR_MODEL_MAPPING.find(colorModel);
    if (it == ENUM_TO_STRING_COLOR_MODEL_MAPPING.end()) {
        Log::Error("UpdateColorModelText: can't map enum {} to color model string", static_cast<int32_t>(colorModel));
        return;
    }

    m_ImageView->SetColorModelText(it->second.c_str());
}

void MainWindow::SetImage(std::unique_ptr<Image>&& image)
{
    m_Image = std::move(image);
    m_ImageView->SetImage(m_Image.get());

    bool enableSaveActions = (m_Image != nullptr);
    m_SaveAction->setEnabled(enableSaveActions);
    m_SaveAsAction->setEnabled(enableSaveActions);
}

bool MainWindow::TrySaveImage(const Image& image, const std::string& filename, ImageFormat format)
{
    if (m_SelectedColorModel == ColorModel::RGB || format == ImageFormat::Ppm) {
        Log::Debug("TrySaveImage: no need to convert image to rgb color model");
        return TrySaveRgbImage(image, filename, format);
    }

    Log::Debug("TrySaveImage: converting image to rgb color model");
    auto rgbImage = ConvertImageToNewModel(image, m_SelectedColorModel, ColorModel::RGB);
    return TrySaveRgbImage(*rgbImage, filename, format);
}

bool MainWindow::TrySaveRgbImage(const Image& image, const std::string& filename, ImageFormat format)
{
    Log::Debug("TrySaveRgbImage: writing image to file: {}", filename);

    try {
        image.WriteToFile(filename, format);
        Log::Debug("TrySaveRgbImage: image is successfully saved");
        return true;
    } catch (const std::exception& exception) {
        Log::Error("TrySaveRgbImage: an error occured while saving image to file: {}", exception.what());
        return false;
    }
}

void MainWindow::SetImagePath(const std::string& newPath)
{
    m_ImagePath = newPath;

    auto imageName = m_ImagePath.empty() ? NEW_IMAGE_DEFAULT_NAME : m_ImagePath;
    setWindowTitle((m_BaseTitle + " - " + imageName).c_str());
}

std::unique_ptr<Image> MainWindow::ConvertImageToNewModel(const Image& image, ColorModel currentModel, ColorModel newModel)
{
    Log::Debug("ConvertImageToNewModel: converting {} to {}", static_cast<uint32_t>(currentModel), static_cast<uint32_t>(newModel));

    if (currentModel == newModel) {
        return std::make_unique<Image>(image);
    }

    auto toRGBConverter = ColorModelConverter::GetConverter(currentModel);
    auto rgbImage = toRGBConverter->ToRGB(image);
    auto fromRGBConverter = ColorModelConverter::GetConverter(newModel);
    auto newImage = fromRGBConverter->FromRGB(*rgbImage);

    return newImage;
}
