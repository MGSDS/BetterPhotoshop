#include "ImageView.hpp"

#include <QGraphicsView>
#include <QPixmap>
#include <QtWidgets>

ImageView::ImageView(QWidget* parent, QObject* sceneParent)
    : m_Scene(new QGraphicsScene(sceneParent))
    , m_SceneView(new QGraphicsView(m_Scene, parent))
{
    QRect hugeRect(-100000, -100000, 200000, 200000);
    m_Scene->addRect(hugeRect);

    m_SceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_SceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_SceneView->scale(1, -1);

    // Temp image, won't be created here
    {
        int size = 16;
        std::vector<uint8_t> data;
        for (int i = 0; i < size * size; i++) {
            data.push_back(rand() % 256); // b
            data.push_back(rand() % 256); // g
            data.push_back(rand() % 256); // r
            data.push_back(255);          // a
        }

        auto* image = new QImage(&data[0], size, size, QImage::Format_ARGB32);
        auto pixmap = QPixmap::fromImage(*image);
        QGraphicsItem* pixmapItem = m_Scene->addPixmap(pixmap);

        m_SceneView->fitInView(pixmapItem, Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    }
}
