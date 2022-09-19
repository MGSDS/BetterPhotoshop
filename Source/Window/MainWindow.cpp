#include "MainWindow.hpp"

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
    setCentralWidget(m_ImageView->GetViewWidget());
}

void MainWindow::OnFileNewAction()
{
    Log::Debug("File->New");
}

void MainWindow::OnFileOpenAction()
{
    auto filename = QFileDialog::getOpenFileName(this, "Open image");
    if (filename.isEmpty()) {
        return;
    }

    Log::Debug("File: {}", filename.toStdString().c_str());
}

void MainWindow::OnFileSaveAction()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save image");
    if (filename.isEmpty()) {
        return;
    }

    Log::Debug("File: {}", filename.toStdString().c_str());
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    const QSize& newSize = event->size();
    Log::Debug("Window resize: {}, {}", newSize.width(), newSize.height());
}
