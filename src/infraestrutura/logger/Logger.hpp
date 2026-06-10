#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <string>

class Logger {
private:
    static std::shared_ptr<spdlog::logger>& get()
    {
        static std::shared_ptr<spdlog::logger> inst;
        return inst;
    }

public:
    static void init()
    {
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        auto logger = std::make_shared<spdlog::logger>(
            "taskmanager",
            spdlog::sinks_init_list{consoleSink}
        );
        logger->set_level(spdlog::level::debug);
        logger->flush_on(spdlog::level::warn);

        get() = logger;
    }

    static void debug(const std::string& msg) { get()->debug(msg); }
    static void info(const std::string& msg)  { get()->info(msg);  }
    static void warn(const std::string& msg)  { get()->warn(msg);  }
    static void error(const std::string& msg) { get()->error(msg); }
};
