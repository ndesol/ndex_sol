#include "nde/AppConfig.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace nde {
namespace {

std::string readAll(const std::filesystem::path& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Configuration file could not be opened: " + path.string());
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

std::string extractString(const std::string& text, const std::string& key, const std::string& fallback = {}) {
    const std::regex pattern("\"" + key + "\"\\s*:\\s*\"([^\"]*)\"");
    std::smatch match;
    return std::regex_search(text, match, pattern) ? match[1].str() : fallback;
}

int extractInt(const std::string& text, const std::string& key, int fallback) {
    const std::regex pattern("\"" + key + "\"\\s*:\\s*(\\d+)");
    std::smatch match;
    return std::regex_search(text, match, pattern) ? std::stoi(match[1].str()) : fallback;
}

bool extractBool(const std::string& text, const std::string& key, bool fallback) {
    const std::regex pattern("\"" + key + "\"\\s*:\\s*(true|false)");
    std::smatch match;
    return std::regex_search(text, match, pattern) ? match[1].str() == "true" : fallback;
}

} // namespace

AppConfig AppConfig::loadFromFile(const std::filesystem::path& path) {
    const auto text = readAll(path);

    AppConfig config;
    config.clientId = extractString(text, "clientId", "nde-node");
    config.dashboardUrl = extractString(text, "dashboardUrl", "https://ndesol.top");
    config.solanaRpcUrl = extractString(text, "solanaRpcUrl", "https://api.mainnet-beta.solana.com");
    config.solanaProgramId = extractString(text, "solanaProgramId", "FkMPbGB3bs2at6cdytVAeX4s9beCUd7wtReUntQSzket");
    config.publicWalletAddress = extractString(text, "publicWalletAddress", "PUBLIC_WALLET_ADDRESS_ONLY");
    config.proxy.type = extractString(text, "type", "none");
    config.proxy.host = extractString(text, "host");
    config.proxy.port = extractInt(text, "port", 0);
    config.proxy.username = extractString(text, "username");
    config.proxy.password = extractString(text, "password");
    config.resourcePolicy.cpuCoreCount = extractInt(text, "cpuCoreCount", 1);
    config.resourcePolicy.memoryMb = extractInt(text, "memoryMb", 1024);
    config.resourcePolicy.maxDiskGb = extractInt(text, "maxDiskGb", 1);
    config.resourcePolicy.maxCpuPercent = extractInt(text, "maxCpuPercent", 25);
    config.resourcePolicy.maxGpuPercent = extractInt(text, "maxGpuPercent", 0);
    config.resourcePolicy.maxDiskMbps = extractInt(text, "maxDiskMbps", 0);
    config.resourcePolicy.enableAmdCpuOptimization = extractBool(text, "enableAmdCpuOptimization", false);
    config.resourcePolicy.allowGpu = extractBool(text, "allowGpu", false);
    config.resourcePolicy.allowDiskTasks = extractBool(text, "allowDiskTasks", false);
    config.validate();
    return config;
}

void AppConfig::validate() const {
    if (clientId.empty()) {
        throw std::runtime_error("clientId is required");
    }
    if (dashboardUrl.rfind("https://", 0) != 0) {
        throw std::runtime_error("dashboardUrl must use https");
    }
    if (solanaRpcUrl.rfind("https://", 0) != 0) {
        throw std::runtime_error("solanaRpcUrl must use https");
    }
    if (solanaProgramId.empty()) {
        throw std::runtime_error("solanaProgramId is required");
    }
    if (publicWalletAddress.empty()) {
        throw std::runtime_error("publicWalletAddress is required");
    }
    if (proxy.type != "none" && proxy.type != "http" && proxy.type != "socks") {
        throw std::runtime_error("proxy.type must be none, http, or socks");
    }
    if (proxy.enabled() && (proxy.host.empty() || proxy.port < 1 || proxy.port > 65535)) {
        throw std::runtime_error("enabled proxy requires host and port");
    }
    if (resourcePolicy.cpuCoreCount < 1 || resourcePolicy.cpuCoreCount > 256) {
        throw std::runtime_error("cpuCoreCount must be between 1 and 256");
    }
    if (resourcePolicy.memoryMb < 128 || resourcePolicy.memoryMb > 1048576) {
        throw std::runtime_error("memoryMb must be between 128 and 1048576");
    }
    if (resourcePolicy.maxDiskGb < 0 || resourcePolicy.maxDiskGb > 1048576) {
        throw std::runtime_error("maxDiskGb must be between 0 and 1048576");
    }
    if (resourcePolicy.maxCpuPercent < 1 || resourcePolicy.maxCpuPercent > 100) {
        throw std::runtime_error("maxCpuPercent must be between 1 and 100");
    }
    if (resourcePolicy.maxGpuPercent < 0 || resourcePolicy.maxGpuPercent > 100) {
        throw std::runtime_error("maxGpuPercent must be between 0 and 100");
    }
    if (resourcePolicy.maxDiskMbps < 0 || resourcePolicy.maxDiskMbps > 1000) {
        throw std::runtime_error("maxDiskMbps must be between 0 and 1000");
    }
}

bool ProxyConfig::enabled() const {
    return type == "http" || type == "socks";
}

std::string ProxyConfig::endpoint() const {
    if (!enabled()) {
        return "direct";
    }

    std::ostringstream output;
    output << type << "://" << host << ':' << port;
    return output.str();
}

} // namespace nde
