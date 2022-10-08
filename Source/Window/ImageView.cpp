#include "ImageView.hpp"

#include <Core/Log.hpp>

#include <QGraphicsView>
#include <QPixmap>
#include <QtWidgets>
#include <QImage>

static Qt::MouseButton MOVE_BUTTON = Qt::MouseButton::LeftButton;
static QRect PAN_RECTANGLE = QRect(-100000, -100000, 200000, 200000);
static qreal SCALE_STEP = 0.05;

ImageView::ImageView(QWidget* parent, QObject* sceneParent)
    : QGraphicsView(parent)
    , m_Scene(new QGraphicsScene(sceneParent))
{
    setScene(m_Scene.get());

    m_Scene->addRect(PAN_RECTANGLE);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

void ImageView::SetImage(const std::shared_ptr<Image>& img) {
    auto image = std::make_unique<QImage>(img->ToDataRGBA32FPx4(), img->GetWidth(), img->GetHeight(), QImage::Format_RGBA32FPx4);
    QPixmap pixmap = QPixmap::fromImage(*image);
    m_Scene->removeItem(m_Image.get());
    m_Image = std::unique_ptr<QGraphicsItem>{m_Scene->addPixmap(pixmap)};
    fitInView(m_Image.get(), Qt::AspectRatioMode::KeepAspectRatio);
}
