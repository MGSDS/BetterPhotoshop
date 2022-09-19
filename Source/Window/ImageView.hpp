#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

class ImageView
{
public:
    ImageView(QWidget* parent = nullptr, QObject* sceneParent = nullptr);
    ~ImageView() = default;

    QGraphicsView* GetViewWidget() { return m_SceneView; } 

private:
    QGraphicsScene* m_Scene = nullptr;
    QGraphicsView* m_SceneView = nullptr;
};
