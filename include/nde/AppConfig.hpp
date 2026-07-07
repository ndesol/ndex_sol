#pragma once

#include <filesystem>
#include <string>

namespace nde {

struct ResourcePolicy {
    int cpuCoreCount = 1;
    int memoryMb = 1024;
    int maxDiskGb = 1;
    int maxCpuPercent = 25;
    int maxGpuPercent = 0;
    int maxDiskMbps = 0;
    bool allowGpu = false;
    bool allowDiskTasks = false;
};

struct ProxyConfig {
    std::string type = "none";
    std::string host;
    int port = 0;
    std::string username;
    std::string password;

    bool enabled() const;
    std::string endpoint() const;
};

struct AppConfig {
    std::string clientId;
    std::string dashboardUrl;
    std::string solanaRpcUrl;
    std::string solanaProgramId;
    std::string publicWalletAddress;
    ProxyConfig proxy;
    ResourcePolicy resourcePolicy;

    static AppConfig loadFromFile(const std::filesystem::path& path);
    void validate() const;
};

} // namespace nde
