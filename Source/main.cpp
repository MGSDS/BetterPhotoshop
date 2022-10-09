#include <QApplication>
#include <QScreen>

#include <Core/Log.hpp>
#include <Window/Application.hpp>

int main(int argc, char* argv[])
{
    Log::Init();

    auto windowSettings = WindowSettings()
        .SetIsMaximized(true)
        .SetTitle("Photoshop");

    Application app(argc, argv, windowSettings);
    return app.Exec();
}
