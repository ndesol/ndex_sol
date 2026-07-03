#pragma once

#include <filesystem>
#include <string>

namespace nde {

struct ResourcePolicy {
    int maxCpuPercent = 25;
    int maxGpuPercent = 0;
    int maxDiskMbps = 0;
    bool allowGpu = false;
    bool allowDiskTasks = false;
};

struct AppConfig {
    std::string clientId;
    std::string dashboardUrl;
    std::string solanaRpcUrl;
    std::string publicWalletAddress;
    ResourcePolicy resourcePolicy;

    static AppConfig loadFromFile(const std::filesystem::path& path);
    void validate() const;
};

} // namespace nde
