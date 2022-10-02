#include <QApplication>
#include <QScreen>

#include <Core/Log.hpp>
#include <Window/Application.hpp>

int main(int argc, char* argv[])
{
    static_assert(sizeof(float) * 8 == 32, "Required size of float is 32 bits");

    Log::Init();

    auto windowSettings = WindowSettings()
        .SetWidth(1280)
        .SetHeight(720)
        .SetTitle("Photoshop");

    Application app(argc, argv, windowSettings);
    return app.Exec();
}
