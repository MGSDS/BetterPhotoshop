#pragma once

#include <Core/Image/ColorModel/ColorModelConverter.hpp>
#include <Window/ImageViewWithInfo.hpp>

#include <QAction>
#include <QDialog>
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
    bool TrySaveImage(const Image& image, const std::string& filename, ImageFormat format);
    bool TrySaveRgbImage(const Image& image, const std::string& filename, ImageFormat format);
    void SetImagePath(const std::string& newPath);

    std::unique_ptr<Image> ConvertImageToNewModel(const Image& image, ColorModel currentModel, ColorModel newModel);

private slots:
    void OnFileNewAction();
    void OnFileOpenAction();
    void OnFileSaveAction();
    void OnFileSaveAsAction();

    void OnImageColorModelActionSelected(ColorModel selectedColorModel);

private:
    std::unique_ptr<ImageViewWithInfo> m_ImageView = nullptr;
    std::unique_ptr<Image> m_Image = nullptr;

    QAction* m_SaveAction = nullptr;
    QAction* m_SaveAsAction = nullptr;

    std::string m_BaseTitle = "";
    std::string m_ImagePath = "";
    ImageFormat m_LastSelectedSaveFormat = ImageFormat::Pgm;
    QString m_ImageFileFilters = "";

    ColorModel m_SelectedColorModel;
    QAction* m_DefaultColorModelAction = nullptr;
};
