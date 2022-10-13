#include "ImageView.hpp"

#include <Core/Log.hpp>
#include "Core/Image/ColorModel/ColorModelConverter.hpp"

#include <QGraphicsView>
#include <QPixmap>
#include <QtWidgets>
#include <QImage>

static Qt::MouseButton MOVE_BUTTON = Qt::MouseButton::LeftButton;
static QRect PAN_RECTANGLE = QRect(-100000, -100000, 200000, 200000);
static qreal SCALE_STEP = 0.05;
static qreal MIN_ZOOM = 0.1;
static qreal MAX_ZOOM = 50.0;

ImageView::ImageView(QWidget* parent)
    : QGraphicsView(parent)
    , m_Scene(std::make_unique<QGraphicsScene>(this))
{
    setScene(m_Scene.get());

    m_Scene->addRect(PAN_RECTANGLE);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    const auto newPos = mapToScene(event->pos());
    emit cursorPosChanged(newPos);

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
    event->accept();

    if (event->button() != MOVE_BUTTON) {
        return;
    }

    m_IsMoveButtonPressed = true;
    m_PrevPanX = event->position().x();
    m_PrevPanY = event->position().y();

}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();

    if (event->button() != MOVE_BUTTON) {
        return;
    }

    m_IsMoveButtonPressed = false;
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if (!(event->modifiers() & Qt::ControlModifier)) {
        QGraphicsView::wheelEvent(event);
        return;
    }
    event->accept();

    auto anchor = transformationAnchor();

    int angle = event->angleDelta().y();
    qreal factor;
    if (angle > 0) {
        factor = 1.0 + SCALE_STEP;
    } else {
        factor = 1.0 - SCALE_STEP;
    }

    qreal clampedZoom = std::clamp(m_CurrentZoom * factor, MIN_ZOOM, MAX_ZOOM);
    factor = clampedZoom / m_CurrentZoom;

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_CurrentZoom *= factor;
    scale(factor, factor);
    setTransformationAnchor(anchor);

    emit zoomChanged(m_CurrentZoom);
}

void ImageView::SetImage(const Image* img) {
    if (m_Image) {
        m_Scene->removeItem(m_Image.get());
    }

    if (!img) {
        m_Image = nullptr;
        emit imageDisappeared();
        return;
    }

    bool imageHasAppeared = !m_Image;

    auto image = std::make_unique<QImage>(img->ToDataRGBA32FPx4(), img->GetWidth(), img->GetHeight(), QImage::Format_RGBA32FPx4);
    QPixmap pixmap = QPixmap::fromImage(*image);
    m_Image = std::unique_ptr<QGraphicsItem>{m_Scene->addPixmap(pixmap)};
    emit imageSizeChanged(QSize(img->GetWidth(), img->GetHeight()));

    CenterOnCurrentImage();

    if (imageHasAppeared) {
        emit imageAppeared();
        emit zoomChanged(1.0);
        emit cursorPosChanged(mapToScene(cursor().pos()));
    }
}

void ImageView::CenterOnCurrentImage()
{
    resetTransform();
    m_CurrentZoom = 1.0;

    centerOn(m_Image.get());

    // Fix weird scaling snap bug
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale(1.0, 1.0);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}
