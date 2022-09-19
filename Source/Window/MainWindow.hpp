#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QImage>
#include <QLabel>
#include <QGraphicsScene>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int width, int height, const char* title);
    ~MainWindow() override = default;

private:
    void InitMenuBar();
    void InitImageView();

private slots:
    void resizeEvent(QResizeEvent *event) override;
    void OnFileNewAction();
    void OnFileOpenAction();
    void OnFileSaveAction();

private:
    QGraphicsScene* m_GraphicsScene;
};
