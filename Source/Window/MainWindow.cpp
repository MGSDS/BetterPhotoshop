#include "MainWindow.hpp"

#include <Core/Image/Image.hpp>

#include <Core/Log.hpp>

#include <QtWidgets>
#include <QPixmap>
#include <QFileDialog>

#include <string>

MainWindow::MainWindow(int width, int height, const char* title)
{
    resize(width, height);
    setWindowTitle(title);

    InitMenuBar();
    InitImageView();
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

    auto* saveAsMenu = fileMenu->addMenu("Save As");
    {
        auto* pgmAction = saveAsMenu->addAction("PGM Image");
        connect(pgmAction, &QAction::triggered, this, [this]() {
            SaveImageToFile(ImageFormat::Pgm, ".pgm");
        });

        auto* ppmAction = saveAsMenu->addAction("PPM Image");
        connect(ppmAction, &QAction::triggered, this, [this]() {
            SaveImageToFile(ImageFormat::Ppm, ".ppm");
        });
    }
}

void MainWindow::InitImageView()
{
    m_ImageView = std::make_unique<ImageView>(this);
    m_Image = std::make_shared<Image>(256, 256);
    m_ImageView->SetImage(m_Image);
    setCentralWidget(m_ImageView.get());
}

void MainWindow::OnFileNewAction()
{
    //TODO: Dialog
    size_t height = 256;
    size_t width = 256;
    m_Image = std::make_shared<Image>(width, height);
    m_ImageView->SetImage(m_Image);

    Log::Debug("File->New");
}

void MainWindow::OnFileOpenAction()
{
    auto filename = QFileDialog::getOpenFileName(this, "Open image", QString());
    if (filename.isEmpty()) {
        return;
    }

    Log::Debug("Trying to read image from file: {}", filename.toStdString().c_str());

    try {
        m_Image = Image::FromFile(filename.toStdString());
        m_ImageView->SetImage(m_Image);
    } catch (const std::exception& exception) {
        Log::Error("An error occured while reading image from file: {}", exception.what());
    }
}

void MainWindow::SaveImageToFile(ImageFormat format, const char* extension)
{
    auto filename = QFileDialog::getSaveFileName(this, "Image", QString(), "Image (*" + QString(extension) + ")");
    if (filename.isEmpty()) {
        return;
    }

    if (!filename.endsWith(extension)) {
        filename += extension;
    }

    Log::Debug("Trying to write image to file: {}", filename.toStdString().c_str());

    try {
        m_Image->WriteToFile(filename.toStdString(), format);
    } catch (const std::exception& exception) {
        Log::Error("An error occured while saving image to file: {}", exception.what());
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    const QSize& newSize = event->size();
    Log::Debug("Window resize: {}, {}", newSize.width(), newSize.height());
}
