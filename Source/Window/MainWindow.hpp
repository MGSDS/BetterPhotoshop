#pragma once

#include <Window/ImageView.hpp>

#include <QMainWindow>

#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int width, int height, const char* title);
    ~MainWindow() override = default;

private:
    void InitMenuBar();
    void InitImageView();

private slots:
    void resizeEvent(QResizeEvent *event) override;
    void OnFileNewAction();
    void OnFileOpenAction();
    void OnFileSaveAction();

private:
    static std::shared_ptr<Image> createEmptyImage(size_t width, size_t height);
    std::unique_ptr<ImageView> m_ImageView;
    std::shared_ptr<Image> m_Image;
};
