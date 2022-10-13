#include "MainWindow.hpp"

#include <Core/Image/Image.hpp>

#include <Core/Log.hpp>

#include <QtWidgets>
#include <QPixmap>
#include <QFileDialog>
#include <QDialog>

#include <string>

QSize NEW_IMAGE_MIN_SIZE = { 1, 1 };
QSize NEW_IMAGE_MAX_SIZE = { 32768, 32768 };
QSize NEW_IMAGE_DEFAULT_SIZE = { 1280, 720 };
std::string NEW_IMAGE_DEFAULT_NAME = "Untitled";

MainWindow::MainWindow(const WindowSettings& settings)
{
    resize(settings.Width, settings.Height);

    if (settings.IsMaximized) {
        setWindowState(Qt::WindowMaximized);
    }

    m_BaseTitle = settings.Title;
    setWindowTitle(m_BaseTitle.c_str());

    InitMenuBar();
    InitImageView();
    InitImageFileFilters(); 
}

void MainWindow::InitMenuBar()
{
    auto* menu = menuBar();

    auto* fileMenu = menu->addMenu("File");
    
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

void MainWindow::InitImageView()
{
    m_ImageView = std::make_unique<ImageViewWithInfo>(this);
    setCentralWidget(m_ImageView.get());
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
    QDialog dialog(this);
    dialog.setWindowTitle("New Image");
    dialog.setFixedSize(200, 150);

    auto labelWidth = new QLabel();
    labelWidth->setText("Width");
    labelWidth->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    auto spinBoxWidth = new QSpinBox();
    spinBoxWidth->setMinimum(NEW_IMAGE_MIN_SIZE.width());
    spinBoxWidth->setMaximum(NEW_IMAGE_MAX_SIZE.width());
    spinBoxWidth->setValue(NEW_IMAGE_DEFAULT_SIZE.width());

    auto widthLayout = new QHBoxLayout();
    widthLayout->addWidget(labelWidth);
    widthLayout->addWidget(spinBoxWidth);

    auto labelHeight = new QLabel();
    labelHeight->setText("Height");
    labelHeight->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    auto spinBoxHeight = new QSpinBox();
    spinBoxHeight->setMinimum(NEW_IMAGE_MIN_SIZE.height());
    spinBoxHeight->setMaximum(NEW_IMAGE_MAX_SIZE.height());
    spinBoxHeight->setValue(NEW_IMAGE_DEFAULT_SIZE.height());

    auto heightLayout = new QHBoxLayout();
    heightLayout->addWidget(labelHeight);
    heightLayout->addWidget(spinBoxHeight);

    auto okButton = new QPushButton();
    okButton->setText("Ok");
    okButton->setDefault(true);
    connect(okButton, &QPushButton::pressed, &dialog, &QDialog::accept);

    auto dialogLayout = new QVBoxLayout();
    dialogLayout->addLayout(widthLayout);
    dialogLayout->addLayout(heightLayout);
    dialogLayout->addWidget(okButton);

    dialog.setLayout(dialogLayout);

    if (!dialog.exec()) {
        return;
    }

    int width = spinBoxWidth->value();
    int height = spinBoxHeight->value();
    Log::Debug("New image width: {}, height: {}", width, height);

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

    Log::Debug("Trying to read image from file: {}", filename.toStdString().c_str());

    try {
        SetImage(Image::FromFile(filename.toStdString()));
    } catch (const std::exception& exception) {
        Log::Error("An error occured while reading image from file: {}", exception.what());
        return;
    }

    SetImagePath(filename.toStdString());
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
        TrySaveCurrentImage(m_ImagePath, m_LastSelectedSaveFormat);
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

    TrySaveCurrentImage(filename.toStdString(), selectedImageFormat);
    SetImagePath(filename.toStdString());
    m_LastSelectedSaveFormat = selectedImageFormat;
}

void MainWindow::SetImage(std::unique_ptr<Image>&& image)
{
    m_Image = std::move(image);
    m_ImageView->SetImage(m_Image.get());

    bool enableSaveActions = (m_Image != nullptr);
    m_SaveAction->setEnabled(enableSaveActions);
    m_SaveAsAction->setEnabled(enableSaveActions);
}

bool MainWindow::TrySaveCurrentImage(const std::string& filename, ImageFormat format)
{
    Log::Debug("TrySaveCurrentImage: writing image to file: {}", filename);

    try {
        m_Image->WriteToFile(filename, format);
        Log::Debug("TrySaveCurrentImage: image is successfully saved");
        return true;
    } catch (const std::exception& exception) {
        Log::Error("TrySaveCurrentImage: an error occured while saving image to file: {}", exception.what());
        return false;
    }
}

void MainWindow::SetImagePath(const std::string& newPath)
{
    m_ImagePath = newPath;

    auto imageName = m_ImagePath.empty() ? NEW_IMAGE_DEFAULT_NAME : m_ImagePath;
    setWindowTitle((m_BaseTitle + " - " + imageName).c_str());
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
    const QSize& newSize = event->size();
    Log::Debug("Window resize: {}, {}", newSize.width(), newSize.height());
}
