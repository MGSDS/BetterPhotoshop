#include "MainWindow.hpp"
#include "Core/Image/PpmImage.hpp"

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

    auto* saveAction = fileMenu->addAction("Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::OnFileSaveAction);
}

void MainWindow::InitImageView()
{
    m_ImageView = std::make_unique<ImageView>(this);
    m_Image = std::make_shared<PpmImage>(256, 256);
    m_ImageView->SetImage(m_Image);
    setCentralWidget(m_ImageView.get());
}

void MainWindow::OnFileNewAction()
{
    //TODO: Dialog
    size_t height = 256;
    size_t width = 256;
    m_Image = std::make_shared<PpmImage>(width, height);
    m_ImageView->SetImage(m_Image);

    Log::Debug("File->New");
}

void MainWindow::OnFileOpenAction()
{
    auto filename = QFileDialog::getOpenFileName(this, "Open image", QString(), tr("Image (*.ppm *.pgm)"));
    if (filename.isEmpty()) {
        return;
    }

    m_Image = Image::FromFile(filename.toStdString());
    m_ImageView->SetImage(m_Image);

    Log::Debug("File: {}", filename.toStdString().c_str());
}

void MainWindow::OnFileSaveAction()
{

    auto filename = QFileDialog::getSaveFileName(this, tr("Image"), QString(), tr(("Image (" + m_Image->GetExtension() + ")").c_str()));
    if (filename.isEmpty()) {
        return;
    }

    m_Image->WriteToFile(filename.toStdString());

    Log::Debug("File: {}", filename.toStdString().c_str());
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    const QSize& newSize = event->size();
    Log::Debug("Window resize: {}, {}", newSize.width(), newSize.height());
}
