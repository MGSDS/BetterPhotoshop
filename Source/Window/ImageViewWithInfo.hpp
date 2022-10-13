#pragma once

#include <Window/ImageView.hpp>

#include <QWidget>
#include <QLabel>
#include <QLayout>

class ImageViewWithInfo : public QWidget
{
    Q_OBJECT

public:
    ImageViewWithInfo(QWidget* parent = nullptr, QObject* sceneParent = nullptr);
    ~ImageViewWithInfo();
    void SetImage(const Image* image);

private slots:
    void OnImageAppeared();
    void OnImageDisappeared();
    void OnImageSizeChanged(const QSize& newSize);
    void OnCursorPosChanged(const QPointF& newPos);
    void OnZoomChanged(float newZoom);

private:
    ImageView* m_ImageView = nullptr;

    QVBoxLayout* m_MainLayout = nullptr;
    QHBoxLayout* m_LabelsLayout = nullptr;
    QLabel* m_SizeLabel = nullptr;
    QLabel* m_CoordsLabel = nullptr;
    QLabel* m_ZoomLabel = nullptr;
};
