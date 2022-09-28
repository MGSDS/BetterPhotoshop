#pragma once

#include <Window/ImageView.hpp>

#include <QMainWindow>

#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int width, int height, const char* title);
    ~MainWindow() override = default;

private:
    void InitMenuBar();
    void InitImageView();
    void SaveImageToFile(ImageFormat format, const char* extension);

private slots:
    void resizeEvent(QResizeEvent *event) override;
    void OnFileNewAction();
    void OnFileOpenAction();

private:
    std::unique_ptr<ImageView> m_ImageView;
    std::shared_ptr<Image> m_Image;
};
