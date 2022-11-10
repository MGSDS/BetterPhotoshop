#include "ImageViewWithInfo.hpp"

#include <Core/Log.hpp>

#include <sstream>

static QWidget* CreateLineSeparator(QWidget* parent)
{
    auto* lineSeparator = new QFrame(parent);
    lineSeparator->setFrameShape(QFrame::Shape::VLine);
    lineSeparator->setFrameShadow(QFrame::Shadow::Sunken);
    return lineSeparator;
}

ImageViewWithInfo::ImageViewWithInfo(QWidget* parent, QObject* sceneParent)
    : QWidget(parent)
    , m_ImageView(new ImageView(this))
    , m_SizeLabel(new QLabel(this))
    , m_CoordsLabel(new QLabel(this))
    , m_ZoomLabel(new QLabel(this))
    , m_ColorModelLabel(new QLabel(this))
    , m_ActiveChannelLabel(new QLabel(this))
    , m_CurrentGammaLabel(new QLabel(this))
{
    m_LabelsLayout = new QHBoxLayout();

    m_SizeLabel->setMinimumWidth(80);
    m_CoordsLabel->setMinimumWidth(80);
    m_ZoomLabel->setMinimumWidth(50);
    m_ColorModelLabel->setMinimumWidth(80);
    m_ActiveChannelLabel->setMinimumWidth(80);
    m_CurrentGammaLabel->setMinimumWidth(50);

    std::vector<QLabel*> leftLabels = { m_SizeLabel, m_CoordsLabel, m_ZoomLabel };
    for (auto* label : leftLabels) {
        label->setAlignment(Qt::AlignCenter);
        m_LabelsLayout->addWidget(CreateLineSeparator(this));
        m_LabelsLayout->addWidget(label);
    }

    m_LabelsLayout->addWidget(CreateLineSeparator(this));
    m_LabelsLayout->addStretch();

    std::vector<QLabel*> rightLabels = { m_CurrentGammaLabel, m_ColorModelLabel, m_ActiveChannelLabel };
    for (auto* label : rightLabels) {
        label->setAlignment(Qt::AlignCenter);
        m_LabelsLayout->addWidget(CreateLineSeparator(this));
        m_LabelsLayout->addWidget(label);
    }

    m_MainLayout = new QVBoxLayout();
    m_MainLayout->addWidget(m_ImageView);
    m_MainLayout->addLayout(m_LabelsLayout);

    setLayout(m_MainLayout);

    connect(m_ImageView, &ImageView::imageSizeChanged, this, &ImageViewWithInfo::OnImageSizeChanged);
    connect(m_ImageView, &ImageView::cursorPosChanged, this, &ImageViewWithInfo::OnCursorPosChanged);
    connect(m_ImageView, &ImageView::zoomChanged, this, &ImageViewWithInfo::OnZoomChanged);
}

void ImageViewWithInfo::SetImage(const Image* image)
{
    m_ImageView->SetImage(image);

    if (!image) {
        ClearImageInfo();
    }
}

void ImageViewWithInfo::CenterOnCurrentImage()
{
    m_ImageView->CenterOnCurrentImage();
}

void ImageViewWithInfo::SetColorModelText(const QString& text)
{
    m_ColorModelLabel->setText(text);
}

void ImageViewWithInfo::SetActiveChannelText(const QString& text)
{
    m_ActiveChannelLabel->setText(text);
}

void ImageViewWithInfo::SetCurrentGammaText(const QString& text)
{
    m_CurrentGammaLabel->setText(text);
}

void ImageViewWithInfo::OnImageSizeChanged(const QSize& newSize)
{
    std::stringstream ss;
    ss << newSize.width() << ", " << newSize.height();
    m_SizeLabel->setText(ss.str().c_str());
}

void ImageViewWithInfo::OnCursorPosChanged(const QPointF& newPos)
{
    std::stringstream ss;
    ss << static_cast<int>(newPos.x()) << ", " << static_cast<int>(newPos.y());
    m_CoordsLabel->setText(ss.str().c_str());
}

void ImageViewWithInfo::OnZoomChanged(float newZoom)
{
    std::stringstream ss;
    ss << static_cast<int>(100 * newZoom) << "%";
    m_ZoomLabel->setText(ss.str().c_str());
}

void ImageViewWithInfo::ClearImageInfo()
{
    std::vector<QLabel*> labels = { m_SizeLabel, m_CoordsLabel, m_ZoomLabel };
    for (auto* label : labels) {
        label->clear();
    }
}
