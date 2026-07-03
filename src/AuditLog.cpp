#include "nde/AuditLog.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace nde {

AuditLog::AuditLog(std::filesystem::path outputPath)
    : outputPath_(std::move(outputPath)), stream_(outputPath_, std::ios::app) {}

void AuditLog::info(const std::string& component, const std::string& message) {
    write("INFO", component, message);
}

void AuditLog::warn(const std::string& component, const std::string& message) {
    write("WARN", component, message);
}

void AuditLog::error(const std::string& component, const std::string& message) {
    write("ERROR", component, message);
}

void AuditLog::write(const std::string& level, const std::string& component, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    const auto line = timestamp() + " [" + level + "] [" + component + "] " + message;
    std::cout << line << '\n';
    if (stream_) {
        stream_ << line << '\n';
    }
}

std::string AuditLog::timestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif

    std::ostringstream output;
    output << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return output.str();
}

} // namespace nde
