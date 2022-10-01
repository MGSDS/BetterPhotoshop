#pragma once

#include <Window/ImageView.hpp>

#include <QMainWindow>

#include <memory>

struct WindowSettings
{
    WindowSettings& SetWidth(int width) { Width = width; return *this; }
    WindowSettings& SetHeight(int height) { Height = height; return *this; }
    WindowSettings& SetTitle(const char* title) { Title = title; return *this; }

    int Width;
    int Height;
    const char* Title;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const WindowSettings& settings);
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
