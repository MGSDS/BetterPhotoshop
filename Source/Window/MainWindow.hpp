#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int width, int height, const char* title);
    ~MainWindow() override;

private:
    void resizeEvent(QResizeEvent *event) override;
};
