#include "nde/SafeWalletAdapter.hpp"

#include <algorithm>
#include <cctype>
#include <functional>
#include <stdexcept>

namespace nde {

SafeWalletAdapter::SafeWalletAdapter(AppConfig config) : config_(std::move(config)) {}

bool SafeWalletAdapter::initialize() {
    if (looksLikeSecretMaterial(config_.publicWalletAddress)) {
        throw std::runtime_error("Secret-like wallet material is not accepted by this client");
    }

    initialized_ = true;
    return initialized_;
}

std::string SafeWalletAdapter::publicIdentity() const {
    return initialized_ ? config_.publicWalletAddress : "uninitialized";
}

std::string SafeWalletAdapter::signDemoChallenge(const std::string& challenge) const {
    if (!initialized_) {
        return {};
    }

    std::hash<std::string> hash;
    return "nde-signature-public-only-" + std::to_string(hash(config_.publicWalletAddress + challenge));
}

bool SafeWalletAdapter::looksLikeSecretMaterial(const std::string& value) {
    const auto lowerContains = [&](const std::string& token) {
        auto lowered = value;
        std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        return lowered.find(token) != std::string::npos;
    };

    return lowerContains("private")
        || lowerContains("secret")
        || lowerContains("seed")
        || lowerContains("mnemonic")
        || value.find('[') != std::string::npos
        || value.size() > 120;
}

} // namespace nde
