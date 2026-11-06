#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <functional>
#include <cassert>

namespace lux {

// Basic types
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using f32 = float;
using f64 = double;

// Logging
enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

void log(LogLevel level, const char* fmt, ...);
void setLogLevel(LogLevel level);

#define LUX_TRACE(...) ::lux::log(::lux::LogLevel::Trace, __VA_ARGS__)
#define LUX_DEBUG(...) ::lux::log(::lux::LogLevel::Debug, __VA_ARGS__)
#define LUX_INFO(...)  ::lux::log(::lux::LogLevel::Info,  __VA_ARGS__)
#define LUX_WARN(...)  ::lux::log(::lux::LogLevel::Warn,  __VA_ARGS__)
#define LUX_ERROR(...) ::lux::log(::lux::LogLevel::Error, __VA_ARGS__)
#define LUX_FATAL(...) ::lux::log(::lux::LogLevel::Fatal, __VA_ARGS__)

// Result type for error handling
template<typename T, typename E = std::string>
struct Result {
    bool ok;
    T value;
    E error;

    static Result success(T v) { return { true, std::move(v), {} }; }
    static Result failure(E e) { return { false, {}, std::move(e) }; }

    bool isOk() const { return ok; }
    bool isErr() const { return !ok; }
};

} // namespace lux