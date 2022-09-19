#include "ImageView.hpp"

#include <Core/Log.hpp>

#include <QGraphicsView>
#include <QPixmap>
#include <QtWidgets>

static Qt::MouseButton MOVE_BUTTON = Qt::MouseButton::LeftButton;
static QRect PAN_RECTANGLE = QRect(-100000, -100000, 200000, 200000);
static qreal SCALE_STEP = 0.05;

ImageView::ImageView(QWidget* parent, QObject* sceneParent)
    : QGraphicsView(parent)
    , m_Scene(new QGraphicsScene(sceneParent))
{
    setScene(m_Scene);

    m_Scene->addRect(PAN_RECTANGLE);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scale(1, -1);

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

        fitInView(pixmapItem, Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    }
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_IsMoveButtonPressed) {
        QGraphicsView::mouseMoveEvent(event);
        return;
    }
    event->accept();
    
    int deltaX = m_PrevPanX - event->position().x();
    int deltaY = m_PrevPanY - event->position().y();
    m_PrevPanX = event->position().x();
    m_PrevPanY = event->position().y();

    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + deltaX);
    verticalScrollBar()->setValue(verticalScrollBar()->value() + deltaY);

}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != MOVE_BUTTON) {
        QGraphicsView::mousePressEvent(event);
        return;
    }
    event->accept();

    m_IsMoveButtonPressed = true;
    m_PrevPanX = event->position().x();
    m_PrevPanY = event->position().y();

}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != MOVE_BUTTON) {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }
    event->accept();

    m_IsMoveButtonPressed = false;
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if (!(event->modifiers() & Qt::ControlModifier)) {
        event->ignore();
        return;
    }
    event->accept();

    auto anchor = transformationAnchor();
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int angle = event->angleDelta().y();
    qreal factor;
    if (angle > 0) {
        factor = 1.0 + SCALE_STEP;
    } else {
        factor = 1.0 - SCALE_STEP;
    }
    scale(factor, factor);

    setTransformationAnchor(anchor);
}
