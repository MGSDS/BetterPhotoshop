#include "MainWindow.hpp"
#include "Core/Image/Dither/Dither.hpp"
#include "Core/Image/Gamma/PowGammaCorrection.hpp"
#include "Core/Utils/Utils.hpp"
#include "Window/Dialogs/LineDialog.hpp"
#include <Core/Image/ColorModel/ColorModelConverter.hpp>
#include <Core/Image/Gamma/SrgbGammaCorrection.hpp>
#include <Core/Image/Image.hpp>
#include <Core/Log.hpp>
#include <Window/Dialogs/NewImageDialog.hpp>


#include <QFileDialog>
#include <QPixmap>
#include <QtWidgets>

#include <string>

std::string NEW_IMAGE_DEFAULT_NAME = "Untitled";
ColorModel DEFAULT_COLOR_MODEL = ColorModel::RGB;
ActiveChannel DEFAULT_ACTIVE_CHANNEL = ActiveChannel::ALL;
float DEFAULT_GAMMA_VALUE = 1.0f;

static void SetActionGroupEnabled(QActionGroup* actionGroup, bool enabled = true)
{
    actionGroup->setVisible(enabled);
    actionGroup->setEnabled(enabled);
}

MainWindow::MainWindow(const WindowSettings& settings)
    : m_BaseTitle(settings.Title)
    , m_SelectedColorModel(DEFAULT_COLOR_MODEL)
    , m_ActiveChannel(DEFAULT_ACTIVE_CHANNEL)
    , m_Gamma(DEFAULT_GAMMA_VALUE)
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
        fileMenu->setToolTipsVisible(true);
        fileMenu->addSection("Actual image");

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

        fileMenu->addSection("Image view");

        m_SaveViewAsAction = fileMenu->addAction("Save view as");
        m_SaveViewAsAction->setWhatsThis("Saving choosen color model and channels view as it is");
        m_SaveViewAsAction->setEnabled(false);
        connect(m_SaveViewAsAction, &QAction::triggered, this, &MainWindow::OnFileSaveViewAsAction);
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

        auto* activeChannelMenu = imageMenu->addMenu("Active channel");
        {
            for (const auto& [colorModelEnum, activeChannelToNameMap] :
                 ENUM_TO_CHANNEL_NAMES_BY_ACTIVE_CHANNEL_COLOR_MODEL_MAPPING) {
                auto* channelActionGroup = new QActionGroup(this);
                channelActionGroup->setExclusive(true);

                for (const auto& [activeChannelEnum, activeChannelName] : activeChannelToNameMap) {
                    auto* channelAction = activeChannelMenu->addAction(activeChannelName.c_str());
                    channelAction->setCheckable(true);
                    channelAction->setChecked(activeChannelEnum == DEFAULT_ACTIVE_CHANNEL);
                    channelActionGroup->addAction(channelAction);

                    connect(channelAction, &QAction::toggled, this, [this, activeChannel = activeChannelEnum](bool checked) {
                        if (checked) {
                            OnActiveChannelSelected(activeChannel);
                        }
                    });
                }

                SetActionGroupEnabled(channelActionGroup, colorModelEnum == DEFAULT_COLOR_MODEL);
                m_ColorModelActionGroupMapping[colorModelEnum] = channelActionGroup;
            }
        }

        auto* gammaMenu = imageMenu->addMenu("Gamma correction");
        {
            auto* assignGammaAction = gammaMenu->addAction("Assign gamma");
            connect(assignGammaAction, &QAction::triggered, this, &MainWindow::OnImageAssignGammaAction);

            m_ConvertGammaAction = gammaMenu->addAction("Convert gamma");
            m_ConvertGammaAction->setEnabled(false);
            connect(m_ConvertGammaAction, &QAction::triggered, this, &MainWindow::OnImageConvertGammaAction);
        }

        auto* ditheringMenu = imageMenu->addMenu("Dithering");
        {
            auto* ditherActionGroup = new QActionGroup(this);
            ditherActionGroup->setExclusive(true);

            for (const auto& [ditherAlgoEnum, ditherAlgoName] : ENUM_TO_STRING_DITHER_ALGO_MAPPING) {
                auto* ditheringAction = ditheringMenu->addAction(ditherAlgoName.c_str());
                ditherActionGroup->addAction(ditheringAction);
                connect(ditheringAction, &QAction::triggered, this, [this, ditherAlgo = ditherAlgoEnum]() {
                    OnDitheringActionSelected(ditherAlgo);
                });
            }
            SetActionGroupEnabled(ditherActionGroup, true);
        }

        auto* drawLineAction = imageMenu->addAction("Draw line");
        connect(drawLineAction, &QAction::triggered, this, &MainWindow::OnLineDrawAction);
    }
}

