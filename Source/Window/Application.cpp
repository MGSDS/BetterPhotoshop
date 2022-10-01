#include "Application.hpp"

#include <Core/Log.hpp>

#include <QScreen>
#include <QFile>

const char* THEME_STYLESHEET_RESOURCE_PATH = ":dark/stylesheet.qss";

Application::Application(int argc, char** argv, const WindowSettings& windowSettings)
    : m_Application(std::make_unique<QApplication>(argc, argv))
    , m_MainWindow(std::make_unique<MainWindow>(windowSettings))
{
    m_MainWindow->move(m_MainWindow->screen()->availableGeometry().topLeft() + QPoint(100, 100));
    m_MainWindow->show();

    InitWidgetTheme();
}

void Application::InitWidgetTheme()
{
    QFile stylesheetFile(THEME_STYLESHEET_RESOURCE_PATH);
    
    if (!stylesheetFile.open(QFile::ReadOnly)) {
        Log::Error("Can't open resource file: {}", THEME_STYLESHEET_RESOURCE_PATH);
        return;
    }

    m_Application->setStyleSheet(stylesheetFile.readAll());
}

int Application::Exec()
{
    return m_Application->exec();
}