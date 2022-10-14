#pragma once

#include <Window/ImageViewWithInfo.hpp>

#include <QAction>
#include <QMainWindow>
#include <QString>

#include <memory>

struct WindowSettings
{
    WindowSettings& SetWidth(int width)
    {
        Width = width;
        return *this;
    }

    WindowSettings& SetHeight(int height)
    {
        Height = height;
        return *this;
    }

    WindowSettings& SetIsMaximized(bool isMaximized)
    {
        IsMaximized = isMaximized;
        return *this;
    }

    WindowSettings& SetTitle(const char* title)
    {
        Title = title;
        return *this;
    }

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
    void InitImageFileFilters();

    void SetImage(std::unique_ptr<Image>&& image);
    bool TrySaveCurrentImage(const std::string& filename, ImageFormat format);
    void SetImagePath(const std::string& newPath);

private slots:
    void resizeEvent(QResizeEvent* event) override;
    void OnFileNewAction();
    void OnFileOpenAction();
    void OnFileSaveAction();
    void OnFileSaveAsAction();

private:
    std::unique_ptr<ImageViewWithInfo> m_ImageView = nullptr;
    std::unique_ptr<Image> m_Image = nullptr;

    QAction* m_SaveAction = nullptr;
    QAction* m_SaveAsAction = nullptr;

    std::string m_BaseTitle = "";
    std::string m_ImagePath = "";
    ImageFormat m_LastSelectedSaveFormat = ImageFormat::Pgm;
    QString m_ImageFileFilters = "";
};
