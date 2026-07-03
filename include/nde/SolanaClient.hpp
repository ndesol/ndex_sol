#pragma once

#include "nde/AppConfig.hpp"
#include "nde/Task.hpp"

#include <cstdint>
#include <string>

namespace nde {

struct SolanaClusterStatus {
    std::uint64_t currentSlot = 0;
    std::uint64_t epoch = 0;
    double simulatedTps = 0.0;
};

class SolanaClient {
public:
    explicit SolanaClient(AppConfig config);

    bool connect();
    SolanaClusterStatus fetchClusterStatus();
    std::string requestValidationPacket(int round);
    bool submitSyntheticProof(const Task& task);

private:
    AppConfig config_;
    bool connected_ = false;
    std::uint64_t slotCursor_ = 281000000;
};

} // namespace nde
