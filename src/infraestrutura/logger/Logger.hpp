#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <string>
#include <filesystem>

class Logger {
public:
    static void init() {
        std::filesystem::create_directories("logs");

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            "logs/app.log", true
        );
        fileSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

        auto logger = std::make_shared<spdlog::logger>(
            "taskmanager",
            spdlog::sinks_init_list{consoleSink, fileSink}
        );

        logger->set_level(spdlog::level::debug);
        spdlog::flush_on(spdlog::level::warn);
        spdlog::set_default_logger(logger);
    }

    static void debug(const std::string& msg) { spdlog::debug(msg); }
    static void info(const std::string& msg)  { spdlog::info(msg);  }
    static void warn(const std::string& msg)  { spdlog::warn(msg);  }
    static void error(const std::string& msg) { spdlog::error(msg); }
};
