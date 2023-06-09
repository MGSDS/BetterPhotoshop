#pragma once

#include <Window/ImageView.hpp>

#include <QLabel>
#include <QLayout>
#include <QWidget>

class ImageViewWithInfo : public QWidget
{
    Q_OBJECT

public:
    ImageViewWithInfo(QWidget* parent = nullptr, QObject* sceneParent = nullptr);
    ~ImageViewWithInfo() override = default;

    void SetImage(const Image* image);
    void CenterOnCurrentImage();

    void SetColorModelText(const QString& text);
    void SetActiveChannelText(const QString& text);
    void SetCurrentGammaText(const QString& text);

private slots:
    void OnImageSizeChanged(const QSize& newSize);
    void OnCursorPosChanged(const QPointF& newPos);
    void OnZoomChanged(float newZoom);
    void OnSelectButtonClicked(const QPointF& pos);

signals:
    void selectButtonClicked(const QPointF& pos);

private:
    void ClearImageInfo();

private:
    ImageView* m_ImageView = nullptr;

    QVBoxLayout* m_MainLayout = nullptr;
    QHBoxLayout* m_LabelsLayout = nullptr;
    QLabel* m_SizeLabel = nullptr;
    QLabel* m_CoordsLabel = nullptr;
    QLabel* m_ZoomLabel = nullptr;
    QLabel* m_ColorModelLabel = nullptr;
    QLabel* m_ActiveChannelLabel = nullptr;
    QLabel* m_CurrentGammaLabel = nullptr;
};
