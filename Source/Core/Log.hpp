#pragma once

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

class Log
{
public:
    static void Init();

    template <typename... Args>
    static void Trace(Args&&... args) { m_Logger->trace(args...); }

    template <typename... Args>
    static void Debug(Args&&... args) { m_Logger->debug(args...); }

    template <typename... Args>
    static void Info(Args&&... args) { m_Logger->info(args...); }

    template <typename... Args>
    static void Warn(Args&&... args) { m_Logger->warn(args...); }

    template <typename... Args>
    static void Error(Args&&... args) { m_Logger->error(args...); }

    template <typename... Args>
    static void Critical(Args&&... args) { m_Logger->critical(args...); }

private:
    static std::unique_ptr<spdlog::logger> m_Logger;
};
