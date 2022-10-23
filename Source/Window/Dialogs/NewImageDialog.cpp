#include "NewImageDialog.hpp"

#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>

QSize NEW_IMAGE_MIN_SIZE = { 1, 1 };
QSize NEW_IMAGE_MAX_SIZE = { 32768, 32768 };
QSize NEW_IMAGE_DEFAULT_SIZE = { 1280, 720 };

NewImageDialog::NewImageDialog(QWidget* parent)
    : QDialog(parent)
    , m_WidthSpinBox(new QSpinBox())
    , m_HeightSpinBox(new QSpinBox())
{
    setWindowTitle("New Image");
    setFixedSize(200, 150);

    auto labelWidth = new QLabel();
    labelWidth->setText("Width");
    labelWidth->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_WidthSpinBox->setMinimum(NEW_IMAGE_MIN_SIZE.width());
    m_WidthSpinBox->setMaximum(NEW_IMAGE_MAX_SIZE.width());
    m_WidthSpinBox->setValue(NEW_IMAGE_DEFAULT_SIZE.width());

    auto widthLayout = new QHBoxLayout();
    widthLayout->addWidget(labelWidth);
    widthLayout->addWidget(m_WidthSpinBox);

    auto labelHeight = new QLabel();
    labelHeight->setText("Height");
    labelHeight->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_HeightSpinBox->setMinimum(NEW_IMAGE_MIN_SIZE.height());
    m_HeightSpinBox->setMaximum(NEW_IMAGE_MAX_SIZE.height());
    m_HeightSpinBox->setValue(NEW_IMAGE_DEFAULT_SIZE.height());

    auto heightLayout = new QHBoxLayout();
    heightLayout->addWidget(labelHeight);
    heightLayout->addWidget(m_HeightSpinBox);

    auto okButton = new QPushButton();
    okButton->setText("Ok");
    okButton->setDefault(true);
    connect(okButton, &QPushButton::pressed, this, &QDialog::accept);

    auto dialogLayout = new QVBoxLayout();
    dialogLayout->addLayout(widthLayout);
    dialogLayout->addLayout(heightLayout);
    dialogLayout->addWidget(okButton);

    setLayout(dialogLayout);
}