void MainWindow::InitImageView()
{
    m_ImageView = std::make_unique<ImageViewWithInfo>(this);
    setCentralWidget(m_ImageView.get());

    UpdateColorModelText(m_SelectedColorModel);
    UpdateActiveChannelsText(m_ActiveChannel);
    m_ImageView->SetCurrentGammaText(QString::number(m_Gamma));
    connect(m_ImageView.get(), &ImageViewWithInfo::selectButtonClicked, this, &MainWindow::OnImageSelectButtonClick);
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
    bool isGrayscaleGradient = dialog.IsGradient();
    Log::Debug("OnFileNewAction: New image width: {}, height: {}. Is gradient: {}", width, height, isGrayscaleGradient);

    m_SelectedColorModel = DEFAULT_COLOR_MODEL;
    m_DefaultColorModelAction->setChecked(true);

    if (isGrayscaleGradient) {
        SetImage(Image::MonochromeGradient(width, height));
    } else {
        SetImage(std::make_unique<Image>(width, height));
    }
    m_ImageView->CenterOnCurrentImage();
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
    m_ImageView->CenterOnCurrentImage();
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

void MainWindow::OnFileSaveViewAsAction()
{
    if (!m_Image) {
        Log::Warn("OnFileSaveAsAction: no image to save");
        return;
    }

    QString selectedFilter;
    auto fileFilters = (m_ActiveChannel == ActiveChannel::ALL) ? m_ImageFileFilters : "PGM Image (*.pgm)";
    auto filename = QFileDialog::getSaveFileName(this, "Save view As", QString(), fileFilters, &selectedFilter);
    if (filename.isEmpty()) {
        return;
    }

    auto formatInfo = FILTER_TO_IMAGE_FORMAT_INFO_MAPPING.find(selectedFilter.toStdString());
    if (formatInfo == FILTER_TO_IMAGE_FORMAT_INFO_MAPPING.end()) {
        Log::Error("OnFileSaveViewAsAction: can't map filter to image format info");
        return;
    }

    const char* selectedFileExtension = formatInfo->second.FileExtension.c_str();
    ImageFormat selectedImageFormat = formatInfo->second.Format;

    if (!filename.endsWith(selectedFileExtension)) {
        filename.append(selectedFileExtension);
    }

    auto visibleImage = TransformImageForQt(*m_Image);
    TrySaveImageWithoutConversion(visibleImage, filename.toStdString(), selectedImageFormat);
}

void MainWindow::OnImageColorModelActionSelected(ColorModel selectedColorModel)
{
    UpdateColorModelText(selectedColorModel);
    OnActiveChannelSelected(ActiveChannel::ALL);

    if (selectedColorModel == m_SelectedColorModel) {
        return;
    }

    ColorModel prevModel = m_SelectedColorModel;
    m_SelectedColorModel = selectedColorModel;

    SetActionGroupEnabled(m_ColorModelActionGroupMapping[prevModel], false);
    SetActionGroupEnabled(m_ColorModelActionGroupMapping[m_SelectedColorModel]);

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

void MainWindow::OnActiveChannelSelected(ActiveChannel selectedActiveChannel)
{
    m_ActiveChannel = selectedActiveChannel;
    Log::Debug("OnActiveChannelSelected: selected active channel: {}", static_cast<int>(selectedActiveChannel));

    UpdateActiveChannelsText(m_ActiveChannel);

    if (m_Image) {
        SetImage(std::move(m_Image));
    }
}

void MainWindow::UpdateActiveChannelsText(ActiveChannel activeChannel)
{
    const auto& channelName =
        ENUM_TO_CHANNEL_NAMES_BY_ACTIVE_CHANNEL_COLOR_MODEL_MAPPING
            .at(m_SelectedColorModel)
            .at(activeChannel);

    Log::Debug("UpdateActiveChannelsText: set channel text to '{}'", channelName.c_str());
    m_ImageView->SetActiveChannelText(channelName.c_str());
}

void MainWindow::SetImage(std::unique_ptr<Image>&& image)
{
    m_Image = std::move(image);
    SetImageForQt(m_Image.get());

    bool enableSaveActions = (m_Image != nullptr);
    m_SaveAction->setEnabled(enableSaveActions);
    m_SaveAsAction->setEnabled(enableSaveActions);
    m_SaveViewAsAction->setEnabled(enableSaveActions);
    m_ConvertGammaAction->setEnabled(enableSaveActions);
}

bool MainWindow::TrySaveImage(const Image& image, const std::string& filename, ImageFormat format)
{
    if (m_SelectedColorModel == ColorModel::RGB || format == ImageFormat::Ppm) {
        Log::Debug("TrySaveActualImage: no need to convert image to rgb color model");
        return TrySaveImageWithoutConversion(image, filename, format);
    }

    Log::Debug("TrySaveActualImage: converting image to rgb color model");
    auto rgbImage = ConvertImageToNewModel(image, m_SelectedColorModel, ColorModel::RGB);
    return TrySaveImageWithoutConversion(*rgbImage, filename, format);
}

bool MainWindow::TrySaveImageWithoutConversion(const Image& image, const std::string& filename, ImageFormat format)
{
    Log::Debug("TrySaveImageWithoutConversion: writing image to file: {}", filename);

    try {
        image.WriteToFile(filename, format);
        Log::Debug("TrySaveImageWithoutConversion: image is successfully saved");
        return true;
    } catch (const std::exception& exception) {
        Log::Error("TrySaveImageWithoutConversion: an error occured while saving image to file: {}", exception.what());
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

void MainWindow::OnImageAssignGammaAction()
{
    bool hasPressedOk = false;
    auto newGamma = QInputDialog::getDouble(this, "Assign new gamma", "New gamma", m_Gamma, 0.0,
                                            std::numeric_limits<int>::max(), 2, &hasPressedOk, {}, 0.01);

    if (!hasPressedOk) {
        return;
    }

    SetGamma(static_cast<float>(newGamma));

    if (!m_Image) {
        return;
    }

    SetImageForQt(m_Image.get());
}

void MainWindow::OnImageConvertGammaAction()
{
    bool hasPressedOk = false;
    auto newGamma = QInputDialog::getDouble(this, "Convert gamma", "New gamma", m_Gamma, 0.0,
                                            std::numeric_limits<int>::max(), 2, &hasPressedOk, {}, 0.01);

    if (!hasPressedOk) {
        return;
    }

    SetGamma(DEFAULT_GAMMA_VALUE);

    if (!m_Image) {
        return;
    }

    ApplyGammaCorrection(*m_Image, newGamma);
    SetImageForQt(m_Image.get());
}

void MainWindow::SetGamma(float newGamma)
{
    m_Gamma = std::max(newGamma, 0.0f);
    if (m_Gamma < 0.001f) {
        m_Gamma = 0.0f;
    }

    QString labelText = (m_Gamma == 0.0f) ? "0 (sRGB)" : QString::number(m_Gamma);
    m_ImageView->SetCurrentGammaText(labelText);
}

Image MainWindow::TransformImageForQt(const Image& image)
{
    auto newImage = image;

    ApplyGammaCorrection(newImage, m_Gamma);
    newImage = Image::CopyWithChannelMask(newImage, m_ActiveChannel);

    return newImage;
}

void MainWindow::SetImageForQt(const Image* image)
{
    if (!image) {
        m_ImageView->SetImage(nullptr);
        return;
    }

    auto transformedImage = TransformImageForQt(*image);
    m_ImageView->SetImage(&transformedImage);
}

void MainWindow::OnLineDrawAction()
{
    // TODO: disable buttons while drawing
    QMessageBox msgBox;
    msgBox.setText("Drawing mode. Click on two points to draw a line.");
    msgBox.setInformativeText("Enable drawing mode?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        m_DrawingMode = true;
        Log::Info("Drawing mode enabled");
    } else {
        Log::Info("Drawing mode not enabled");
    }
}

void MainWindow::OnImageSelectButtonClick(const QPointF& pos)
{
    if (!m_DrawingMode) {
        return;
    }

    Log::Info("Drawing mode enabled. Left click {},{}", pos.x(), pos.y());
    if (!(pos.x() >= 0 && pos.x() <= m_Image->GetWidth() && pos.y() >= 0 && pos.y() <= m_Image->GetHeight())) {
        Log::Info("Point is located outside of the image. Skipping");
        return;
    }

    m_SelectedPoints.push_back(pos);

    QMessageBox msgBox;
    std::string msg = "Point (" + std::to_string(static_cast<int>(pos.x())) + "," + std::to_string(static_cast<int>(pos.y())) + ") selected";
    msgBox.setText(QString(msg.c_str()));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    if (m_SelectedPoints.size() >= 2) {
        bool hasPressedOk = false;
        QList<int> res = LineDialog::getInts(this, &hasPressedOk);
        int lineWidth = res[0];
        m_DrawingMode = false;
        if (hasPressedOk) {
            Pixel color = Pixel(Utils::NormByte(res[1], 255.0f),
                                Utils::NormByte(res[2], 255.0f),
                                Utils::NormByte(res[3], 255.0f),
                                Utils::NormByte(res[4], 255.0f));

            switch (m_ActiveChannel) {
                case ActiveChannel::ZEROTH:
                    color.channels[1] = 0;
                    color.channels[2] = 0;
                    break;
                case ActiveChannel::FIRST:
                    color.channels[0] = 0;
                    color.channels[2] = 0;
                    break;
                case ActiveChannel::SECOND:
                    color.channels[0] = 0;
                    color.channels[1] = 0;
                    break;
                case ActiveChannel::ALL:
                    break;
            }

            auto line = Painter::DrawLine(m_Image->GetWidth(), m_Image->GetHeight(),
                                          m_SelectedPoints[0].x(), m_SelectedPoints[0].y(),
                                          m_SelectedPoints[1].x(), m_SelectedPoints[1].y(),
                                          lineWidth, color);
            Gamma::Correct(line, m_Gamma);
            m_Image->AddLayer(line);
            this->SetImage(std::move(m_Image));
        }
        m_SelectedPoints.clear();
    }
}

void MainWindow::ApplyGammaCorrection(Image& image, float gammaValue)
{
    if (m_Gamma == 0.0f) {
        Gamma::CorrectToSrgb(image);
    } else {
        Gamma::Correct(image, gammaValue);
    }
}

void MainWindow::OnDitheringActionSelected(DitherAlgo ditheringType)
{
    bool ok = false;
    int depth = QInputDialog::getDouble(this, "Dithering", "Depth", 1, 1, 8, 0, &ok, {}, 1);
    if (!ok) {
        return;
    }

    auto algo = Dither::GetDither(ditheringType);
    auto image = algo->Apply(*m_Image, depth);
    SetImage(std::move(image));
}