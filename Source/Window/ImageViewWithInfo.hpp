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
    void SetColorModelText(const QString& text);

private slots:
    void OnImageSizeChanged(const QSize& newSize);
    void OnCursorPosChanged(const QPointF& newPos);
    void OnZoomChanged(float newZoom);

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
};
