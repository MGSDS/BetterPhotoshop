#include "MainWindow.hpp"

#include <QtWidgets>

#include <iostream> 

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
    std::cout << "Window resize: " << newSize.width() << ", " << newSize.height() << '\n';
}
