#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Core/Image/Image.hpp"

class ImageView : public QGraphicsView
{
public:
    ImageView(QWidget* parent = nullptr, QObject* sceneParent = nullptr);
    ~ImageView() = default;
    void SetImage(const std::shared_ptr<Image>& image);

private:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene* m_Scene = nullptr;
    bool m_IsMoveButtonPressed = false;
    int m_PrevPanX = 0, m_PrevPanY = 0;
};
