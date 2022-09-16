#include <QApplication>
#include <QScreen>

#include <Window/MainWindow.hpp>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window(1280, 720, "Photoshop");
    window.move(window.screen()->availableGeometry().topLeft() + QPoint(100, 100));
    window.show();

    return app.exec();
}
