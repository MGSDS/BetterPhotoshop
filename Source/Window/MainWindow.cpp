#include "MainWindow.hpp"

#include <Core/Image/Image.hpp>

#include <Core/Log.hpp>

#include <QtWidgets>
#include <QPixmap>
#include <QFileDialog>
#include <QDialog>

#include <string>

QSize NEW_IMAGE_MIN_SIZE = { 1, 1 };
QSize NEW_IMAGE_MAX_SIZE = { 32768, 32768 };
QSize NEW_IMAGE_DEFAULT_SIZE = { 1280, 720 };

MainWindow::MainWindow(const WindowSettings& settings)
{
    resize(settings.Width, settings.Height);
    setWindowTitle(settings.Title);

    InitMenuBar();
    InitImageView();
}

void MainWindow::InitMenuBar()
{
    auto* menu = menuBar();

    auto* fileMenu = menu->addMenu("File");
    
    auto* newAction = fileMenu->addAction("New");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::OnFileNewAction);

    auto* openAction = fileMenu->addAction("Open");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::OnFileOpenAction);

    auto* saveAsMenu = fileMenu->addMenu("Save As");
    {
        auto* pgmAction = saveAsMenu->addAction("PGM Image");
        connect(pgmAction, &QAction::triggered, this, [this]() {
            SaveImageToFile(ImageFormat::Pgm, ".pgm");
        });

        auto* ppmAction = saveAsMenu->addAction("PPM Image");
        connect(ppmAction, &QAction::triggered, this, [this]() {
            SaveImageToFile(ImageFormat::Ppm, ".ppm");
        });
    }
}

void MainWindow::InitImageView()
{
    m_ImageView = std::make_unique<ImageView>(this);
    m_Image = std::make_shared<Image>(NEW_IMAGE_DEFAULT_SIZE.width(), NEW_IMAGE_DEFAULT_SIZE.height());
    m_ImageView->SetImage(m_Image);
    setCentralWidget(m_ImageView.get());
}

void MainWindow::OnFileNewAction()
{
    QDialog dialog(this);
    dialog.setWindowTitle("New Image");
    dialog.setFixedSize(200, 150);

    auto labelWidth = new QLabel();
    labelWidth->setText("Width");
    labelWidth->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    auto spinBoxWidth = new QSpinBox();
    spinBoxWidth->setMinimum(NEW_IMAGE_MIN_SIZE.width());
    spinBoxWidth->setMaximum(NEW_IMAGE_MAX_SIZE.width());
    spinBoxWidth->setValue(NEW_IMAGE_DEFAULT_SIZE.width());

    auto widthLayout = new QHBoxLayout();
    widthLayout->addWidget(labelWidth);
    widthLayout->addWidget(spinBoxWidth);

    auto labelHeight = new QLabel();
    labelHeight->setText("Height");
    labelHeight->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    auto spinBoxHeight = new QSpinBox();
    spinBoxHeight->setMinimum(NEW_IMAGE_MIN_SIZE.height());
    spinBoxHeight->setMaximum(NEW_IMAGE_MAX_SIZE.height());
    spinBoxHeight->setValue(NEW_IMAGE_DEFAULT_SIZE.height());

    auto heightLayout = new QHBoxLayout();
    heightLayout->addWidget(labelHeight);
    heightLayout->addWidget(spinBoxHeight);

    auto okButton = new QPushButton();
    okButton->setText("Ok");
    okButton->setDefault(true);
    connect(okButton, &QPushButton::pressed, &dialog, &QDialog::accept);

    auto dialogLayout = new QVBoxLayout();
    dialogLayout->addLayout(widthLayout);
    dialogLayout->addLayout(heightLayout);
    dialogLayout->addWidget(okButton);

    dialog.setLayout(dialogLayout);

    if (!dialog.exec()) {
        return;
    }

    int width = spinBoxWidth->value();
    int height = spinBoxHeight->value();
    Log::Debug("New image width: {}, height: {}", width, height);

    m_Image = std::make_shared<Image>(width, height);
    m_ImageView->SetImage(m_Image);
}

void MainWindow::OnFileOpenAction()
{
    QFileDialog dialog(this);
    dialog.setViewMode(QFileDialog::List);
    if (!dialog.exec()) {
        return;
    }

    auto filename = dialog.selectedFiles()[0];

    if (filename.isEmpty()) {
        return;
    }

    Log::Debug("Trying to read image from file: {}", filename.toStdString().c_str());

    try {
        m_Image = Image::FromFile(filename.toStdString());
        m_ImageView->SetImage(m_Image);
    } catch (const std::exception& exception) {
        Log::Error("An error occured while reading image from file: {}", exception.what());
    }
}

void MainWindow::SaveImageToFile(ImageFormat format, const char* extension)
{
    auto filename = QFileDialog::getSaveFileName(this, "Image", QString(), "Image (*" + QString(extension) + ")");
    if (filename.isEmpty()) {
        return;
    }

    if (!filename.endsWith(extension)) {
        filename += extension;
    }

    Log::Debug("Trying to write image to file: {}", filename.toStdString().c_str());

    try {
        m_Image->WriteToFile(filename.toStdString(), format);
    } catch (const std::exception& exception) {
        Log::Error("An error occured while saving image to file: {}", exception.what());
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    const QSize& newSize = event->size();
    Log::Debug("Window resize: {}, {}", newSize.width(), newSize.height());
}
