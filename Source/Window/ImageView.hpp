#pragma once

#include "Core/Image/Image.hpp"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

class ImageView : public QGraphicsView
{
    Q_OBJECT

public:
    ImageView(QWidget* parent = nullptr);
    ~ImageView() = default;
    void SetImage(const Image* image);

private:
    void CenterOnCurrentImage();

private slots:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

signals:
    void imageAppeared();
    void imageDisappeared();
    void imageSizeChanged(const QSize& newSize);
    void cursorPosChanged(const QPointF& newPos);
    void zoomChanged(float newZoom);

private:
    std::unique_ptr<QGraphicsScene> m_Scene = nullptr;
    std::unique_ptr<QGraphicsItem> m_Image = nullptr;
    bool m_IsMoveButtonPressed = false;
    int m_PrevPanX = 0, m_PrevPanY = 0;
    qreal m_CurrentZoom = 1.0;
};