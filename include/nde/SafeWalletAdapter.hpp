#pragma once

#include "nde/AppConfig.hpp"

#include <string>

namespace nde {

class SafeWalletAdapter {
public:
    explicit SafeWalletAdapter(AppConfig config);

    bool initialize();
    std::string publicIdentity() const;
    std::string signDemoChallenge(const std::string& challenge) const;

private:
    static bool looksLikeSecretMaterial(const std::string& value);

    AppConfig config_;
    bool initialized_ = false;
};

} // namespace nde
