#include "Log.hpp"

std::unique_ptr<spdlog::logger> Log::m_Logger;

void Log::Init()
{
    std::vector<spdlog::sink_ptr> sinks = {
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
    };

    sinks[0]->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");

    m_Logger = std::make_unique<spdlog::logger>("", sinks.begin(), sinks.end());
    m_Logger->set_level(spdlog::level::trace);
    m_Logger->flush_on(spdlog::level::trace);
}
