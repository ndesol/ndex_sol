#include "nde/SolanaClient.hpp"

#include <functional>
#include <iostream>
#include <sstream>

namespace nde {

SolanaClient::SolanaClient(AppConfig config) : config_(std::move(config)) {}

bool SolanaClient::connect() {
    connected_ = true;
    std::cout << "Solana RPC boundary connected: " << config_.solanaRpcUrl << '\n';
    return connected_;
}

SolanaClusterStatus SolanaClient::fetchClusterStatus() {
    if (!connected_) {
        return {};
    }

    slotCursor_ += 64;
    return {slotCursor_, slotCursor_ / 432000, 4200.5};
}

std::string SolanaClient::requestValidationPacket(int round) {
    std::ostringstream packet;
    packet << "ledger-packet-round-" << round << "-slot-" << slotCursor_;
    return packet.str();
}

bool SolanaClient::submitSyntheticProof(const Task& task) {
    if (!connected_) {
        return false;
    }

    std::cout << "Solana proof accepted for " << task.id
              << " proof=" << task.syntheticProof << '\n';
    return true;
}

} // namespace nde
