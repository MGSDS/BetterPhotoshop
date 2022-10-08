#pragma once

#include <QApplication>
#include <Window/MainWindow.hpp>

#include <memory>

class Application
{
public:
    Application(int& argc, char** argv, const WindowSettings& windowSettings);
    
    int Exec();

private:
    void InitWidgetTheme();

private:
    std::unique_ptr<QApplication> m_Application;
    std::unique_ptr<MainWindow> m_MainWindow;
};
