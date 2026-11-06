#include "lux/Core.h"
#include <cstdio>
#include <cstdarg>
#include <chrono>
#include <ctime>

namespace lux {

static LogLevel s_logLevel = LogLevel::Trace;

void setLogLevel(LogLevel level) {
    s_logLevel = level;
}

void log(LogLevel level, const char* fmt, ...) {
    if (level < s_logLevel) return;

    const char* levelStr[] = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };

    // Timestamp
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif

    char timeBuf[32];
    std::strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &tm);

    // Format message
    char msgBuf[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, sizeof(msgBuf), fmt, args);
    va_end(args);

    // Color codes
    const char* color = "";
    const char* reset = "\033[0m";
    switch (level) {
        case LogLevel::Trace: color = "\033[90m"; break;
        case LogLevel::Debug: color = "\033[36m"; break;
        case LogLevel::Info:  color = "\033[32m"; break;
        case LogLevel::Warn:  color = "\033[33m"; break;
        case LogLevel::Error: color = "\033[31m"; break;
        case LogLevel::Fatal: color = "\033[35m"; break;
    }

    printf("%s[%s] [%s] %s%s\n", color, timeBuf, levelStr[(int)level], msgBuf, reset);
    fflush(stdout);
}

} // namespace lux