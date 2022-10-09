#pragma once

#include <Window/ImageView.hpp>

#include <QMainWindow>
#include <QMenu>

#include <memory>

struct WindowSettings
{
    WindowSettings& SetWidth(int width) { Width = width; return *this; }
    WindowSettings& SetHeight(int height) { Height = height; return *this; }
    WindowSettings& SetIsMaximized(bool isMaximized) { IsMaximized = isMaximized; return *this; }
    WindowSettings& SetTitle(const char* title) { Title = title; return *this; }

    int Width = 1280;
    int Height = 720;
    bool IsMaximized = false;
    const char* Title = "Photoshop";
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
    void SetImage(std::unique_ptr<Image>&& image);

private slots:
    void resizeEvent(QResizeEvent *event) override;
    void OnFileNewAction();
    void OnFileOpenAction();

private:
    std::unique_ptr<ImageView> m_ImageView;
    std::unique_ptr<Image> m_Image;

    QMenu* m_SaveAsMenu;
};
