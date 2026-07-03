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
    config.publicWalletAddress = extractString(text, "publicWalletAddress", "PUBLIC_WALLET_ADDRESS_ONLY");
    config.resourcePolicy.maxCpuPercent = extractInt(text, "maxCpuPercent", 25);
    config.resourcePolicy.maxGpuPercent = extractInt(text, "maxGpuPercent", 0);
    config.resourcePolicy.maxDiskMbps = extractInt(text, "maxDiskMbps", 0);
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
    if (publicWalletAddress.empty()) {
        throw std::runtime_error("publicWalletAddress is required");
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

} // namespace nde
