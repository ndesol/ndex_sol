#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>

namespace nde {

class AuditLog {
public:
    explicit AuditLog(std::filesystem::path outputPath);

    void info(const std::string& component, const std::string& message);
    void warn(const std::string& component, const std::string& message);
    void error(const std::string& component, const std::string& message);

private:
    void write(const std::string& level, const std::string& component, const std::string& message);
    static std::string timestamp();

    std::filesystem::path outputPath_;
    std::ofstream stream_;
    std::mutex mutex_;
};

} // namespace nde
