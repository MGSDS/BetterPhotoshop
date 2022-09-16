#include "MainWindow.hpp"

#include <Core/Log.hpp>

#include <QtWidgets>

MainWindow::MainWindow(int width, int height, const char* title)
{
    resize(width, height);
    setWindowTitle(title);
}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    const QSize& newSize = event->size();
    Log::Debug("Window resize: {}, {}", newSize.width(), newSize.height());

}
