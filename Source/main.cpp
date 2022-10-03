#include <QApplication>
#include <QScreen>

#include <Core/Log.hpp>
#include <Window/Application.hpp>

int main(int argc, char* argv[])
{
    Log::Init();

    auto windowSettings = WindowSettings()
        .SetWidth(1280)
        .SetHeight(720)
        .SetTitle("Photoshop");

    Application app(argc, argv, windowSettings);
    return app.Exec();
}
