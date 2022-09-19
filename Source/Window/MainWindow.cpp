#include "MainWindow.hpp"

#include <Core/Log.hpp>

#include <QtWidgets>
#include <QPixmap>

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
    m_GraphicsScene = new QGraphicsScene(this);

    int size = 16;

    std::vector<uint8_t> data(size * size * 4, 255);
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;

    data[data.size() - 2] = 0;
    data[data.size() - 3] = 0;
    data[data.size() - 4] = 0;

    auto* hugeRect = m_GraphicsScene->addRect(-100000, -100000, 200000, 200000);

    auto* image = new QImage(&data[0], size, size, QImage::Format_ARGB32);
    auto pixmap = QPixmap::fromImage(*image);
    auto* pixmapItem = m_GraphicsScene->addPixmap(pixmap);

    QGraphicsView* view = new QGraphicsView(m_GraphicsScene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->fitInView(pixmapItem, Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    view->scale(1, -1);

    setCentralWidget(view);
}

void MainWindow::OnFileNewAction()
{
    Log::Debug("File->New");
}

void MainWindow::OnFileOpenAction()
{
    Log::Debug("File->Open");
}

void MainWindow::OnFileSaveAction()
{
    Log::Debug("File->Save");
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    const QSize& newSize = event->size();
    Log::Debug("Window resize: {}, {}", newSize.width(), newSize.height());
}
