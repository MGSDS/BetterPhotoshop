#pragma once

#include <Core/Image/ColorModel/ColorModelConverter.hpp>
#include <Core/Image/Editors/Painter.hpp>
#include <Core/Image/Filter/Filter.hpp>
#include <Window/ImageViewWithInfo.hpp>
#include <Core/Image/Resize/Resizer.hpp>

#include <QAction>
#include <QDialog>
#include <QMainWindow>
#include <QString>

#include "Core/Image/Dither/Dither.hpp"
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
    void InitWindow(const WindowSettings& settings);
    void InitMenuBar();
    void InitImageView();
    void InitImageFileFilters();

    void SetImage(std::unique_ptr<Image>&& image);
    bool TrySaveImage(const Image& image, const std::string& filename, ImageFormat format);
    bool TrySaveImageWithoutConversion(const Image& image, const std::string& filename, ImageFormat format);
    void SetImagePath(const std::string& newPath);

    void UpdateColorModelText(ColorModel selectedColorModel);
    void UpdateActiveChannelsText(ActiveChannel activeChannels);

    std::unique_ptr<Image> ConvertImageToNewModel(const Image& image, ColorModel currentModel, ColorModel newModel);

    void SetGamma(float newGamma);
    void ApplyGammaCorrection(Image& image, float gammaValue);

private slots:
    void OnFileNewAction();
    void OnFileOpenAction();
    void OnFileSaveAction();
    void OnFileSaveAsAction();
    void OnFileSaveViewAsAction();
    void OnLineDrawAction();
    void OnApplyFilterAction(FilterAlgo filter);
    void OnImageSelectButtonClick(const QPointF& pos);

    void OnImageColorModelActionSelected(ColorModel selectedColorModel);
    void OnActiveChannelSelected(ActiveChannel activeChannels);
    void OnDitheringActionSelected(DitherAlgo ditheringType);
    void OnResizeActionSelected(ResizeAlgo resizeType);

    void OnImageAssignGammaAction();
    void OnImageConvertGammaAction();

    Image TransformImageForQt(const Image& image);
    void SetImageForQt(const Image* image);

private:
    std::unique_ptr<ImageViewWithInfo> m_ImageView = nullptr;
    std::unique_ptr<Image> m_Image = nullptr;
    ActiveChannel m_ActiveChannel;

    QAction* m_SaveAction = nullptr;
    QAction* m_SaveAsAction = nullptr;
    QAction* m_SaveViewAsAction = nullptr;
    QAction* m_ConvertGammaAction = nullptr;

    std::string m_BaseTitle = "";
    std::string m_ImagePath = "";
    ImageFormat m_LastSelectedSaveFormat = ImageFormat::Pgm;
    QString m_ImageFileFilters = "";
    bool m_DrawingMode = false;
    std::vector<QPointF> m_SelectedPoints;

    ColorModel m_SelectedColorModel;
    QAction* m_DefaultColorModelAction = nullptr;

    float m_Gamma;

    std::unordered_map<ColorModel, QActionGroup*> m_ColorModelActionGroupMapping;
};
